#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <QThread>
#include "readfileworker.h"
#include "workerresult.h"

namespace Farman
{

static constexpr int unitReadSize = 65536 * 8;
static constexpr int progressMax = 1000;

ReadFileWorker::ReadFileWorker(const QString& filePath, QByteArray* buffer, QObject *parent/* = Q_NULLPTR*/) :
    Worker(parent),
    m_filePath(filePath),
    m_buffer(buffer)
{
    Q_ASSERT(m_buffer != Q_NULLPTR);
}

ReadFileWorker::~ReadFileWorker()
{
    qDebug() << "ReadFileWorker::~ReadFileWorker()";
}

void ReadFileWorker::run()
{
    qDebug() << "start ReadFileWorker::run()";

    emitOutputConsole(tr("%1 reading...").arg(m_filePath));

    if(isAborted())
    {
        emitOutputConsole(tr("Aborted.\n"));
        emitFinished(static_cast<int>(WorkerResult::Abort));

        return;
    }

    QFile file(m_filePath);
    if(!file.open(QFile::ReadOnly))
    {
        emitOutputConsole(tr("Failed.\n"));
        emitFinished(static_cast<int>(WorkerResult::ErrorOpenFile));

        return;
    }

    qint64 fileSize = file.size();
    if(fileSize == 0)
    {
        file.close();

        emitOutputConsole(tr("File size is 0.\n"));
        emitFinished(static_cast<int>(WorkerResult::ErrorSizeZero));

        return;
    }

    emitStart(0, progressMax);

    while(!file.atEnd())
    {
        thread()->msleep(1);                    // sleep を入れないとコンソールの描画が乱れる場合がある

        if(isAborted())
        {
            file.close();

            emitOutputConsole(tr("Aborted.\n"));
            emitFinished(static_cast<int>(WorkerResult::Abort));

            return;
        }

        QByteArray data(file.read(unitReadSize));
        if(data.size() <= 0)
        {
            file.close();

            emitOutputConsole(tr("Failed.\n"));
            emitFinished(static_cast<int>(WorkerResult::ErrorReadFile));

            return;
        }

        m_buffer->append(data);
        emitProgress(m_buffer->size() * progressMax / fileSize);
    }

    file.close();

    emitOutputConsole(tr("Success.\n"));

    qDebug() << "finish ReadFileWorker::run()";

    emitFinished(static_cast<int>(WorkerResult::Success));
}

}           // namespace Farman
