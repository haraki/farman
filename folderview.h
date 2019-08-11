#ifndef FOLDERVIEW_H
#define FOLDERVIEW_H

#include <QTableView>
#include <QString>
#include "types.h"

namespace Farman
{

class FolderModel;

class FolderView : public QTableView
{
    Q_OBJECT

public:
    explicit FolderView(QWidget *parent = Q_NULLPTR);
    virtual ~FolderView() Q_DECL_OVERRIDE;

    void setCursorAppearance(int width, const QColor& activeColor, const QColor& inactiveColor);
    void setCursorBehaivior(bool loopMove, bool moveOpenViewer);
    void setDragAndDropBehavior(DragAndDropBehaviorType behaviorType);

    using QTableView::setModel;
    void setModel(FolderModel *folderModel);

    QItemSelectionModel::SelectionFlags selectionCommand(const QModelIndex &index, const QEvent *e = Q_NULLPTR) const Q_DECL_OVERRIDE;

    void setCursor(const QModelIndex& index);
    void movePreviousCursor();
    void moveNextCursor();

    void selectCurrent(QItemSelectionModel::SelectionFlag selectionFlag = QItemSelectionModel::Toggle);

    void refresh(const QModelIndex& topLeft, const QModelIndex& bottomRight);

Q_SIGNALS:
    void open(const QString& path);
    void openWithApp(const QString& path);
    void openWithTextEditor(const QString& dirPath, const QStringList& fileNames);
    void copyFile(const QStringList& srcPaths, const QString& dstDirPath);
    void moveFile(const QStringList& srcPaths, const QString& dstDirPath);
    void selectedFile(const QString& path, QItemSelectionModel::SelectionFlag selectionFlag);

private Q_SLOTS:
    void onDoubleClicked(const QModelIndex&);

private:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent *e) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *e) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *e) Q_DECL_OVERRIDE;

    void emitOpen(const QString& path);
    void emitOpenWithApp(const QString& path);
    void emitCopyFile(const QStringList& srcPaths, const QString& dstDirPath);
    void emitMoveFile(const QStringList& srcPaths, const QString& dstDirPath);
    void emitSelectedFile(const QString& path, QItemSelectionModel::SelectionFlag selectionFlag);

    bool m_loopMove;
    bool m_moveOpenViewer;
    DragAndDropBehaviorType m_dragAndDropBehaviorType;
};

}           // namespace Farman

#endif // FOLDERVIEW_H
