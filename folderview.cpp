#include <QDebug>
#include <QKeyEvent>
#include <QItemSelectionModel>
#include "folderview.h"
#include "folderviewstyleditemdelegate.h"
#include "foldermodel.h"

FolderView::FolderView(QWidget *parent/* = Q_NULLPTR*/)
    : QTableView(parent)
{
    setItemDelegate(new FolderViewStyledItemDelegate(this));
}

FolderView::~FolderView()
{

}

QItemSelectionModel::SelectionFlags FolderView::selectionCommand(const QModelIndex& index, const QEvent* e/* = Q_NULLPTR*/) const
{
    QItemSelectionModel::SelectionFlags ret = QTableView::selectionCommand(index, e);
    ret &= ~QItemSelectionModel::SelectionFlag::ClearAndSelect;

    const QKeyEvent* keyEv = dynamic_cast<const QKeyEvent*>(e);
    if(keyEv != Q_NULLPTR)
    {
        Qt::Key key = static_cast<Qt::Key>(keyEv->key());

        qDebug() << "FolderView::selectionCommand : " << key;

        if(key == Qt::Key_Space)
        {
            const FolderModel* folderModel = dynamic_cast<FolderModel*>(this->model());
            if(folderModel != Q_NULLPTR)
            {
                if(folderModel->fileName(index) != "..")
                {
                    ret |= QItemSelectionModel::SelectionFlag::Toggle;
                }

                if(index.row() + 1 < folderModel->rowCount(index.parent()))
                {
                    QItemSelectionModel* selectionModel = this->selectionModel();
                    if(selectionModel != Q_NULLPTR)
                    {
                        // カーソルを1行下に移動
                        QModelIndex newIndex = folderModel->index(index.row() + 1, 0, index.parent());
                        selectionModel->setCurrentIndex(newIndex, QItemSelectionModel::NoUpdate);
                    }
                }
            }
        }

        qDebug() << "ret == " << ret;
    }

    return ret;
}

void FolderView::refresh(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    emit dataChanged(topLeft, bottomRight);
}
