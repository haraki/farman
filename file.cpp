#include <QDebug>
#include "file.h"
#include "copyworker.h"
#include "removeworker.h"
#include "overwritedialog.h"
#include "workingdialog.h"
#include "settings.h"
#include "mainwindow.h"

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
    connect(this,
            SIGNAL(outputConsole(const QString)),
            MainWindow::getInstance(),
            SLOT(onOutputConsole(const QString)));
}

File::~File()
{
}

bool File::copyFile(const QStringList& srcPaths, const QString& dstDirPath)
{
    CopyWorker* copyWorker = new CopyWorker(srcPaths, dstDirPath, false);

    connect(copyWorker,
            SIGNAL(outputConsole(const QString)),
            MainWindow::getInstance(),
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

    WorkingDialog dialog(copyWorker, Settings::getInstance()->getAutoDialogCloseCopy(), qobject_cast<QWidget*>(parent()));
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
            MainWindow::getInstance(),
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

    WorkingDialog dialog(copyWorker, Settings::getInstance()->getAutoDialogCloseMove(), qobject_cast<QWidget*>(parent()));
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
            MainWindow::getInstance(),
            SLOT(onOutputConsole(const QString)));
    connect(worker,
            SIGNAL(finished(int)),
            this,
            SLOT(onRemoveFileFinished(int)));
    connect(worker,
            SIGNAL(error(QString)),
            this,
            SLOT(onRemoveFileError(QString)));

    WorkingDialog dialog(worker, Settings::getInstance()->getAutoDialogCloseRemove(), qobject_cast<QWidget*>(parent()));
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
        emitOutputConsole(tr("Failed to make directory.\n"));
        return false;
    }

    emitOutputConsole(tr("Made directory.\n"));

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
                                const QFile::Permissions& newPermissions,
                                const QDateTime& newCreated,
                                const QDateTime& newLastModified)
{
    QFile file(path);
    QFileInfo fileInfo(file);

    bool changePermission   = file.permissions() != newPermissions;
    bool changeCreated      = fileInfo.created() != newCreated;
    bool changeLastModified = fileInfo.lastModified() != newLastModified;

    if(changePermission || changeCreated || changeLastModified)
    {
        emitOutputConsole(QString("%1 ... ").arg(path));

        QStringList outputStrings;

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

}           // namespace Farman
