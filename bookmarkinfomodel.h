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

    explicit BookmarkInfoModel(bool system = true, bool icon = true, QObject *parent = Q_NULLPTR);
    ~BookmarkInfoModel() Q_DECL_OVERRIDE;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex& index) const Q_DECL_OVERRIDE;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count,
                  const QModelIndex &destinationParent, int destinationChild) Q_DECL_OVERRIDE;

    // 自身が保持するブックマーク情報を Settings のインスタンスに反映する
    // ファイル(settings.ini)への保存はアプリ終了時に行われる
    int saveToSettings();

protected:
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

private:
    int initialize(bool system);

    QList<BookmarkInfo> m_infos;
    QFileSystemModel* m_fileSystemModel;

    bool m_showIcon;
    QIcon* m_bookmarkIcon;
};

}           // namespace Farman

#endif // BOOKMARKINFOMODEL_H
