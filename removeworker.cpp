#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include "removeworker.h"
#include "mainwindow.h"

namespace Farman
{

RemoveWorker::RemoveWorker(const QStringList& paths, QObject *parent)
    : Worker(parent)
    , m_paths(paths)
{
    connect(this,
            SIGNAL(outputConsole(const QString)),
            MainWindow::getInstance(),
            SLOT(onOutputConsole(const QString)));
}

RemoveWorker::~RemoveWorker()
{

}

void RemoveWorker::run()
{
    qDebug() << "start RemoveWorker::run()";

    emitStart(0, m_paths.size());

    int progress = 0;
    for(auto path : m_paths)
    {
        if(isAborted())
        {
            emitOutputConsole(tr("Aborted.\n"));
            emitFinished(static_cast<int>(Result::Abort));

            return;
        }

        if(QFileInfo(path).isDir())
        {
            emitOutputConsole(QString("%1 ... ").arg(path));
            qDebug() << "remove dir  :" << path;

            if(!QDir(path).removeRecursively())
            {
                // ディレクトリ削除失敗
                emitOutputConsole(tr("Failed remove directory.\n"));
                qDebug() << "remove dir error  :" << path;
                emitFinished(static_cast<int>(Result::ErrorRemoveDir));

                return;
            }

            emitOutputConsole(tr("Removed directory.\n"));
        }
        else
        {
            emitOutputConsole(QString("%1 ... ").arg(path));
            qDebug() << "remove file :" << path;

            if(!QFile::remove(path))
            {
                // ファイル削除失敗
                emitOutputConsole(QString("Failed remove file"));
                qDebug() << "remove file error :" << path;
                emitFinished(static_cast<int>(Result::ErrorRemoveFile));

                return;
            }

            emitOutputConsole(tr("Removed file.\n"));
        }

        emitProcess(QString(tr("%1 file(s) remove...done.")).arg(progress + 1));

        progress++;
        emitProgress(progress);
    }

    qDebug() << "finish RemoveWorker::run()";

    emitFinished(static_cast<int>(Result::Success));
}

}           // namespace Farman
