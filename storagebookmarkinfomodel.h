#ifndef STORAGEBOOKMARKINFOMODEL_H
#define STORAGEBOOKMARKINFOMODEL_H

#include <QAbstractTableModel>
#include <QFileSystemModel>
#include <QStorageInfo>

namespace Farman
{

class StorageBookmarkInfo
{
public:
    enum class Type : int
    {
        Storage,
        Bookmark,
    };

    StorageBookmarkInfo(Type type, const QString& name, const QString& path)
    {
        m_type = type;
        m_name = name;
        m_path = path;
    }

    ~StorageBookmarkInfo() = default;

    Type getType() const { return m_type; }
    QString getTypeName() const { return (m_type == Type::Storage) ? "Storage" : (m_type == Type::Bookmark) ? "Bookmark" : "Unknown"; }
    QString getName() const { return m_name; }
    QString getPath() const { return m_path; }

private:
    StorageBookmarkInfo() = delete;

    Type m_type;
    QString m_name;
    QString m_path;
};

class StorageBookmarkInfoModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Roles
    {
        TypeRole = Qt::UserRole + 1,
        NameRole,
        PathRole,
    };

    explicit StorageBookmarkInfoModel(QObject *parent = Q_NULLPTR);
    ~StorageBookmarkInfoModel() Q_DECL_OVERRIDE;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

protected:
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

private:
    int initialize();

    QList<StorageBookmarkInfo> m_infos;
    QFileSystemModel* m_fileSystemModel;
};

}           // namespace Farman

#endif // STORAGEBOOKMARKINFOMODEL_H
