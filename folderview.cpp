﻿#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QItemSelectionModel>
#include <QHeaderView>
#include <QMimeData>
#include <QMenu>
#include "folderview.h"
#include "folderviewstyleditemdelegate.h"
#include "foldermodel.h"
#include "file.h"
#include "default_settings.h"
#include "types.h"

namespace Farman
{

FolderView::FolderView(QWidget *parent/* = Q_NULLPTR*/)
    : QTableView(parent)
    , m_loopMove(DEFAULT_ALLOW_CURSOR_AROUND)
    , m_moveOpenViewer(DEFAULT_MOVE_CURSOR_OPEN_VIEWER)
    , m_dragAndDropBehaviorType(DEFAULT_DRAG_AND_DROP_BEHAVIOR_TYPE)
{
    setItemDelegate(new FolderViewStyledItemDelegate(this));

    connect(this,
            SIGNAL(doubleClicked(const QModelIndex&)),
            this,
            SLOT(onDoubleClicked(const QModelIndex&)));
}

FolderView::~FolderView()
{

}

void FolderView::setCursorAppearance(int width, const QColor& activeColor, const QColor& inactiveColor)
{
    FolderViewStyledItemDelegate* styledItemDelegate = dynamic_cast<FolderViewStyledItemDelegate*>(itemDelegate());
    Q_ASSERT(styledItemDelegate != NULL);

    styledItemDelegate->setCursorAppearance(width, activeColor, inactiveColor);
}

void FolderView::setCursorBehaivior(bool loopMove, bool moveOpenViewer)
{
    m_loopMove = loopMove;
    m_moveOpenViewer = moveOpenViewer;
}

void FolderView::setDragAndDropBehavior(DragAndDropBehaviorType behaviorType)
{
    m_dragAndDropBehaviorType = behaviorType;
}

void FolderView::setModel(FolderModel *model)
{
    QTableView::setModel(model);
}

QItemSelectionModel::SelectionFlags FolderView::selectionCommand(const QModelIndex& index, const QEvent* e/* = Q_NULLPTR*/) const
{
    QItemSelectionModel::SelectionFlags ret = QTableView::selectionCommand(index, e);

    // ここでは選択処理は行わない
    ret &= ~(QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Toggle);

    return ret;
}

void FolderView::selectCurrent(QItemSelectionModel::SelectionFlag selectionFlag/* = QItemSelectionModel::Toggle*/)
{
    FolderModel* folderModel = qobject_cast<FolderModel*>(model());
    Q_ASSERT(folderModel != Q_NULLPTR);

    const QModelIndex index = currentIndex();
    if(!index.isValid() || folderModel->rowCount(index.parent()) == 0)
    {
        return;
    }

    if(folderModel->fileName(index) != "..")
    {
        folderModel->setSelect(index.row(), selectionFlag, index.parent());

        emitSelectedFile(folderModel->filePath(index), selectionFlag);
    }
}

void FolderView::setCursor(const QModelIndex& index)
{
    if(index.isValid())
    {
        setCurrentIndex(index);
        scrollTo(index);
    }
}

void FolderView::movePreviousCursor()
{
    FolderModel* folderModel = qobject_cast<FolderModel*>(model());
    Q_ASSERT(folderModel != Q_NULLPTR);

    const QModelIndex index = currentIndex();
    if(!index.isValid() || folderModel->rowCount(index.parent()) == 0)
    {
        return;
    }

    int rowNext = index.row() - 1;
    if(rowNext < 0)
    {
        if(!m_loopMove)
        {
            return;
        }

        rowNext = folderModel->rowCount(index.parent()) - 1;
    }

    QModelIndex newIndex = folderModel->index(rowNext, 0, index.parent());

    setCursor(newIndex);
}

void FolderView::moveNextCursor()
{
    FolderModel* folderModel = qobject_cast<FolderModel*>(model());
    Q_ASSERT(folderModel != Q_NULLPTR);

    const QModelIndex index = currentIndex();
    if(!index.isValid() || folderModel->rowCount(index.parent()) == 0)
    {
        return;
    }

    int rowNext = index.row() + 1;
    if(rowNext == folderModel->rowCount(index.parent()))
    {
        if(!m_loopMove)
        {
            return;
        }

        rowNext = 0;
    }

    QModelIndex newIndex = folderModel->index(rowNext, 0, index.parent());

    setCursor(newIndex);
}

void FolderView::refresh(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    // フィルタ設定などで FolderView に表示するものが存在しない場合、ResizeMode がリセット？されてしまう不具合回避のため、refresh の度に再設定する
    QHeaderView* hHeader = horizontalHeader();
    hHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
    hHeader->setSectionResizeMode(0, QHeaderView::Stretch);

    emit dataChanged(topLeft, bottomRight);
}

void FolderView::onDoubleClicked(const QModelIndex& index)
{
    FolderModel* folderModel = qobject_cast<FolderModel*>(model());
    Q_ASSERT(folderModel != Q_NULLPTR);

    if(!index.isValid())
    {
        return;
    }

    const QString path = folderModel->fileInfo(index).absoluteFilePath();

    emitOpen(path);

    // ダブルクリックの場合は非選択状態にする
    folderModel->clearSelected();
    setCurrentIndex(index);
}

void FolderView::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Up:
        movePreviousCursor();
        e->accept();
        return;
    case Qt::Key_Down:
        moveNextCursor();
        e->accept();
        return;
    case Qt::Key_PageUp:
    case Qt::Key_PageDown:
        QTableView::keyPressEvent(e);
        return;
    case Qt::Key_Space:
        selectCurrent();
        moveNextCursor();
        e->accept();
        return;
    case Qt::Key_Return:
    {
        // Return は Designer のショートカットの設定では効かないようなので、ハードコーディングする
        FolderModel* folderModel = qobject_cast<FolderModel*>(model());
        Q_ASSERT(folderModel != Q_NULLPTR);

        const QModelIndex index = currentIndex();
        if(!index.isValid())
        {
            return;
        }

        const QString path = folderModel->fileInfo(index).absoluteFilePath();
        if(e->modifiers() & Qt::ShiftModifier)
        {
            emitOpenWithApp(path);
        }
        else
        {
            emitOpen(path);
        }

        if(m_moveOpenViewer)
        {
            moveNextCursor();
        }

        e->accept();
        return;
    }
    default:
        break;
    }

    if(e->modifiers() & Qt::ShiftModifier && !e->text().isEmpty())
    {
        QString text = e->text();

        if(e->key() == Qt::Key_Greater)
        {
            // SHIFT + '.'キーを押すと '>' となってしまうため、強制的に '.' にする
            text = ".";
        }

        // keyboradSearch() 内で文字を連結させず、必ず1文字目として検索させるようにするため、一時的にキー入力のインターバル値を0にする
        int backupInterval = QApplication::keyboardInputInterval();
        QApplication::setKeyboardInputInterval(0);

        keyboardSearch(text);

        QApplication::setKeyboardInputInterval(backupInterval);

        e->accept();

        return;
    }

    e->ignore();
}

void FolderView::mousePressEvent(QMouseEvent *e)
{
    qDebug() << "FolderView::mousePressEvent()";

    QTableView::mousePressEvent(e);

    if(e->buttons() & Qt::LeftButton && indexAt(e->pos()).isValid())
    {
        QItemSelectionModel::SelectionFlag selectionFlag = QItemSelectionModel::ClearAndSelect;
        if(e->modifiers() & Qt::ControlModifier)
        {
            selectionFlag = QItemSelectionModel::Toggle;
        }
        else if(e->modifiers() & Qt::ShiftModifier)
        {
        }

        selectCurrent(selectionFlag);
    }
}

void FolderView::dragEnterEvent(QDragEnterEvent *e)
{
    qDebug() << "FolderView::dragEnterEvent()";

    FolderView* source = qobject_cast<FolderView*>(e->source());
    if(this != source)
    {
        e->accept();
    }
}

void FolderView::dragMoveEvent(QDragMoveEvent *e)
{
    qDebug() << "FolderView::dragMoveEvent()";

    FolderView* source = qobject_cast<FolderView*>(e->source());
    if(this != source)
    {
        e->accept();
    }
}

void FolderView::dropEvent(QDropEvent *e)
{
    qDebug() << "FolderView::dropEvent()";

    FolderView* source = qobject_cast<FolderView*>(e->source());
    if(this != source)
    {
        FolderModel* folderModel = qobject_cast<FolderModel*>(model());
        Q_ASSERT(folderModel != Q_NULLPTR);

        QString dstDirPath = folderModel->filePath(rootIndex());
        QStringList srcPaths;
        for(QUrl url : e->mimeData()->urls())
        {
            QString srcPath = QFileInfo(url.toLocalFile()).absoluteFilePath();
            if(!srcPath.isNull() && !srcPath.isEmpty())
            {
                // 末尾が'/'だとコピーできない
                while(srcPath.endsWith('/'))
                {
                    srcPath.chop(1);
                }
                srcPaths.push_back(srcPath);
            }
        }

        qDebug() << "srcPaths : " << srcPaths;
        qDebug() << "dstDirPath : " << dstDirPath;

        DragAndDropBehaviorType behaviorType = m_dragAndDropBehaviorType;
        if(behaviorType == DragAndDropBehaviorType::Select)
        {
            // コピー or 移動選択用ポップアップメニュー表示
            QMenu menu(this);
            QAction *actionCopy = menu.addAction(tr("Copy"));
            QAction *actionMove = menu.addAction(tr("Move"));
            menu.addSeparator();
            menu.addAction(tr("Cancel"));

            QAction *selected = menu.exec(mapToGlobal(e->pos()));
            if(selected == actionCopy)
            {
                behaviorType = DragAndDropBehaviorType::Copy;
            }
            else if(selected == actionMove)
            {
                behaviorType = DragAndDropBehaviorType::Move;
            }
        }

        if(behaviorType == DragAndDropBehaviorType::Copy)
        {
            emitCopyFile(srcPaths, dstDirPath);
        }
        else if(behaviorType == DragAndDropBehaviorType::Move)
        {
            emitMoveFile(srcPaths, dstDirPath);
        }

        e->accept();
    }
}

void FolderView::emitOpen(const QString& path)
{
    emit open(path);
}

void FolderView::emitOpenWithApp(const QString& path)
{
    emit openWithApp(path);
}

void FolderView::emitCopyFile(const QStringList& srcPaths, const QString& dstDirPath)
{
    emit copyFile(srcPaths, dstDirPath);
}

void FolderView::emitMoveFile(const QStringList& srcPaths, const QString& dstDirPath)
{
    emit moveFile(srcPaths, dstDirPath);
}

void FolderView::emitSelectedFile(const QString& path, QItemSelectionModel::SelectionFlag selectionFlag)
{
    emit selectedFile(path, selectionFlag);
}

}           // namespace Farman
