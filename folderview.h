﻿#ifndef FOLDERVIEW_H
#define FOLDERVIEW_H

#include <QTableView>
#include <QString>

class FolderView : public QTableView
{
public:
    explicit FolderView(QWidget *parent = Q_NULLPTR);
    virtual ~FolderView();

    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;

    QItemSelectionModel::SelectionFlags selectionCommand(const QModelIndex &index, const QEvent *e = Q_NULLPTR) const Q_DECL_OVERRIDE;

    void refresh(const QModelIndex& topLeft, const QModelIndex& bottomRight);
};

#endif // FOLDERVIEW_H
