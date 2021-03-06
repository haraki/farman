﻿#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>
#include <QProcess>
#include <QMessageBox>
#include <QInputDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "folderform.h"
#include "doublefolderpanel.h"
#include "settings.h"
#include "preferencesdialog.h"
#include "bookmarkmanager.h"
#include "bookmarkmanagerdialog.h"
#include "viewerdispatcher.h"
#include "imageviewer.h"
#include "file.h"
#include "aboutdialog.h"

namespace Farman
{

MainWindow::MainWindow(QWidget *parent/* = Q_NULLPTR*/)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_file(Q_NULLPTR)
    , m_viewerDispatcher(Q_NULLPTR)
{
    ui->setupUi(this);
#ifdef QT_DEBUG
    this->setWindowTitle(this->windowTitle() + " (Debug)");
#endif
    m_file = new File(this);
    m_viewerDispatcher = new ViewerDispatcher(ui->mainWidget);

    DoubleFolderPanel* doubleFolderPanel = new DoubleFolderPanel(ui->mainWidget);

    connect(m_file,
            SIGNAL(outputConsole(const QString&)),
            this,
            SLOT(onOutputConsole(const QString&)));
    connect(this,
            SIGNAL(outputConsole(const QString&)),
            this,
            SLOT(onOutputConsole(const QString&)));

    connect(doubleFolderPanel,
            SIGNAL(statusChanged(const QString&)),
            this,
            SLOT(onStatusChanged(const QString&)));
    connect(doubleFolderPanel,
            SIGNAL(outputConsole(const QString&)),
            this,
            SLOT(onOutputConsole(const QString&)));
    connect(doubleFolderPanel,
            SIGNAL(focusChanged(PaneType, bool)),
            this,
            SLOT(onFocusChanged(PaneType, bool)));
    connect(doubleFolderPanel,
            SIGNAL(directoryLoaded(PaneType, const QString&)),
            this,
            SLOT(onDirectoryLoaded(PaneType, const QString&)));
    connect(doubleFolderPanel,
            SIGNAL(directoryBookmarked(PaneType, const QString&, bool)),
            this,
            SLOT(onDirectoryBookmarked(PaneType, const QString&, bool)));

    connect(doubleFolderPanel,
            SIGNAL(openFile(const QString&, ViewerType)),
            this,
            SLOT(onOpenFile(const QString&, ViewerType)));
    connect(doubleFolderPanel,
            SIGNAL(openWithApp(const QString&)),
            this,
            SLOT(onOpenWithApp(const QString&)));
    connect(doubleFolderPanel,
            SIGNAL(openWithTextEditor(const QString&, const QStringList&)),
            this,
            SLOT(onOpenWithTextEditor(const QString&, const QStringList&)));

    connect(doubleFolderPanel,
            SIGNAL(copyFile(const QStringList&, const QString&)),
            m_file,
            SLOT(onCopyFile(const QStringList&, const QString&)));
    connect(doubleFolderPanel,
            SIGNAL(moveFile(const QStringList&, const QString&)),
            m_file,
            SLOT(onMoveFile(const QStringList&, const QString&)));
    connect(doubleFolderPanel,
            SIGNAL(removeFile(const QStringList&)),
            m_file,
            SLOT(onRemoveFile(const QStringList&)));
    connect(doubleFolderPanel,
            SIGNAL(makeDirectory(const QString&, const QString&)),
            m_file,
            SLOT(onMakeDirectory(const QString&, const QString&)));
    connect(doubleFolderPanel,
            SIGNAL(createNewFile(const QString&, const QString&)),
            m_file,
            SLOT(onCreateNewFile(const QString&, const QString&)));
    connect(doubleFolderPanel,
            SIGNAL(renameFile(const QString&, const QString&, const QString&)),
            m_file,
            SLOT(onRenameFile(const QString&, const QString&, const QString&)));
    connect(doubleFolderPanel,
#ifdef Q_OS_WIN
            SIGNAL(changeFileAttributes(const QString&,
                                        const WinFileAttrFlags&,
                                        const QDateTime&,
                                        const QDateTime&)),
#else
            SIGNAL(changeFileAttributes(const QString&,
                                        const QFile::Permissions&,
                                        const QDateTime&,
                                        const QDateTime&)),
#endif
            m_file,
#ifdef Q_OS_WIN
            SLOT(onChangeFileAttributes(const QString&,
                                        const WinFileAttrFlags&,
                                        const QDateTime&,
                                        const QDateTime&))
#else
            SLOT(onChangeFileAttributes(const QString&,
                                        const QFile::Permissions&,
                                        const QDateTime&,
                                        const QDateTime&))
#endif
           );

    connect(m_file,
            SIGNAL(createNewFileFinished(const QString&)),
            this,
            SLOT(onCreateNewFileFinished(const QString&)));

    ui->mainWidget->layout()->addWidget(doubleFolderPanel);

    SizeAtStartup sizeType = Settings::getInstance()->getSizeAtStartupType();
    if(sizeType == SizeAtStartup::Fixed || sizeType == SizeAtStartup::LastTime)
    {
        QSize size = Settings::getInstance()->getSizeAtStartup();
        this->resize(size);
    }

    PositionAtStartup posType = Settings::getInstance()->getPositionAtStartupType();
    if(posType == PositionAtStartup::Fixed || posType == PositionAtStartup::LastTime)
    {
        QPoint pos = Settings::getInstance()->getPositionAtStartup();
        this->move(pos);
    }

    ui->actionConsole->setChecked(Settings::getInstance()->getConsoleVisible());
    setVisibleConsole(Settings::getInstance()->getConsoleVisible());

    updateSettings();
    doubleFolderPanel->refresh();

    resizeDocks({ui->consoleDockWidget}, {ui->consoleDockWidget->minimumHeight()}, Qt::Vertical);
}

MainWindow::~MainWindow()
{
    delete m_viewerDispatcher;
    m_viewerDispatcher = Q_NULLPTR;

    delete m_file;
    m_file = Q_NULLPTR;

    delete ui;
}

void MainWindow::initFont()
{
    ui->consolePlainTextEdit->setFont(Settings::getInstance()->getFontSetting("console"));
}

void MainWindow::initPalette()
{
    QPalette pal;

    pal = ui->consolePlainTextEdit->palette();
    pal.setColor(QPalette::Text, Settings::getInstance()->getColorSetting("console_text"));
    pal.setColor(QPalette::Base, Settings::getInstance()->getColorSetting("console_background"));
    ui->consolePlainTextEdit->setAutoFillBackground(true);
    ui->consolePlainTextEdit->setPalette(pal);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    qDebug() << "MainWindow::closeEvent()";

    if(Settings::getInstance()->getConfirmQuit())
    {
        if(QMessageBox::question(this, tr("Quit"), tr("Are you sure want to quit?")) != QMessageBox::Yes)
        {
            event->ignore();
            return;
        }
    }

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->closeEvent(event);
    }

    SizeAtStartup sizeType = Settings::getInstance()->getSizeAtStartupType();
    if(sizeType == SizeAtStartup::LastTime)
    {
        Settings::getInstance()->setSizeAtStartup(this->size());
    }

    PositionAtStartup positionType = Settings::getInstance()->getPositionAtStartupType();
    if(positionType == PositionAtStartup::LastTime)
    {
        Settings::getInstance()->setPositionAtStartup(this->pos());
    }

    Settings::getInstance()->flush();

    QMainWindow::closeEvent(event);
}

void MainWindow::emitOutputConsole(const QString& consoleString)
{
    emit outputConsole(consoleString);
}

void MainWindow::onOpenFile(ViewerType viewerType)
{
    qDebug() << "MainWindow::onOpenFile()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    FolderForm* activeForm = doubleFolderPanel->getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    QString path = activeForm->getCurrentFileInfo().absoluteFilePath();

    openFile(path, viewerType);
}

void MainWindow::onOpenFile(const QString& path, ViewerType viewerType/* = ViewerType::Auto*/)
{
    qDebug() << "MainWindow::onOpenFile()";

    openFile(path, viewerType);
}

void MainWindow::onCloseViewer(const QString& viewerObjectName)
{
    qDebug() << "MainWindow::onCloseViewer()";

    closeViewer(viewerObjectName);
}

void MainWindow::onOpenWithApp(const QString& path)
{
    qDebug() << "MainWindow::onOpenWithApp()";

    openWithApp(path);
}

void MainWindow::onOpenWithTextEditor(const QString& dirPath, const QStringList& filePaths)
{
    qDebug() << "MainWindow::onOpenWithTextEditor()";

    openWithTextEditor(dirPath, filePaths);
}

void MainWindow::onCreateNewFileFinished(const QString& filePath)
{
    qDebug() << "MainWindow::onCreateNewFileFinished : " << filePath;

    QStringList filePaths;
    filePaths.push_back(filePath);

    openWithTextEditor(QFileInfo(filePath).absolutePath(), filePaths);
}

void MainWindow::onOpenWithTerminal(const QString& dirPath)
{
    qDebug() << "MainWindow::onOpenWithTerminal()";

    openWithTerminal(dirPath);
}

void MainWindow::onStatusChanged(const QString& statusString)
{
    qDebug() << "MainWindow::onStatusChanged : " << statusString;

    statusBar()->showMessage(statusString);
}

void MainWindow::onOutputConsole(const QString& consoleString)
{
    qDebug() << "MainWindow::onOutputConsole : " << consoleString;

    ui->consolePlainTextEdit->insertPlainText(consoleString);
    ui->consolePlainTextEdit->moveCursor(QTextCursor::End);
}

void MainWindow::onFocusChanged(PaneType pane, bool inFocus)
{
    Q_ASSERT(pane == PaneType::Left || pane == PaneType::Right);

    qDebug() << "MainWindow::onFocusChanged : pane : "
             << (pane == PaneType::Left ? "left" : pane == PaneType::Right ? "right" : "unknown") << ", inFocus : " << inFocus;

    if(!inFocus)
    {
        return;
    }

    checkBookmark();
    checkHistory();
}

void MainWindow::onDirectoryLoaded(PaneType pane, const QString& path)
{
    Q_ASSERT(pane == PaneType::Left || pane == PaneType::Right);

    qDebug() << "MainWindow::onDirectoryLoaded : pane : "
             << (pane == PaneType::Left ? "left" : pane == PaneType::Right ? "right" : "unknown") << ", path : " << path;

    checkBookmark();
    checkHistory();
}

void MainWindow::onDirectoryBookmarked(PaneType pane, const QString &path, bool marked)
{
    Q_ASSERT(pane == PaneType::Left || pane == PaneType::Right);

    qDebug() << "MainWindow::onDirectoryBookmarked : pane : "
             << (pane == PaneType::Left ? "left" : pane == PaneType::Right ? "right" : "unknown")
             << ", path : " << path << ", marked : " << marked;

    checkBookmark();
}

void MainWindow::onActionHistoryTriggered()
{
    qDebug() << "MainWindow::onActionHistoryTriggered()";

    QAction* historyAction = dynamic_cast<QAction*>(sender());
    Q_ASSERT(historyAction != Q_NULLPTR);

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    FolderForm* activeForm = doubleFolderPanel->getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    activeForm->setPath(historyAction->text());
}

void MainWindow::on_actionOpen_triggered()
{
    qDebug() << "MainWindow::on_actionOpen_triggered()";

    onOpenFile(ViewerType::Auto);
}

void MainWindow::on_actionOpenWithTextViewer_triggered()
{
    qDebug() << "MainWindow::on_actionOpenWithTextViewer_triggered()";

    onOpenFile(ViewerType::Text);
}

void MainWindow::on_actionOpenWithHexViewer_triggered()
{
    qDebug() << "MainWindow::on_actionOpenWithHexViewer_triggered()";

    onOpenFile(ViewerType::Hex);
}

void MainWindow::on_actionOpenWithImageViewer_triggered()
{
    qDebug() << "MainWindow::on_actionOpenWithImageViewer_triggered()";

    onOpenFile(ViewerType::Image);
}

void MainWindow::on_actionOpenWithApp_triggered()
{
    qDebug() << "MainWindow::on_actionOpenWithApp_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    FolderForm* activeForm = doubleFolderPanel->getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    const QString path = activeForm->getCurrentFileInfo().absoluteFilePath();

    onOpenWithApp(path);
}

void MainWindow::on_actionOpenWithTextEditor_triggered()
{
    qDebug() << "MainWindow::on_actionOpenWithTextEditor_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    FolderForm* activeForm = doubleFolderPanel->getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    QList<QFileInfo> selectedFileInfoList = activeForm->getSelectedFileInfoList();
    if(selectedFileInfoList.size() == 0)
    {
        return;
    }

    QStringList filePaths;
    for(auto fileInfo : selectedFileInfoList)
    {
        filePaths.push_back(fileInfo.absoluteFilePath());
    }

    onOpenWithTextEditor(activeForm->getCurrentDirPath(), filePaths);
}

void MainWindow::on_actionCreateNewFile_triggered()
{
    qDebug() << "MainWindow::on_actionCreateNewFile_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->onCreateNewFile();
}

void Farman::MainWindow::on_actionOpenWithTerminal_triggered()
{
    qDebug() << "MainWindow::on_actionOpenWithTerminal_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    FolderForm* activeForm = doubleFolderPanel->getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    onOpenWithTerminal(activeForm->getCurrentDirPath());
}

void MainWindow::on_actionQuit_triggered()
{
    qDebug() << "MainWindow::on_actionQuit_triggered()";

    close();
}

void MainWindow::on_actionSinglePane_triggered()
{
    qDebug() << "MainWindow::on_actionSinglePane_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->onSetPaneMode(PaneMode::Single);
}

void MainWindow::on_actionDualPane_triggered()
{
    qDebug() << "MainWindow::on_actionDualPane_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->onSetPaneMode(PaneMode::Dual);
}

void MainWindow::on_actionSortSettings_triggered()
{
    qDebug() << "MainWindow::on_actionSortSetting_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->onChangeSortSettings();
}

void MainWindow::on_actionFilterSettings_triggered()
{
    qDebug() << "MainWindow::on_actionFilterSettings_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->onChangeFilterSettings();
}

void MainWindow::on_actionCopy_triggered()
{
    qDebug() << "MainWindow::on_actionCopy_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->onCopy();
}

void MainWindow::on_actionMove_triggered()
{
    qDebug() << "MainWindow::on_actionMove_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->onMove();
}

void MainWindow::on_actionRemove_triggered()
{
    qDebug() << "MainWindow::on_actionRemove_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->onRemove();
}

void MainWindow::on_actionMakeDirectory_triggered()
{
    qDebug() << "MainWindow::on_actionMakeDirectory_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->onMakeDirectory();
}

void MainWindow::on_actionRename_triggered()
{
    qDebug() << "MainWindow::on_actionRename_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->onRename();
}

void MainWindow::on_actionAttributes_triggered()
{
    qDebug() << "MainWindow::on_actionAttributes_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->onAttributes();
}

void MainWindow::on_actionSelectAll_triggered()
{
    qDebug() << "MainWindow::on_actionSelectAll_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->onSelectAll();
}

void MainWindow::on_actionDeselectAll_triggered()
{
    qDebug() << "MainWindow::on_actionDeselectAll_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->onDeselectAll();
}

void MainWindow::on_actionCopyFullPath_triggered()
{
    qDebug() << "MainWindow::on_actionCopyFullPath_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->onCopyFullPath();
}

void MainWindow::on_actionCopyFileName_triggered()
{
    qDebug() << "MainWindow::on_actionCopyFileName_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->onCopyFileName();
}

void MainWindow::on_actionPrevious_triggered()
{
    qDebug() << "MainWindow::on_actionPrevious_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    FolderForm* activeForm = doubleFolderPanel->getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    activeForm->onPreviousDir();
}

void MainWindow::on_actionNext_triggered()
{
    qDebug() << "MainWindow::on_actionNext_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    FolderForm* activeForm = doubleFolderPanel->getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    activeForm->onNextDir();
}

void MainWindow::on_actionGoToFolder_triggered()
{
    qDebug() << "MainWindow::on_actionGoToFolder_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    FolderForm* activeForm = doubleFolderPanel->getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    activeForm->onChangeDir();
}

void MainWindow::on_actionSetSameFolderToTarget_triggered()
{
    qDebug() << "MainWindow::on_actionSetSameFolderToTarget_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->onSetSameFolderToTarget();
}

void MainWindow::on_actionSetSameFolderFromTarget_triggered()
{
    qDebug() << "MainWindow::on_actionSetSameFolderFromTarget_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->onSetSameFolderFromTarget();
}

void MainWindow::on_actionBookmark_toggled(bool arg1)
{
    qDebug() << "MainWindow::on_actionBookmark_toggled()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    FolderForm* activeForm = doubleFolderPanel->getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    activeForm->onBookmarkDir(arg1);
    activeForm->checkBookmark();
}

void MainWindow::on_actionSelectStorageBookmark_triggered()
{
    qDebug() << "MainWindow::on_actionSelectStorageBookmark_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->onSelectStorageBookmark();
}

void MainWindow::on_actionBookmarkManager_triggered()
{
    qDebug() << "MainWindow::on_actionBookmarkManager_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    FolderForm* activeFolderForm = doubleFolderPanel->getActiveFolderForm();
    Q_ASSERT(activeFolderForm != Q_NULLPTR);

    BookmarkManagerDialog dialog(activeFolderForm->getCurrentDirPath(), this);
    if(dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    checkBookmark();
    activeFolderForm->checkBookmark();

    FolderForm* inactiveFolderForm = doubleFolderPanel->getInactiveFolderForm();
    Q_ASSERT(inactiveFolderForm != Q_NULLPTR);
    inactiveFolderForm->checkBookmark();
}

void MainWindow::on_actionPreferences_triggered()
{
    qDebug() << "MainWindow::on_actionPreferences_triggered()";

    launchPreferencesDialog();
}

void MainWindow::on_actionConsole_triggered(bool checked)
{
    qDebug() << "MainWindow::on_actionConsole_triggered : " << checked;

    setVisibleConsole(checked);
}

void MainWindow::on_consoleDockWidget_visibilityChanged(bool visible)
{
    qDebug() << "MainWindow::on_consoleDockWidget_visibilityChanged : " << visible;

    ui->actionConsole->setChecked(visible);
}

void MainWindow::on_actionAbout_triggered()
{
    qDebug() << "MainWindow::on_actionAbout_triggered()";

    about();
}

int MainWindow::openFile(const QString& path, ViewerType viewerType/* = ViewerType::Auto*/)
{
    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    QFileInfo fileInfo = QFileInfo(path);

    if(path.isEmpty() || !fileInfo.exists())
    {
        return -1;
    }

    if(fileInfo.isDir())
    {
        // ディレクトリ移動
        if(viewerType == ViewerType::Auto)
        {
            FolderForm* activeForm = doubleFolderPanel->getActiveFolderForm();
            Q_ASSERT(activeForm != Q_NULLPTR);

            int ret = activeForm->setPath(path);
            if(ret < 0)
            {
                emitOutputConsole(tr("Folder can not be opened : %1\n").arg(path));
            }
        }
        else
        {
            emitOutputConsole(tr("Folder can not be opened with the viewer : %1\n").arg(path));
        }
    }
    else
    {
        // ファイルを Viewer でオープン
        ViewerBase* viewer = m_viewerDispatcher->dispatcher(path, viewerType);
        if(viewer == Q_NULLPTR)
        {
            return -1;
        }

        connect(viewer,
                SIGNAL(closeViewer(const QString)),
                this,
                SLOT(onCloseViewer(const QString)));

        // 余計な操作ができないよう、ビュアー時はメニューは無効化
        ui->menuBar->setEnabled(false);

        ui->mainWidget->layout()->addWidget(viewer);
        ui->mainWidget->installEventFilter(viewer);

        doubleFolderPanel->setVisible(false);

        viewer->start(this);
    }

    return 0;
}

int MainWindow::closeViewer(const QString& viewerObjectName)
{
    ui->menuBar->setEnabled(true);

    QWidget* viewer = ui->mainWidget->findChild<QWidget*>(viewerObjectName);
    if(viewer == Q_NULLPTR)
    {
        return -1;
    }

    viewer->setVisible(false);
    ui->mainWidget->layout()->removeWidget(viewer);
    delete viewer;

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    if(!doubleFolderPanel->isVisible())
    {
        doubleFolderPanel->setVisible(true);
    }

    return 0;
}

int MainWindow::openWithApp(const QString& path)
{
    if(path.isEmpty() || !QFileInfo(path).exists())
    {
        return -1;
    }

    if(!QDesktopServices::openUrl(QUrl("file:///" + QUrl::toPercentEncoding(path))))
    {
        qDebug() << "open url error:" << path;
        emitOutputConsole(tr("Open failed : %1\n").arg(path));

        return -1;
    }

    return 0;
}

int MainWindow::openWithTextEditor(const QString& dirPath, const QStringList& filePaths)
{
    if(dirPath.isEmpty() || !QDir(dirPath).exists())
    {
        return -1;
    }

    QString appPath = Settings::getInstance()->getTextEditorPath();
    QString args = Settings::getInstance()->getTextEditorArgs();

    if(appPath.isEmpty())
    {
        if(QMessageBox::warning(this,
                                tr("Error"),
                                tr("The external text editor has not been set.<br />Do you want to set it?"),
                                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            launchPreferencesDialog(PreferencesDialogTabPage::ExternalApp);
        }

        return 0;
    }

#ifdef Q_OS_MAC
    QString command = "open -a ";
#else
    QString command = "";
#endif
    args.replace("$M", "\"" + filePaths.join("\" \"") + "\"");          // 複数ファイル
    args.replace("$F", "\"" + filePaths.at(0) + "\"");                  // 最後に選択されたファイル or カーソルが指すファイル

    command += "\"" + appPath + "\" " + args;

    qDebug() << "dirPath : " << dirPath << ", filePaths : " << filePaths << ", command : " << command;

    if(!launchExternalApp(command, dirPath))
    {
        return -1;
    }

    return 0;
}

int MainWindow::openWithTerminal(const QString& dirPath)
{
    if(dirPath.isEmpty() || !QDir(dirPath).exists())
    {
        return -1;
    }

    QString appPath = Settings::getInstance()->getTerminalPath();
    QString args = Settings::getInstance()->getTerminalArgs();

    if(appPath.isEmpty())
    {
        if(QMessageBox::warning(this,
                                tr("Error"),
                                tr("The external terminal has not been set.<br />Do you want to set it?"),
                                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            launchPreferencesDialog(PreferencesDialogTabPage::ExternalApp);
        }

        return 0;
    }

#ifdef Q_OS_MAC
    QString command = "open -n -a ";
#else
    QString command = "";
#endif
    args.replace("$C", "\"" + dirPath + "\"");                  // カレントディレクトリ

    command += "\"" + appPath + "\" " + args;

    qDebug() << "dirPath : " << dirPath << ", command : " << command;

    if(!launchExternalApp(command, dirPath))
    {
        return -1;
    }

    return 0;
}

void MainWindow::updateSettings()
{
    initFont();
    initPalette();

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    doubleFolderPanel->updateSettings();
}

void MainWindow::setVisibleConsole(bool visible)
{
    ui->consoleDockWidget->blockSignals(true);          // visibilityChanged が発生するため、一旦シグナルをブロックする
    ui->consoleDockWidget->setVisible(visible);
    ui->consoleDockWidget->blockSignals(false);

    Settings::getInstance()->setConsoleVisible(visible);
}

QDialog::DialogCode MainWindow::launchPreferencesDialog(PreferencesDialogTabPage page/* = PreferencesDialogTabPage::General*/)
{
    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    FolderForm* l_folderForm = doubleFolderPanel->getFolderForm(PaneType::Left);
    Q_ASSERT(l_folderForm != Q_NULLPTR);

    QString leftDirPath = l_folderForm->getCurrentDirPath();

    FolderForm* r_folderForm = doubleFolderPanel->getFolderForm(PaneType::Right);
    Q_ASSERT(r_folderForm != Q_NULLPTR);

    QString rightDirPath = r_folderForm->getCurrentDirPath();

    PreferencesDialog dialog(this->size(), this->pos(), leftDirPath, rightDirPath, page, this);
    QDialog::DialogCode ret = static_cast<QDialog::DialogCode>(dialog.exec());
    if(ret == QDialog::Accepted)
    {
        updateSettings();
        doubleFolderPanel->refresh();
    }

    return ret;
}

bool MainWindow::launchExternalApp(const QString& command, const QString dirPath)
{
    QProcess process(this);
    process.setWorkingDirectory(dirPath);

    if(!process.startDetached(command))
    {
        QMessageBox::critical(this, tr("Error"), tr("Launch external application failed.") + "<br/>command : " + command);

        return false;
    }

    return true;
}

void MainWindow::checkBookmark()
{
    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    FolderForm* activeForm = doubleFolderPanel->getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    ui->actionBookmark->blockSignals(true);
    if(BookmarkManager::getInstance()->search(activeForm->getCurrentDirPath()) >= 0)
    {
        ui->actionBookmark->setChecked(true);
    }
    else
    {
        ui->actionBookmark->setChecked(false);
    }
    ui->actionBookmark->blockSignals(false);
}

void MainWindow::checkHistory()
{
    qDebug() << "MainWindow::checkHistory()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    Q_ASSERT(doubleFolderPanel != Q_NULLPTR);

    FolderForm* activeForm = doubleFolderPanel->getActiveFolderForm();
    Q_ASSERT(activeForm != Q_NULLPTR);

    ui->actionPrevious->setEnabled(!activeForm->getPreviousDirPath().isEmpty());
    ui->actionNext->setEnabled(!activeForm->getNextDirPath().isEmpty());

    updateHistoryMenu(activeForm->getHistoryManager());
}

void MainWindow::updateHistoryMenu(const HistoryManager* historyManager)
{
    QMenu* menuHistory = ui->menuGo->findChild<QMenu*>("menuHistory", Qt::FindDirectChildrenOnly);

    Q_ASSERT(menuHistory != Q_NULLPTR);

    clearHistoryMenu(menuHistory);

    for(auto action : menuHistory->actions())
    {
        qDebug() << action->objectName();
        action->setVisible(false);
    }

    const QList<Qt::Key> shortCutKeys =
    {
        Qt::Key_1,
        Qt::Key_2,
        Qt::Key_3,
        Qt::Key_4,
        Qt::Key_5,
        Qt::Key_6,
        Qt::Key_7,
        Qt::Key_8,
        Qt::Key_9,
        Qt::Key_0,
    };

    int keysIndex = 0;
    for(auto history : historyManager->getList())
    {
        QAction* historyAction = new QAction(history);
        connect(historyAction, SIGNAL(triggered()), this, SLOT(onActionHistoryTriggered()));
        if(keysIndex < shortCutKeys.length())
        {
            historyAction->setShortcut(QKeySequence(Qt::CTRL + shortCutKeys[keysIndex]));
            keysIndex++;
        }

        menuHistory->addAction(historyAction);
    }
}

void MainWindow::clearHistoryMenu(QMenu* menuHistory)
{
    for(auto historyAction : menuHistory->actions())
    {
        if(historyAction->isVisible())
        {
            disconnect(historyAction, SIGNAL(triggered()), this, SLOT(onActionHistoryTriggered()));
            menuHistory->removeAction(historyAction);
        }
        else
        {
            historyAction->setVisible(true);
        }
    }
}

void MainWindow::about()
{
    AboutDialog dialog(this);
    dialog.exec();
}

}           // namespace Farman
