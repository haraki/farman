﻿#include <QStorageInfo>
#include <QDebug>
#include "settings.h"
#include "bookmarkinfomodel.h"

namespace Farman
{

BookmarkInfoModel::BookmarkInfoModel(bool system/* = true*/, bool showIcon/* = true*/, QObject *parent/* = Q_NULLPTR*/)
    : QAbstractTableModel(parent)
    , m_fileSystemModel(new QFileSystemModel(this))
    , m_showIcon(showIcon)
    , m_bookmarkIcon(new QIcon(":/images/bookmark_on.svg"))
{
    initialize(system);
}

BookmarkInfoModel::~BookmarkInfoModel()
{
    delete m_bookmarkIcon;
    delete m_fileSystemModel;
}

int BookmarkInfoModel::initialize(bool system)
{
    qDebug() << "BookmarkInfoModel::initialize() : system : " << system;

    m_infos.clear();

    if(system)
    {
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
        if(m_showIcon && index.column() == 0)
        {
            if(info.getType() == BookmarkInfo::Type::System)
            {
                return m_fileSystemModel->fileIcon(m_fileSystemModel->index(info.getPath()));
            }
            else if (info.getType() == BookmarkInfo::Type::User)
            {
                return *m_bookmarkIcon;
            }
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

bool BookmarkInfoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    qDebug() << "BookmarkInfoModel::setData() : row : " << index.row() << ", column : " << index.column() << ", value : " << value;

    if(!index.isValid() || (index.column() > 1) || role != Qt::EditRole)
    {
        return false;
    }

    if(data(index, role) != value)
    {
        if(index.column() == 0)
        {
            m_infos[index.row()].setName(value.toString());
        }
        else            // if(index.column() == 1)
        {
            m_infos[index.row()].setPath(value.toString());
        }

        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags BookmarkInfoModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flag = QAbstractTableModel::flags(index);
    if(!index.isValid())
    {
        flag |= Qt::ItemIsEditable;
    }

    return flag;
}

bool BookmarkInfoModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for(int i = 0;i < count;i++)
    {
        m_infos.insert(row + i, BookmarkInfo(BookmarkInfo::Type::User, "", ""));
    }

    endInsertRows();

    return true;
}

bool BookmarkInfoModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for(;count > 0;count--)
    {
        m_infos.removeAt(row);
    }

    endRemoveRows();

    return true;
}

bool BookmarkInfoModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count,
                                 const QModelIndex &destinationParent, int destinationChild)
{
    Q_UNUSED(sourceParent);
    Q_UNUSED(destinationParent);

    if(destinationChild >= sourceRow && destinationChild <= sourceRow + count - 1)
    {
        return false;
    }

    if(!beginMoveRows(QModelIndex(), sourceRow, sourceRow + count - 1, QModelIndex(), destinationChild))
    {
        return false;
    }

    for(int i = 0;i < count;i++)
    {
        m_infos.insert(destinationChild + i, m_infos[sourceRow]);

        int removeIndex = (destinationChild > sourceRow) ? sourceRow : sourceRow + 1;

        m_infos.removeAt(removeIndex);
    }

    endMoveRows();

    return true;
}

int BookmarkInfoModel::saveToSettings()
{
    QList<QPair<QString, QString>> dirPathList;
    for(auto& info : m_infos)
    {
        dirPathList.push_back({info.getName(), info.getPath()});
    }

    Settings::getInstance()->setBookmarkDirPathList(dirPathList);

    return 0;
}

QHash<int, QByteArray> BookmarkInfoModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TypeRole] = "type";
    roles[NameRole] = "name";
    roles[PathRole] = "path";
    return roles;
}

}           // namespace Farman