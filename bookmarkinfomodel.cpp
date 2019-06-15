#include <QStorageInfo>
#include <QDebug>
#include "settings.h"
#include "bookmarkinfomodel.h"

namespace Farman
{

BookmarkInfoModel::BookmarkInfoModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_fileSystemModel(new QFileSystemModel(this))
{
    initialize();
}

int BookmarkInfoModel::initialize()
{
    qDebug() << "BookmarkInfoModel::initialize()";

    m_infos.clear();

    for(const QStorageInfo& volume : QStorageInfo::mountedVolumes())
    {
        qDebug() << "Name : " << volume.name();
        qDebug() << "Display Name : " << volume.displayName();
        qDebug() << "Device:" << volume.device();
        qDebug() << "Root path:" << volume.rootPath();
        qDebug() << "File system type:" << volume.fileSystemType();
        qDebug() << "Is valid" << volume.isValid();
        qDebug() << "Is root" << volume.isRoot();
        qDebug() << "Is ready" << volume.isReady();
        qDebug() << "Is read only" << volume.isReadOnly();
        qDebug() << QString("Bytes available: %L1 Bytes").arg(volume.bytesAvailable());
        qDebug() << QString("Bytes free: %L1 Bytes").arg(volume.bytesFree());
        qDebug() << QString("Bytes total: %L1 Bytes").arg(volume.bytesTotal()) << endl;

        if(volume.isValid() && volume.isReady()
#ifdef Q_OS_LINUX
            && (volume.isRoot() || volume.rootPath().startsWith("/media"))
#endif
          )
        {
            m_infos.push_back({BookmarkInfo::Type::System,
#ifdef Q_OS_LINUX
                               volume.device(),
#else
                               volume.displayName(),
#endif
                               volume.rootPath()});
        }
    }

    for(auto& bookmark : Settings::getInstance()->getBookmarkDirPathList())
    {
        m_infos.push_back({BookmarkInfo::Type::User, bookmark.first, bookmark.second});
    }

    return 0;
}

QVariant BookmarkInfoModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation);

    if(role == Qt::DisplayRole)
    {
        if(section == 0)
        {
            return tr("Name");
        }
        else if(section == 1)
        {
            return tr("Path");
        }
    }

    return QVariant();
}
#if 0
bool BookmarkInfoModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}
#endif
int BookmarkInfoModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 0;
    }

    return m_infos.count();
}

int BookmarkInfoModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 0;
    }

    return 2;
}

QVariant BookmarkInfoModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() < 0 || index.row() >= m_infos.count())
    {
        return QVariant();
    }

    const BookmarkInfo& info = m_infos[index.row()];

    switch (role)
    {
    case Qt::DisplayRole:
        if(index.column() == 0)
        {
            return info.getName();
        }
        else if(index.column() == 1)
        {
            return info.getPath();
        }
        break;
    case Qt::DecorationRole:
        if(index.column() == 0)
        {
            return m_fileSystemModel->fileIcon(m_fileSystemModel->index(info.getPath()));
        }
        break;
    case TypeRole:
        return info.getTypeName();
    case NameRole:
        return info.getName();
    case PathRole:
        return info.getPath();
    }

    return QVariant();
}
#if 0
bool BookmarkInfoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags BookmarkInfoModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

bool BookmarkInfoModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
}

bool BookmarkInfoModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
}

bool BookmarkInfoModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
}

bool BookmarkInfoModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
}
#endif
QHash<int, QByteArray> BookmarkInfoModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TypeRole] = "type";
    roles[NameRole] = "name";
    roles[PathRole] = "path";
    return roles;
}

}           // namespace Farman
