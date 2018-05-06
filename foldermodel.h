#ifndef FOLDERMODEL_H
#define FOLDERMODEL_H

#include <QSortFilterProxyModel>
#include <QDir>
#include <QFont>
#include <QBrush>
#include <QItemSelectionModel>

class QFileSystemModel;
class QFileIconProvider;

namespace Farman
{

class FolderModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit FolderModel(QObject *parent = Q_NULLPTR);
    ~FolderModel();

    void updateSettings();

    using QSortFilterProxyModel::index;
    QModelIndex index(const QString &path, int column = 0) const;

    void setSorting(QDir::SortFlags sort);
    QDir::SortFlags sorting() const;

    void setDotFirst(bool enable);
    bool dotFirst();

    int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &modelIndex, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) Q_DECL_OVERRIDE;

    void refresh();

    QItemSelectionModel* getSelectionModel();
    void setSelect(int row, QItemSelectionModel::SelectionFlags selectionFlags, const QModelIndex &parentIndex);
    QModelIndexList getSelectedIndexList();
    void clearSelected();

#ifdef Q_OS_WIN
    bool isDrive(const QModelIndex& index) const;
#endif

    // QFileSystemModel specific API
    QModelIndex setRootPath(const QString &path);
    QString rootPath() const;
    QDir rootDirectory() const;

    void setIconProvider(QFileIconProvider *provider);
    QFileIconProvider *iconProvider() const;

    void setFilter(QDir::Filters filters);
    QDir::Filters filter() const;

    void setResolveSymlinks(bool enable);
    bool resolveSymlinks() const;

    void setReadOnly(bool enable);
    bool isReadOnly() const;

    void setNameFilterDisables(bool enable);
    bool nameFilterDisables() const;

    void setNameFilters(const QStringList &filters);
    QStringList nameFilters() const;

    QString filePath(const QModelIndex &index) const;
    bool isDir(const QModelIndex &index) const;
    qint64 size(const QModelIndex &index) const;
    QString type(const QModelIndex &index) const;
    QDateTime lastModified(const QModelIndex &index) const;

    QModelIndex mkdir(const QModelIndex &parent, const QString &name);
    bool rmdir(const QModelIndex &index);
    QString fileName(const QModelIndex &index) const;
    QIcon fileIcon(const QModelIndex &index) const;
    QFile::Permissions permissions(const QModelIndex &index) const;
    QFileInfo fileInfo(const QModelIndex &index) const;
    bool remove(const QModelIndex &index);

private:
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const Q_DECL_OVERRIDE;

    bool isSelected(const QModelIndex& index) const;

    // don't use
    Qt::CaseSensitivity sortCaseSensitivity() const;
    void setSortCaseSensitivity(Qt::CaseSensitivity cs);

    enum class SectionType : int
    {
        Unknown = -1,

        FileName = 0,
        FileSize,
        FileType,
        LastModified,

        SectionTypeNum
    };

    SectionType getSectionTypeFromColumn(int column) const;

    enum class BrushType : int
    {
        Unknown = -1,

        Normal,
        Normal_Selected,
        Folder,
        Folder_Selected,
        ReadOnly,
        ReadOnly_Selected,
        Hidden,
        Hidden_Selected,
        System,
        System_Selected,

        Background,
        Selected_Background,

        BrushTypeNum
    };

    QBrush getTextBrush(const QModelIndex& index) const;
    QBrush getBackgroundBrush(const QModelIndex& index) const;
    QBrush getBrush(BrushType brushType) const;
    void initFont();
    void initBrush();

    QFileSystemModel* m_fsModel;
    QDir::SortFlags m_sortFlags;
    Qt::SortOrder m_sortOrder;
    bool m_dotFirst;

    QFont m_font;
    QMap<BrushType, QBrush> m_brush;

    QItemSelectionModel* m_selectionModel;
};

}           // namespace Farman

#endif // FOLDERMODEL_H
