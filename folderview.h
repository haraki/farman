#ifndef FOLDERVIEW_H
#define FOLDERVIEW_H

#include <QTableView>
#include <QString>

namespace Farman
{

class FolderModel;

class FolderView : public QTableView
{
    Q_OBJECT

public:
    explicit FolderView(QWidget *parent = Q_NULLPTR);
    virtual ~FolderView();

    using QTableView::setModel;
    void setModel(FolderModel *folderModel);

    QItemSelectionModel::SelectionFlags selectionCommand(const QModelIndex &index, const QEvent *e = Q_NULLPTR) const Q_DECL_OVERRIDE;

    void setCursor(const QModelIndex& index);
    void movePreviousCursor();
    void moveNextCursor();

    void selectCurrent(QItemSelectionModel::SelectionFlag selectionFlag = QItemSelectionModel::Toggle);

    void refresh(const QModelIndex& topLeft, const QModelIndex& bottomRight);

private:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent *e) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *e) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *e) Q_DECL_OVERRIDE;
};

}           // namespace Farman

#endif // FOLDERVIEW_H
