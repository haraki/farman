#include <qdir.h>
#include <QKeyEvent>
#include <QDebug>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QClipboard>
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
#include "selectbookmarkdialog.h"
#include "file.h"

#ifdef Q_OS_WIN
#   include "win32.h"
#else
#   include "xnix.h"
#endif

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

    FileSizeFormatType fileSizeFormatType = (paneMode == PaneMode::Single) ? Settings::getInstance()->getSinglePaneFileSizeFormatType() :
                                                                             Settings::getInstance()->getDualPaneFileSizeFormatType();
    bool fileSizeComma = (paneMode == PaneMode::Single) ? Settings::getInstance()->getSinglePaneFileSizeDetailCommaEnable() :
                                                          Settings::getInstance()->getDualPaneFileSizeDetailCommaEnable();

    DateFormatType dateFormatType = (paneMode == PaneMode::Single) ? Settings::getInstance()->getSinglePaneDateFormatType() :
                                                                     Settings::getInstance()->getDualPaneDateFormatType();
    QString dateOrgString = (paneMode == PaneMode::Single) ? Settings::getInstance()->getSinglePaneDateFormatOriginalString() :
                                                             Settings::getInstance()->getDualPaneDateFormatOriginalString();

    for(auto pane : {PaneType::Left, PaneType::Right})
    {
        QString path = QDir::homePath();
        FolderAtStartup folderAtStartup = Settings::getInstance()->getFolderAtStartup(pane);
        if(folderAtStartup == FolderAtStartup::LastTime || folderAtStartup == FolderAtStartup::Fixed)
        {
            path = Settings::getInstance()->getFolderPath(pane);
        }
        if(!QDir(path).exists())
        {
            path = QDir::currentPath();
        }

        FilterFlags filterFlags = Settings::getInstance()->getFilterSettings(pane);
        SectionType sortSectionType = Settings::getInstance()->getSortSectionType(pane);
        SectionType sortSectionType2nd = Settings::getInstance()->getSortSectionType2nd(pane);
        SortDirsType sortDirsType = Settings::getInstance()->getSortDirsType(pane);
        bool sortDotFirst = Settings::getInstance()->getSortDotFirst(pane);
        Qt::CaseSensitivity sortCaseSensitivity = Settings::getInstance()->getSortCaseSensitivity(pane);
        Qt::SortOrder sortOrder = Settings::getInstance()->getSortOrder(pane);

        FolderForm* folderForm = new FolderForm(pane,
                                                filterFlags,
                                                sortSectionType,
                                                sortSectionType2nd,
                                                sortDirsType,
                                                sortDotFirst,
                                                sortCaseSensitivity,
                                                sortOrder,
                                                fileSizeFormatType,
                                                fileSizeComma,
                                                dateFormatType,
                                                dateOrgString,
                                                this);
        Q_ASSERT(folderForm != Q_NULLPTR);

        folderForm->setObjectName(QStringLiteral("folderForm"));
        folderForm->setPath(path);

        QVBoxLayout* vLayout = new QVBoxLayout();
        vLayout->setSpacing(6);
        vLayout->setContentsMargins(0, 0, 0, 0);
        vLayout->addWidget(folderForm);

        if(pane == PaneType::Left)
        {
            connect(folderForm,
                    SIGNAL(currentChanged(QFileInfo,QFileInfo)),
                    this,
                    SLOT(onLeftCurrentChanged(QFileInfo,QFileInfo)));
            connect(folderForm,
                    SIGNAL(directoryLoaded(const QString&)),
                    this,
                    SLOT(onLeftDirectoryLoaded(const QString&)));
            connect(folderForm,
                    SIGNAL(directoryBookmarked(const QString&, bool)),
                    this,
                    SLOT(onLeftDirectoryBookmarked(const QString&, bool)));
            connect(folderForm,
                    SIGNAL(focusChanged(bool)),
                    this,
                    SLOT(onLeftFocusChanged(bool)));

            ui->leftPanel->setLayout(vLayout);
        }
        else
        {
            connect(folderForm,
                    SIGNAL(currentChanged(QFileInfo,QFileInfo)),
                    this,
                    SLOT(onRightCurrentChanged(QFileInfo,QFileInfo)));
            connect(folderForm,
                    SIGNAL(directoryLoaded(const QString&)),
                    this,
                    SLOT(onRightDirectoryLoaded(const QString&)));
            connect(folderForm,
                    SIGNAL(directoryBookmarked(const QString&, bool)),
                    this,
                    SLOT(onRightDirectoryBookmarked(const QString&, bool)));
            connect(folderForm,
                    SIGNAL(focusChanged(bool)),
                    this,
                    SLOT(onRightFocusChanged(bool)));

            ui->rightPanel->setLayout(vLayout);
        }

        FolderView* folderView = folderForm->findChild<FolderView*>("folderView");
        Q_ASSERT(folderView != Q_NULLPTR);

        folderView->installEventFilter(this);

        connect(folderView,
                SIGNAL(open(const QString&)),
                this,
                SLOT(onOpenFile(const QString&)));
        connect(folderView,
                SIGNAL(openWithApp(const QString&)),
                this,
                SLOT(onOpenWithApp(const QString&)));
        connect(folderView,
                SIGNAL(copyFile(const QStringList&, const QString&)),
                this,
                SLOT(onCopyFile(const QStringList&, const QString&)));
        connect(folderView,
                SIGNAL(moveFile(const QStringList&, const QString&)),
                this,
                SLOT(onMoveFile(const QStringList&, const QString&)));
    }

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

    for(auto pane : {PaneType::Left, PaneType::Right})
    {
        FolderAtStartup folderAtStartup = Settings::getInstance()->getFolderAtStartup(pane);
        if(folderAtStartup == FolderAtStartup::LastTime)
        {
            FolderForm* folderForm = getFolderForm(pane);
            Q_ASSERT(folderForm != Q_NULLPTR);

            Settings::getInstance()->setFolderPath(pane, folderForm->getCurrentDirPath());
        }
    }
}

void DoubleFolderPanel::updateSettings()
{
    const QFont viewFont = Settings::getInstance()->getFontSetting("folderView");

    const QFont pathFont = Settings::getInstance()->getFontSetting("folderPath");

    const QColor pathColor = Settings::getInstance()->getColorSetting("folderPath_text");

    const QColor pathBgColor = Settings::getInstance()->getColorSetting("folderPath_background");

    FileSizeFormatType fileSizeFormatType = (m_paneMode == PaneMode::Single) ? Settings::getInstance()->getSinglePaneFileSizeFormatType() :
                                                                               Settings::getInstance()->getDualPaneFileSizeFormatType();
    bool fileSizeComma = (m_paneMode == PaneMode::Single) ? Settings::getInstance()->getSinglePaneFileSizeDetailCommaEnable() :
                                                            Settings::getInstance()->getDualPaneFileSizeDetailCommaEnable();

    DateFormatType dateFormatType = (m_paneMode == PaneMode::Single) ? Settings::getInstance()->getSinglePaneDateFormatType() :
                                                                       Settings::getInstance()->getDualPaneDateFormatType();
    QString dateOrgString = (m_paneMode == PaneMode::Single) ? Settings::getInstance()->getSinglePaneDateFormatOriginalString() :
                                                               Settings::getInstance()->getDualPaneDateFormatOriginalString();

    FolderForm* activeForm = getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    activeForm->setAppearance(viewFont, pathFont, pathColor, pathBgColor);
    activeForm->setFileSizeFormatType(fileSizeFormatType);
    activeForm->setFileSizeComma(fileSizeComma);
    activeForm->setDateFormatType(dateFormatType);
    activeForm->setDateFormatOriginalString(dateOrgString);

    FolderForm* inactiveForm = getInactiveFolderForm();
    Q_ASSERT(inactiveForm != Q_NULLPTR);

    inactiveForm->setAppearance(viewFont, pathFont, pathColor, pathBgColor);
    inactiveForm->setFileSizeFormatType(fileSizeFormatType);
    inactiveForm->setFileSizeComma(fileSizeComma);
    inactiveForm->setDateFormatType(dateFormatType);
    inactiveForm->setDateFormatOriginalString(dateOrgString);

    updateFolderViewColorsSettings();
}

void DoubleFolderPanel::updateFolderViewColorsSettings()
{
    QMap<FolderViewColorRoleType, QColor> folderViewColors;

    folderViewColors[FolderViewColorRoleType::Normal]              = Settings::getInstance()->getColorSetting("folderView_normal");
    folderViewColors[FolderViewColorRoleType::Normal_Selected]     = Settings::getInstance()->getColorSetting("folderView_normal_selected");
    folderViewColors[FolderViewColorRoleType::Folder]              = Settings::getInstance()->getColorSetting("folderView_folder");
    folderViewColors[FolderViewColorRoleType::Folder_Selected]     = Settings::getInstance()->getColorSetting("folderView_folder_selected");
    folderViewColors[FolderViewColorRoleType::ReadOnly]            = Settings::getInstance()->getColorSetting("folderView_readOnly");
    folderViewColors[FolderViewColorRoleType::ReadOnly_Selected]   = Settings::getInstance()->getColorSetting("folderView_readOnly_selected");
    folderViewColors[FolderViewColorRoleType::Hidden]              = Settings::getInstance()->getColorSetting("folderView_hidden");
    folderViewColors[FolderViewColorRoleType::Hidden_Selected]     = Settings::getInstance()->getColorSetting("folderView_hidden_selected");
    folderViewColors[FolderViewColorRoleType::System]              = Settings::getInstance()->getColorSetting("folderView_system");
    folderViewColors[FolderViewColorRoleType::System_Selected]     = Settings::getInstance()->getColorSetting("folderView_system_selected");

    folderViewColors[FolderViewColorRoleType::Background]          = Settings::getInstance()->getColorSetting("folderView_background");
    folderViewColors[FolderViewColorRoleType::Selected_Background] = Settings::getInstance()->getColorSetting("folderView_selected_background");

    FolderForm* activeForm = getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    activeForm->setAppearanceFolderViewColors(folderViewColors, Settings::getInstance()->getFolderColorTopPriority());

    if(Settings::getInstance()->getEnableInactiveFontColor())
    {
        folderViewColors[FolderViewColorRoleType::Normal]              =
        folderViewColors[FolderViewColorRoleType::Folder]              =
        folderViewColors[FolderViewColorRoleType::ReadOnly]            =
        folderViewColors[FolderViewColorRoleType::Hidden]              =
        folderViewColors[FolderViewColorRoleType::System]              = Settings::getInstance()->getColorSetting("folderView_inactive");
        folderViewColors[FolderViewColorRoleType::Normal_Selected]     =
        folderViewColors[FolderViewColorRoleType::Folder_Selected]     =
        folderViewColors[FolderViewColorRoleType::ReadOnly_Selected]   =
        folderViewColors[FolderViewColorRoleType::Hidden_Selected]     =
        folderViewColors[FolderViewColorRoleType::System_Selected]     = Settings::getInstance()->getColorSetting("folderView_inactive_selected");

        folderViewColors[FolderViewColorRoleType::Background]          = Settings::getInstance()->getColorSetting("folderView_inactive_background");
        folderViewColors[FolderViewColorRoleType::Selected_Background] = Settings::getInstance()->getColorSetting("folderView_inactive_selected_background");
    }

    FolderForm* inactiveForm = getInactiveFolderForm();
    Q_ASSERT(inactiveForm != Q_NULLPTR);

    inactiveForm->setAppearanceFolderViewColors(folderViewColors, Settings::getInstance()->getFolderColorTopPriority());
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
        if(keyEvent == Q_NULLPTR)
        {
            break;
        }

        Qt::Key key = static_cast<Qt::Key>(keyEvent->key());

        qDebug() << "DoubleFolderPanel::eventFilter : " << key;

        FolderForm* activeForm = getActiveFolderForm();
        Q_ASSERT(activeForm != Q_NULLPTR);

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

        break;
    }
    default:
        break;
    }

    return ret;
}

void DoubleFolderPanel::onSelectStorageBookmark()
{
    FolderForm* activeForm = getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    SelectBookmarkDialog dialog(parentWidget());
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
    Q_ASSERT(activeForm != Q_NULLPTR);

    SectionType sectionType = activeForm->getSortSectionType();
    SectionType sectionType2nd = activeForm->getSortSectionType2nd();
    SortDirsType dirsType = activeForm->getSortDirsType();
    bool dotFirst = activeForm->getSortDotFirst();
    Qt::CaseSensitivity caseSensitivity = activeForm->getSortCaseSensitivity();
    Qt::SortOrder order = activeForm->getSortOrder();

    SortDialog dialog(sectionType, sectionType2nd, dirsType, dotFirst, caseSensitivity, order, parentWidget());
    if(dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    sectionType = dialog.getSortSectionType();
    sectionType2nd = dialog.getSortSectionType2nd();
    dirsType = dialog.getSortDirsType();
    dotFirst = dialog.getSortDotFirst();
    caseSensitivity = dialog.getSortCaseSensitivity();
    order = dialog.getSortOrder();

    activeForm->setSortSettings(sectionType, sectionType2nd, dirsType, dotFirst, caseSensitivity, order);

    PaneType pane = activeForm->getPaneType();

    Settings::getInstance()->setSortSectionType(pane, sectionType);
    Settings::getInstance()->setSortSectionType2nd(pane, sectionType2nd);
    Settings::getInstance()->setSortDirsType(pane, dirsType);
    Settings::getInstance()->setSortDotFirst(pane, dotFirst);
    Settings::getInstance()->setSortCaseSensitivity(pane, caseSensitivity);
    Settings::getInstance()->setSortOrder(pane, order);

    activeForm->refresh();
}

void DoubleFolderPanel::onChangeFilterSettings()
{
    FolderForm* activeForm = getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

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

    PaneType pane = activeForm->getPaneType();

    Settings::getInstance()->setFilterSettings(pane, filterFlags);

    activeForm->refresh(true);          // 選択状態のファイルがあると refresh でクラッシュするので選択状態を解除する
}

void DoubleFolderPanel::onCopy()
{
    qDebug() << "DoubleFolderPanel::onCopy()";

    FolderForm* activeForm = getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    FolderForm* inactiveForm = getInactiveFolderForm();
    Q_ASSERT(inactiveForm != Q_NULLPTR);

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
    Q_ASSERT(activeForm != Q_NULLPTR);

    FolderForm* inactiveForm = getInactiveFolderForm();
    Q_ASSERT(inactiveForm != Q_NULLPTR);

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
    Q_ASSERT(activeForm != Q_NULLPTR);

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
    Q_ASSERT(activeForm != Q_NULLPTR);

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
    Q_ASSERT(activeForm != Q_NULLPTR);

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
    Q_ASSERT(activeForm != Q_NULLPTR);

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
    Q_ASSERT(activeForm != Q_NULLPTR);

    QString currentFileName = activeForm->getCurrentFileName();
    if(currentFileName.isEmpty() || currentFileName == "..")
    {
        return;
    }

    QFileInfo fileInfo = activeForm->getCurrentFileInfo();
    QString filePath = fileInfo.absoluteFilePath();
    QFile file(filePath);
#ifdef Q_OS_WIN
    WinFileAttrFlags fileAttrFlags = Win32::getFileAttrFlags(filePath);
#endif

    FileAttributesDialog dialog(fileInfo,
                                file.permissions(),
#ifdef Q_OS_WIN
                                fileAttrFlags,
#endif
                                fileInfo.fileTime(QFile::FileBirthTime),
                                fileInfo.fileTime(QFile::FileModificationTime),
                                getFileSizeOnDisk(filePath),
                                parentWidget());
    if(dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    emitChangeFileAttributes(filePath,
#ifdef Q_OS_WIN
                             dialog.getFileAttrFlags(),
#else
                             dialog.getPermissions(),
#endif
                             dialog.getCreated(),
                             dialog.getLastModified());

    refresh();
}

void DoubleFolderPanel::onSelectAll()
{
    qDebug() << "DoubleFolderPanel::onSelectAll()";

    FolderForm* activeForm = getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    activeForm->onSelectAll();
}

void DoubleFolderPanel::onDeselectAll()
{
    qDebug() << "DoubleFolderPanel::onDeselectAll()";

    FolderForm* activeForm = getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    activeForm->onDeselectAll();
}

void DoubleFolderPanel::onCopyFullPath()
{
    qDebug() << "DoubleFolderPanel::onCopyFullPath()";

    FolderForm* activeForm = getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    QClipboard* clipboard = QApplication::clipboard();
    Q_ASSERT(clipboard != Q_NULLPTR);

    QStringList filePaths;
    for(auto fileInfo : activeForm->getSelectedFileInfoList())
    {
        filePaths.push_back(fileInfo.absoluteFilePath());
    }

    clipboard->setText(filePaths.join(LINEFEED_CODE));

    emitOutputConsole(tr("Copy full path to clipboard.\n"));
}

void DoubleFolderPanel::onCopyFileName()
{
    qDebug() << "DoubleFolderPanel::onCopyFileName()";

    FolderForm* activeForm = getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    QClipboard* clipboard = QApplication::clipboard();
    Q_ASSERT(clipboard != Q_NULLPTR);

    QStringList fileNames;
    for(auto fileInfo : activeForm->getSelectedFileInfoList())
    {
        fileNames.push_back(fileInfo.fileName());
    }

    clipboard->setText(fileNames.join(LINEFEED_CODE));

    emitOutputConsole(tr("Copy file name to clipboard\n"));
}

void DoubleFolderPanel::onSetSameFolderToTarget()
{
    FolderForm* activeForm = getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    FolderForm* inactiveForm = getInactiveFolderForm();
    Q_ASSERT(inactiveForm != Q_NULLPTR);

    inactiveForm->setPath(activeForm->getCurrentDirPath());
}

void DoubleFolderPanel::onSetSameFolderFromTarget()
{
    FolderForm* activeForm = getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    FolderForm* inactiveForm = getInactiveFolderForm();
    Q_ASSERT(inactiveForm != Q_NULLPTR);

    activeForm->setPath(inactiveForm->getCurrentDirPath());
}

void DoubleFolderPanel::onLeftCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo)
{
    qDebug() << "DoubleFolderPanel::onLeftCurrentChanged : old : " << oldFileInfo.filePath() << " new : " << newFileInfo.filePath();

    if(m_activePane == PaneType::Left)
    {
        emitStatusChanged(newFileInfo.absoluteFilePath());
    }
}

void DoubleFolderPanel::onRightCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo)
{
    qDebug() << "DoubleFolderPanel::onRightCurrentChanged : old : " << oldFileInfo.filePath() << " new : " << newFileInfo.filePath();

    if(m_activePane == PaneType::Right)
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
        Q_ASSERT(activeForm != Q_NULLPTR);

        emitStatusChanged(activeForm->getCurrentFileInfo().absoluteFilePath());
    }

    emitFocusChanged(PaneType::Left, inFocus);
}

void DoubleFolderPanel::onRightFocusChanged(bool inFocus)
{
    qDebug() << "DoubleFolderPanel::onRightFocusChanged : inFocus : " << inFocus;

    if(inFocus)
    {
        m_activePane = PaneType::Right;     // マウスクリックによるフォーカス変更時は setActivePane() が呼ばれないため、ここで書き換える

        FolderForm* activeForm = getActiveFolderForm();
        Q_ASSERT(activeForm != Q_NULLPTR);

        emitStatusChanged(activeForm->getCurrentFileInfo().absoluteFilePath());
    }

    emitFocusChanged(PaneType::Right, inFocus);
}

void DoubleFolderPanel::onLeftDirectoryLoaded(const QString& path)
{
    qDebug() << "DoubleFolderPanel::onLeftDirectoryLoaded : path : " << path;

    emitDirectoryLoaded(PaneType::Left, path);
}

void DoubleFolderPanel::onRightDirectoryLoaded(const QString& path)
{
    qDebug() << "DoubleFolderPanel::onRightDirectoryLoaded : path : " << path;

    emitDirectoryLoaded(PaneType::Right, path);
}

void DoubleFolderPanel::onLeftDirectoryBookmarked(const QString &path, bool marked)
{
    qDebug() << "DoubleFolderPanel::onLeftDirectoryBookmarked : path : " << path << ", marked : " << marked;

    FolderForm* folderForm = getFolderForm(PaneType::Right);
    Q_ASSERT(folderForm != Q_NULLPTR);

    folderForm->checkBookmark();

    emitDirectoryBookmarked(PaneType::Left, path, marked);
}

void DoubleFolderPanel::onRightDirectoryBookmarked(const QString &path, bool marked)
{
    qDebug() << "DoubleFolderPanel::onRightDirectoryBookmarked : path : " << path << ", marked : " << marked;

    FolderForm* folderForm = getFolderForm(PaneType::Left);
    Q_ASSERT(folderForm != Q_NULLPTR);

    folderForm->checkBookmark();

    emitDirectoryBookmarked(PaneType::Right, path, marked);
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

void DoubleFolderPanel::emitFocusChanged(PaneType pane, bool inFocus)
{
    emit focusChanged(pane, inFocus);
}

void DoubleFolderPanel::emitDirectoryLoaded(PaneType pane, const QString& path)
{
    emit directoryLoaded(pane, path);
}

void DoubleFolderPanel::emitDirectoryBookmarked(PaneType pane, const QString& path, bool marked)
{
    emit directoryBookmarked(pane, path, marked);
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
#ifdef Q_OS_WIN
                                                 const WinFileAttrFlags& newFileAttrFlags,
#else
                                                 const QFile::Permissions& newPermissions,
#endif
                                                 const QDateTime& newCreated,
                                                 const QDateTime& newLastModified)
{
    emit changeFileAttributes(path,
#ifdef Q_OS_WIN
                              newFileAttrFlags,
#else
                              newPermissions,
#endif
                              newCreated,
                              newLastModified);
}

void DoubleFolderPanel::setPaneMode(PaneMode paneMode)
{
    m_paneMode = paneMode;

    FolderForm* activeForm   = getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);
    FolderForm* inactiveForm = getInactiveFolderForm();
    Q_ASSERT(inactiveForm != Q_NULLPTR);

    switch(paneMode)
    {
    case PaneMode::Single:
        if(m_activePane == PaneType::Left)
        {
            ui->leftPanel->setVisible(true);
            ui->rightPanel->setVisible(false);
        }
        else
        {
            ui->leftPanel->setVisible(false);
            ui->rightPanel->setVisible(true);
        }

        activeForm->setFileSizeFormatType(Settings::getInstance()->getSinglePaneFileSizeFormatType());
        activeForm->setFileSizeComma(Settings::getInstance()->getSinglePaneFileSizeDetailCommaEnable());
        activeForm->setDateFormatType(Settings::getInstance()->getSinglePaneDateFormatType());
        activeForm->setDateFormatOriginalString(Settings::getInstance()->getSinglePaneDateFormatOriginalString());
        inactiveForm->setFileSizeFormatType(Settings::getInstance()->getSinglePaneFileSizeFormatType());
        inactiveForm->setFileSizeComma(Settings::getInstance()->getSinglePaneFileSizeDetailCommaEnable());
        inactiveForm->setDateFormatType(Settings::getInstance()->getSinglePaneDateFormatType());
        inactiveForm->setDateFormatOriginalString(Settings::getInstance()->getSinglePaneDateFormatOriginalString());

        break;

    case PaneMode::Dual:
        ui->leftPanel->setVisible(true);
        ui->rightPanel->setVisible(true);

        activeForm->setFileSizeFormatType(Settings::getInstance()->getDualPaneFileSizeFormatType());
        activeForm->setFileSizeComma(Settings::getInstance()->getDualPaneFileSizeDetailCommaEnable());
        activeForm->setDateFormatType(Settings::getInstance()->getDualPaneDateFormatType());
        activeForm->setDateFormatOriginalString(Settings::getInstance()->getDualPaneDateFormatOriginalString());
        inactiveForm->setFileSizeFormatType(Settings::getInstance()->getDualPaneFileSizeFormatType());
        inactiveForm->setFileSizeComma(Settings::getInstance()->getDualPaneFileSizeDetailCommaEnable());
        inactiveForm->setDateFormatType(Settings::getInstance()->getDualPaneDateFormatType());
        inactiveForm->setDateFormatOriginalString(Settings::getInstance()->getDualPaneDateFormatOriginalString());

        break;

    default:
        break;
    }
}

void DoubleFolderPanel::setActivePane(PaneType pane)
{
    qDebug() << "DoubleFolderPanel::setActivePane()" << ((pane == PaneType::Left) ? "left" : "right");

    m_activePane = pane;

    updateFolderViewColorsSettings();

    Settings::getInstance()->setActivePane(pane);

    FolderForm* folderForm = getFolderForm(pane);
    Q_ASSERT(folderForm != Q_NULLPTR);

    FolderView* folderView = folderForm->findChild<FolderView*>("folderView");
    Q_ASSERT(folderView != Q_NULLPTR);

    folderView->setFocus();
}

FolderForm* DoubleFolderPanel::getFolderForm(PaneType pane)
{
    for(auto folderForm : findChildren<FolderForm*>(QStringLiteral("folderForm")))
    {
        if(folderForm->getPaneType() == pane)
        {
            return folderForm;
        }
    }

    return Q_NULLPTR;
}

FolderForm* DoubleFolderPanel::getActiveFolderForm()
{
    return getFolderForm(m_activePane);
}

FolderForm* DoubleFolderPanel::getInactiveFolderForm()
{
    return getFolderForm((m_activePane == PaneType::Left) ? PaneType::Right : PaneType::Left);
}

void DoubleFolderPanel::refresh()
{
    FolderForm* activeForm = getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    activeForm->refresh();

    FolderForm* inactiveForm = getInactiveFolderForm();
    Q_ASSERT(inactiveForm != Q_NULLPTR);

    inactiveForm->refresh();
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
