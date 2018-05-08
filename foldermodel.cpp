#include <QFileSystemModel>
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <QItemSelectionModel>
#include <QFileIconProvider>
#include "foldermodel.h"
#include "settings.h"

namespace Farman
{

FolderModel::FolderModel(QObject *parent/* = Q_NULLPTR*/) :
    QSortFilterProxyModel(parent),
    m_fsModel(new QFileSystemModel(this)),
    m_sortSectionType(SectionType::FileName),
    m_sortDirsType(SortDirsType::NoSpecify),
    m_sortDotFirst(true),
    m_sortOrder(Qt::AscendingOrder),
    m_selectionModel(new QItemSelectionModel(this))
{
    setSortCaseSensitivity(Qt::CaseInsensitive);

    m_fsModel->setRootPath(QDir::currentPath());
    m_fsModel->setFilter(QDir::AllDirs | QDir::Files);

    setSourceModel(m_fsModel);
}

FolderModel::~FolderModel()
{
    delete m_selectionModel;
    delete m_fsModel;
}

void FolderModel::updateSettings()
{
    initFont();
    initBrush();
}

QModelIndex FolderModel::index(const QString &path, int column/* = 0*/) const
{
    return mapFromSource(m_fsModel->index(path, column));
}

void FolderModel::setSortSectionType(SectionType sectionType)
{
    m_sortSectionType = sectionType;
}

SectionType FolderModel::sortSectionType() const
{
    return m_sortSectionType;
}

void FolderModel::setSortDirsType(SortDirsType dirsType)
{
    m_sortDirsType = dirsType;
}

SortDirsType FolderModel::sortDirsType() const
{
    return m_sortDirsType;
}

void FolderModel::setSortDotFirst(bool dotFirst)
{
    m_sortDotFirst = dotFirst;
}

bool FolderModel::sortDotFirst() const
{
    return m_sortDotFirst;
}

void FolderModel::setSortOrder(Qt::SortOrder order)
{
    m_sortOrder = order;
}

Qt::SortOrder FolderModel::sortOrder() const
{
    return m_sortOrder;
}

int FolderModel::columnCount(const QModelIndex& parent) const
{
    if(parent.column() > 0)
    {
        return 0;
    }

    return 4;
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
            if(!fi.isDir() && !fi.baseName().isEmpty())
            {
                ret = fi.baseName();
            }
            else
            {
                ret = fi.fileName();
            }
            break;
        case SectionType::FileType:
            if(!fi.isDir() && !fi.baseName().isEmpty())
            {
                ret = fi.suffix();
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

    case QFileSystemModel::FileIconRole:
        if(sectionType == SectionType::FileName)
        {
            ret = fileIcon(modelIndex);
        }

        break;

    case QFileSystemModel::FilePathRole:
        if(sectionType == SectionType::FileName)
        {
            ret = filePath(modelIndex);
        }

        break;

    case QFileSystemModel::FileNameRole:
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
        case SectionType::FileType:
            ret = tr("Type");
            break;
        case SectionType::FileSize:
            ret = tr("Size");
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
        ret = QSortFilterProxyModel::headerData(section, orientation, role);
    }

//    qDebug() << "headerData(" << section << "," << orientation << "," << static_cast<Qt::ItemDataRole>(role) << ") : ret = " << ret;

    return ret;
}

void FolderModel::sort(int column, Qt::SortOrder order/* = Qt::AscendingOrder*/)
{
    m_sortSectionType = getSectionTypeFromColumn(column);
    m_sortOrder = order;

    refresh();
}

void FolderModel::refresh()
{
    QSortFilterProxyModel::sort(0, m_sortOrder);
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

bool FolderModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QFileInfo l_info = m_fsModel->fileInfo(source_left);
    QFileInfo r_info = m_fsModel->fileInfo(source_right);
    bool ascOrder = (m_sortOrder == Qt::AscendingOrder);

    if(m_sortDotFirst)
    {
        if(l_info.fileName() == ".")
        {
            return ascOrder;
        }
        else if(r_info.fileName() == ".")
        {
            return !ascOrder;
        }
        else if(l_info.fileName() == ".." && r_info.fileName() != ".")
        {
            return ascOrder;
        }
        else if(r_info.fileName() == ".." && l_info.fileName() != ".")
        {
            return !ascOrder;
        }
    }

    if(m_sortDirsType == SortDirsType::First)
    {
        if(!l_info.isDir() && r_info.isDir())
        {
            return !ascOrder;
        }
        else if(l_info.isDir() && !r_info.isDir())
        {
            return ascOrder;
        }
    }
    else if(m_sortDirsType == SortDirsType::Last)
    {
        if(!l_info.isDir() && r_info.isDir())
        {
            return ascOrder;
        }
        else if(l_info.isDir() && !r_info.isDir())
        {
            return !ascOrder;
        }
    }

    if(m_sortSectionType == SectionType::FileSize)
    {
        if(!l_info.isDir() && !r_info.isDir())
        {
            return l_info.size() < r_info.size();
        }
    }
    else if(m_sortSectionType == SectionType::FileType)
    {
        const QString& l_type = (!l_info.isDir() && !l_info.baseName().isEmpty()) ? l_info.suffix() : "";
        const QString& r_type = (!r_info.isDir() && !r_info.baseName().isEmpty()) ? r_info.suffix() : "";

        if(sortCaseSensitivity() == Qt::CaseInsensitive)
        {
            return l_type.toLower() < r_type.toLower();
        }
        else
        {
            return l_type < r_type;
        }
    }
    else if(m_sortSectionType == SectionType::LastModified)
    {
        return l_info.lastModified() < r_info.lastModified();
    }
    else
    {
        const QString& l_name = (!l_info.isDir() && !l_info.baseName().isEmpty()) ? l_info.baseName() : l_info.fileName();
        const QString& r_name = (!r_info.isDir() && !r_info.baseName().isEmpty()) ? r_info.baseName() : r_info.fileName();

        if(sortCaseSensitivity() == Qt::CaseInsensitive)
        {
            return l_name.toLower() < r_name.toLower();
        }
        else
        {
            return l_name < r_name;
        }
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

SectionType FolderModel::getSectionTypeFromColumn(int column) const
{
    // Todo: 将来的に可変にする
    if(column == 0)
    {
        return SectionType::FileName;
    }
    else if(column == 1)
    {
        return SectionType::FileType;
    }
    else if(column == 2)
    {
        return SectionType::FileSize;
    }
    else if(column == 3)
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

// QFileSystemModel specific API
QModelIndex FolderModel::setRootPath(const QString &path)
{
    return mapFromSource(m_fsModel->setRootPath(path));
}

QString FolderModel::rootPath() const
{
    return m_fsModel->rootPath();
}

QDir FolderModel::rootDirectory() const
{
    return m_fsModel->rootDirectory();
}

void FolderModel::setIconProvider(QFileIconProvider *provider)
{
    m_fsModel->setIconProvider(provider);
}

QFileIconProvider* FolderModel::iconProvider() const
{
    return m_fsModel->iconProvider();
}

void FolderModel::setFilter(QDir::Filters filters)
{
    m_fsModel->setFilter(filters);
}

QDir::Filters FolderModel::filter() const
{
    return m_fsModel->filter();
}

void FolderModel::setResolveSymlinks(bool enable)
{
    m_fsModel->setResolveSymlinks(enable);
}

bool FolderModel::resolveSymlinks() const
{
    return m_fsModel->resolveSymlinks();
}

void FolderModel::setReadOnly(bool enable)
{
    m_fsModel->setReadOnly(enable);
}

bool FolderModel::isReadOnly() const
{
    return m_fsModel->isReadOnly();
}

void FolderModel::setNameFilterDisables(bool enable)
{
    m_fsModel->setNameFilterDisables(enable);
}

bool FolderModel::nameFilterDisables() const
{
    return m_fsModel->nameFilterDisables();
}

void FolderModel::setNameFilters(const QStringList &filters)
{
    return m_fsModel->setNameFilters(filters);
}

QStringList FolderModel::nameFilters() const
{
    return m_fsModel->nameFilters();
}

QString FolderModel::filePath(const QModelIndex &index) const
{
    return m_fsModel->filePath(mapToSource(index));
}

bool FolderModel::isDir(const QModelIndex &index) const
{
    return m_fsModel->isDir(mapToSource(index));
}

qint64 FolderModel::size(const QModelIndex &index) const
{
    return m_fsModel->size(mapToSource(index));
}

QString FolderModel::type(const QModelIndex &index) const
{
    return m_fsModel->type(mapToSource(index));
}

QDateTime FolderModel::lastModified(const QModelIndex &index) const
{
    return m_fsModel->lastModified(mapToSource(index));
}

QModelIndex FolderModel::mkdir(const QModelIndex &parent, const QString &name)
{
    return mapFromSource(m_fsModel->mkdir(mapToSource(parent), name));
}

bool FolderModel::rmdir(const QModelIndex &index)
{
    return m_fsModel->rmdir(mapToSource(index));
}

QString FolderModel::fileName(const QModelIndex &index) const
{
    return m_fsModel->fileName(mapToSource(index));
}

QIcon FolderModel::fileIcon(const QModelIndex &index) const
{
    return m_fsModel->fileIcon(mapToSource(index));
}

QFile::Permissions FolderModel::permissions(const QModelIndex &index) const
{
    return m_fsModel->permissions(mapToSource(index));
}

QFileInfo FolderModel::fileInfo(const QModelIndex &index) const
{
    return m_fsModel->fileInfo(mapToSource(index));
}

bool FolderModel::remove(const QModelIndex &index)
{
    return m_fsModel->remove(mapToSource(index));
}

}           // namespace Farman
