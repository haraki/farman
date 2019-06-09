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
#include "settings.h"
#include "fileoperationdialog.h"
#include "fileattributesdialog.h"
#include "selectstoragefavoritedialog.h"
#include "file.h"

namespace Farman
{

DoubleFolderPanel::DoubleFolderPanel(QWidget* parent/* = Q_NULLPTR*/)
    : QWidget(parent)
    , ui(new Ui::DoubleFolderPanel)
    , m_paneMode(DEFAULT_PANE_MODE)
    , m_activePane(DEFAULT_ACTIVE_PANE)
{
    ui->setupUi(this);

    PaneMode paneMode = Settings::getInstance()->getPaneMode();
    PaneType activePane = Settings::getInstance()->getActivePane();

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

    FilterFlags l_filterFlags = Settings::getInstance()->getLeftFilterSettings();
    QStringList l_nameMaskfilters = Settings::getInstance()->getLeftNameMaskFilterSettings();
    SectionType l_sortSectionType = Settings::getInstance()->getLeftSortSectionType();
    SortDirsType l_sortDirsType = Settings::getInstance()->getLeftSortDirsType();
    bool l_sortDotFirst = Settings::getInstance()->getLeftSortDotFirst();
    Qt::CaseSensitivity l_sortCaseSensitivity = Settings::getInstance()->getLeftSortCaseSensitivity();
    Qt::SortOrder l_sortOrder = Settings::getInstance()->getLeftSortOrder();

    QVBoxLayout* l_vLayout = new QVBoxLayout();
    l_vLayout->setSpacing(6);
    l_vLayout->setObjectName(QStringLiteral("l_vLayout"));
    l_vLayout->setContentsMargins(0, 0, 0, 0);

    FolderForm* l_folderForm = new FolderForm(l_filterFlags,
                                              l_nameMaskfilters,
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

        connect(l_folderView,
                SIGNAL(open(const QString&)),
                this,
                SLOT(onOpenFile(const QString&)));
        connect(l_folderView,
                SIGNAL(openWithApp(const QString&)),
                this,
                SLOT(onOpenWithApp(const QString&)));
        connect(l_folderView,
                SIGNAL(copyFile(const QStringList&, const QString&)),
                this,
                SLOT(onCopyFile(const QStringList&, const QString&)));
        connect(l_folderView,
                SIGNAL(moveFile(const QStringList&, const QString&)),
                this,
                SLOT(onMoveFile(const QStringList&, const QString&)));
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

    FilterFlags r_filterFlags = Settings::getInstance()->getRightFilterSettings();
    QStringList r_nameMaskfilters = Settings::getInstance()->getRightNameMaskFilterSettings();
    SectionType r_sortSectionType = Settings::getInstance()->getRightSortSectionType();
    SortDirsType r_sortDirsType = Settings::getInstance()->getRightSortDirsType();
    bool r_sortDotFirst = Settings::getInstance()->getRightSortDotFirst();
    Qt::CaseSensitivity r_sortCaseSensitivity = Settings::getInstance()->getRightSortCaseSensitivity();
    Qt::SortOrder r_sortOrder = Settings::getInstance()->getRightSortOrder();

    QVBoxLayout* r_vLayout = new QVBoxLayout();
    r_vLayout->setSpacing(6);
    r_vLayout->setObjectName(QStringLiteral("r_vLayout"));
    r_vLayout->setContentsMargins(0, 0, 0, 0);

    FolderForm* r_folderForm = new FolderForm(r_filterFlags,
                                              r_nameMaskfilters,
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

        connect(r_folderView,
                SIGNAL(open(const QString&)),
                this,
                SLOT(onOpenFile(const QString&)));
        connect(r_folderView,
                SIGNAL(openWithApp(const QString&)),
                this,
                SLOT(onOpenWithApp(const QString&)));
        connect(r_folderView,
                SIGNAL(copyFile(const QStringList&, const QString&)),
                this,
                SLOT(onCopyFile(const QStringList&, const QString&)));
        connect(r_folderView,
                SIGNAL(moveFile(const QStringList&, const QString&)),
                this,
                SLOT(onMoveFile(const QStringList&, const QString&)));
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

    setActivePane(activePane);
    setPaneMode(paneMode);
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
    const QFont viewFont = Settings::getInstance()->getFontSetting("folderView");

    QMap<ColorRoleType, QColor> colors;

    colors[ColorRoleType::Normal]              = Settings::getInstance()->getColorSetting("folderView_normal");
    colors[ColorRoleType::Normal_Selected]     = Settings::getInstance()->getColorSetting("folderView_normal_selected");
    colors[ColorRoleType::Folder]              = Settings::getInstance()->getColorSetting("folderView_folder");
    colors[ColorRoleType::Folder_Selected]     = Settings::getInstance()->getColorSetting("folderView_folder_selected");
    colors[ColorRoleType::ReadOnly]            = Settings::getInstance()->getColorSetting("folderView_readOnly");
    colors[ColorRoleType::ReadOnly_Selected]   = Settings::getInstance()->getColorSetting("folderView_readOnly_selected");
    colors[ColorRoleType::Hidden]              = Settings::getInstance()->getColorSetting("folderView_hidden");
    colors[ColorRoleType::Hidden_Selected]     = Settings::getInstance()->getColorSetting("folderView_hidden_selected");
    colors[ColorRoleType::System]              = Settings::getInstance()->getColorSetting("folderView_system");
    colors[ColorRoleType::System_Selected]     = Settings::getInstance()->getColorSetting("folderView_system_selected");

    colors[ColorRoleType::Background]          = Settings::getInstance()->getColorSetting("folderView_background");
    colors[ColorRoleType::Selected_Background] = Settings::getInstance()->getColorSetting("folderView_selected_background");

    const QFont pathFont = Settings::getInstance()->getFontSetting("folderPath");

    const QColor pathColor = Settings::getInstance()->getColorSetting("folderPath_text");

    const QColor pathBgColor = Settings::getInstance()->getColorSetting("folderPath_background");

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm != Q_NULLPTR)
    {
        activeForm->setAppearance(viewFont, pathFont, colors, pathColor, pathBgColor);
    }

    FolderForm* inactiveForm = getInactiveFolderForm();
    if(inactiveForm != Q_NULLPTR)
    {
        inactiveForm->setAppearance(viewFont, pathFont, colors, pathColor, pathBgColor);
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
                if(m_paneMode == PaneMode::Single || m_activePane == PaneType::Left)
                {
                    activeForm->onGoToParentDir();
                }
                else
                {
                    setActivePane(PaneType::Left);
                }

                ret = true;

                break;

            case Qt::Key_Right:
                if(m_paneMode == PaneMode::Dual)
                {
                    if(m_activePane == PaneType::Right)
                    {
                        activeForm->onGoToParentDir();
                    }
                    else
                    {
                        setActivePane(PaneType::Right);
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

void DoubleFolderPanel::onSelectStorageFavorite()
{
    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm == Q_NULLPTR)
    {
        return;
    }

    SelectStorageFavoriteDialog dialog(parentWidget());
    if(dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    QString selectedPath = dialog.getSelectedPath();

    int ret = activeForm->setPath(selectedPath);
    if(ret < 0)
    {
        emitOutputConsole(tr("Folder can not be opened : %1\n").arg(selectedPath));
    }
}

void DoubleFolderPanel::onSetPaneMode(PaneMode paneMode)
{
    Settings::getInstance()->setPaneMode(paneMode);

    setPaneMode(paneMode);

    refresh();
}

void DoubleFolderPanel::onChangeSortSettings()
{
    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm == Q_NULLPTR)
    {
        return;
    }

    SectionType sectionType = activeForm->getSortSectionType();
    SortDirsType dirsType = activeForm->getSortDirsType();
    bool dotFirst = activeForm->getSortDotFirst();
    Qt::CaseSensitivity caseSensitivity = activeForm->getSortCaseSensitivity();
    Qt::SortOrder order = activeForm->getSortOrder();

    SortDialog dialog(sectionType, dirsType, dotFirst, caseSensitivity, order, parentWidget());
    if(dialog.exec() != QDialog::Accepted)
    {
        return;
    }

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

void DoubleFolderPanel::onChangeFilterSettings()
{
    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm == Q_NULLPTR)
    {
        return;
    }

    FilterFlags filterFlags = activeForm->getFilterFlags();
    QStringList nameMaskFilters = activeForm->getNameMaskFilters();

    FilterDialog dialog(filterFlags, nameMaskFilters, parentWidget());
    if(dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    filterFlags = dialog.getFilterFlags();
    nameMaskFilters = dialog.getNameMaskFilters();
    activeForm->setFilterFlags(filterFlags);
    activeForm->setNameMaskFilters(nameMaskFilters);
    if(activeForm->objectName() == "l_folderForm")
    {
        Settings::getInstance()->setLeftFilterSettings(filterFlags);
        Settings::getInstance()->setLeftNameMaskFilterSettings(nameMaskFilters);
    }
    else
    {
        Settings::getInstance()->setRightFilterSettings(filterFlags);
        Settings::getInstance()->setRightNameMaskFilterSettings(nameMaskFilters);
    }

    activeForm->refresh(true);          // 選択状態のファイルがあると refresh でクラッシュするので選択状態を解除する
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
                               parentWidget());
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

    emitCopyFile(srcPaths, dstDirPath);
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
                               parentWidget());
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

    emitMoveFile(srcPaths, dstDirPath);
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
                               parentWidget());
    if(dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    QStringList paths;
    for(QFileInfo fileInfo : selectedFileInfoList)
    {
        paths.push_back(fileInfo.absoluteFilePath());
    }

    emitRemoveFile(paths);
}

void DoubleFolderPanel::onMakeDirectory()
{
    qDebug() << "DoubleFolderPanel::onMakeDirectory()";

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm == Q_NULLPTR)
    {
        return;
    }

    QString currentDirPath = activeForm->getCurrentDirPath();
    if(currentDirPath.isEmpty())
    {
        return;
    }

    bool ok = false;
    QString dirName = QInputDialog::getText(parentWidget(),
                                            tr("Make a new directory"),
                                            tr("Directory name:"),
                                            QLineEdit::Normal, QString(), &ok);

    if(!ok || dirName.isEmpty())
    {
        // キャンセル
        return;
    }

    emitMakeDirectory(currentDirPath, dirName);
}

void DoubleFolderPanel::onCreateNewFile()
{
    qDebug() << "DoubleFolderPanel::onCreateNewFile()";

    FolderForm* activeForm = getActiveFolderForm();
    if(activeForm == Q_NULLPTR)
    {
        return;
    }

    QString currentDirPath = activeForm->getCurrentDirPath();
    if(currentDirPath.isEmpty())
    {
        return;
    }

    bool ok = false;
    QString fileName = QInputDialog::getText(this,
                                             tr("Create a new file"),
                                             tr("File name:"),
                                             QLineEdit::Normal, QString(), &ok);

    if(!ok || fileName.isEmpty())
    {
        // キャンセル
        return;
    }

    emitCreateNewFile(currentDirPath, fileName);
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
    if(oldName.isEmpty() || oldName == "..")
    {
        return;
    }

    QString currentDirPath = activeForm->getCurrentDirPath();
    if(currentDirPath.isEmpty())
    {
        return;
    }

    RenameDialog dialog(oldName, parentWidget());
    if(dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    QString newName = dialog.getNewName();
    if(newName.isEmpty() || newName == "." || newName == "..")
    {
        return;
    }
    emitRenameFile(currentDirPath, oldName, newName);

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

    QString currentFileName = activeForm->getCurrentFileName();
    if(currentFileName.isEmpty() || currentFileName == "..")
    {
        return;
    }

    QFileInfo fileInfo = activeForm->getCurrentFileInfo();
    QFile file(fileInfo.absoluteFilePath());

    FileAttributesDialog dialog(fileInfo,
                                file.permissions(),
                                file.fileTime(QFile::FileBirthTime),
                                file.fileTime(QFile::FileModificationTime),
                                parentWidget());
    if(dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    emitChangeFileAttributes(fileInfo.absoluteFilePath(),
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
        m_activePane = PaneType::Left;      // マウスクリックによるフォーカス変更時は setActivePane() が呼ばれないため、ここで書き換える

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
        m_activePane = PaneType::Right;     // マウスクリックによるフォーカス変更時は setActivePane() が呼ばれないため、ここで書き換える

        FolderForm* activeForm = getActiveFolderForm();
        if(activeForm != Q_NULLPTR && activeForm->objectName() == "r_folderForm")
        {
            emitStatusChanged(activeForm->getCurrentFileInfo().absoluteFilePath());
        }
    }
}

void DoubleFolderPanel::onOpenFile(const QString& path, ViewerType viewerType/* = ViewerType::Auto*/)
{
    qDebug() << "DoubleFolderPanel::onOpenFile : path : " << path;

    emitOpenFile(path, viewerType);
}

void DoubleFolderPanel::onOpenWithApp(const QString& path)
{
    qDebug() << "DoubleFolderPanel::onOpenWithApp : path : " << path;

    emitOpenWithApp(path);
}

void DoubleFolderPanel::onCopyFile(const QStringList& srcPaths, const QString& dstPath)
{
    qDebug() << "DoubleFolderPanel::onCopyFile";

    emitCopyFile(srcPaths, dstPath);
}

void DoubleFolderPanel::onMoveFile(const QStringList& srcPaths, const QString& dstPath)
{
    qDebug() << "DoubleFolderPanel::onMoveFile";

    emitMoveFile(srcPaths, dstPath);
}

void DoubleFolderPanel::emitOutputConsole(const QString& consoleString)
{
    emit outputConsole(consoleString);
}

void DoubleFolderPanel::emitStatusChanged(const QString& statusString)
{
    emit statusChanged(statusString);
}

void DoubleFolderPanel::emitOpenFile(const QString& path, ViewerType viewerType/* = ViewerType::Auto*/)
{
    emit openFile(path, viewerType);
}

void DoubleFolderPanel::emitOpenWithApp(const QString& path)
{
    emit openWithApp(path);
}

void DoubleFolderPanel::emitCopyFile(const QStringList& srcPaths, const QString& dstPath)
{
    emit copyFile(srcPaths, dstPath);
}

void DoubleFolderPanel::emitMoveFile(const QStringList& srcPaths, const QString& dstPath)
{
    emit moveFile(srcPaths, dstPath);
}

void DoubleFolderPanel::emitRemoveFile(const QStringList& paths)
{
    emit removeFile(paths);
}

void DoubleFolderPanel::emitMakeDirectory(const QString& path, const QString& dirName)
{
    emit makeDirectory(path, dirName);
}

void DoubleFolderPanel::emitCreateNewFile(const QString& path, const QString& fileName)
{
    emit createNewFile(path, fileName);
}

void DoubleFolderPanel::emitRenameFile(const QString& path, const QString& oldName, const QString& newName)
{
    emit renameFile(path, oldName, newName);
}

void DoubleFolderPanel::emitChangeFileAttributes(const QString& path,
                                                 const QFile::Permissions& newPermissions,
                                                 const QDateTime& newCreated,
                                                 const QDateTime& newLastModified)
{
    emit changeFileAttributes(path, newPermissions, newCreated, newLastModified);
}

void DoubleFolderPanel::setPaneMode(PaneMode paneMode)
{
    m_paneMode = paneMode;

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

void DoubleFolderPanel::setActivePane(PaneType pane)
{
    qDebug() << "DoubleFolderPanel::setActivePane()" << ((pane == PaneType::Left) ? "left" : "right");

    m_activePane = pane;

    Settings::getInstance()->setActivePane(pane);

    FolderForm* folderForm = findChild<FolderForm*>((pane == PaneType::Left) ? "l_folderForm" : "r_folderForm");
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
    return findChild<FolderForm*>((m_activePane == PaneType::Left) ? "l_folderForm" : "r_folderForm");
}

FolderForm* DoubleFolderPanel::getInactiveFolderForm()
{
    return findChild<FolderForm*>((m_activePane == PaneType::Left) ? "r_folderForm" : "l_folderForm");
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
        setActivePane(m_activePane);
    }
}

}           // namespace Farman
