﻿#include <QVariant>
#include <QDebug>
#include <QDateTime>
#include <QFont>
#include <QPalette>
#include <QItemSelectionModel>
#include "foldermodel.h"
#include "settings.h"

namespace Farman
{

FolderModel::FolderModel(const QStringList &nameFilters, QDir::Filters filters, QDir::SortFlags sort, QObject *parent/* = Q_NULLPTR*/)
    : QDirModel(nameFilters, filters, sort, parent)
    , m_selectionModel(Q_NULLPTR)
{
    m_selectionModel = new QItemSelectionModel(this);
}

FolderModel::FolderModel(QObject *parent/* = Q_NULLPTR*/)
    : QDirModel(parent)
    , m_selectionModel(Q_NULLPTR)
{
    m_selectionModel = new QItemSelectionModel(this);
}

FolderModel::~FolderModel()
{
    delete m_selectionModel;
}

void FolderModel::updateSettings()
{
    initFont();
    initBrush();
}

int FolderModel::columnCount(const QModelIndex& parent) const
{
    if(parent.column() > 0)
    {
        return 0;
    }

    return 3;
}

QVariant FolderModel::data(const QModelIndex &modelIndex, int role) const
{
    QVariant ret = QVariant();

    if(!modelIndex.isValid())
    {
        return ret;
    }

    SectionType sectionType = getSectionTypeFromColumn(modelIndex.column());
    Q_ASSERT(sectionType != SectionType::Unknown);

    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
    {
        QFileInfo fi = fileInfo(modelIndex);

        switch(sectionType)
        {
        case SectionType::FileName:
#ifdef Q_OS_WIN
            if(fi.fileName() != ".." && isDrive(modelIndex))
            {
                ret = fi.absolutePath();
            }
            else
#endif
            {
                ret = fi.fileName();
            }
            break;
        case SectionType::FileSize:
#ifdef Q_OS_WIN
            if(isDrive(modelIndex))
            {
                ret = QString("<Drive>");
            }
            else
#endif
            if(fi.isDir())
            {
                ret = QString("<Folder>");
            }
            else
            {
                ret = fi.size();
            }
            break;
        case SectionType::FileType:
            ret = iconProvider()->type(fi);
            break;
        case SectionType::LastModified:
            ret = fi.lastModified().toString("yyyy-MM-dd HH:mm:ss");
            break;
        default:
            break;
        }

        break;
    }
    case Qt::FontRole:
        return m_font;

    case Qt::TextAlignmentRole:
        if(sectionType == SectionType::FileSize || sectionType == SectionType::LastModified)
        {
            ret = Qt::AlignRight;
        }
        else
        {
            ret = Qt::AlignLeft;
        }

        break;

    case Qt::TextColorRole:
        ret = getTextBrush(modelIndex);

        break;

    case Qt::BackgroundRole:
        ret = getBackgroundBrush(modelIndex);

        break;

    case FileIconRole:
        if(sectionType == SectionType::FileName)
        {
            ret = fileIcon(modelIndex);
        }

        break;

    case FilePathRole:
        if(sectionType == SectionType::FileName)
        {
            ret = filePath(modelIndex);
        }

        break;

    case FileNameRole:
        if(sectionType == SectionType::FileName)
        {
            ret = fileName(modelIndex);
        }

        break;

    }

//    qDebug() << "data(" << modelIndex << "," << static_cast<Qt::ItemDataRole>(role) << ") : ret = " << ret;

    return ret;
}

QVariant FolderModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant ret;

    SectionType sectionType = getSectionTypeFromColumn(section);
    Q_ASSERT(sectionType != SectionType::Unknown);

    if(role == Qt::DisplayRole)
    {
        switch(sectionType)
        {
        case SectionType::FileName:
            ret = tr("Name");
            break;
        case SectionType::FileSize:
            ret = tr("Size");
            break;
        case SectionType::FileType:
            ret = tr("Type");
            break;
        case SectionType::LastModified:
            ret = tr("Last modified");
            break;
        default:
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

QItemSelectionModel* FolderModel::getSelectionModel()
{
    return m_selectionModel;
}

void FolderModel::setSelect(int row, QItemSelectionModel::SelectionFlags selectionFlags, const QModelIndex &parentIndex)
{
    if(m_selectionModel != Q_NULLPTR)
    {
        QItemSelection selection(index(row, 0, parentIndex), index(row, columnCount() - 1, parentIndex));
        m_selectionModel->select(selection, selectionFlags);
    }
}

QModelIndexList FolderModel::getSelectedIndexList()
{
    QModelIndexList indexList;

    if(m_selectionModel != Q_NULLPTR)
    {
        indexList = m_selectionModel->selectedRows();
    }

    return indexList;
}

void FolderModel::clearSelected()
{
    if(m_selectionModel != Q_NULLPTR)
    {
        m_selectionModel->clear();
    }
}

FolderModel::SectionType FolderModel::getSectionTypeFromColumn(int column) const
{
    // Todo: 将来的に可変にする
    if(column == 0)
    {
        return SectionType::FileName;
    }
    else if(column == 1)
    {
        return SectionType::FileSize;
    }
    else if(column == 2)
//    {
//        return SectionType::FileType;
//    }
//    else if(column == 3)
    {
        return SectionType::LastModified;
    }

    return SectionType::Unknown;
}

QBrush FolderModel::getTextBrush(const QModelIndex& index) const
{
    QBrush ret;

    // TODO: System file(for Win)

    if((fileName(index) != "..") && (fileInfo(index).isHidden()))
    {
        if(isSelected(index))
        {
            ret = getBrush(BrushType::Hidden_Selected);
        }
        else
        {
            ret = getBrush(BrushType::Hidden);
        }
    }
    else if((fileName(index) != "..") && (!fileInfo(index).isWritable()))
    {
        if(isSelected(index))
        {
            ret = getBrush(BrushType::ReadOnly_Selected);
        }
        else
        {
            ret = getBrush(BrushType::ReadOnly);
        }
    }
    else if(fileInfo(index).isDir())
    {
        if(isSelected(index))
        {
            ret = getBrush(BrushType::Folder_Selected);
        }
        else
        {
            ret = getBrush(BrushType::Folder);
        }
    }
    else
    {
        if(isSelected(index))
        {
            ret = getBrush(BrushType::Normal_Selected);
        }
        else
        {
            ret = getBrush(BrushType::Normal);
        }
    }

    return ret;
}

QBrush FolderModel::getBackgroundBrush(const QModelIndex& index) const
{
    QBrush ret;

    if(isSelected(index))
    {
        ret = getBrush(BrushType::Selected_Background);
    }
    else
    {
        ret = getBrush(BrushType::Background);
    }

    return ret;
}

QBrush FolderModel::getBrush(BrushType brushType) const
{
    QBrush ret;

    QMap<BrushType, QBrush>::const_iterator itr = m_brush.find(brushType);
    if(itr != m_brush.end())
    {
        ret = *itr;
    }

    return ret;
}

void FolderModel::initFont()
{
    m_font = QFont(Settings::getInstance()->getFontSetting("folderView"));
}

void FolderModel::initBrush()
{
    auto createBrush = [](const QString& colorSettingType)
    {
        return QBrush(Settings::getInstance()->getColorSetting(colorSettingType));
    };

    m_brush[BrushType::Normal]              = createBrush("folderView_normal");
    m_brush[BrushType::Normal_Selected]     = createBrush("folderView_normal_selected");
    m_brush[BrushType::Folder]              = createBrush("folderView_folder");
    m_brush[BrushType::Folder_Selected]     = createBrush("folderView_folder_selected");
    m_brush[BrushType::ReadOnly]            = createBrush("folderView_readOnly");
    m_brush[BrushType::ReadOnly_Selected]   = createBrush("folderView_readOnly_selected");
    m_brush[BrushType::Hidden]              = createBrush("folderView_hidden");
    m_brush[BrushType::Hidden_Selected]     = createBrush("folderView_hidden_selected");
    m_brush[BrushType::System]              = createBrush("folderView_system");
    m_brush[BrushType::System_Selected]     = createBrush("folderView_system_selected");

    m_brush[BrushType::Background]          = createBrush("folderView_background");
    m_brush[BrushType::Selected_Background] = createBrush("folderView_selected_background");
}

bool FolderModel::isSelected(const QModelIndex& index) const
{
    if(m_selectionModel != Q_NULLPTR)
    {
        return m_selectionModel->isSelected(index);
    }

    return false;
}

#ifdef Q_OS_WIN
bool FolderModel::isDrive(const QModelIndex& index) const
{
    QFileInfo fi = fileInfo(index);
    foreach(QFileInfo drive, QDir::drives())
    {
        if(drive.absoluteFilePath() == fi.absoluteFilePath())
        {
            return true;
        }
    }

    return false;
}
#endif

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

}           // namespace Farman
