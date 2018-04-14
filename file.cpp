#include <QDebug>
#include "file.h"
#include "copyworker.h"
#include "removeworker.h"
#include "overwritedialog.h"
#include "workingdialog.h"

namespace Farman
{

File* File::s_instance = Q_NULLPTR;

void File::create(QObject *parent/* = Q_NULLPTR*/)
{
    Q_ASSERT(s_instance == Q_NULLPTR);
    s_instance = new File(parent);
    Q_ASSERT(s_instance != Q_NULLPTR);
}

File* File::getInstance()
{
    return s_instance;
}

File::File(QObject *parent/* = Q_NULLPTR*/) :
    QObject(parent)
{
}

File::~File()
{
}

bool File::copyFile(const QStringList& srcPaths, const QString& dstDirPath)
{
    CopyWorker* copyWorker = new CopyWorker(srcPaths, dstDirPath, false);

    connect(copyWorker,
            SIGNAL(outputConsole(const QString)),
            this,
            SLOT(onOutputConsole(const QString)));
    connect(copyWorker,
            SIGNAL(finished(int)),
            this,
            SLOT(onCopyFileFinished(int)));
    connect(copyWorker,
            SIGNAL(error(QString)),
            this,
            SLOT(onCopyFileError(QString)));
    connect(copyWorker,
            SIGNAL(confirmOverwrite(QString,QString,int)),
            this,
            SLOT(onConfirmOverwrite(QString,QString,int)));

    WorkingDialog dialog(copyWorker, qobject_cast<QWidget*>(parent()));
    if(dialog.exec() == QDialog::Accepted)
    {
        return true;
    }

    return false;
}

bool File::moveFile(const QStringList& srcPaths, const QString& dstPath)
{
    CopyWorker* copyWorker = new CopyWorker(srcPaths, dstPath, true);

    connect(copyWorker,
            SIGNAL(outputConsole(const QString)),
            this,
            SLOT(onOutputConsole(const QString)));
    connect(copyWorker,
            SIGNAL(finished(int)),
            this,
            SLOT(onMoveFileFinished(int)));
    connect(copyWorker,
            SIGNAL(error(QString)),
            this,
            SLOT(onMoveFileError(QString)));
    connect(copyWorker,
            SIGNAL(confirmOverwrite(QString,QString,int)),
            this,
            SLOT(onConfirmOverwrite(QString,QString,int)));

    WorkingDialog dialog(copyWorker, qobject_cast<QWidget*>(parent()));
    if(dialog.exec() == QDialog::Accepted)
    {
        return true;
    }

    return false;
}

bool File::removeFile(const QStringList& paths)
{
    Worker* worker = new RemoveWorker(paths);

    connect(worker,
            SIGNAL(outputConsole(const QString)),
            this,
            SLOT(onOutputConsole(const QString)));
    connect(worker,
            SIGNAL(finished(int)),
            this,
            SLOT(onRemoveFileFinished(int)));
    connect(worker,
            SIGNAL(error(QString)),
            this,
            SLOT(onRemoveFileError(QString)));

    WorkingDialog dialog(worker, qobject_cast<QWidget*>(parent()));
    if(dialog.exec() == QDialog::Accepted)
    {
        return true;
    }

    return false;
}

bool File::makeDirectory(const QString& path, const QString& dirName)
{
    QDir dir(path);
    QString absPath = dir.absoluteFilePath(dirName);

    emitOutputConsole(QString("%1 ... ").arg(absPath));
    qDebug() << absPath;

    if(dir.exists(dirName))
    {
        // 既に存在しているので何もしない
        emitOutputConsole(tr("is exists.\n"));
        return false;
    }

    if(!dir.mkdir(dirName))
    {
        // ディレクトリ作成失敗
        emitOutputConsole(tr("Failed make directory.\n"));
        return false;
    }

    emitOutputConsole(tr("Made directory.\n"));

    return true;
}

bool File::renameFile(const QString& path, const QString& oldName, const QString& newName)
{
    QDir dir(path);
    if(!dir.rename(oldName, newName))
    {
        return false;
    }

    emitOutputConsole(QString("%1 >> %2 ... ").arg(oldName).arg(newName));
    emitOutputConsole(tr("Renamed.\n"));

    return true;
}

bool File::changeFileAttributes(const QString& path,
                                             const QFile::Permissions& newPermissions,
                                             const QDateTime& newCreated,
                                             const QDateTime& newLastModified)
{
    QFile file(path);

    if(file.permissions() != newPermissions)
    {
        if(!file.setPermissions(newPermissions))
        {
            qDebug() << "Failed change permissions. " << file.errorString();
        }
    }

    QFileInfo fileInfo(file);

    bool changeCreated      = fileInfo.created() != newCreated;
    bool changeLastModified = fileInfo.lastModified() != newLastModified;

    if(changeCreated || changeLastModified)
    {
        if(file.open(QFile::ReadWrite))
        {
            if(changeCreated && !file.setFileTime(newCreated, QFile::FileBirthTime))
            {
                qDebug() << "Failed change Created time. " << file.errorString();
            }

            if(changeLastModified && !file.setFileTime(newLastModified, QFile::FileModificationTime))
            {
                qDebug() << "Failed change Last modified time. " << file.errorString();
            }

            file.close();
        }
        else
        {
            qDebug() << "Failed open file. " << file.errorString();
        }
    }

    return true;
}

void File::emitOutputConsole(const QString& consoleString)
{
    emit outputConsole(consoleString);
}

void File::onCopyFileFinished(int result)
{
    qDebug() << "DoubleFolderPanel::onCopyFileFinished : result : " << result;
}

void File::onCopyFileError(const QString& err)
{
    qDebug() << "DoubleFolderPanel::onCopyFileError : err : " << err;
}

void File::onMoveFileFinished(int result)
{
    qDebug() << "DoubleFolderPanel::onMoveFileFinished : result : " << result;
}

void File::onMoveFileError(const QString& err)
{
    qDebug() << "DoubleFolderPanel::onMoveFileError : err : " << err;
}

void File::onRemoveFileFinished(int result)
{
    qDebug() << "DoubleFolderPanel::onRemoveFileFinished : result : " << result;
}

void File::onRemoveFileError(const QString& err)
{
    qDebug() << "DoubleFolderPanel::onRemoveFileError : err : " << err;
}

void File::onConfirmOverwrite(const QString& srcFilePath, const QString& dstFilePath, int methodType)
{
    CopyWorker* copyWorker = qobject_cast<CopyWorker*>(sender());
    if(copyWorker != Q_NULLPTR)
    {
        OverwriteDialog dialog(srcFilePath, dstFilePath, static_cast<OverwriteMethodType>(methodType), qobject_cast<QWidget*>(parent()));
        if(dialog.exec() == QDialog::Accepted)
        {
            copyWorker->finishConfirmOverwrite(dialog.getMethodType(), dialog.getKeepSetting(), dialog.getRenameFileName());
        }
        else
        {
            copyWorker->cancelConfirmOverwrite();
        }
    }
}

void File::onOutputConsole(const QString& consoleString)
{
    emitOutputConsole(consoleString);
}

}           // namespace Farman
