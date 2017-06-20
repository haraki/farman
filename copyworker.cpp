﻿#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include "copyworker.h"
#include "overwritedialog.h"

namespace Farman
{

CopyWorker::CopyWorker(const QStringList& srcPaths, const QString& dstPath, bool moveMode, QObject *parent)
    : Worker(parent)
    , m_srcPaths(srcPaths)
    , m_dstPath(dstPath)
    , m_moveMode(moveMode)
    , m_methodType(OverwriteMethodType::Default)
    , m_methodTypeKeep(false)
{

}

CopyWorker::~CopyWorker()
{

}

void CopyWorker::process()
{
    qDebug() << "start CopyWorker::process()";

    QMap<QString, QString> copyList;
    QList<QString> removeDirList;

    // コピーするファイル・ディレクトリのリストを作成
    for(auto srcPath : m_srcPaths)
    {
        if(isAborted())
        {
            emitFinished(static_cast<int>(Result::Abort));

            return;
        }

        int ret = makeList(srcPath, m_dstPath, copyList, removeDirList);
        if(ret != static_cast<int>(Result::Success))
        {
            qDebug() << "makeList() : ret =" << ret;
            emitFinished(ret);

            return;
        }
    }

    emitMinMax(0, copyList.size());
    emitProgress(0);

    int progress = 0;
    for(QMap<QString, QString>::const_iterator itr = copyList.cbegin();itr != copyList.cend();itr++)
    {
        int ret = copyExec(itr.key(), itr.value());
        if(ret < 0)
        {
            qDebug() << "copyExec() : ret =" << QString("%1").arg(ret, 0, 16);
            emitFinished(ret);

            return;
        }

        progress++;
        emitProgress(progress);
    }

    if(m_moveMode)
    {
        // ディレクトリ削除
        for(auto dirPath : removeDirList)
        {
            qDebug() << "remove dir :" << dirPath;

            if(!QDir().rmdir(dirPath))
            {
                // 移動元のディレクトリ削除失敗
                qDebug() << "remove dir error :" << dirPath;
                emitFinished(static_cast<int>(Result::ErrorRemoveDir));

                return;
            }
        }
    }

    qDebug() << "finish CopyWorker::process()";

    emitFinished(static_cast<int>(Result::Success));
}

int CopyWorker::makeList(const QString& srcPath, const QString& dstDirPath, QMap<QString, QString>& copyList, QList<QString>& removeDirList)
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
            int ret = makeList(srcChildFileInfo.absoluteFilePath(), dstFileInfo.absoluteFilePath(), copyList, removeDirList);
            if(ret < 0)
            {
                return ret;
            }
        }

        if(m_moveMode)
        {
            // 最後にディレクトリをまとめて削除するためのリストを作成
            removeDirList.push_back(srcFileInfo.absoluteFilePath());
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
            // コピー先にファイルが存在している
            QString renameFileName = dstFileInfo.fileName();
            if(!m_methodTypeKeep || m_methodType == OverwriteMethodType::Rename)
            {
                OverwriteDialog dialog(srcFileInfo.absoluteFilePath(), dstFileInfo.absoluteFilePath(), m_methodType, dstFileInfo.fileName());
                if(dialog.exec() == QDialog::Rejected)
                {
                    // 中断
                    return static_cast<int>(Result::Abort);
                }

                m_methodType = dialog.getMethodType();
                m_methodTypeKeep = dialog.getKeepSetting();

                renameFileName = dialog.getRenameFileName();
            }

            if(m_methodType == OverwriteMethodType::Overwrite)
            {
                if(!QFile::remove(dstFileInfo.absoluteFilePath()))
                {
                    return static_cast<int>(Result::ErrorRemoveFile);
                }

                break;
            }
            else if(m_methodType == OverwriteMethodType::OverwriteIfNewer)
            {
                if(srcFileInfo.lastModified() <= dstFileInfo.lastModified())
                {
                    return static_cast<int>(Result::Skip);
                }

                if(!QFile::remove(dstFileInfo.absoluteFilePath()))
                {
                    return static_cast<int>(Result::ErrorRemoveFile);
                }

                break;
            }
            else if(m_methodType == OverwriteMethodType::Skip)
            {
                return static_cast<int>(Result::Skip);
            }
            else if(m_methodType == OverwriteMethodType::Rename)
            {
                dstFileInfo.setFile(dstFileInfo.absolutePath(), renameFileName);
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

        if(m_moveMode)
        {
            qDebug() << "remove file : " << srcFileInfo.absoluteFilePath();

            if(!QFile::remove(srcFileInfo.absoluteFilePath()))
            {
                // 移動元のファイル削除失敗
                return static_cast<int>(Result::ErrorRemoveFile);
            }
        }
    }

    return static_cast<int>(Result::Success);
}

}           // namespace Farman
