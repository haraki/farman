#include <QDebug>
#include <QKeyEvent>
#include <QHeaderView>
#include <QFileDialog>
#include <QItemSelectionModel>
#include "folderform.h"
#include "ui_folderform.h"
#include "foldermodel.h"

namespace Farman
{

FolderForm::FolderForm(QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent/* = Q_NULLPTR*/)
    : QWidget(parent)
    , ui(new Ui::FolderForm)
    , m_folderModel(Q_NULLPTR)
{
    ui->setupUi(this);

    m_folderModel = new FolderModel(this);
    m_folderModel->setReadOnly(true);
    m_folderModel->setFilter(filterFlags);
    m_folderModel->setSorting(sortFlags);
    ui->folderView->setModel(m_folderModel);

    ui->folderView->setSelectionModel(m_folderModel->getSelectionModel());

    connect(m_folderModel->getSelectionModel(),
            SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,
            SLOT(onCurrentChanged(const QModelIndex&, const QModelIndex&)));
    connect(ui->folderView,
            SIGNAL(doubleClicked(const QModelIndex&)),
            this,
            SLOT(onOpen(const QModelIndex&)));

    ui->folderView->installEventFilter(this);
}

FolderForm::~FolderForm()
{
    delete m_folderModel;
    delete ui;
}

bool FolderForm::eventFilter(QObject *watched, QEvent *e)
{
    Q_UNUSED(watched);

    bool ret = false;

    switch (e->type()) {
    case QEvent::KeyPress:
    {
        Qt::Key key = static_cast<Qt::Key>(dynamic_cast<QKeyEvent*>(e)->key());

        qDebug() << "FolderForm::eventFilter : " << key;

        switch(key)
        {
        case Qt::Key_Return:
            onOpen();

            ret = true;

            break;

        case Qt::Key_Space:
            onSelect();

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

void FolderForm::setFilterFlags(QDir::Filters filterFlags)
{
    if(m_folderModel != Q_NULLPTR)
    {
        m_folderModel->setFilter(filterFlags);
    }
}

void FolderForm::setSortFlags(QDir::SortFlags sortFlags)
{
    if(m_folderModel != Q_NULLPTR)
    {
        m_folderModel->setSorting(sortFlags);
    }
}

void FolderForm::setPath(const QString& dirPath, const QString& beforePath/* = QString() */)
{
    if(m_folderModel != Q_NULLPTR)
    {
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

        if(dir.entryInfoList(filterFlags, m_folderModel->sorting()).size() == 0)
        {
            qDebug() << dirPath << " size() == 0";
            return;
        }

        m_folderModel->setFilter(filterFlags);

        m_folderModel->clearSelected();

        QModelIndex newDirIndex = m_folderModel->index(dirPath);
        ui->folderView->setRootIndex(newDirIndex);

        QModelIndex newCursorIndex;
        if(!beforePath.isEmpty())
        {
            // 前回のパスが子ディレクトリであれば、そこを初期カーソル位置とする
            newCursorIndex = m_folderModel->index(beforePath);
        }

        if(!newCursorIndex.isValid() || newCursorIndex.parent() != newDirIndex || newCursorIndex.row() < 0)
        {
            // 初期カーソル位置はリストの先頭
            newCursorIndex = newDirIndex.child(0, 0);
        }
        ui->folderView->setCurrentIndex(newCursorIndex);
        ui->folderView->scrollTo(newCursorIndex);

        ui->folderPathEdit->setText(dirPath);
    }
}

QDir::Filters FolderForm::getFilterFlags()
{
    return m_folderModel->filter();
}

QDir::SortFlags FolderForm::getSortFlags()
{
    return m_folderModel->sorting();
}

QString FolderForm::getPath()
{
    return m_folderModel->filePath(ui->folderView->rootIndex());
}

void FolderForm::onOpen(const QModelIndex& index)
{
    Q_UNUSED(index);

    onOpen();
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

void FolderForm::onSelect()
{
    const QModelIndex currentIndex = ui->folderView->currentIndex();
    if(m_folderModel->fileName(currentIndex) != "..")
    {
        m_folderModel->setSelect(currentIndex.row(), QItemSelectionModel::Toggle, currentIndex.parent());
    }

    if(currentIndex.row() + 1 < m_folderModel->rowCount(currentIndex.parent()))
    {
        QModelIndex newIndex = m_folderModel->index(currentIndex.row() + 1, 0, currentIndex.parent());

        ui->folderView->setCurrentIndex(newIndex);
        ui->folderView->scrollTo(newIndex);
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
    QString dirPath = QFileDialog::getExistingDirectory(this,
                                                        tr("Select folder."),
                                                        m_folderModel->filePath(ui->folderView->rootIndex()),
                                                        QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly);

    if(!dirPath.isEmpty())
    {
        setPath(dirPath);
    }
}

void FolderForm::emitCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo)
{
    emit currentChanged(newFileInfo, oldFileInfo);
}

}           // namespace Farman
