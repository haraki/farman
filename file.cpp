#include <QString>
#include <QDebug>
#include "file.h"
#include "copyworker.h"
#include "removeworker.h"
#include "overwritedialog.h"
#include "workingdialog.h"
#include "settings.h"
#include "mainwindow.h"
#include "win32.h"

namespace Farman
{

File::File(MainWindow* mainWindow)
    : QObject()
    , m_mainWindow(mainWindow)
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
            m_mainWindow,
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

    WorkingDialog dialog(copyWorker, Settings::getInstance()->getAutoDialogCloseCopy(), true, m_mainWindow);
    dialog.setSubProgressLabelFormat(tr("%1 / %2 Bytes"));
    int result = dialog.exec();
    Settings::getInstance()->setAutoDialogCloseCopy(dialog.getAutoClose());

    if(result == QDialog::Accepted)
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
            m_mainWindow,
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

    WorkingDialog dialog(copyWorker, Settings::getInstance()->getAutoDialogCloseMove(), true, m_mainWindow);
    dialog.setSubProgressLabelFormat(tr("%1 / %2 Bytes"));
    int result = dialog.exec();
    Settings::getInstance()->setAutoDialogCloseMove(dialog.getAutoClose());

    if(result == QDialog::Accepted)
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
            m_mainWindow,
            SLOT(onOutputConsole(const QString)));
    connect(worker,
            SIGNAL(finished(int)),
            this,
            SLOT(onRemoveFileFinished(int)));
    connect(worker,
            SIGNAL(error(QString)),
            this,
            SLOT(onRemoveFileError(QString)));

    WorkingDialog dialog(worker, Settings::getInstance()->getAutoDialogCloseRemove(), false, m_mainWindow);
    int result = dialog.exec();
    Settings::getInstance()->setAutoDialogCloseRemove(dialog.getAutoClose());

    if(result == QDialog::Accepted)
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

    if(dir.exists(dirName))
    {
        // 既に存在しているので何もしない
        emitOutputConsole(tr("is exists.\n"));
        return false;
    }

    if(!dir.mkdir(dirName))
    {
        // ディレクトリ作成失敗
        emitOutputConsole(tr("Failed to make a directory.\n"));
        return false;
    }

    emitOutputConsole(tr("Made a directory.\n"));

    return true;
}

bool File::createNewFile(const QString &path, const QString &fileName)
{
    QDir dir(path);
    QString absPath = dir.absoluteFilePath(fileName);

    emitOutputConsole(QString("%1 ... ").arg(absPath));

    if(dir.exists(fileName))
    {
        // 既に存在しているので何もしない
        emitOutputConsole(tr("is exists.\n"));
        return false;
    }

    QFile newFile(absPath);

    if(!newFile.open(QIODevice::WriteOnly))
    {
        // ファイル作成失敗
        emitOutputConsole(tr("Failed to create a new file.\n"));
        return false;
    }

    newFile.close();

    emitOutputConsole(tr("Created a new file.\n"));

    emitCreateNewFileFinished(absPath);

    return true;
}

bool File::renameFile(const QString& path, const QString& oldName, const QString& newName)
{
    QDir dir(path);
    QString oldAbsPath = dir.absoluteFilePath(oldName);

    emitOutputConsole(QString("%1 >> %2 ... ").arg(oldAbsPath).arg(newName));

    if(!dir.rename(oldName, newName))
    {
        // リネーム失敗
        emitOutputConsole(tr("Failed to rename.\n"));
        return false;
    }

    emitOutputConsole(tr("Renamed.\n"));

    return true;
}

bool File::changeFileAttributes(const QString& path,
#ifdef Q_OS_WIN
                                const WinFileAttrFlags& newFileAttrFlags,
#else
                                const QFile::Permissions& newPermissions,
#endif
                                const QDateTime& newCreated,
                                const QDateTime& newLastModified)
{
    QFile file(path);
    QFileInfo fileInfo(file);

    qDebug() << "created : " << fileInfo.created() << ", newCreated : " << newCreated;
    qDebug() << "lastModified : " << fileInfo.lastModified() << ", newLastModified : " << newLastModified;

#ifdef Q_OS_WIN
    bool changeFileAttr     = Win32::getFileAttrFlags(path) != newFileAttrFlags;
#else
    bool changePermission   = file.permissions() != newPermissions;
#endif
    bool changeCreated      = fileInfo.created() != newCreated;
    bool changeLastModified = fileInfo.lastModified() != newLastModified;

    if(changeCreated || changeLastModified
#ifdef Q_OS_WIN
       || changeFileAttr
#else
       || changePermission
#endif
      )
    {
        emitOutputConsole(QString("%1 ... ").arg(path));

        QStringList outputStrings;

#ifdef Q_OS_WIN
        if(changeFileAttr)
        {
            if(Win32::setFileAttrFlags(path, newFileAttrFlags))
            {
                outputStrings.append(tr("Changed file attributes"));
            }
            else
            {
                outputStrings.append(tr("Failed to change file attributes"));
                qDebug() << "Failed to change file attributes.";
            }
        }
#else
        if(changePermission)
        {
            if(file.setPermissions(newPermissions))
            {
                outputStrings.append(tr("Changed permissions"));
            }
            else
            {
                outputStrings.append(tr("Failed to change permissions"));
                qDebug() << "Failed to change permissions. " << file.errorString();
            }
        }
#endif
        if(changeCreated || changeLastModified)
        {
            if(file.open(QFile::ReadWrite))
            {
                if(changeCreated)
                {
                    if(file.setFileTime(newCreated, QFile::FileBirthTime))
                    {
                        outputStrings.append(tr("Changed created time"));
                    }
                    else
                    {
                        outputStrings.append(tr("Failed to changed created time"));
                        qDebug() << "Failed to change created time. " << file.errorString();
                    }
                }

                if(changeLastModified)
                {
                    if(file.setFileTime(newLastModified, QFile::FileModificationTime))
                    {
                        outputStrings.append(tr("Changed last modified time"));
                    }
                    else
                    {
                        outputStrings.append(tr("Failed to changed last modified time"));
                        qDebug() << "Failed to change last modified time. " << file.errorString();
                    }
                }

                file.close();
            }
            else
            {
                if(changeCreated)
                {
                    outputStrings.append(tr("Failed to changed created time"));
                }
                if(changeLastModified)
                {
                    outputStrings.append(tr("Failed to changed last modified time"));
                }

                qDebug() << "Failed to open file. " << file.errorString();
            }
        }

        emitOutputConsole(outputStrings.join(", ") + ".\n");
    }

    return true;
}

void File::emitCreateNewFileFinished(const QString& filePath)
{
    emit createNewFileFinished(filePath);
}

void File::emitOutputConsole(const QString& consoleString)
{
    emit outputConsole(consoleString);
}

void File::onCopyFile(const QStringList& srcPaths, const QString& dstPath)
{
    copyFile(srcPaths, dstPath);
}

void File::onMoveFile(const QStringList& srcPaths, const QString& dstPath)
{
    moveFile(srcPaths, dstPath);
}

void File::onRemoveFile(const QStringList& paths)
{
    removeFile(paths);
}

void File::onMakeDirectory(const QString& path, const QString& dirName)
{
    makeDirectory(path, dirName);
}

void File::onCreateNewFile(const QString& path, const QString& fileName)
{
    createNewFile(path, fileName);
}

void File::onRenameFile(const QString& path, const QString& oldName, const QString& newName)
{
    renameFile(path, oldName, newName);
}

void File::onChangeFileAttributes(const QString& path,
#ifdef Q_OS_WIN
                                  const WinFileAttrFlags& newFileAttrFlags,
#else
                                  const QFile::Permissions& newPermissions,
#endif
                                  const QDateTime& newCreated,
                                  const QDateTime& newLastModified)
{
    changeFileAttributes(path,
#ifdef Q_OS_WIN
                         newFileAttrFlags,
#else
                         newPermissions,
#endif
                         newCreated,
                         newLastModified);
}

void File::onCopyFileFinished(int result)
{
    qDebug() << "onCopyFileFinished : result : " << result;
}

void File::onCopyFileError(const QString& err)
{
    qDebug() << "onCopyFileError : err : " << err;
}

void File::onMoveFileFinished(int result)
{
    qDebug() << "onMoveFileFinished : result : " << result;
}

void File::onMoveFileError(const QString& err)
{
    qDebug() << "onMoveFileError : err : " << err;
}

void File::onRemoveFileFinished(int result)
{
    qDebug() << "onRemoveFileFinished : result : " << result;
}

void File::onRemoveFileError(const QString& err)
{
    qDebug() << "onRemoveFileError : err : " << err;
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

}           // namespace Farman
