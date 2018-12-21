#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>
#include <QMessageBox>
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
    , m_nameFilters()
{
    ui->setupUi(this);

    m_file = new File(this);
    m_viewerDispatcher = new ViewerDispatcher(ui->mainWidget);

    DoubleFolderPanel* doubleFolderPanel = new DoubleFolderPanel(ui->mainWidget);

    connect(m_file,
            SIGNAL(outputConsole(const QString)),
            this,
            SLOT(onOutputConsole(const QString)));

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
            SIGNAL(renameFile(const QString&, const QString&, const QString&)),
            m_file,
            SLOT(onRenameFile(const QString&, const QString&, const QString&)));
    connect(doubleFolderPanel,
            SIGNAL(changeFileAttributes(const QString&, const QFile::Permissions&, const QDateTime&, const QDateTime&)),
            m_file,
            SLOT(onChangeFileAttributes(const QString&, const QFile::Permissions&, const QDateTime&, const QDateTime&)));

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
        if(QMessageBox::question(this, tr("Confirm"), tr("quit?")) != QMessageBox::Yes)
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

    onOpenFile(path, viewerType);
}

void MainWindow::onOpenFile(const QString& path, ViewerType viewerType/* = ViewerType::Auto*/)
{
    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel == Q_NULLPTR)
    {
        return;
    }

    if(QFileInfo(path).isDir())
    {
        if(viewerType == ViewerType::Auto)
        {
            FolderForm* activeFolderForm = doubleFolderPanel->getActiveFolderForm();
            if(activeFolderForm == Q_NULLPTR)
            {
                return;
            }

            int ret = activeFolderForm->setPath(path);
            if(ret < 0)
            {
                onOutputConsole(tr("Folder can not be opened.\n"));
            }
        }
        else
        {
            onOutputConsole(tr("Folder can not be opened with the viewer.\n"));
        }

        return;
    }

    ViewerBase* viewer = m_viewerDispatcher->dispatcher(path, viewerType);
    if(viewer == Q_NULLPTR)
    {
        return;
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

void MainWindow::onCloseViewer(const QString& viewerObjectName)
{
    qDebug() << "MainWindow::onCloseViewer()";

    ui->menuBar->setEnabled(true);

    QWidget* viewer = ui->mainWidget->findChild<QWidget*>(viewerObjectName);
    if(viewer == Q_NULLPTR)
    {
        return;
    }

    viewer->setVisible(false);
    ui->mainWidget->layout()->removeWidget(viewer);
    delete viewer;

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel == Q_NULLPTR)
    {
        return;
    }

    if(!doubleFolderPanel->isVisible())
    {
        doubleFolderPanel->setVisible(true);
    }
}

void MainWindow::onOpenWithApp(const QString& path)
{
    qDebug() << "MainWindow::onOpenWithApp()";

    if(!QDesktopServices::openUrl(QUrl("file:///" + path)))
    {
        qDebug() << "open url error:" << path;
        onOutputConsole(tr("Open failed : ") + path);
    }
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

void MainWindow::on_actionPreferences_triggered()
{
    qDebug() << "MainWindow::on_actionPreferences_triggered()";

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

    PreferencesDialog dialog(this->size(), this->pos(), leftDirPath, rightDirPath, this);
    if(dialog.exec() == QDialog::Accepted)
    {
        updateSettings();
        doubleFolderPanel->refresh();
    }
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

void MainWindow::about()
{
    QMessageBox::about(this,
                       tr("About farman"),
                       tr("<font size = '+8'>farman</font>") +
                       tr("<p>Version ") + APP_VERSION + "</p>" +
                       tr("<p>Copyright 2018 MASHSOFT All right reserved.</p>"));

}

}           // namespace Farman
