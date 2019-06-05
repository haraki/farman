#include <QDebug>
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
#include "viewerdispatcher.h"
#include "imageviewer.h"
#include "file.h"

namespace Farman
{

MainWindow::MainWindow(QWidget *parent/* = Q_NULLPTR*/)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_file(Q_NULLPTR)
    , m_viewerDispatcher(Q_NULLPTR)
{
    ui->setupUi(this);

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
            SIGNAL(changeFileAttributes(const QString&, const QFile::Permissions&, const QDateTime&, const QDateTime&)),
            m_file,
            SLOT(onChangeFileAttributes(const QString&, const QFile::Permissions&, const QDateTime&, const QDateTime&)));

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

int MainWindow::openFile(const QString& path, ViewerType viewerType/* = ViewerType::Auto*/)
{
    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel == Q_NULLPTR)
    {
        return -1;
    }

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
            FolderForm* activeFolderForm = doubleFolderPanel->getActiveFolderForm();
            if(activeFolderForm == Q_NULLPTR)
            {
                return -1;
            }

            int ret = activeFolderForm->setPath(path);
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
    if(doubleFolderPanel == Q_NULLPTR)
    {
        return -1;
    }

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

    if(!QDesktopServices::openUrl(QUrl("file:///" + path)))
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

void MainWindow::onOpenFile(ViewerType viewerType)
{
    qDebug() << "MainWindow::onOpenFile()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel == Q_NULLPTR)
    {
        return;
    }

    FolderForm* activeFolderForm = doubleFolderPanel->getActiveFolderForm();
    if(activeFolderForm == Q_NULLPTR)
    {
        return;
    }

    QString path = activeFolderForm->getCurrentFileInfo().absoluteFilePath();

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
    if(doubleFolderPanel == Q_NULLPTR)
    {
        return;
    }

    FolderForm* activeFolderForm = doubleFolderPanel->getActiveFolderForm();
    if(activeFolderForm == Q_NULLPTR)
    {
        return;
    }

    const QString path = activeFolderForm->getCurrentFileInfo().absoluteFilePath();

    onOpenWithApp(path);
}

void MainWindow::on_actionOpenWithTextEditor_triggered()
{
    qDebug() << "MainWindow::on_actionOpenWithTextEditor_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel == Q_NULLPTR)
    {
        return;
    }

    FolderForm* activeFolderForm = doubleFolderPanel->getActiveFolderForm();
    if(activeFolderForm == Q_NULLPTR)
    {
        return;
    }

    QList<QFileInfo> selectedFileInfoList = activeFolderForm->getSelectedFileInfoList();
    if(selectedFileInfoList.size() == 0)
    {
        return;
    }

    QStringList filePaths;
    for(auto fileInfo : selectedFileInfoList)
    {
        filePaths.push_back(fileInfo.absoluteFilePath());
    }

    onOpenWithTextEditor(activeFolderForm->getCurrentDirPath(), filePaths);
}

void MainWindow::on_actionCreateNewFile_triggered()
{
    qDebug() << "MainWindow::on_actionCreateNewFile_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->onCreateNewFile();
    }
}

void MainWindow::on_actionPreferences_triggered()
{
    qDebug() << "MainWindow::on_actionPreferences_triggered()";

    launchPreferencesDialog();
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
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->onSetPaneMode(PaneMode::Single);
    }
}

void MainWindow::on_actionDualPane_triggered()
{
    qDebug() << "MainWindow::on_actionDualPane_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->onSetPaneMode(PaneMode::Dual);
    }
}

void MainWindow::on_actionSortSettings_triggered()
{
    qDebug() << "MainWindow::on_actionSortSetting_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->onChangeSortSettings();
    }
}

void MainWindow::on_actionFilterSettings_triggered()
{
    qDebug() << "MainWindow::on_actionFilterSettings_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->onChangeFilterSettings();
    }
}

void MainWindow::on_actionCopy_triggered()
{
    qDebug() << "MainWindow::on_actionCopy_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->onCopy();
    }
}

void MainWindow::on_actionMove_triggered()
{
    qDebug() << "MainWindow::on_actionMove_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->onMove();
    }
}

void MainWindow::on_actionRemove_triggered()
{
    qDebug() << "MainWindow::on_actionRemove_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->onRemove();
    }
}

void MainWindow::on_actionMakeDirectory_triggered()
{
    qDebug() << "MainWindow::on_actionMakeDirectory_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->onMakeDirectory();
    }
}

void MainWindow::on_actionRename_triggered()
{
    qDebug() << "MainWindow::on_actionRename_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->onRename();
    }
}

void MainWindow::on_actionAttributes_triggered()
{
    qDebug() << "MainWindow::on_actionAttributes_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->onAttributes();
    }
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

void MainWindow::updateSettings()
{
    initFont();
    initPalette();

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->updateSettings();
    }
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
    QString leftDirPath = "";
    QString rightDirPath = "";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        FolderForm* l_folderForm = doubleFolderPanel->getLeftFolderForm();
        if(l_folderForm != Q_NULLPTR)
        {
            leftDirPath = l_folderForm->getCurrentDirPath();
        }

        FolderForm* r_folderForm = doubleFolderPanel->getRightFolderForm();
        if(r_folderForm != Q_NULLPTR)
        {
            rightDirPath = r_folderForm->getCurrentDirPath();
        }
    }

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

void MainWindow::about()
{
    QMessageBox::about(this,
                       tr("About farman"),
                       tr("<font size = '+8'>farman</font>") +
                       tr("<p>Version ") + APP_VERSION + "</p>" +
                       tr("<p>Copyright 2018 MASHSOFT All right reserved.</p>"));

}

}           // namespace Farman
