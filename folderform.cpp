#include <QDebug>
#include <QResizeEvent>
#include <QHeaderView>
#include "folderform.h"
#include "ui_folderform.h"
#include "foldermodel.h"

FolderForm::FolderForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FolderForm),
    m_filterFlags(QDir::AllEntries),
    m_sortFlags(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name),
    m_folderModel(nullptr)
{
    ui->setupUi(this);

    m_folderModel = new FolderModel(this);
    m_folderModel->setReadOnly(true);
    m_folderModel->setFilter(m_filterFlags);
    m_folderModel->setSorting(m_sortFlags);

    ui->folderView->setModel(m_folderModel);

    connect(ui->folderView->horizontalHeader(), SIGNAL(sectionResized(int,int,int)),       this, SLOT(columnResized(int,int,int)));
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

void FolderForm::setPath(QString& path)
{
    if(m_folderModel != nullptr)
    {
        QModelIndex index = m_folderModel->index(path);
        ui->folderView->setRootIndex(index);

        ui->lineEdit->setText(path);

        ui->folderView->selectRow(0);
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

    QModelIndex currentIndex = ui->folderView->currentIndex();

    if(m_folderModel->isDir(currentIndex))
    {
        QString path = m_folderModel->filePath(currentIndex);

        qDebug() << "================== onOpen() : " << currentIndex << " -> " << path;

        setPath(path);
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

