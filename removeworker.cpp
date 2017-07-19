#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include "removeworker.h"

namespace Farman
{

RemoveWorker::RemoveWorker(const QStringList& paths, QObject *parent)
    : Worker(parent)
    , m_paths(paths)
{

}

RemoveWorker::~RemoveWorker()
{

}

void RemoveWorker::process()
{
    qDebug() << "start RemoveWorker::process()";

    emitPrepare("Preparing remove...");

    QList<QString> removeList;

    // コピーするファイル・ディレクトリのリストを作成
    for(auto path : m_paths)
    {
        if(isAborted())
        {
            emitFinished(static_cast<int>(Result::Abort));

            return;
        }

        int ret = makeList(path, removeList);
        if(ret != static_cast<int>(Result::Success))
        {
            qDebug() << "makeList() : ret =" << QString("%1").arg(ret, 0, 16);
            emitFinished(ret);

            return;
        }
    }

    emitMinMax(0, removeList.size());
    emitProgress(0);

    int progress = 0;
    for(auto path : removeList)
    {
        QFileInfo fileInfo(path);
        if(fileInfo.isDir())
        {
            qDebug() << "remove dir  :" << path;

            if(!QDir().rmdir(path))
            {
                // ディレクトリ削除失敗
                qDebug() << "remove dir error  :" << path;
                emitFinished(static_cast<int>(Result::ErrorRemoveDir));

                return;
            }
        }
        else
        {
            qDebug() << "remove file :" << path;

            if(!QFile::remove(path))
            {
                // ファイル削除失敗
                qDebug() << "remove file error :" << path;
                emitFinished(static_cast<int>(Result::ErrorRemoveFile));

                return;
            }
        }

        progress++;
        emitProgress(progress);
    }

    qDebug() << "finish RemoveWorker::process()";

    emitFinished(static_cast<int>(Result::Success));
}

int RemoveWorker::makeList(const QString& path, QList<QString>& removeList)
{
    if(isAborted())
    {
        return static_cast<int>(Result::Abort);
    }

    QFileInfo fileInfo(path);

    if(fileInfo.isDir())
    {
        // 再帰処理でディレクトリ内のエントリをリストに追加する
        QDir          removeDir(path);
        QFileInfoList childFileInfoList = removeDir.entryInfoList(QDir::AllEntries |
                                                                  QDir::AccessMask |
                                                                  QDir::AllDirs |
                                                                  QDir::NoDotAndDotDot,
                                                                  QDir::DirsFirst);

        for(auto childFileInfo : childFileInfoList)
        {
            int ret = makeList(childFileInfo.absoluteFilePath(), removeList);
            if(ret != static_cast<int>(Result::Success))
            {
                return ret;
            }
        }
    }

    removeList.push_back(fileInfo.absoluteFilePath());
    qDebug() << fileInfo.absoluteFilePath();

    return static_cast<int>(Result::Success);
}

}           // namespace Farman
