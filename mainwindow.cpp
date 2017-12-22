#include <QDebug>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "folderview.h"
#include "foldermodel.h"
#include "folderform.h"
#include "doublefolderpanel.h"
#include "settings.h"
#include "optiondialog.h"

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

void MainWindow::on_actionOpenInApp_triggered()
{
    qDebug() << "MainWindow::on_actionOpenInApp_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->onOpenInApp();
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

void Farman::MainWindow::on_actionSortSettings_triggered()
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

void MainWindow::on_actionOption_triggered()
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

    OptionDialog dialog(this->size(), this->pos(), leftDirPath, rightDirPath, this);
    if(dialog.exec() == QDialog::Accepted)
    {
        updateSettings();
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
