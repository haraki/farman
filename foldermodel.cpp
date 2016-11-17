#include "foldermodel.h"

#include <QVariant>
#include <QDebug>
#include <QDateTime>
#include <QPalette>

FolderModel::FolderModel(const QStringList &nameFilters, QDir::Filters filters, QDir::SortFlags sort, QObject *parent)
    : QDirModel(nameFilters, filters, sort, parent)
{
}

FolderModel::FolderModel(QObject *parent)
    : QDirModel(parent)
{
}

FolderModel::~FolderModel()
{

}

QVariant FolderModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;

    switch(role)
    {
    case Qt::DisplayRole:
        switch(index.column())
        {
        case 0:
            ret = fileInfo(index).fileName();
            break;
        case 1:
            ret = fileInfo(index).size();
            break;
        case 3:
            ret = fileInfo(index).lastModified().toString("yyyy-MM-dd HH:mm:ss");
            break;
        }

        break;

    case Qt::TextColorRole:
        if((fileName(index) != "..") && (fileInfo(index).isHidden()))
        {
            ret = m_palette.dark();
        }

        break;
    }

    if(ret.isNull())
    {
        ret = QDirModel::data(index, role);
    }

//    qDebug() << "data(" << index << "," << static_cast<Qt::ItemDataRole>(role) << ") : ret = " << ret;

    return ret;
}

QVariant FolderModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant ret;

    if(role == Qt::DisplayRole)
    {
        switch(section)
        {
        case 0:
            ret = QString("ファイル名");
            break;
        case 1:
            ret = QString("サイズ");
            break;
        case 2:
            ret = QString("種類");
            break;
        case 3:
            ret = QString("更新日時");
            break;
        }

    }

    if(ret.isNull())
    {
        ret = QDirModel::headerData(section, orientation, role);
    }

//    qDebug() << "headerData(" << section << "," << orientation << "," << static_cast<Qt::ItemDataRole>(role) << ") : ret = " << ret;

    return ret;
}

void FolderModel::sort(int column, Qt::SortOrder order)
{
    qDebug() << "================= sort(" << column << ", " << order << ")";

    QDirModel::sort(column, order);

    qDebug() << "================= sorting : " << QDirModel::sorting();
}


#if 0
FolderModel::FolderModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QVariant FolderModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

bool FolderModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

QModelIndex FolderModel::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex FolderModel::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int FolderModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int FolderModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

bool FolderModel::hasChildren(const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

bool FolderModel::canFetchMore(const QModelIndex &parent) const
{
    // FIXME: Implement me!
    return false;
}

void FolderModel::fetchMore(const QModelIndex &parent)
{
    // FIXME: Implement me!
}

QVariant FolderModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}

bool FolderModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags FolderModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

bool FolderModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
}

bool FolderModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    beginInsertColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endInsertColumns();
}

bool FolderModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
}

bool FolderModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    beginRemoveColumns(parent, column, column + count - 1);
    // FIXME: Implement me!
    endRemoveColumns();
}

void FolderModel::setIconProvider(QFileIconProvider *provider)
{
    // FIXME: Implement me!
}

QFileIconProvider* FolderModel::iconProvider() const
{
    // FIXME: Implement me!
}

void FolderModel::setNameFilters(const QStringList &filters)
{
    // FIXME: Implement me!
}

QStringList FolderModel::nameFilters() const
{
    // FIXME: Implement me!
}

void FolderModel::setFilter(QDir::Filters filters)
{
    // FIXME: Implement me!
}

QDir::Filters FolderModel::filter() const
{
    // FIXME: Implement me!
}

void FolderModel::setSorting(QDir::SortFlags sort)
{
    // FIXME: Implement me!
}

QDir::SortFlags FolderModel::sorting() const
{
    // FIXME: Implement me!
}

void FolderModel::setResolveSymlinks(bool enable)
{
    // FIXME: Implement me!
}

bool FolderModel::resolveSymlinks() const
{
    // FIXME: Implement me!
}

void FolderModel::setReadOnly(bool enable)
{
    // FIXME: Implement me!
}

bool FolderModel::isReadOnly() const
{
    // FIXME: Implement me!
}

void FolderModel::setLazyChildCount(bool enable)
{
    // FIXME: Implement me!
}

bool FolderModel::lazyChildCount() const
{
    // FIXME: Implement me!
}

QModelIndex FolderModel::index(const QString &path, int column) const
{
    // FIXME: Implement me!
}

bool FolderModel::isDir(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

QModelIndex FolderModel::mkdir(const QModelIndex &parent, const QString &name)
{
    // FIXME: Implement me!
}

bool FolderModel::rmdir(const QModelIndex &index)
{
    // FIXME: Implement me!
}

bool FolderModel::remove(const QModelIndex &index)
{
    // FIXME: Implement me!
}

QString FolderModel::filePath(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

QString FolderModel::fileName(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

QIcon FolderModel::fileIcon(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

QFileInfo FolderModel::fileInfo(const QModelIndex &index) const
{
    // FIXME: Implement me!
}
#endif
