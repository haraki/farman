#include <QFileSystemModel>
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <QItemSelectionModel>
#include <QFileIconProvider>
#include "foldermodel.h"
#include "settings.h"
#include "types.h"

#ifdef Q_OS_WIN
#   include <Windows.h>
#endif

namespace Farman
{

// QFileSystemModel の固定フィルタ値
static Q_DECL_CONSTEXPR QDir::Filters FIX_FILTER_FLAGS = QDir::AllEntries |
                                                         QDir::Readable   |
                                                         QDir::Writable   |
                                                         QDir::Executable |
                                                         QDir::Modified   |
                                                         QDir::Hidden     |
                                                         QDir::System     |
                                                         QDir::NoDot;


FolderModel::FolderModel(QObject *parent/* = Q_NULLPTR*/) :
    QSortFilterProxyModel(parent),
    m_sortColumn(0),
    m_sortDirsType(SortDirsType::NoSpecify),
    m_sortDotFirst(true),
    m_sortOrder(Qt::AscendingOrder),
    m_selectionModel(new QItemSelectionModel(this))
{
    setSortCaseSensitivity(Qt::CaseInsensitive);

    QFileSystemModel* fsModel = new QFileSystemModel(this);

    fsModel->setFilter(FIX_FILTER_FLAGS);
    fsModel->setNameFilterDisables(false);

    // layoutChanged シグナルを受け取るスロット QSortFilterProxyModel::_q_sourceLayoutChanged() 内において、フィルタ及びソートが実行される
    // フィルタ及びソート実行後に FolderModel::onLayoutChanged() を呼ばれるようにするため、
    // 上記のシグナルとスロットを connect している setSourceModel() を必ず先に呼び出しておく必要がある
    // (同一シグナルに対するスロットの呼び出し順は、connect された順(Qt v5.12.3 で確認))
    setSourceModel(fsModel);

    connect(fsModel,
            SIGNAL(rootPathChanged(QString)),
            this,
            SLOT(onRootPathChanged(QString)));
    connect(fsModel,
            SIGNAL(rootPathChanged(QString)),
            this,
            SLOT(onRootPathChanged(QString)));
    connect(fsModel,
            SIGNAL(directoryLoaded(QString)),
            this,
            SLOT(onDirectoryLoaded(QString)));
    connect(fsModel,
            SIGNAL(layoutChanged(const QList<QPersistentModelIndex>&, QAbstractItemModel::LayoutChangeHint)),
            this,
            SLOT(onLayoutChanged(const QList<QPersistentModelIndex>&, QAbstractItemModel::LayoutChangeHint)));
    connect(fsModel,
            SIGNAL(layoutAboutToBeChanged(const QList<QPersistentModelIndex>&, QAbstractItemModel::LayoutChangeHint)),
            this,
            SLOT(onLayoutAboutToBeChanged(const QList<QPersistentModelIndex>&, QAbstractItemModel::LayoutChangeHint)));
}

FolderModel::~FolderModel()
{
    delete m_selectionModel;
}

void FolderModel::onRootPathChanged(const QString &newPath)
{
    emitRootPathChanged(newPath);
}

void FolderModel::onFileRenamed(const QString &path, const QString &oldName, const QString &newName)
{
    emitFileRenamed(path, oldName, newName);
}

void FolderModel::onDirectoryLoaded(const QString &path)
{
    emitDirectoryLoaded(path);
}

void FolderModel::onLayoutChanged(const QList<QPersistentModelIndex> &parents/* = QList<QPersistentModelIndex>()*/, QAbstractItemModel::LayoutChangeHint hint/* = QAbstractItemModel::NoLayoutChangeHint*/)
{
    emitLayoutChanged(parents, hint);
}

void FolderModel::onLayoutAboutToBeChanged(const QList<QPersistentModelIndex> &parents/* = QList<QPersistentModelIndex>()*/, QAbstractItemModel::LayoutChangeHint hint/* = QAbstractItemModel::NoLayoutChangeHint*/)
{
    emitLayoutAboutToBeChanged(parents, hint);
}

QModelIndex FolderModel::index(const QString &path, int column/* = 0*/) const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return mapFromSource(fsModel->index(path, column));
}

void FolderModel::setSortSectionType(SectionType sectionType)
{
    setSortColumn(getColumnFromSectionType(sectionType));
}

SectionType FolderModel::sortSectionType() const
{
    return getSectionTypeFromColumn(m_sortColumn);
}

void FolderModel::setSortColumn(int column)
{
    m_sortColumn = column;
}

int FolderModel::sortColumn() const
{
    return m_sortColumn;
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

void FolderModel::setFilterFlags(FilterFlags filterFlags)
{
    m_filterFlags = filterFlags;
}

FilterFlags FolderModel::getFilterFlags() const
{
    return m_filterFlags;
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
            if(!fi.isDir() && !fi.completeBaseName().isEmpty())
            {
                ret = fi.completeBaseName();
            }
            else
            {
                ret = fi.fileName();
            }
            break;
        case SectionType::FileType:
            if(!fi.isDir() && !fi.completeBaseName().isEmpty())
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
                PaneMode paneMode = Settings::getInstance()->getPaneMode();
                FileSizeFormatType formatType = (paneMode == PaneMode::Single) ? Settings::getInstance()->getSinglePaneFileSizeFormatType() :
                                                                                 Settings::getInstance()->getDualPaneFileSizeFormatType();
                if(formatType == FileSizeFormatType::Detail)
                {
                    bool comma = (paneMode == PaneMode::Single) ? Settings::getInstance()->getSinglePaneFileSizeDetailCommaEnable() :
                                                                  Settings::getInstance()->getDualPaneFileSizeDetailCommaEnable();

                    ret = (comma) ? QLocale(QLocale::English).toString(fi.size()) : QString::number(fi.size());
                }
                else
                {
                    ret = QLocale().formattedDataSize(fi.size(), 2,
                                                      (formatType == FileSizeFormatType::IEC) ? QLocale::DataSizeIecFormat :
                                                                                                QLocale::DataSizeSIFormat);
                }
            }
            break;
        case SectionType::LastModified:
        {
            PaneMode paneMode = Settings::getInstance()->getPaneMode();
            DateFormatType formatType = (paneMode == PaneMode::Single) ? Settings::getInstance()->getSinglePaneDateFormatType() :
                                                                         Settings::getInstance()->getDualPaneDateFormatType();
            switch(formatType)
            {
            case DateFormatType::ISO:
                ret = fi.lastModified().toString(Qt::ISODate);
                break;
            case DateFormatType::Original:
            {
                QString orgFormat = (paneMode == PaneMode::Single) ? Settings::getInstance()->getSinglePaneDateFormatOriginalString() :
                                                                     Settings::getInstance()->getDualPaneDateFormatOriginalString();

                ret = fi.lastModified().toString(orgFormat);
                break;
            }
            case DateFormatType::Default:
            default:
                ret = fi.lastModified().toString(Qt::TextDate);
                break;
            }
            break;
        }
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
    m_sortColumn = column;
    m_sortOrder = order;

    refresh();
}

void FolderModel::refresh()
{
    QSortFilterProxyModel::sort(m_sortColumn, m_sortOrder);

    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());
    QString backup = fsModel->rootPath();
    fsModel->setRootPath("");
    fsModel->setRootPath(backup);
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

bool FolderModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    // source_parent は mapToSource されているようなので、QFileSystemModel に渡す際は mapToSource しない
    QFileInfo pfi = fsModel->fileInfo(source_parent);

//    qDebug() << "FolderModel::filterAcceptsRow() : source_row : " << source_row << ", source_parent : " << pfi.filePath();

    QModelIndex childIndex = fsModel->index(source_row, 0, source_parent);
    QFileInfo cfi = fsModel->fileInfo(childIndex);

    if(pfi.isRoot() && cfi.fileName() == "..")
    {
        return false;
    }

    if(!(m_filterFlags & FilterFlag::Hidden) && cfi.isHidden() && cfi.fileName() != "..")
    {
        return false;
    }

#ifdef Q_OS_WIN
    if(!(m_filterFlags & FilterFlag::System) && isWindowsSystemFile(cfi))
    {
        return false;
    }
#endif
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

bool FolderModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    QFileInfo l_info = fsModel->fileInfo(source_left);
    QFileInfo r_info = fsModel->fileInfo(source_right);
    bool ascOrder = (m_sortOrder == Qt::AscendingOrder);

//    qDebug() << "FolderModel::lessThan() : source_left : " << l_info.filePath() << ", source_right : " << r_info.filePath();

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

    SectionType sortSectionType = getSectionTypeFromColumn(m_sortColumn);

    if(sortSectionType == SectionType::FileSize)
    {
        if(!l_info.isDir() && !r_info.isDir())
        {
            return l_info.size() < r_info.size();
        }
    }
    else if(sortSectionType == SectionType::FileType)
    {
        QString l_type = (!l_info.isDir() && !l_info.completeBaseName().isEmpty()) ? l_info.suffix() : "";
        QString r_type = (!r_info.isDir() && !r_info.completeBaseName().isEmpty()) ? r_info.suffix() : "";

        if(l_type.isEmpty() && r_type.isEmpty())
        {
            l_type = l_info.fileName();
            r_type = r_info.fileName();
        }

        if(sortCaseSensitivity() == Qt::CaseInsensitive && l_type.toLower() != r_type.toLower())
        {
            return l_type.toLower() < r_type.toLower();
        }
        else
        {
            return l_type < r_type;
        }
    }
    else if(sortSectionType == SectionType::LastModified)
    {
        return l_info.lastModified() < r_info.lastModified();
    }
    else
    {
        QString l_name = (!l_info.isDir() && !l_info.completeBaseName().isEmpty()) ? l_info.completeBaseName() : l_info.fileName();
        QString r_name = (!r_info.isDir() && !r_info.completeBaseName().isEmpty()) ? r_info.completeBaseName() : r_info.fileName();

        if(sortCaseSensitivity() == Qt::CaseInsensitive && l_name.toLower() != r_name.toLower())
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

bool FolderModel::isWindowsSystemFile(const QFileInfo& fileInfo) const
{
    DWORD attrFlags = ::GetFileAttributes(fileInfo.filePath().toStdWString().c_str());
    if(attrFlags != static_cast<DWORD>(-1) && (attrFlags & FILE_ATTRIBUTE_SYSTEM))
    {
        return true;
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

int FolderModel::getColumnFromSectionType(SectionType sectionType) const
{
    if(sectionType == SectionType::FileName)
    {
        return 0;
    }
    else if(sectionType == SectionType::FileType)
    {
        return 1;
    }
    else if(sectionType == SectionType::FileSize)
    {
        return 2;
    }
    else if(sectionType == SectionType::LastModified)
    {
        return 3;
    }

    return -1;
}

QBrush FolderModel::getTextBrush(const QModelIndex& index) const
{
    QBrush ret;

    QFileInfo fi = fileInfo(index);
    bool selected = isSelected(index);

    if(fi.isDir())
    {
        if(selected)
        {
            ret = getBrush(ColorRoleType::Folder_Selected);
        }
        else
        {
            ret = getBrush(ColorRoleType::Folder);
        }
    }
#ifdef Q_OS_WIN
    else if(fi.fileName() != ".." && isWindowsSystemFile(fi))
    {
        if(selected)
        {
            ret = getBrush(ColorRoleType::System_Selected);
        }
        else
        {
            ret = getBrush(ColorRoleType::System);
        }
    }
#endif
    else if((fi.fileName() != "..") && (fi.isHidden()))
    {
        if(selected)
        {
            ret = getBrush(ColorRoleType::Hidden_Selected);
        }
        else
        {
            ret = getBrush(ColorRoleType::Hidden);
        }
    }
    else if((fi.fileName() != "..") && (!fi.isWritable()))
    {
        if(selected)
        {
            ret = getBrush(ColorRoleType::ReadOnly_Selected);
        }
        else
        {
            ret = getBrush(ColorRoleType::ReadOnly);
        }
    }
    else
    {
        if(selected)
        {
            ret = getBrush(ColorRoleType::Normal_Selected);
        }
        else
        {
            ret = getBrush(ColorRoleType::Normal);
        }
    }

    return ret;
}

QBrush FolderModel::getBackgroundBrush(const QModelIndex& index) const
{
    QBrush ret;

    if(isSelected(index))
    {
        ret = getBrush(ColorRoleType::Selected_Background);
    }
    else
    {
        ret = getBrush(ColorRoleType::Background);
    }

    return ret;
}

QBrush FolderModel::getBrush(ColorRoleType colorRole) const
{
    QBrush ret;

    QMap<ColorRoleType, QBrush>::const_iterator itr = m_brushes.find(colorRole);
    if(itr != m_brushes.end())
    {
        ret = *itr;
    }

    return ret;
}

void FolderModel::setFont(const QFont& font)
{
    m_font = font;
}

void FolderModel::initBrushes(const QMap<ColorRoleType, QColor>& colors)
{
    m_brushes.clear();

    for(auto colorRole : colors.keys())
    {
        m_brushes[colorRole] = QBrush(colors[colorRole]);
    }
}

bool FolderModel::isSelected(const QModelIndex& index) const
{
    if(m_selectionModel != Q_NULLPTR)
    {
        return m_selectionModel->isSelected(index);
    }

    return false;
}

void FolderModel::emitRootPathChanged(const QString &newPath)
{
    emit rootPathChanged(newPath);
}

void FolderModel::emitFileRenamed(const QString &path, const QString &oldName, const QString &newName)
{
    emit fileRenamed(path, oldName, newName);
}

void FolderModel::emitDirectoryLoaded(const QString &path)
{
    emit directoryLoaded(path);
}

void FolderModel::emitLayoutChanged(const QList<QPersistentModelIndex>& parents/* = QList<QPersistentModelIndex>()*/, QAbstractItemModel::LayoutChangeHint hint/* = QAbstractItemModel::NoLayoutChangeHint*/)
{
    emit layoutChanged(parents, hint);
}

void FolderModel::emitLayoutAboutToBeChanged(const QList<QPersistentModelIndex>& parents/* = QList<QPersistentModelIndex>()*/, QAbstractItemModel::LayoutChangeHint hint/* = QAbstractItemModel::NoLayoutChangeHint*/)
{
    emit layoutAboutToBeChanged(parents, hint);
}

// QFileSystemModel specific API
QModelIndex FolderModel::setRootPath(const QString &path)
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return mapFromSource(fsModel->setRootPath(path));
}

QString FolderModel::rootPath() const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->rootPath();
}

QDir FolderModel::rootDirectory() const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->rootDirectory();
}

void FolderModel::setIconProvider(QFileIconProvider *provider)
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    fsModel->setIconProvider(provider);
}

QFileIconProvider* FolderModel::iconProvider() const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->iconProvider();
}

void FolderModel::setResolveSymlinks(bool enable)
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    fsModel->setResolveSymlinks(enable);
}

bool FolderModel::resolveSymlinks() const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->resolveSymlinks();
}

void FolderModel::setReadOnly(bool enable)
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    fsModel->setReadOnly(enable);
}

bool FolderModel::isReadOnly() const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->isReadOnly();
}

void FolderModel::setNameFilterDisables(bool enable)
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    fsModel->setNameFilterDisables(enable);
}

bool FolderModel::nameFilterDisables() const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->nameFilterDisables();
}

void FolderModel::setNameFilters(const QStringList &filters)
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->setNameFilters(filters);
}

QStringList FolderModel::nameFilters() const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->nameFilters();
}

QString FolderModel::filePath(const QModelIndex &index) const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->filePath(mapToSource(index));
}

bool FolderModel::isDir(const QModelIndex &index) const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->isDir(mapToSource(index));
}

qint64 FolderModel::size(const QModelIndex &index) const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->size(mapToSource(index));
}

QString FolderModel::type(const QModelIndex &index) const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->type(mapToSource(index));
}

QDateTime FolderModel::lastModified(const QModelIndex &index) const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->lastModified(mapToSource(index));
}

QModelIndex FolderModel::mkdir(const QModelIndex &parent, const QString &name)
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return mapFromSource(fsModel->mkdir(mapToSource(parent), name));
}

bool FolderModel::rmdir(const QModelIndex &index)
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->rmdir(mapToSource(index));
}

QString FolderModel::fileName(const QModelIndex &index) const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->fileName(mapToSource(index));
}

QIcon FolderModel::fileIcon(const QModelIndex &index) const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->fileIcon(mapToSource(index));
}

QFile::Permissions FolderModel::permissions(const QModelIndex &index) const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->permissions(mapToSource(index));
}

QFileInfo FolderModel::fileInfo(const QModelIndex &index) const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->fileInfo(mapToSource(index));
}

bool FolderModel::remove(const QModelIndex &index)
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    return fsModel->remove(mapToSource(index));
}

}           // namespace Farman
