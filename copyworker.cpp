#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include "copyworker.h"
#include "overwritedialog.h"

namespace Farman
{

CopyWorker::CopyWorker(const QStringList& srcPaths, const QString& dstPath, QObject *parent)
    : Worker(parent)
    , m_srcPaths(srcPaths)
    , m_dstPath(dstPath)
    , m_methodType(OverwriteMethodType::Default)
{

}

CopyWorker::~CopyWorker()
{

}

void CopyWorker::process()
{
    qDebug() << "start CopyWorker::process()";

    QMap<QString, QString> copyList;

    // コピーするファイル・ディレクトリのリストを作成
    for(auto srcPath : m_srcPaths)
    {
        if(isAborted())
        {
            emitFinished(static_cast<int>(Result::Abort));

            return;
        }

        int ret = makeList(srcPath, m_dstPath, copyList);
        if(ret != static_cast<int>(Result::Success))
        {
            qDebug() << "makeList() : ret =" << ret;
            emitFinished(ret);

            return;
        }
    }

    for(QMap<QString, QString>::const_iterator itr = copyList.cbegin();itr != copyList.cend();itr++)
    {
        int ret = copyExec(itr.key(), itr.value());
        if(ret < 0)
        {
            qDebug() << "copyExec() : ret =" << ret;
            emitFinished(ret);

            return;
        }
    }

    qDebug() << "finish CopyWorker::process()";

    emitFinished(static_cast<int>(Result::Success));
}

int CopyWorker::makeList(const QString& srcPath, const QString& dstDirPath, QMap<QString, QString>& copyList)
{
    if(isAborted())
    {
        return static_cast<int>(Result::Abort);
    }

    QFileInfo srcFileInfo(srcPath);
    QDir      dstDir(dstDirPath);
    QString   dstPath = dstDir.absoluteFilePath(srcFileInfo.fileName());
    QFileInfo dstFileInfo(dstPath);

    copyList.insert(srcFileInfo.absoluteFilePath(), dstFileInfo.absoluteFilePath());

    qDebug() << srcFileInfo.absoluteFilePath() << ">>" << dstFileInfo.absoluteFilePath();

    if(srcFileInfo.isDir())
    {
        // 再帰処理でコピー元ディレクトリ内のエントリをリストに追加する
        QDir          srcDir(srcPath);
        QFileInfoList srcChildFileInfoList = srcDir.entryInfoList(QDir::AllEntries |
                                                                  QDir::AccessMask |
                                                                  QDir::AllDirs |
                                                                  QDir::NoDotAndDotDot,
                                                                  QDir::DirsFirst);

        for(auto srcChildFileInfo : srcChildFileInfoList)
        {
            int ret = makeList(srcChildFileInfo.absoluteFilePath(), dstFileInfo.absoluteFilePath(), copyList);
            if(ret < 0)
            {
                return ret;
            }
        }
    }

    return static_cast<int>(Result::Success);
}

int CopyWorker::copyExec(const QString& srcPath, const QString& dstPath)
{
    if(isAborted())
    {
        return static_cast<int>(Result::Abort);
    }

    QFileInfo srcFileInfo(srcPath);
    QFileInfo dstFileInfo(dstPath);

    qDebug() << srcFileInfo.absoluteFilePath() << ">>" << dstFileInfo.absoluteFilePath();

    if(srcFileInfo.isDir())
    {
        // ディレクトリの場合はコピー先にディレクトリを作成する
        if(!dstFileInfo.exists())
        {
            QDir dstDir(dstPath);

            if(!dstDir.mkdir(dstPath))
            {
                // ディレクトリ作成失敗
                return static_cast<int>(Result::ErrorMakeDir);
            }

            qDebug() << "succeed mkdir(" << dstPath << ")";
        }
    }
    else
    {
        // ファイル
        while(dstFileInfo.exists())
        {
            // コピー先にファイルが存在している場合は確認する
            OverwriteDialog dialog(m_methodType, dstFileInfo.fileName());
            if(dialog.exec() == QDialog::Rejected)
            {
                // 中断
                return static_cast<int>(Result::Abort);
            }

            m_methodType = dialog.getMethodType();
            if(m_methodType == OverwriteMethodType::Overwrite)
            {
                break;
            }
            else if(m_methodType == OverwriteMethodType::OverwriteIfNewer)
            {
                if(srcFileInfo.lastModified() <= dstFileInfo.lastModified())
                {
                    return static_cast<int>(Result::Skip);
                }
            }
            else if(m_methodType == OverwriteMethodType::Skip)
            {
                return static_cast<int>(Result::Skip);
            }
            else if(m_methodType == OverwriteMethodType::Rename)
            {
                dstFileInfo.setFile(dstFileInfo.absolutePath(), dialog.getRenameFileName());
            }
            else
            {
                // ここにくることはありえないはず
                return static_cast<int>(Result::ErrorFatal);
            }
        }

        if(!QFile::copy(srcFileInfo.absoluteFilePath(), dstFileInfo.absoluteFilePath()))
        {
            // コピー失敗
            return static_cast<int>(Result::ErrorCopyFile);
        }
    }

    return static_cast<int>(Result::Success);
}

}           // namespace Farman
