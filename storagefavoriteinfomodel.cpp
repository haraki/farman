#include <QAbstractListModel>
#include <QStorageInfo>
#include <QDebug>
#include "storagefavoriteinfomodel.h"
#include "settings.h"

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
            m_infos.push_back({StorageFavoriteInfo::Type::Storage,
#ifdef Q_OS_LINUX
                               volume.device(),
#else
                               volume.displayName(),
#endif
                               volume.rootPath()});
        }
    }

    for(auto& favorite : Settings::getInstance()->getFavoriteDirPathList())
    {
        m_infos.push_back({StorageFavoriteInfo::Type::Favorite, favorite.first, favorite.second});
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
