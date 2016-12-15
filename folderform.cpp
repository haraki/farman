#include <QDebug>
#include <QResizeEvent>
#include <QHeaderView>
#include "folderform.h"
#include "ui_folderform.h"
#include "foldermodel.h"

FolderForm::FolderForm(QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FolderForm),
    m_filterFlags(filterFlags),
    m_sortFlags(sortFlags),
    m_folderModel(nullptr)
{
    ui->setupUi(this);

    m_folderModel = new FolderModel(this);
    m_folderModel->setReadOnly(true);
    m_folderModel->setFilter(m_filterFlags);
    m_folderModel->setSorting(m_sortFlags);

    ui->folderView->setModel(m_folderModel);

    ui->folderView->installEventFilter(this);

    connect(ui->folderView->horizontalHeader(), SIGNAL(sectionResized(int,int,int)),       this, SLOT(onColumnResized(int,int,int)));
    connect(ui->folderView,                     SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onOpen(const QModelIndex&)));
}

FolderForm::~FolderForm()
{
    delete m_folderModel;
    delete ui;
}

void FolderForm::setFilterFlags(QDir::Filters filterFlags)
{
    m_filterFlags = filterFlags;
    if(m_folderModel != nullptr)
    {
        m_folderModel->setFilter(filterFlags);
    }
}

void FolderForm::setSortFlags(QDir::SortFlags sortFlags)
{
    m_sortFlags = sortFlags;
    if(m_folderModel != nullptr)
    {
        m_folderModel->setSorting(sortFlags);
    }
}

void FolderForm::setPath(const QString& dirPath, const QString& beforePath/* = QString() */)
{
    if(m_folderModel != nullptr)
    {
        QDir dir(dirPath);
        QDir::Filters filterFlags;

        if(dir.isRoot())
        {
            filterFlags = m_filterFlags | QDir::NoDotDot;
        }
        else
        {
            filterFlags = m_filterFlags & ~QDir::NoDotDot;
        }

        if(dir.entryInfoList(filterFlags, m_sortFlags).size() == 0)
        {
            qDebug() << dirPath << " size() == 0";
            return;
        }

        m_folderModel->clearChecked();
        m_folderModel->setFilter(filterFlags);

        QModelIndex newDirIndex = m_folderModel->index(dirPath);
        ui->folderView->setRootIndex(newDirIndex);

        QModelIndex beforeIndex;
        if(!beforePath.isEmpty())
        {
            beforeIndex = m_folderModel->index(beforePath);
        }

        if(beforeIndex.parent() != newDirIndex || beforeIndex.row() < 0)
        {
            ui->folderView->selectRow(0);
            ui->folderView->scrollToTop();
        }
        else
        {
            ui->folderView->setCurrentIndex(beforeIndex);
            ui->folderView->scrollTo(beforeIndex);
        }

        ui->lineEdit->setText(dirPath);
    }
}

void FolderForm::resizeEvent(QResizeEvent *event)
{
//    QSize diff = event->size() - event->oldSize();

//    qDebug() << "<<<< RESIZE >>>> " << event->size();

    // FolderForm がリサイズされる場合は先頭カラムの幅が変わる
    // その際 columnResized() が呼び出されないよう、一時的にシグナルを止める
    ui->folderView->horizontalHeader()->blockSignals(true);
    ui->folderView->setColumnWidth(0, event->size().width() - getTotalColumnWidth(0));
    ui->folderView->horizontalHeader()->blockSignals(false);
}

void FolderForm::onColumnResized(int column, int oldWidth, int newWidth)
{
//    qDebug() << "resizeSection(" << column << ", " << oldWidth << ", " << newWidth << ")";

    int lastColumn = ui->folderView->horizontalHeader()->count() - 1;

    if(column < lastColumn)
    {
        // 最後尾カラムの幅を算出
        int newLastColumnWidth = ui->folderView->columnWidth(lastColumn) - (newWidth - oldWidth);
        if(newLastColumnWidth < ui->folderView->horizontalHeader()->minimumSectionSize())
        {
            newLastColumnWidth = ui->folderView->horizontalHeader()->minimumSectionSize();
        }

        ui->folderView->horizontalHeader()->blockSignals(true);

        int totalColumnWidth = getTotalColumnWidth(lastColumn);
        if(ui->folderView->width() < totalColumnWidth + newLastColumnWidth)
        {
            // カラム幅変更後の全カラムの幅の合計値が FolderView のサイズ内に収まらない場合、
            // 変更されたカラムの幅、及び最後尾カラムの幅を再算出して設定する
            int newColumnWidth = ui->folderView->width() - getTotalColumnWidth(column);
            ui->folderView->setColumnWidth(column, newColumnWidth);

            newLastColumnWidth = ui->folderView->width() - getTotalColumnWidth(lastColumn);
        }
        ui->folderView->setColumnWidth(lastColumn, newLastColumnWidth);

        ui->folderView->horizontalHeader()->blockSignals(false);
    }
}

void FolderForm::onOpen(const QModelIndex& index)
{
    Q_UNUSED(index);

    onOpen();
}

void FolderForm::onOpen()
{
    const QModelIndex currentIndex = ui->folderView->currentIndex();

    if(m_folderModel->isDir(currentIndex))
    {
        const QString newPath = m_folderModel->filePath(currentIndex);

        qDebug() << "================== onOpen() : " << currentIndex << " -> " << newPath;

        const QString beforePath = m_folderModel->filePath(ui->folderView->rootIndex());

        setPath(newPath, beforePath);
    }
}

void FolderForm::onGoToParent()
{
    const QModelIndex currentDirIndex = ui->folderView->rootIndex();
    const QString currentPath = m_folderModel->filePath(currentDirIndex);

    if(!QDir(currentPath).isRoot())
    {
        const QString newPath = m_folderModel->filePath(currentDirIndex.parent());

        qDebug() << "================== onGoToParent() : " << newPath;

        setPath(newPath, currentPath);
    }
}

void FolderForm::onToggleCheck()
{
    QModelIndex currentIndex = ui->folderView->currentIndex();

    if(m_folderModel->fileName(currentIndex) != "..")
    {
        m_folderModel->toggleCheck(currentIndex);
    }

//    qDebug() << currentIndex.row() << "," << m_folderModel->rowCount(currentIndex.parent());

    if(currentIndex.row() + 1 < m_folderModel->rowCount(currentIndex.parent()))
    {
        // カーソルを1行下に移動
        QModelIndex newIndex = m_folderModel->index(currentIndex.row() + 1, 0, currentIndex.parent());
        ui->folderView->setCurrentIndex(newIndex);
    }
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

bool FolderForm::eventFilter(QObject *watched, QEvent *e)
{
    Q_UNUSED(watched);

    bool ret = false;

    switch (e->type()) {
    case QEvent::KeyPress:
    {
        Qt::Key key = static_cast<Qt::Key>(dynamic_cast<QKeyEvent*>(e)->key());

        qDebug() << key;

        switch(key)
        {
        case Qt::Key_Return:
            this->onOpen();

            ret = true;

            break;

        case Qt::Key_Left:
            this->onGoToParent();

            ret = true;

            break;

        case Qt::Key_Space:
            this->onToggleCheck();

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
