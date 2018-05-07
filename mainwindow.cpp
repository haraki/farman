#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "folderview.h"
#include "foldermodel.h"
#include "folderform.h"
#include "doublefolderpanel.h"
#include "settings.h"
#include "preferencesdialog.h"
#include "viewerdispatcher.h"
#include "imageviewer.h"
#include "file.h"

namespace Farman
{

MainWindow* MainWindow::s_instance = Q_NULLPTR;

void MainWindow::create()
{
    Q_ASSERT(s_instance == Q_NULLPTR);
    s_instance = new MainWindow();
    Q_ASSERT(s_instance != Q_NULLPTR);
    s_instance->initialize();
    s_instance->show();
}

MainWindow* MainWindow::getInstance()
{
    return s_instance;
}

MainWindow::MainWindow(QWidget *parent/* = Q_NULLPTR*/)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_nameFilters()
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialize()
{
    File::create(this);
    connect(File::getInstance(),
            SIGNAL(outputConsole(const QString)),
            this,
            SLOT(onOutputConsole(const QString)));

    DoubleFolderPanel* doubleFolderPanel = new DoubleFolderPanel(ui->mainWidget);

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

    resizeDocks({ui->consoleDockWidget}, {ui->consoleDockWidget->minimumHeight()}, Qt::Vertical);
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

void MainWindow::onOpen(const QModelIndex& index/* = QModelIndex()*/)
{
    Q_UNUSED(index);

    qDebug() << "MainWindow::onOpen()";

    onOpen(ViewerType::Auto);
}

void MainWindow::onOpen(ViewerType viewerType)
{
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

    QFileInfo fileInfo = activeFolderForm->getCurrentFileInfo();
    if(fileInfo.isDir())
    {
        if(viewerType == ViewerType::Auto)
        {
            int ret = 0;

            if(fileInfo.fileName() == "..")
            {
                ret = activeFolderForm->onGoToParentDir();
            }
            else
            {
                ret = activeFolderForm->onGoToChildDir();
            }

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

    ViewerBase* viewer = ViewerDispatcher::getInstance()->dispatcher(fileInfo.absoluteFilePath(), viewerType, ui->mainWidget);
    if(viewer == Q_NULLPTR)
    {
        return;
    }

    // 余計な操作ができないよう、ビュアー時はメニューは無効化
    ui->menuBar->setEnabled(false);

    ui->mainWidget->layout()->addWidget(viewer);
    ui->mainWidget->installEventFilter(viewer);

    doubleFolderPanel->setVisible(false);

    viewer->start();
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

void MainWindow::onOpenWithApp(const QModelIndex& index/* = QModelIndex()*/)
{
    Q_UNUSED(index);

    qDebug() << "MainWindow::onOpenWithApp()";

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

    onOpen();
}

void MainWindow::on_actionOpenWithApp_triggered()
{
    qDebug() << "MainWindow::on_actionOpenWithApp_triggered()";

    onOpenWithApp();
}

void MainWindow::on_actionOpenWithTextViewer_triggered()
{
    qDebug() << "MainWindow::on_actionOpenWithTextViewer_triggered()";

    onOpen(ViewerType::Text);
}

void MainWindow::on_actionOpenWithHexViewer_triggered()
{
    qDebug() << "MainWindow::on_actionOpenWithHexViewer_triggered()";

    onOpen(ViewerType::Hex);
}

void MainWindow::on_actionOpenWithImageViewer_triggered()
{
    qDebug() << "MainWindow::on_actionOpenWithImageViewer_triggered()";

    onOpen(ViewerType::Image);
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
    }
}

void MainWindow::on_actionQuit_triggered()
{
    qDebug() << "MainWindow::on_actionQuit_triggered()";

    close();
}

void MainWindow::on_actionSingleView_triggered()
{
    qDebug() << "MainWindow::on_actionSingleView_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->onSetViewMode(ViewMode::Single);
    }
}

void MainWindow::on_actionDoubleView_triggered()
{
    qDebug() << "MainWindow::on_actionDoubleView_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->onSetViewMode(ViewMode::Double);
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

void MainWindow::updateSettings()
{
    initFont();
    initPalette();

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->updateSettings();
        doubleFolderPanel->refresh();
    }
}

}           // namespace Farman
