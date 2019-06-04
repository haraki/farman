#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QTemporaryFile>
#include <QDateTime>
#include <QMutex>
#include <QThread>
#include "copyworker.h"
#include "workerresult.h"
#include "settings.h"
#include "default_settings.h"

namespace Farman
{

CopyWorker::CopyWorker(const QStringList& srcPaths, const QString& dstPath, bool moveMode, QObject *parent)
    : Worker(parent)
    , m_srcPaths(srcPaths)
    , m_dstPath(dstPath)
    , m_moveMode(moveMode)
    , m_methodType(DEFAULT_OVERWRITE_METHOD_TYPE)
    , m_methodTypeKeep(false)
    , m_renameFileName("")
    , m_copyUnitSize(DEFAULT_COPY_UNIT_SIZE)
{
    m_copyUnitSize = Settings::getInstance()->getCopyUnitSize();
}

CopyWorker::~CopyWorker()
{

}

void CopyWorker::run()
{
    qDebug() << "start CopyWorker::run()";

    m_timer.start();

    QString prepareStr = (m_moveMode) ? tr("Preparing move...") : tr("Preparing copy...");
    QString prepareAbortStr = (m_moveMode) ? tr("Preparing move...aborted.") : tr("Preparing copy...aborted.");
    QString prepareFailedStr = (m_moveMode) ? tr("Preparing move...failed.") : tr("Preparing copy...failed.");

    emitProcess(prepareStr);

    QMap<QString, QString> copyList;
    QList<QString> removeDirList;

    // コピーするファイル・ディレクトリのリストを作成
    for(auto srcPath : m_srcPaths)
    {
        if(isAborted())
        {
            //emitOutputConsole(tr("Aborted.\n"));          // makeList() 内部でコンソール出力しているので、ここではコンソール出力しない
            emitProcess(prepareAbortStr);
            emitFinished(static_cast<int>(WorkerResult::Abort));

            return;
        }

        int ret = makeList(srcPath, m_dstPath, copyList, removeDirList);
        if(isError(ret))
        {
            qDebug() << "makeList() : ret =" << QString("%1").arg(ret, 0, 16);
            emitProcess(prepareFailedStr);
            emitFinished(ret);

            return;
        }
    }

    emitStart(0, copyList.size());

    QString preStr = (m_moveMode) ? tr("%1 file(s) move...") : tr("%1 file(s) copy...");
    QString postStr = (m_moveMode) ? tr("%1 file(s) move...done.") : tr("%1 file(s) copy...done.");
    QString abortStr = (m_moveMode) ? tr("%1 file(s) move...aborted.") : tr("%1 file(s) copy...aborted.");
    QString failedStr = (m_moveMode) ? tr("%1 file(s) move...failed.") : tr("%1 file(s) copy...failed.");

    int progress = 0;
    for(QMap<QString, QString>::const_iterator itr = copyList.cbegin();itr != copyList.cend();itr++)
    {
        thread()->msleep(1);                    // sleep を入れないと Abort できない場合がある

        emitProcess(QString(preStr).arg(progress + 1));

        int ret = copyExec(itr.key(), itr.value());
        if(isAborted())
        {
            //emitOutputConsole(tr("Aborted.\n"));          // copyExec() 内部でコンソール出力しているので、ここではコンソール出力しない
            emitProcess(QString(abortStr).arg(progress + 1));
            emitFinished(static_cast<int>(WorkerResult::Abort));

            return;
        }

        if(isError(ret))
        {
            qDebug() << "copyExec() : ret =" << QString("%1").arg(ret, 0, 16);
            emitProcess(QString(failedStr).arg(progress + 1));
            emitFinished(ret);

            return;
        }

        emitProcess(QString(postStr).arg(progress + 1));

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
                emitProcess(QString(tr("remove %1 folder failed.")).arg(dirPath));
                emitFinished(static_cast<int>(WorkerResult::ErrorRemoveDir));

                return;
            }
        }
    }

    emitOutputConsole(QString("Total time : %L1 ms.\n").arg(m_timer.elapsed()));

    qDebug() << "finish CopyWorker::run()";

    emitFinished(static_cast<int>(WorkerResult::Success));
}

int CopyWorker::makeList(const QString& srcPath, const QString& dstDirPath, QMap<QString, QString>& copyList, QList<QString>& removeDirList)
{
    if(isAborted())
    {
        emitOutputConsole(tr("Aborted.\n"));
        return static_cast<int>(WorkerResult::Abort);
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
            if(ret == static_cast<int>(WorkerResult::Abort) || isError(ret))
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

    return static_cast<int>(WorkerResult::Success);
}

int CopyWorker::copyExec(const QString& srcPath, const QString& dstPath)
{
    if(isAborted())
    {
        emitOutputConsole(tr("Aborted.\n"));
        return static_cast<int>(WorkerResult::Abort);
    }

    QFileInfo srcFileInfo(srcPath);
    QFileInfo dstFileInfo(dstPath);

    emitOutputConsole(QString("%1 >> %2 ... ").arg(srcFileInfo.absoluteFilePath()).arg(dstFileInfo.absoluteFilePath()));
    qDebug() << srcFileInfo.absoluteFilePath() << ">>" << dstFileInfo.absoluteFilePath();

    if(srcFileInfo.isDir())
    {
        // ディレクトリの場合はコピー先にディレクトリを作成する
        if(dstFileInfo.exists())
        {
            emitOutputConsole(tr("is exists.\n"));
        }
        else
        {
            QDir dstDir(dstPath);

            if(!dstDir.mkdir(dstPath))
            {
                // ディレクトリ作成失敗
                emitOutputConsole(tr("Failed make directory.\n"));
                return static_cast<int>(WorkerResult::ErrorMakeDir);
            }

            emitOutputConsole(tr("Made directory.\n"));

            qDebug() << "succeed mkdir(" << dstPath << ")";
        }
    }
    else
    {
        // ファイル
        while(dstFileInfo.exists())
        {
            // コピー先にファイルが存在している
            m_renameFileName = "";
            if(!m_methodTypeKeep || m_methodType == OverwriteMethodType::Rename)
            {
                showConfirmOverwrite(srcFileInfo.absoluteFilePath(), dstFileInfo.absoluteFilePath(), m_methodType);
                if(isAborted())
                {
                    // 中断
                    emitOutputConsole(tr("Aborted.\n"));
                    return static_cast<int>(WorkerResult::Abort);
                }
            }

            if(m_methodType == OverwriteMethodType::Overwrite)
            {
                if(!QFile::remove(dstFileInfo.absoluteFilePath()))
                {
                    emitOutputConsole((m_moveMode) ? tr("Failed move.\n") : tr("Failed copy.\n"));
                    return static_cast<int>(WorkerResult::ErrorRemoveFile);
                }

                break;
            }
            else if(m_methodType == OverwriteMethodType::OverwriteIfNewer)
            {
                if(srcFileInfo.lastModified() <= dstFileInfo.lastModified())
                {
                    emitOutputConsole(tr("Skipped.\n"));
                    return static_cast<int>(WorkerResult::Skip);
                }

                if(!QFile::remove(dstFileInfo.absoluteFilePath()))
                {
                    emitOutputConsole((m_moveMode) ? tr("Failed move.\n") : tr("Failed copy.\n"));
                    return static_cast<int>(WorkerResult::ErrorRemoveFile);
                }

                break;
            }
            else if(m_methodType == OverwriteMethodType::Skip)
            {
                emitOutputConsole(tr("Skipped.\n"));
                return static_cast<int>(WorkerResult::Skip);
            }
            else if(m_methodType == OverwriteMethodType::Rename)
            {
                dstFileInfo.setFile(dstFileInfo.absolutePath(), m_renameFileName);
            }
            else
            {
                // ここにくることはありえないはず
                emitOutputConsole(tr("Fatal error.\n"));
                return static_cast<int>(WorkerResult::ErrorFatal);
            }
        }

        int result = copy(srcFileInfo.absoluteFilePath(), dstFileInfo.absoluteFilePath());
        if(result == static_cast<int>(WorkerResult::Abort))
        {
            emitOutputConsole(tr("Aborted.\n"));
            return static_cast<int>(WorkerResult::Abort);
        }
        else if(isError(result))
        {
            // コピー失敗
            emitOutputConsole((m_moveMode) ? tr("Failed move.\n") : tr("Failed copy.\n"));
            return static_cast<int>(WorkerResult::ErrorCopyFile);
        }

        if(m_moveMode)
        {
            qDebug() << "remove file : " << srcFileInfo.absoluteFilePath();

            if(!QFile::remove(srcFileInfo.absoluteFilePath()))
            {
                // 移動元のファイル削除失敗
                emitOutputConsole(tr("Failed move.\n"));
                return static_cast<int>(WorkerResult::ErrorRemoveFile);
            }
        }

        emitOutputConsole((m_moveMode) ? tr("Moved\n") : tr("Copied\n"));
    }

    return static_cast<int>(WorkerResult::Success);
}

void CopyWorker::showConfirmOverwrite(const QString& srcFilePath, const QString& dstFilePath, OverwriteMethodType methodType)
{
    emitConfirmOverwrite(srcFilePath, dstFilePath, methodType);

    m_timer.invalidate();

    QMutex mutex;
    {
        QMutexLocker locker(&mutex);

        m_confirmWait.wait(&mutex);
    }

    m_timer.restart();
}

void CopyWorker::emitConfirmOverwrite(const QString& srcFilePath, const QString& dstFilePath, OverwriteMethodType methodType)
{
    emit confirmOverwrite(srcFilePath, dstFilePath, static_cast<int>(methodType));
}

void CopyWorker::finishConfirmOverwrite(OverwriteMethodType methodType, bool methodTypeKeep, const QString& renameFileName)
{
    m_methodType = methodType;
    m_methodTypeKeep = methodTypeKeep;
    m_renameFileName = renameFileName;

    m_confirmWait.wakeAll();
}

void CopyWorker::cancelConfirmOverwrite()
{
    abort();

    m_confirmWait.wakeAll();
}

void CopyWorker::emitStartSub(int min, int max)
{
    emit startSub(min, max);
}

void CopyWorker::emitProgressSub(int value)
{
    emit progressSub(value);
}

int CopyWorker::copy(const QString& srcPath, const QString& dstPath)
{
    qDebug() << "CopyWorker::copy() : " << srcPath << " >> " << dstPath;

    QFile srcFile(srcPath);
    if(!srcFile.open(QIODevice::ReadOnly))
    {
        return static_cast<int>(WorkerResult::ErrorCopyFile);
    }

    QTemporaryFile dstFile(QString(QLatin1String("%1/temp.XXXXXX")).arg(QFileInfo(dstPath).path()));

    if(!dstFile.open())
    {
        srcFile.close();
        return static_cast<int>(WorkerResult::ErrorCopyFile);
    }

    QByteArray buffer;
    WorkerResult result = WorkerResult::Success;
    qint64 remineSize = srcFile.size();
    qint64 totalSize = 0;

    emitStartSub(0, static_cast<int>(remineSize / 1024));

    while(!srcFile.atEnd())
    {
        thread()->msleep(1);                    // sleep を入れないと Abort できない場合がある

        if(isAborted())
        {
            result = WorkerResult::Abort;
            break;
        }

        qint64 readSize = (remineSize > m_copyUnitSize) ? m_copyUnitSize : remineSize;

        buffer = srcFile.read(readSize);
        if(buffer.size() < readSize)
        {
            result = WorkerResult::ErrorCopyFile;
            break;
        }

        qint64 wroteSize = dstFile.write(buffer);
        if(wroteSize < readSize)
        {
            result = WorkerResult::ErrorCopyFile;
            break;
        }

        remineSize -= readSize;
        totalSize += readSize;

        emitProgressSub(static_cast<int>(totalSize / 1024));
    }

    if(result == WorkerResult::Success)
    {
        dstFile.rename(dstPath);
        dstFile.setAutoRemove(false);
        if(!dstFile.setPermissions(srcFile.permissions()))
        {
            qDebug() << "set permissons error!! : " << dstPath;
            result = WorkerResult::ErrorCopyFile;
        }
    }

    srcFile.close();
    dstFile.close();

    return static_cast<int>(result);
}

}           // namespace Farman
