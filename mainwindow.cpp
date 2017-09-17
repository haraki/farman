﻿#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "folderview.h"
#include "foldermodel.h"
#include "folderform.h"
#include "doublefolderpanel.h"
#include "sortdialog.h"
#include "filterdialog.h"
#include "settings.h"

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
    ViewMode viewMode = static_cast<ViewMode>(Settings::getInstance()->value("main/viewMode", static_cast<int>(ViewMode::Double)).toInt());

    QString path = QDir::currentPath();
    qDebug() << path;

    DoubleFolderPanel* doubleFolderPanel = new DoubleFolderPanel(viewMode,
                                                                 path, DEFAULT_FILTER_FLAGS, DEFAULT_SORT_FLAGS,
                                                                 path, DEFAULT_FILTER_FLAGS, DEFAULT_SORT_FLAGS,
                                                                 ui->mainWidget);

    ui->mainWidget->layout()->addWidget(doubleFolderPanel);

    resizeDocks({ui->consoleDockWidget}, {ui->consoleDockWidget->minimumHeight()}, Qt::Vertical);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        ViewMode viewMode = doubleFolderPanel->getViewMode();
        Settings::getInstance()->setValue("main/viewMode", static_cast<int>(viewMode));
    }

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

void MainWindow::on_actionSingleView_triggered()
{
    qDebug() << "MainWindow::on_actionSingleView_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->changeViewMode(ViewMode::Single);
    }
}

void MainWindow::on_actionDoubleView_triggered()
{
    qDebug() << "MainWindow::on_actionDoubleView_triggered()";

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        doubleFolderPanel->changeViewMode(ViewMode::Double);
    }
}

void Farman::MainWindow::on_actionSortSettings_triggered()
{
    qDebug() << "MainWindow::on_actionSortSetting_triggered()";

    QDir::SortFlags sortFlags = DEFAULT_SORT_FLAGS;

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        FolderForm* activeFolderForm = doubleFolderPanel->getActiveFolderForm();
        if(activeFolderForm != Q_NULLPTR)
        {
            sortFlags = activeFolderForm->getSortFlags();

            SortDialog dialog(sortFlags, this);
            if(dialog.exec())
            {
                sortFlags = dialog.getSortFlags();
                activeFolderForm->setSortFlags(sortFlags);
            }
        }
    }
}

void MainWindow::on_actionFilterSettings_triggered()
{
    qDebug() << "MainWindow::on_actionFilterSettings_triggered()";

    QDir::Filters filterFlags = DEFAULT_FILTER_FLAGS;

    DoubleFolderPanel* doubleFolderPanel = ui->mainWidget->findChild<DoubleFolderPanel*>("DoubleFolderPanel");
    if(doubleFolderPanel != Q_NULLPTR)
    {
        FolderForm* activeFolderForm = doubleFolderPanel->getActiveFolderForm();
        if(activeFolderForm != Q_NULLPTR)
        {
            filterFlags = activeFolderForm->getFilterFlags();

            FilterDialog dialog(filterFlags, this);
            if(dialog.exec())
            {
                filterFlags = dialog.getFilterFlags();
                activeFolderForm->setFilterFlags(filterFlags);
            }
        }
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

}           // namespace Farman
