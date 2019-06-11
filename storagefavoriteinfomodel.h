#ifndef STORAGEFAVORITEINFOMODEL_H
#define STORAGEFAVORITEINFOMODEL_H

#include <QAbstractListModel>
#include <QFileSystemModel>
#include <QStorageInfo>

namespace Farman
{

class StorageFavoriteInfo
{
public:
    enum class Type : int
    {
        Storage,
        Favorite,
    };

    StorageFavoriteInfo(Type type, const QString& name, const QString& path)
    {
        m_type = type;
        m_name = name;
        m_path = path;
    }

    ~StorageFavoriteInfo() = default;

    Type getType() const { return m_type; }
    QString getTypeName() const { return (m_type == Type::Storage) ? "Storage" : (m_type == Type::Favorite) ? "Favorite" : "Unknown"; }
    QString getName() const { return m_name; }
    QString getPath() const { return m_path; }

private:
    StorageFavoriteInfo() = delete;

    Type m_type;
    QString m_name;
    QString m_path;
};

class StorageFavoriteInfoModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        TypeRole = Qt::UserRole + 1,
        NameRole,
        PathRole,
    };

    explicit StorageFavoriteInfoModel(QObject *parent = Q_NULLPTR);
    ~StorageFavoriteInfoModel() Q_DECL_OVERRIDE;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

protected:
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

private:
    int initialize();

    QList<StorageFavoriteInfo> m_infos;
    QFileSystemModel* m_fileSystemModel;
};

}           // namespace Farman

#endif // STORAGEFAVORITEINFOMODEL_H
