#include <qdir.h>
#include <QKeyEvent>
#include <QDebug>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include "doublefolderpanel.h"
#include "ui_doublefolderpanel.h"
#include "folderform.h"
#include "folderview.h"
#include "sortdialog.h"
#include "filterdialog.h"
#include "renamedialog.h"
#include "mainwindow.h"
#include "settings.h"
#include "fileoperationdialog.h"
#include "fileattributesdialog.h"
#include "file.h"

namespace Farman
{

DoubleFolderPanel::DoubleFolderPanel(QWidget* parent/* = Q_NULLPTR*/)
    : QWidget(parent)
    , ui(new Ui::DoubleFolderPanel)
    , m_paneMode(PaneMode::Default)
    , m_activeFolderFormName("l_folderForm")
{
    ui->setupUi(this);

    PaneMode paneMode = Settings::getInstance()->getPaneMode();

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

    SectionType l_sortSectionType = Settings::getInstance()->getLeftSortSectionType();
    SortDirsType l_sortDirsType = Settings::getInstance()->getLeftSortDirsType();
    bool l_sortDotFirst = Settings::getInstance()->getLeftSortDotFirst();
    Qt::CaseSensitivity l_sortCaseSensitivity = Settings::getInstance()->getLeftSortCaseSensitivity();
    Qt::SortOrder l_sortOrder = Settings::getInstance()->getLeftSortOrder();

    SectionType r_sortSectionType = Settings::getInstance()->getRightSortSectionType();
    SortDirsType r_sortDirsType = Settings::getInstance()->getRightSortDirsType();
    bool r_sortDotFirst = Settings::getInstance()->getRightSortDotFirst();
    Qt::CaseSensitivity r_sortCaseSensitivity = Settings::getInstance()->getRightSortCaseSensitivity();
    Qt::SortOrder r_sortOrder = Settings::getInstance()->getRightSortOrder();

    QDir::Filters l_filterFlags = Settings::getInstance()->getLeftFilterSettings();
    QDir::Filters r_filterFlags = Settings::getInstance()->getRightFilterSettings();

    connect(this,
            SIGNAL(statusChanged(const QString&)),
            MainWindow::getInstance(),
            SLOT(onStatusChanged(const QString&)));

    QVBoxLayout* l_vLayout = new QVBoxLayout();
    l_vLayout->setSpacing(6);
    l_vLayout->setObjectName(QStringLiteral("l_vLayout"));
    l_vLayout->setContentsMargins(0, 0, 0, 0);

    FolderForm* l_folderForm = new FolderForm(l_filterFlags,
                                              l_sortSectionType,
                                              l_sortDirsType,
                                              l_sortDotFirst,
                                              l_sortCaseSensitivity,
                                              l_sortOrder,
                                              this);
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

    FolderForm* r_folderForm = new FolderForm(r_filterFlags,
                                              r_sortSectionType,
                                              r_sortDirsType,
                                              r_sortDotFirst,
                                              r_sortCaseSensitivity,
                                              r_sortOrder,
                                              this);
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

    setActiveFolderForm(m_activeFolderFormName);

    onSetPaneMode(paneMode);
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

void DoubleFolderPanel::updateSettings()
{
    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR)
    {
        activeForm->updateSettings();
    }

    FolderForm* inactiveForm = getInactiveFolderForm();
    if(inactiveForm != Q_NULLPTR)
    {
        inactiveForm->updateSettings();
    }
}

bool DoubleFolderPanel::eventFilter(QObject *watched, QEvent *e)
{
    Q_UNUSED(watched);

    bool ret = false;

    switch (e->type())
    {
    case QEvent::KeyPress:
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(e);
        if(keyEvent != Q_NULLPTR)
        {
            Qt::Key key = static_cast<Qt::Key>(keyEvent->key());

            qDebug() << "DoubleFolderPanel::eventFilter : " << key;

            FolderForm* activeForm = getActiveFolderForm();
            if(activeForm == Q_NULLPTR)
            {
                break;
            }

            switch(key)
            {
            case Qt::Key_Left:
                if(m_paneMode == PaneMode::Single || activeForm->objectName() == "l_folderForm")
                {
                    activeForm->onGoToParentDir();
                }
                else
                {
                    setActiveFolderForm("l_folderForm");
                }

                ret = true;

                break;

            case Qt::Key_Right:
                if(m_paneMode == PaneMode::Dual)
                {
                    if(activeForm->objectName() == "r_folderForm")
                    {
                        activeForm->onGoToParentDir();
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
        }

        break;
    }
    default:
        break;
    }

    return ret;
}

void DoubleFolderPanel::onSetPaneMode(PaneMode paneMode)
{
    m_paneMode = paneMode;
    Settings::getInstance()->setPaneMode(paneMode);

    switch(paneMode)
    {
    case PaneMode::Single:
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

    case PaneMode::Dual:
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
        SectionType sectionType = activeForm->getSortSectionType();
        SortDirsType dirsType = activeForm->getSortDirsType();
        bool dotFirst = activeForm->getSortDotFirst();
        Qt::CaseSensitivity caseSensitivity = activeForm->getSortCaseSensitivity();
        Qt::SortOrder order = activeForm->getSortOrder();

        SortDialog dialog(sectionType, dirsType, dotFirst, caseSensitivity, order, this);
        if(dialog.exec())
        {
            sectionType = dialog.getSortSectionType();
            dirsType = dialog.getSortDirsType();
            dotFirst = dialog.getSortDotFirst();
            caseSensitivity = dialog.getSortCaseSensitivity();
            order = dialog.getSortOrder();

            activeForm->setSortSettings(sectionType, dirsType, dotFirst, caseSensitivity, order);
            if(activeForm->objectName() == "l_folderForm")
            {
                Settings::getInstance()->setLeftSortSectionType(sectionType);
                Settings::getInstance()->setLeftSortDirsType(dirsType);
                Settings::getInstance()->setLeftSortDotFirst(dotFirst);
                Settings::getInstance()->setLeftSortCaseSensitivity(caseSensitivity);
                Settings::getInstance()->setLeftSortOrder(order);
            }
            else
            {
                Settings::getInstance()->setRightSortSectionType(sectionType);
                Settings::getInstance()->setRightSortDirsType(dirsType);
                Settings::getInstance()->setRightSortDotFirst(dotFirst);
                Settings::getInstance()->setRightSortCaseSensitivity(caseSensitivity);
                Settings::getInstance()->setRightSortOrder(order);
            }

            activeForm->refresh();
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

            activeForm->refresh();
        }
    }
}

void DoubleFolderPanel::onCopy()
{
    qDebug() << "DoubleFolderPanel::onCopy()";

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm == Q_NULLPTR)
    {
        return;
    }

    FolderForm* inactiveForm = getInactiveFolderForm();
    if(inactiveForm == Q_NULLPTR)
    {
        return;
    }

    QList<QFileInfo> selectedFileInfoList = activeForm->getSelectedFileInfoList();
    if(selectedFileInfoList.size() == 0)
    {
        return;
    }

    FileOperationDialog dialog(FileOperationDialog::OperationType::Copy,
                               activeForm->getCurrentDirPath(),
                               selectedFileInfoList,
                               inactiveForm->getCurrentDirPath(),
                               this->parentWidget());
    if(dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    QString dstDirPath = dialog.getDstDirPath();
    QStringList srcPaths;
    for(QFileInfo fileInfo : selectedFileInfoList)
    {
        srcPaths.push_back(fileInfo.absoluteFilePath());
    }

    File::getInstance()->copyFile(srcPaths, dstDirPath);
}

void DoubleFolderPanel::onMove()
{
    qDebug() << "DoubleFolderPanel::onMove()";

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm == Q_NULLPTR)
    {
        return;
    }

    FolderForm* inactiveForm = getInactiveFolderForm();
    if(inactiveForm == Q_NULLPTR)
    {
        return;
    }

    QList<QFileInfo> selectedFileInfoList = activeForm->getSelectedFileInfoList();
    if(selectedFileInfoList.size() == 0)
    {
        return;
    }

    FileOperationDialog dialog(FileOperationDialog::OperationType::Move,
                               activeForm->getCurrentDirPath(),
                               selectedFileInfoList,
                               inactiveForm->getCurrentDirPath(),
                               this->parentWidget());
    if(dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    QString dstDirPath = dialog.getDstDirPath();
    QStringList srcPaths;
    for(QFileInfo fileInfo : selectedFileInfoList)
    {
        srcPaths.push_back(fileInfo.absoluteFilePath());
    }

    File::getInstance()->moveFile(srcPaths, dstDirPath);
}

void DoubleFolderPanel::onRemove()
{
    qDebug() << "DoubleFolderPanel::onRemove()";

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm == Q_NULLPTR)
    {
        return;
    }

    QList<QFileInfo> selectedFileInfoList = activeForm->getSelectedFileInfoList();
    if(selectedFileInfoList.size() == 0)
    {
        return;
    }

    FileOperationDialog dialog(FileOperationDialog::OperationType::Remove,
                               activeForm->getCurrentDirPath(),
                               selectedFileInfoList,
                               "",
                               this->parentWidget());
    if(dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    QStringList paths;
    for(QFileInfo fileInfo : selectedFileInfoList)
    {
        paths.push_back(fileInfo.absoluteFilePath());
    }

    File::getInstance()->removeFile(paths);
}

void DoubleFolderPanel::onMakeDirectory()
{
    qDebug() << "DoubleFolderPanel::onMakeDirectory()";

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm == Q_NULLPTR)
    {
        return;
    }

    bool ok = false;
    QString dirName = QInputDialog::getText(this->parentWidget(),
                                            tr("Make directory"),
                                            tr("Directory name:"),
                                            QLineEdit::Normal, QString(), &ok);

    if(!ok || dirName.isEmpty())
    {
        // キャンセル
        return;
    }

    File::getInstance()->makeDirectory(activeForm->getCurrentDirPath(), dirName);
}

void DoubleFolderPanel::onRename()
{
    qDebug() << "DoubleFolderPanel::onRename()";

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm == Q_NULLPTR)
    {
        return;
    }

    QString oldName = activeForm->getCurrentFileName();
    if(oldName == "..")
    {
        return;
    }

    RenameDialog dialog(oldName);
    if(dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    QString newName = dialog.getNewName();
    if(!File::getInstance()->renameFile(activeForm->getCurrentDirPath(), oldName, newName))
    {
        return;
    }

    activeForm->setCursor(newName);
}

void DoubleFolderPanel::onAttributes()
{
    qDebug() << "DoubleFolderPanel::onAttributes()";

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm == Q_NULLPTR)
    {
        return;
    }

    if(activeForm->getCurrentFileName() == "..")
    {
        return;
    }

    QFileInfo fileInfo = activeForm->getCurrentFileInfo();
    QFile file(fileInfo.absoluteFilePath());

    FileAttributesDialog dialog(fileInfo,
                                file.permissions(),
                                file.fileTime(QFile::FileBirthTime),
                                file.fileTime(QFile::FileModificationTime));
    if(dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    File::getInstance()->changeFileAttributes(fileInfo.absoluteFilePath(),
                                              dialog.getPermissions(),
                                              dialog.getCreated(),
                                              dialog.getLastModified());
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

        m_activeFolderFormName = objectName;
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

    return qobject_cast<FolderForm*>(fw->parent());
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

void DoubleFolderPanel::setVisible(bool visible)
{
    QWidget::setVisible(visible);

    if(visible)
    {
        setActiveFolderForm(m_activeFolderFormName);
    }
}

}           // namespace Farman
