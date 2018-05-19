﻿#include <QDebug>
#include <QKeyEvent>
#include <QHeaderView>
#include <QFileDialog>
#include <QItemSelectionModel>
#include "mainwindow.h"
#include "folderform.h"
#include "ui_folderform.h"
#include "foldermodel.h"
#include "settings.h"

namespace Farman
{

FolderForm::FolderForm(QDir::Filters filterFlags,
                       SectionType sortSectionType,
                       SortDirsType sortDirsType,
                       bool sortDotFirst,
                       Qt::CaseSensitivity sortCaseSensitivity,
                       Qt::SortOrder sortOrder,
                       QWidget *parent/* = Q_NULLPTR*/)
    : QWidget(parent)
    , ui(new Ui::FolderForm)
    , m_folderModel(new FolderModel(this))
    , m_beforePath()
    , m_isSettingPath(false)
{
    ui->setupUi(this);

    m_folderModel->setReadOnly(true);
    m_folderModel->setDynamicSortFilter(true);
    m_folderModel->setSortLocaleAware(true);
    m_folderModel->setFilter(filterFlags);
    m_folderModel->setSortSectionType(sortSectionType);
    m_folderModel->setSortDirsType(sortDirsType);
    m_folderModel->setSortDotFirst(sortDotFirst);
    m_folderModel->setSortCaseSensitivity(sortCaseSensitivity);
    m_folderModel->setSortOrder(sortOrder);
    ui->folderView->setModel(m_folderModel);

    ui->folderView->setSelectionModel(m_folderModel->getSelectionModel());

    connect(m_folderModel->getSelectionModel(),
            SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,
            SLOT(onCurrentChanged(const QModelIndex&, const QModelIndex&)));
    connect(ui->folderView,
            SIGNAL(doubleClicked(const QModelIndex&)),
            MainWindow::getInstance(),
            SLOT(onOpen(const QModelIndex&)));
    connect(m_folderModel,
            SIGNAL(directoryLoaded(const QString&)),
            this,
            SLOT(onDirectoryLoaded(const QString&)));

    ui->folderView->installEventFilter(this);
}

FolderForm::~FolderForm()
{
    delete m_folderModel;
    delete ui;
}

void FolderForm::updateSettings()
{
    initFont();
    initPalette();

    m_folderModel->updateSettings();
}

void FolderForm::initFont()
{
    ui->folderPathEdit->setFont(Settings::getInstance()->getFontSetting("folderPath"));
}

void FolderForm::initPalette()
{
    QPalette pal;

    pal = ui->folderView->palette();
    pal.setColor(QPalette::Base, Settings::getInstance()->getColorSetting("folderView_background"));
    ui->folderView->setAutoFillBackground(true);
    ui->folderView->setPalette(pal);

    pal = ui->folderPathEdit->palette();
    pal.setColor(QPalette::Text, Settings::getInstance()->getColorSetting("folderPath_text"));
    pal.setColor(QPalette::Base, Settings::getInstance()->getColorSetting("folderPath_background"));
    ui->folderPathEdit->setAutoFillBackground(true);
    ui->folderPathEdit->setPalette(pal);
}

bool FolderForm::eventFilter(QObject *watched, QEvent *e)
{
    Q_UNUSED(watched);

    bool ret = false;

    switch (e->type())
    {
    case QEvent::FocusIn:
        emitFocusChanged(true);

        break;

    case QEvent::FocusOut:
        emitFocusChanged(false);
        break;

    default:
        break;
    }

    return ret;
}

void FolderForm::setFilterFlags(QDir::Filters filterFlags)
{
    m_folderModel->setFilter(filterFlags);

    refresh();
}

QDir::Filters FolderForm::getFilterFlags() const
{
    return m_folderModel->filter();
}

void FolderForm::setSortSettings(SectionType sectionType, SortDirsType dirsType, bool dotFirst, Qt::CaseSensitivity caseSensitivity, Qt::SortOrder order)
{
    m_folderModel->setSortSectionType(sectionType);
    m_folderModel->setSortDirsType(dirsType);
    m_folderModel->setSortDotFirst(dotFirst);
    m_folderModel->setSortCaseSensitivity(caseSensitivity);
    m_folderModel->setSortOrder(order);

    refresh();
}

SectionType FolderForm::getSortSectionType() const
{
    return m_folderModel->sortSectionType();
}

int FolderForm::getSortColumn() const
{
    return m_folderModel->sortColumn();
}

SortDirsType FolderForm::getSortDirsType() const
{
    return m_folderModel->sortDirsType();
}

bool FolderForm::getSortDotFirst() const
{
    return m_folderModel->sortDotFirst();
}

Qt::CaseSensitivity FolderForm::getSortCaseSensitivity() const
{
    return m_folderModel->sortCaseSensitivity();
}

Qt::SortOrder FolderForm::getSortOrder() const
{
    return m_folderModel->sortOrder();
}

int FolderForm::setPath(const QString& dirPath, const QString& beforePath/* = QString() */)
{
    if(m_isSettingPath)
    {
        // 前回の setPath() が終わっていない
        qDebug() << "previous setPath() has not ended.";

        return -1;
    }

    QDir dir(dirPath);
    QDir::Filters filterFlags;

    // ルートディレクトリの場合は".."を表示しないようにする
    if(dir.isRoot())
    {
        filterFlags = m_folderModel->filter() | QDir::NoDotDot;
    }
    else
    {
        filterFlags = m_folderModel->filter() & ~QDir::NoDotDot;
    }

    if(dir.entryInfoList(filterFlags).size() == 0)
    {
        qDebug() << dirPath << " size() == 0";

        return -1;
    }

    m_beforePath = beforePath;
    m_isSettingPath = true;

    m_folderModel->clearSelected();

    m_folderModel->setFilter(filterFlags);

    m_folderModel->setRootPath(dirPath);

    ui->folderPathEdit->setText(dirPath);

    return 0;
}

QString FolderForm::getCurrentDirPath()
{
    return m_folderModel->filePath(ui->folderView->rootIndex());
}

QString FolderForm::getCurrentFileName()
{
    return m_folderModel->fileName(ui->folderView->currentIndex());
}

QFileInfo FolderForm::getCurrentFileInfo()
{
    return m_folderModel->fileInfo(ui->folderView->currentIndex());
}

QList<QFileInfo> FolderForm::getSelectedFileInfoList()
{
    QModelIndexList indexList = m_folderModel->getSelectedIndexList();
    if(indexList.size() == 0 && getCurrentFileName() != "..")
    {
        // 選択しているアイテムが無くても、".." 以外の箇所にカーソルがあれば、それを選択しているものとする
        indexList.push_back(ui->folderView->currentIndex());
    }

    QList<QFileInfo> selectedFileInfoList;
    for(auto modelIndex : indexList)
    {
        selectedFileInfoList.push_back(m_folderModel->fileInfo(modelIndex).absoluteFilePath());
    }

    return selectedFileInfoList;
}

void FolderForm::onCurrentChanged(const QModelIndex& newIndex, const QModelIndex& oldIndex)
{
    qDebug() << "FolderForm::onCurrentChanged : old : " << oldIndex.row() << " new : " << newIndex.row();

    QModelIndex topLeft, bottomRight;
    if(newIndex.row() < oldIndex.row())
    {
        topLeft = m_folderModel->index(newIndex.row(), 0);
        bottomRight = m_folderModel->index(oldIndex.row(), m_folderModel->columnCount());
    }
    else
    {
        topLeft = m_folderModel->index(oldIndex.row(), 0);
        bottomRight = m_folderModel->index(newIndex.row(), m_folderModel->columnCount());
    }

    // カーソルが移動した箇所を再描画する
    emit ui->folderView->refresh(topLeft, bottomRight);

    emitCurrentChanged((newIndex.row() >= 0) ? m_folderModel->fileInfo(newIndex) : QFileInfo(),
                       (oldIndex.row() >= 0) ? m_folderModel->fileInfo(oldIndex) : QFileInfo());
}

void FolderForm::onDirectoryLoaded(const QString& path)
{
    qDebug() << "directory loaded." << path;

    if(m_isSettingPath)
    {
        // setPath() によって発生した場合はカーソル位置を再設定する
        QModelIndex newDirIndex = m_folderModel->index(path);
        ui->folderView->setRootIndex(newDirIndex);

        QModelIndex newCursorIndex;

        if(!m_beforePath.isEmpty())
        {
            // 前回のパスが子ディレクトリであれば、そこを初期カーソル位置とする
            newCursorIndex = m_folderModel->index(m_beforePath);
            m_beforePath.clear();
        }

        if(!newCursorIndex.isValid() || newCursorIndex.parent() != newDirIndex || newCursorIndex.row() < 0)
        {
            // 初期カーソル位置はリストの先頭
            newCursorIndex = m_folderModel->index(0, 0, newDirIndex);
        }

        ui->folderView->setCursor(newCursorIndex);

        m_isSettingPath = false;
    }
}

void FolderForm::setCursor(const QString& fileName)
{
    const QString currentPath = getCurrentDirPath();
    QModelIndex newIndex = m_folderModel->index(QFileInfo(currentPath, fileName).absoluteFilePath());

    ui->folderView->setCursor(newIndex);
}

int FolderForm::onGoToChildDir()
{
    const QModelIndex currentIndex = ui->folderView->currentIndex();

    if(!m_folderModel->isDir(currentIndex))
    {
        return -1;
    }

    const QString newPath = m_folderModel->filePath(currentIndex);

    qDebug() << "================== onGoToChildDir() : " << currentIndex << " -> " << newPath;

    const QString currentPath = m_folderModel->filePath(ui->folderView->rootIndex());

    return setPath(newPath, currentPath);
}

int FolderForm::onGoToParentDir()
{
    const QModelIndex currentDirIndex = ui->folderView->rootIndex();
    const QString currentPath = m_folderModel->filePath(currentDirIndex);

#ifdef Q_OS_WIN
    if(currentPath == "")
#else           // Q_OS_WIN
    if(QDir(currentPath).isRoot())
#endif          // Q_OS_WIN
    {
        return -1;
    }

    const QString newPath = m_folderModel->filePath(currentDirIndex.parent());

    qDebug() << "================== onGoToParentDir() : " << newPath;

    return setPath(newPath, currentPath);
}

void FolderForm::refresh()
{
    m_folderModel->refresh();

    QModelIndex cursorIndex = ui->folderView->currentIndex();
    if(!cursorIndex.isValid() || cursorIndex.parent() != ui->folderView->rootIndex() || cursorIndex.row() < 0)
    {
        cursorIndex = m_folderModel->index(0, 0, ui->folderView->rootIndex());
    }

    ui->folderView->setCursor(cursorIndex);
}

int FolderForm::getTotalColumnWidth(int withOutColumn)
{
    int totalWidth = 0;

    for(int i = 0;i < ui->folderView->horizontalHeader()->count();i++)
    {
        if(i != withOutColumn)
        {
            totalWidth += ui->folderView->columnWidth(i);
        }
    }

    return totalWidth;
}

void FolderForm::on_folderSelectButton_clicked()
{
    const QString currentPath = m_folderModel->filePath(ui->folderView->rootIndex());

    QString dirPath = QFileDialog::getExistingDirectory(this,
                                                        tr("Select folder."),
                                                        currentPath,
                                                        QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly);

    if(!dirPath.isEmpty())
    {
        setPath(dirPath, currentPath);
    }
}

void FolderForm::emitCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo)
{
    emit currentChanged(newFileInfo, oldFileInfo);
}

void FolderForm::emitFocusChanged(bool inFocus)
{
    emit focusChanged(inFocus);
}

}           // namespace Farman
