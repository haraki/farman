#include <qdir.h>
#include <QKeyEvent>
#include <QDebug>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QDesktopServices>
#include "doublefolderpanel.h"
#include "ui_doublefolderpanel.h"
#include "folderform.h"
#include "folderview.h"
#include "copyworker.h"
#include "removeworker.h"
#include "sortdialog.h"
#include "filterdialog.h"
#include "overwritedialog.h"
#include "workingdialog.h"
#include "renamedialog.h"
#include "mainwindow.h"
#include "settings.h"

namespace Farman
{

DoubleFolderPanel::DoubleFolderPanel(QWidget* parent/* = Q_NULLPTR*/)
    : QWidget(parent)
    , ui(new Ui::DoubleFolderPanel)
    , m_viewMode(ViewMode::Default)
{
    ui->setupUi(this);

    ViewMode viewMode = Settings::getInstance()->getViewMode();

    QString l_path = QDir::homePath();
    FolderAtStartup l_folderAtStartup = Settings::getInstance()->getLeftFolderAtStartup();
    if(l_folderAtStartup == FolderAtStartup::LastTime || l_folderAtStartup == FolderAtStartup::Fixed)
    {
        l_path = Settings::getInstance()->getLeftFolderPath();
    }
    if(!QDir(l_path).exists())
    {
        l_path = QDir::currentPath();
    }

    QString r_path = QDir::homePath();
    FolderAtStartup r_folderAtStartup = Settings::getInstance()->getRightFolderAtStartup();
    if(r_folderAtStartup == FolderAtStartup::LastTime || r_folderAtStartup == FolderAtStartup::Fixed)
    {
        r_path = Settings::getInstance()->getRightFolderPath();
    }
    if(!QDir(r_path).exists())
    {
        r_path = QDir::currentPath();
    }

    QDir::SortFlags l_sortFlags = Settings::getInstance()->getLeftSortSettings();
    QDir::SortFlags r_sortFlags = Settings::getInstance()->getRightSortSettings();
    QDir::Filters l_filterFlags = Settings::getInstance()->getLeftFilterSettings();
    QDir::Filters r_filterFlags = Settings::getInstance()->getRightFilterSettings();

    connect(this,
            SIGNAL(statusChanged(const QString)),
            MainWindow::getInstance(),
            SLOT(onStatusChanged(const QString)));

    connect(this,
            SIGNAL(outputConsole(const QString)),
            MainWindow::getInstance(),
            SLOT(onOutputConsole(const QString)));

    QVBoxLayout* l_vLayout = new QVBoxLayout();
    l_vLayout->setSpacing(6);
    l_vLayout->setObjectName(QStringLiteral("l_vLayout"));
    l_vLayout->setContentsMargins(0, 0, 0, 0);

    FolderForm* l_folderForm = new FolderForm(l_filterFlags, l_sortFlags, this);
    l_folderForm->setObjectName(QStringLiteral("l_folderForm"));
    l_folderForm->setPath(l_path);

    l_vLayout->addWidget(l_folderForm);

    ui->leftPanel->setLayout(l_vLayout);

    FolderView* l_folderView = l_folderForm->findChild<FolderView*>("folderView");
    if(l_folderView != Q_NULLPTR)
    {
        l_folderView->installEventFilter(this);
    }

    QVBoxLayout* r_vLayout = new QVBoxLayout();
    r_vLayout->setSpacing(6);
    r_vLayout->setObjectName(QStringLiteral("r_vLayout"));
    r_vLayout->setContentsMargins(0, 0, 0, 0);

    FolderForm* r_folderForm = new FolderForm(r_filterFlags, r_sortFlags, this);
    r_folderForm->setObjectName(QStringLiteral("r_folderForm"));
    r_folderForm->setPath(r_path);

    r_vLayout->addWidget(r_folderForm);

    ui->rightPanel->setLayout(r_vLayout);

    FolderView* r_folderView = r_folderForm->findChild<FolderView*>("folderView");
    if(r_folderView != Q_NULLPTR)
    {
        r_folderView->installEventFilter(this);
    }

    connect(l_folderForm,
            SIGNAL(currentChanged(QFileInfo,QFileInfo)),
            this,
            SLOT(onLeftCurrentChanged(QFileInfo,QFileInfo)));
    connect(r_folderForm,
            SIGNAL(currentChanged(QFileInfo,QFileInfo)),
            this,
            SLOT(onRightCurrentChanged(QFileInfo,QFileInfo)));
    connect(l_folderForm,
            SIGNAL(focusChanged(bool)),
            this,
            SLOT(onLeftFocusChanged(bool)));
    connect(r_folderForm,
            SIGNAL(focusChanged(bool)),
            this,
            SLOT(onRightFocusChanged(bool)));

    setActiveFolderForm("l_folderForm");

    onSetViewMode(viewMode);
}

DoubleFolderPanel::~DoubleFolderPanel()
{
    delete ui;
}

void DoubleFolderPanel::closeEvent(QCloseEvent* e)
{
    Q_UNUSED(e);

    qDebug() << "DoubleFolderPanel::closeEvent()";

    FolderAtStartup leftfolderAtStartup = Settings::getInstance()->getLeftFolderAtStartup();
    if(leftfolderAtStartup == FolderAtStartup::LastTime)
    {
        FolderForm* l_folderForm = findChild<FolderForm*>("l_folderForm");
        if(l_folderForm != Q_NULLPTR)
        {
            Settings::getInstance()->setLeftFolderPath(l_folderForm->getCurrentDirPath());
        }
    }

    FolderAtStartup rightFolderAtStartup = Settings::getInstance()->getRightFolderAtStartup();
    if(rightFolderAtStartup == FolderAtStartup::LastTime)
    {
        FolderForm* r_folderForm = findChild<FolderForm*>("r_folderForm");
        if(r_folderForm != Q_NULLPTR)
        {
            Settings::getInstance()->setRightFolderPath(r_folderForm->getCurrentDirPath());
        }
    }
}

bool DoubleFolderPanel::eventFilter(QObject *watched, QEvent *e)
{
    Q_UNUSED(watched);

    bool ret = false;

    switch (e->type()) {
    case QEvent::KeyPress:
    {
        Qt::Key key = static_cast<Qt::Key>(dynamic_cast<QKeyEvent*>(e)->key());

        qDebug() << "DoubleFolderPanel::eventFilter : " << key;

        FolderForm* activeForm = getActiveFolderForm();
        if(activeForm == Q_NULLPTR)
        {
            break;
        }

        switch(key)
        {
        case Qt::Key_Left:
            if(m_viewMode == ViewMode::Single || activeForm->objectName() == "l_folderForm")
            {
                activeForm->onGoToParent();
            }
            else
            {
                setActiveFolderForm("l_folderForm");
            }

            ret = true;

            break;

        case Qt::Key_Right:
            if(m_viewMode == ViewMode::Double)
            {
                if(activeForm->objectName() == "r_folderForm")
                {
                    activeForm->onGoToParent();
                }
                else
                {
                    setActiveFolderForm("r_folderForm");
                }
            }

            ret = true;

            break;

        default:
            break;
        }

        break;
    }
    default:
        break;
    }

    return ret;
}

void DoubleFolderPanel::onOpenInApp()
{
    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR)
    {
        const QString path = activeForm->getCurrentFileInfo().absoluteFilePath();
        if(!QDesktopServices::openUrl(QUrl("file:///" + path)))
        {
            qDebug() << "open url error:" << path;
        }
    }
}

void DoubleFolderPanel::onSetViewMode(ViewMode viewMode)
{
    m_viewMode = viewMode;
    Settings::getInstance()->setViewMode(viewMode);

    switch(viewMode)
    {
    case ViewMode::Single:
        if(getActiveFolderForm()->objectName() == "l_folderForm")
        {
            ui->leftPanel->setVisible(true);
            ui->rightPanel->setVisible(false);
        }
        else
        {
            ui->leftPanel->setVisible(false);
            ui->rightPanel->setVisible(true);
        }

        break;

    case ViewMode::Double:
        ui->leftPanel->setVisible(true);
        ui->rightPanel->setVisible(true);

        break;

    default:
        break;
    }
}

void DoubleFolderPanel::onChangeSortSettings()
{
    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR)
    {
        QDir::SortFlags sortFlags = activeForm->getSortFlags();

        SortDialog dialog(sortFlags, this);
        if(dialog.exec())
        {
            sortFlags = dialog.getSortFlags();
            activeForm->setSortFlags(sortFlags);
            if(activeForm->objectName() == "l_folderForm")
            {
                Settings::getInstance()->setLeftSortSettings(sortFlags);
            }
            else
            {
                Settings::getInstance()->setRightSortSettings(sortFlags);
            }
        }
    }
}

void DoubleFolderPanel::onChangeFilterSettings()
{
    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR)
    {
        QDir::Filters filterFlags = activeForm->getFilterFlags();

        FilterDialog dialog(filterFlags, this);
        if(dialog.exec())
        {
            filterFlags = dialog.getFilterFlags();
            activeForm->setFilterFlags(filterFlags);
            if(activeForm->objectName() == "l_folderForm")
            {
                Settings::getInstance()->setLeftFilterSettings(filterFlags);
            }
            else
            {
                Settings::getInstance()->setRightFilterSettings(filterFlags);
            }
        }
    }
}

void DoubleFolderPanel::onCopy()
{
    qDebug() << "DoubleFolderPanel::onCopy()";

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR)
    {
        QList<QFileInfo> selectedFileInfoList = activeForm->getSelectedFileInfoList();
        if(selectedFileInfoList.size() > 0)
        {
            FolderForm* inactiveForm = getInactiveFolderForm();
            if(inactiveForm != Q_NULLPTR)
            {
                QStringList srcPaths;

                for(QFileInfo fileInfo : selectedFileInfoList)
                {
                    srcPaths.push_back(fileInfo.absoluteFilePath());

                    qDebug() << fileInfo.absoluteFilePath();
                }

                copyFile(srcPaths, inactiveForm->getCurrentDirPath());
            }
        }
    }
}

void DoubleFolderPanel::onMove()
{
    qDebug() << "DoubleFolderPanel::onMove()";

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR)
    {
        QList<QFileInfo> selectedFileInfoList = activeForm->getSelectedFileInfoList();
        if(selectedFileInfoList.size() > 0)
        {
            FolderForm* inactiveForm = getInactiveFolderForm();
            if(inactiveForm != Q_NULLPTR)
            {
                QStringList srcPaths;

                for(QFileInfo fileInfo : selectedFileInfoList)
                {
                    srcPaths.push_back(fileInfo.absoluteFilePath());

                    qDebug() << fileInfo.absoluteFilePath();
                }

                moveFile(srcPaths, inactiveForm->getCurrentDirPath());
            }
        }
    }
}

void DoubleFolderPanel::onRemove()
{
    qDebug() << "DoubleFolderPanel::onRemove()";

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR)
    {
        QList<QFileInfo> selectedFileInfoList = activeForm->getSelectedFileInfoList();
        if(selectedFileInfoList.size() > 0)
        {
            QStringList paths;

            for(QFileInfo fileInfo : selectedFileInfoList)
            {
                paths.push_back(fileInfo.absoluteFilePath());

                qDebug() << fileInfo.absoluteFilePath();
            }

            removeFile(paths);
        }
    }
}

void DoubleFolderPanel::onMakeDirectory()
{
    qDebug() << "DoubleFolderPanel::onMakeDirectory()";

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR)
    {
        makeDirectory(activeForm->getCurrentDirPath());
    }
}

void DoubleFolderPanel::onRename()
{
    qDebug() << "DoubleFolderPanel::onRename()";

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR)
    {
        if(activeForm->getCurrentFileName() != "..")
        {
            renameFile(activeForm->getCurrentDirPath(), activeForm->getCurrentFileName());
        }
    }
}

void DoubleFolderPanel::onLeftCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo)
{
    qDebug() << "DoubleFolderPanel::onLeftCurrentChanged : old : " << oldFileInfo.filePath() << " new : " << newFileInfo.filePath();

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR && activeForm->objectName() == "l_folderForm")
    {
        emitStatusChanged(newFileInfo.absoluteFilePath());
    }
}

void DoubleFolderPanel::onRightCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo)
{
    qDebug() << "DoubleFolderPanel::onRightCurrentChanged : old : " << oldFileInfo.filePath() << " new : " << newFileInfo.filePath();

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR && activeForm->objectName() == "r_folderForm")
    {
        emitStatusChanged(newFileInfo.absoluteFilePath());
    }
}

void DoubleFolderPanel::onLeftFocusChanged(bool inFocus)
{
    qDebug() << "DoubleFolderPanel::onLeftFocusChanged : inFocus : " << inFocus;

    if(inFocus)
    {
        FolderForm* activeForm = getActiveFolderForm();
        if(activeForm != Q_NULLPTR && activeForm->objectName() == "l_folderForm")
        {
            emitStatusChanged(activeForm->getCurrentFileInfo().absoluteFilePath());
        }
    }
}

void DoubleFolderPanel::onRightFocusChanged(bool inFocus)
{
    qDebug() << "DoubleFolderPanel::onRightFocusChanged : inFocus : " << inFocus;

    if(inFocus)
    {
        FolderForm* activeForm = getActiveFolderForm();
        if(activeForm != Q_NULLPTR && activeForm->objectName() == "r_folderForm")
        {
            emitStatusChanged(activeForm->getCurrentFileInfo().absoluteFilePath());
        }
    }
}

void DoubleFolderPanel::emitStatusChanged(const QString& statusString)
{
    emit statusChanged(statusString);
}

void DoubleFolderPanel::emitOutputConsole(const QString& consoleString)
{
    emit outputConsole(consoleString);
}

void DoubleFolderPanel::setActiveFolderForm(const QString& objectName)
{
    FolderForm* folderForm = findChild<FolderForm*>(objectName);
    if(folderForm != Q_NULLPTR)
    {
        FolderView* folderView = folderForm->findChild<FolderView*>("folderView");
        if(folderView != Q_NULLPTR)
        {
            folderView->setFocus();
        }
    }
}

FolderForm* DoubleFolderPanel::getActiveFolderForm()
{
    QWidget* fw = focusWidget();
    if(fw == Q_NULLPTR)
    {
        return Q_NULLPTR;
    }

    if(fw->objectName() != "folderView")
    {
        return Q_NULLPTR;
    }

    return dynamic_cast<FolderForm*>(fw->parent());
}

FolderForm* DoubleFolderPanel::getInactiveFolderForm()
{
    FolderForm* inactiveForm = Q_NULLPTR;
    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR)
    {
        if(activeForm->objectName() == "r_folderForm")
        {
            inactiveForm = findChild<FolderForm*>("l_folderForm");
        }
        else
        {
            inactiveForm = findChild<FolderForm*>("r_folderForm");
        }
    }

    return inactiveForm;
}

FolderForm* DoubleFolderPanel::getLeftFolderForm()
{
    return findChild<FolderForm*>("l_folderForm");
}

FolderForm* DoubleFolderPanel::getRightFolderForm()
{
    return findChild<FolderForm*>("r_folderForm");
}

void DoubleFolderPanel::copyFile(const QStringList& srcPaths, const QString& dstPath)
{
    if(QMessageBox::question(this->parentWidget(), tr("Confirm"), tr("copy?")) == QMessageBox::Yes)
    {
        CopyWorker* copyWorker = new CopyWorker(srcPaths, dstPath, false);

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

        WorkingDialog dialog(copyWorker, this);
        if(dialog.exec())
        {

        }

        return;
    }
}

void DoubleFolderPanel::moveFile(const QStringList& srcPaths, const QString& dstPath)
{
    if(QMessageBox::question(this->parentWidget(), tr("Confirm"), tr("move?")) == QMessageBox::Yes)
    {
        CopyWorker* copyWorker = new CopyWorker(srcPaths, dstPath, true);

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

        WorkingDialog dialog(copyWorker, this);
        if(dialog.exec())
        {

        }

        return;
    }
}

void DoubleFolderPanel::removeFile(const QStringList& paths)
{
    if(QMessageBox::question(this->parentWidget(), tr("Confirm"), tr("remove?")) == QMessageBox::Yes)
    {
        Worker* worker = new RemoveWorker(paths);

        connect(worker,
                SIGNAL(finished(int)),
                this,
                SLOT(onRemoveFileFinished(int)));
        connect(worker,
                SIGNAL(error(QString)),
                this,
                SLOT(onRemoveFileError(QString)));

        WorkingDialog dialog(worker, this);
        if(dialog.exec())
        {

        }

        return;
    }
}

void DoubleFolderPanel::makeDirectory(const QString& path)
{
    bool ok = false;
    QString name = QInputDialog::getText(this->parentWidget(),
                                         tr("Make directory"),
                                         tr("Directory name:"),
                                         QLineEdit::Normal, QString(), &ok);

    if(!ok || name.isEmpty())
    {
        // キャンセル
        return;
    }

    QDir dir(path);
    QString absPath = dir.absoluteFilePath(name);

    emitOutputConsole(QString("%1 ... ").arg(absPath));
    qDebug() << absPath;

    if(dir.exists(name))
    {
        // 既に存在しているので何もしない
        emitOutputConsole(tr("is exists.\n"));
        return;
    }

    if(!dir.mkdir(name))
    {
        // ディレクトリ作成失敗
        emitOutputConsole(tr("Failed make directory.\n"));
        return;
    }

    emitOutputConsole(tr("Made directory.\n"));

    refresh();
}

void DoubleFolderPanel::renameFile(const QString& path, const QString& name)
{
    RenameDialog dialog(name);
    if(dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    QString newFileName = dialog.getNewFileName();
    if(newFileName.isEmpty() || name == newFileName)
    {
        return;
    }

    QDir dir(path);
    if(!dir.rename(name, newFileName))
    {
        return;
    }

    emitOutputConsole(QString("%1 >> %2 ... ").arg(name).arg(newFileName));
    emitOutputConsole(tr("Renamed.\n"));

    refresh();

    FolderForm* activeFolderForm = getActiveFolderForm();
    if(activeFolderForm != Q_NULLPTR)
    {
        activeFolderForm->setCursor(newFileName);
    }
}

void DoubleFolderPanel::refresh()
{
    FolderForm* activeFolderForm = getActiveFolderForm();
    if(activeFolderForm != Q_NULLPTR)
    {
        activeFolderForm->refresh();
    }

    FolderForm* inactiveFolderForm = getInactiveFolderForm();
    if(inactiveFolderForm != Q_NULLPTR)
    {
        inactiveFolderForm->refresh();
    }
}

void DoubleFolderPanel::onCopyFileFinished(int result)
{
    qDebug() << "DoubleFolderPanel::onCopyFileFinished : result : " << result;

    refresh();
}

void DoubleFolderPanel::onCopyFileError(const QString& err)
{
    qDebug() << "DoubleFolderPanel::onCopyFileError : err : " << err;

    refresh();
}

void DoubleFolderPanel::onMoveFileFinished(int result)
{
    qDebug() << "DoubleFolderPanel::onMoveFileFinished : result : " << result;

    refresh();
}

void DoubleFolderPanel::onMoveFileError(const QString& err)
{
    qDebug() << "DoubleFolderPanel::onMoveFileError : err : " << err;

    refresh();
}

void DoubleFolderPanel::onRemoveFileFinished(int result)
{
    qDebug() << "DoubleFolderPanel::onRemoveFileFinished : result : " << result;

    refresh();
}

void DoubleFolderPanel::onRemoveFileError(const QString& err)
{
    qDebug() << "DoubleFolderPanel::onRemoveFileError : err : " << err;

    refresh();
}

void DoubleFolderPanel::onConfirmOverwrite(const QString& srcFilePath, const QString& dstFilePath, int methodType)
{
    CopyWorker* copyWorker = dynamic_cast<CopyWorker*>(sender());
    if(copyWorker != nullptr)
    {
        OverwriteDialog dialog(srcFilePath, dstFilePath, static_cast<OverwriteMethodType>(methodType));
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
