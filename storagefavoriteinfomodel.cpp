#include <QAbstractListModel>
#include <QStorageInfo>
#include <QDebug>
#include "storagefavoriteinfomodel.h"

namespace Farman
{

StorageFavoriteInfoModel::StorageFavoriteInfoModel(QObject *parent/*= Q_NULLPTR*/)
    : QAbstractListModel(parent)
{
    initialize();
}

int StorageFavoriteInfoModel::initialize()
{
    qDebug() << "StorageFavoriteInfoModel::initialize()";

    m_infos.clear();

    for(const QStorageInfo& storage : QStorageInfo::mountedVolumes())
    {
        if(storage.isValid())
        {
            m_infos.push_back({StorageFavoriteInfo::Type::Storage, storage.displayName(), storage.rootPath()});

            qDebug() << "name : " << m_infos.back().getName() << ", path : " << m_infos.back().getPath();
        }
    }

    return 0;
}

int StorageFavoriteInfoModel::rowCount(const QModelIndex &parent/* = Q_NULLPTR*/) const
{
    if(parent.isValid())
    {
        return 0;
    }

    return m_infos.count();
}

QHash<int, QByteArray> StorageFavoriteInfoModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TypeRole] = "type";
    roles[NameRole] = "name";
    roles[PathRole] = "path";
    return roles;
}

QVariant StorageFavoriteInfoModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() < 0 || index.row() >= m_infos.count())
    {
        return QVariant();
    }

    const StorageFavoriteInfo& info = m_infos[index.row()];

    switch (role)
    {
    case Qt::DisplayRole:
        return info.getName() + " [" + info.getPath() + "]";
    case TypeRole:
        return info.getTypeName();
    case NameRole:
        return info.getName();
    case PathRole:
        return info.getPath();
    }

    return QVariant();
}

}           // namespace Farman
