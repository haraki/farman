#include <QFileSystemModel>
#include <QDir>
#include <QDateTime>
#include <QDebug>
#include <QItemSelectionModel>
#include <QFileIconProvider>
#include <functional>
#include "foldermodel.h"
#include "types.h"
#include "win32.h"

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
    m_sortColumn2nd(-1),
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

void FolderModel::setSortSectionType2nd(SectionType sectionType2nd)
{
    setSortColumn2nd(getColumnFromSectionType(sectionType2nd));
}

SectionType FolderModel::sortSectionType() const
{
    return getSectionTypeFromColumn(m_sortColumn);
}

SectionType FolderModel::sortSectionType2nd() const
{
    return getSectionTypeFromColumn(m_sortColumn2nd);
}

void FolderModel::setSortColumn(int column)
{
    m_sortColumn = column;
}

int FolderModel::sortColumn() const
{
    return m_sortColumn;
}

void FolderModel::setSortColumn2nd(int column)
{
    m_sortColumn2nd = column;
}

int FolderModel::sortColumn2nd() const
{
    return m_sortColumn2nd;
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

void FolderModel::setAttrFilterFlags(AttrFilterFlags attrfilterFlags)
{
    m_attrFilterFlags = attrfilterFlags;
}

AttrFilterFlags FolderModel::getAttrFilterFlags() const
{
    return m_attrFilterFlags;
}

void FolderModel::setFileSizeFormatType(FileSizeFormatType formatType)
{
    m_fileSizeFormatType = formatType;
}

FileSizeFormatType FolderModel::getFileSizeFormatType() const
{
    return m_fileSizeFormatType;
}

void FolderModel::setFileSizeComma(bool comma)
{
    m_fileSizeComma = comma;
}

bool FolderModel::getFileSizeComma() const
{
    return m_fileSizeComma;
}

void FolderModel::setDateFormatType(DateFormatType formatType)
{
    m_dateFormatType = formatType;
}

DateFormatType FolderModel::getDateFormatType() const
{
    return m_dateFormatType;
}

void FolderModel::setDateFormatOriginalString(const QString& orgString)
{
    m_dateFormatOriginalString = orgString;
}

QString FolderModel::getDateFormatOriginalString() const
{
    return m_dateFormatOriginalString;
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
            if(fi.isDir())
            {
                ret = QString("<Folder>");
            }
            else
            {
                if(m_fileSizeFormatType == FileSizeFormatType::Detail)
                {
                    ret = (m_fileSizeComma) ? QLocale(QLocale::English).toString(fi.size()) : QString::number(fi.size());
                }
                else
                {
                    ret = QLocale().formattedDataSize(fi.size(), 2,
                                                      (m_fileSizeFormatType == FileSizeFormatType::IEC) ? QLocale::DataSizeIecFormat :
                                                                                                          QLocale::DataSizeSIFormat);
                }
            }
            break;

        case SectionType::LastModified:
        {
            switch(m_dateFormatType)
            {
            case DateFormatType::ISO:
                ret = fi.lastModified().toString(Qt::ISODate);
                break;
            case DateFormatType::Original:
            {
                ret = fi.lastModified().toString(m_dateFormatOriginalString);
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
    m_sortColumn2nd = -1;
    m_sortOrder = order;

    refresh();
}

void FolderModel::refresh()
{
    QSortFilterProxyModel::sort(0, m_sortOrder);

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

void FolderModel::setSelectAll()
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());
    const QModelIndex& parentIndex = mapFromSource(fsModel->index(fsModel->rootPath()));

    bool dotDotCheck = true;        // ".."は一つしか存在しないので、一つ発見したら後は".."の存在をチェックしないようにするためのフラグ
    for(int row = 0;row < rowCount(parentIndex);row++)
    {
        if(!dotDotCheck || fsModel->fileName(index(row, 0, parentIndex)) != "..")
        {
            setSelect(row, QItemSelectionModel::Select, parentIndex);
        }
        else
        {
            dotDotCheck = false;
        }
    }
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
        // ルートディレクトリより上には行けないようにする
        return false;
    }

    if((!(m_attrFilterFlags & AttrFilterFlag::Hidden) && cfi.isHidden())
#ifdef Q_OS_WIN
    || (!(m_attrFilterFlags & AttrFilterFlag::System) && Win32::isSystemFile(cfi.absoluteFilePath()))
#endif
      )
    {
        // 隠しファイル非表示時、カレントディレクトリが属するディレクトリは隠しであっても表示する
        // 非表示にすると、隠し属性の親ディレクトリに移動した際、FolderView が正常に表示されなくなってしまう現象を回避するため
        if(!cfi.isDir())
        {
            return false;
        }

        // source_row(childIndex)が指すディレクトリが、カレントディレクトリ(fsModel->rootPath())が属するディレクトリであるかを確認する
        std::function<bool (const QModelIndex&)> isBelong =
                [&isBelong, &fsModel, &childIndex](const QModelIndex& dirIndex)
        {
            if(!dirIndex.isValid() || fsModel->fileInfo(dirIndex).isRoot())
            {
                return false;
            }
            else if(childIndex == dirIndex)
            {
                return true;
            }
            return isBelong(dirIndex.parent());
        };

        if(isBelong(fsModel->index(fsModel->rootPath())))
        {
            return true;
        }
        else if(cfi.fileName() != "..")
        {
            return false;
        }
    }

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
    SectionType sortSectionType2nd = getSectionTypeFromColumn(m_sortColumn2nd);
    Qt::CaseSensitivity caseSensitivity = sortCaseSensitivity();

    return sectionTypeLessThan(l_info, r_info, sortSectionType, sortSectionType2nd, caseSensitivity);
}

bool FolderModel::sectionTypeLessThan(QFileInfo& l_info, QFileInfo& r_info, SectionType sectionType, SectionType sectionType2nd, Qt::CaseSensitivity caseSensitivity) const
{
    if(sectionType == SectionType::FileSize)
    {
        if(!l_info.isDir() && !r_info.isDir())
        {
            if(sectionType2nd != SectionType::NoSpecify && l_info.size() == r_info.size())
            {
                return sectionTypeLessThan(l_info, r_info, sectionType2nd, SectionType::NoSpecify, caseSensitivity);
            }
            else
            {
                return l_info.size() < r_info.size();
            }
        }
    }
    else if(sectionType == SectionType::FileType)
    {
        QString l_type = (!l_info.isDir() && !l_info.completeBaseName().isEmpty()) ? l_info.suffix() : "";
        QString r_type = (!r_info.isDir() && !r_info.completeBaseName().isEmpty()) ? r_info.suffix() : "";

        if(l_type.isEmpty() && r_type.isEmpty())
        {
            l_type = l_info.fileName();
            r_type = r_info.fileName();
        }

        if(caseSensitivity == Qt::CaseInsensitive)
        {
            l_type = l_type.toLower();
            r_type = r_type.toLower();
        }

        if(sectionType2nd != SectionType::NoSpecify && l_type == r_type)
        {
            return sectionTypeLessThan(l_info, r_info, sectionType2nd, SectionType::NoSpecify, caseSensitivity);
        }
        else
        {
            return l_type < r_type;
        }
    }
    else if(sectionType == SectionType::LastModified)
    {
        if(sectionType2nd != SectionType::NoSpecify && l_info.lastModified() == r_info.lastModified())
        {
            return sectionTypeLessThan(l_info, r_info, sectionType2nd, SectionType::NoSpecify, caseSensitivity);
        }
        else
        {
            return l_info.lastModified() < r_info.lastModified();
        }
    }
    else
    {
        QString l_name = (!l_info.isDir() && !l_info.completeBaseName().isEmpty()) ? l_info.completeBaseName() : l_info.fileName();
        QString r_name = (!r_info.isDir() && !r_info.completeBaseName().isEmpty()) ? r_info.completeBaseName() : r_info.fileName();

        if(caseSensitivity == Qt::CaseInsensitive)
        {
            l_name = l_name.toLower();
            r_name = r_name.toLower();
        }

        if(sectionType2nd != SectionType::NoSpecify && l_name == r_name)
        {
            return sectionTypeLessThan(l_info, r_info, sectionType2nd, SectionType::NoSpecify, caseSensitivity);
        }
        else
        {
            return l_name < r_name;
        }
    }

    return false;
}

int FolderModel::getFileNum()
{
    return getFileDirNum(QDir::Files);
}

int FolderModel::getDirNum()
{
    return getFileDirNum(QDir::Dirs);
}

int FolderModel::getFileDirNum()
{
    return getFileDirNum(QDir::Files | QDir::Dirs);
}

int FolderModel::getFileDirNum(QDir::Filters filters)
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    QModelIndex index = fsModel->index(fsModel->rootPath());
    if(!index.isValid())
    {
        return -1;
    }

    QFileInfo pfi = fileInfo(mapFromSource(index));
    if(!pfi.isDir())
    {
        return -1;
    }

    QDir dir = QDir(pfi.absoluteFilePath());
    QDir::Filters dirFilters = QDir::NoDotAndDotDot | (fsModel->filter() & ~QDir::AllEntries) | filters;

    int count = 0;
    for(const QFileInfo& cfi : dir.entryInfoList(fsModel->nameFilters(), dirFilters))
    {
        if(!(m_attrFilterFlags & AttrFilterFlag::Hidden) && cfi.isHidden())
        {
            continue;
        }

#ifdef Q_OS_WIN
        if(!(m_attrFilterFlags & FilterFlag::System) && Win32::isSystemFile(cfi.absoluteFilePath()))
        {
            continue;
        }
#endif
        count++;
    }

    return count;
}

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

    return SectionType::NoSpecify;
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

    if(m_folderColorTopPriority && fi.isDir())
    {
        if(selected)
        {
            ret = getBrush(FolderViewColorRoleType::Folder_Selected);
        }
        else
        {
            ret = getBrush(FolderViewColorRoleType::Folder);
        }
    }
#ifdef Q_OS_WIN
    else if(fi.fileName() != ".." && Win32::isSystemFile(fi.absoluteFilePath()))
    {
        if(selected)
        {
            ret = getBrush(FolderViewColorRoleType::System_Selected);
        }
        else
        {
            ret = getBrush(FolderViewColorRoleType::System);
        }
    }
#endif
    else if((fi.fileName() != "..") && (fi.isHidden()))
    {
        if(selected)
        {
            ret = getBrush(FolderViewColorRoleType::Hidden_Selected);
        }
        else
        {
            ret = getBrush(FolderViewColorRoleType::Hidden);
        }
    }
    else if((fi.fileName() != "..") && (!fi.isWritable()))
    {
        if(selected)
        {
            ret = getBrush(FolderViewColorRoleType::ReadOnly_Selected);
        }
        else
        {
            ret = getBrush(FolderViewColorRoleType::ReadOnly);
        }
    }
    else if(!m_folderColorTopPriority && fi.isDir())
    {
        if(selected)
        {
            ret = getBrush(FolderViewColorRoleType::Folder_Selected);
        }
        else
        {
            ret = getBrush(FolderViewColorRoleType::Folder);
        }
    }
    else
    {
        if(selected)
        {
            ret = getBrush(FolderViewColorRoleType::Normal_Selected);
        }
        else
        {
            ret = getBrush(FolderViewColorRoleType::Normal);
        }
    }

    return ret;
}

QBrush FolderModel::getBackgroundBrush(const QModelIndex& index) const
{
    QBrush ret;

    if(isSelected(index))
    {
        ret = getBrush(FolderViewColorRoleType::Selected_Background);
    }
    else
    {
        ret = getBrush(FolderViewColorRoleType::Background);
    }

    return ret;
}

QBrush FolderModel::getBrush(FolderViewColorRoleType colorRole) const
{
    QBrush ret;

    QMap<FolderViewColorRoleType, QBrush>::const_iterator itr = m_brushes.find(colorRole);
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

void FolderModel::initBrushes(const QMap<FolderViewColorRoleType, QColor>& colors, bool folderColorTopPrio)
{
    m_brushes.clear();

    for(auto colorRole : colors.keys())
    {
        m_brushes[colorRole] = QBrush(colors[colorRole]);
    }

    m_folderColorTopPriority = folderColorTopPrio;
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
    QStringList filterList = filters;

    // 必ず親ディレクトリ("..")は表示
    if(filterList[0] != "..")
    {
        filterList.push_front("..");
    }

    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    fsModel->setNameFilters(filterList);
}

QStringList FolderModel::nameFilters() const
{
    QFileSystemModel* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

    QStringList filterList = fsModel->nameFilters();
    if(filterList[0] == "..")
    {
        filterList.pop_front();
    }

    return filterList;
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
