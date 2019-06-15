#ifndef BOOKMARKINFOMODEL_H
#define BOOKMARKINFOMODEL_H

#include <QAbstractTableModel>
#include <QFileSystemModel>
#include "bookmarkinfo.h"

namespace Farman
{

class BookmarkInfoModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Roles
    {
        TypeRole = Qt::UserRole + 1,
        NameRole,
        PathRole,
    };

    explicit BookmarkInfoModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

#if 0
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;
#endif

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
#if 0
    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
#endif
protected:
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

private:
    int initialize();

    QList<BookmarkInfo> m_infos;
    QFileSystemModel* m_fileSystemModel;
};

}           // namespace Farman

#endif // BOOKMARKINFOMODEL_H
