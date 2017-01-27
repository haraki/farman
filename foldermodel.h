#ifndef FOLDERMODEL_H
#define FOLDERMODEL_H

#include <QSet>
#include <QAbstractItemModel>
#include <qdir.h>
#include <QDir>
#include <QDirModel>
#include <QPalette>
#include <QItemSelectionModel>

class QObject;
class QStringList;
class QDirModel;
class QVariant;
class QItemSelectionModel;

namespace Farman
{

class FolderModel : public QDirModel
{
public:
    explicit FolderModel(const QStringList &nameFilters, QDir::Filters filters, QDir::SortFlags sort, QObject *parent = Q_NULLPTR);
    explicit FolderModel(QObject *parent = Q_NULLPTR);
    ~FolderModel();
    QVariant data(const QModelIndex &modelIndex, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) Q_DECL_OVERRIDE;
    QItemSelectionModel* getSelectionModel();
    void setSelect(int row, QItemSelectionModel::SelectionFlags selectionFlags, const QModelIndex &parentIndex);
    void clearSelected();

private:
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

        Background,
        Background_Selected,

        Normal,
        Normal_Selected,
        ReadOnly,
        ReadOnly_Selected,
        Hidden,
        Hidden_Selected,

        BrushTypeNum
    };

    QBrush getTextBrush(const QModelIndex& index) const;
    QBrush getBackgroundBrush(const QModelIndex& index) const;
    QBrush getBrush(BrushType brushType) const;
    void initBrush();

    QMap<BrushType, QBrush> m_brush;

    bool isSelected(const QModelIndex& index) const;

    QItemSelectionModel* m_selectionModel;
};



#if 0
class QDirModel;
class QFileIconProvider;

class FolderModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit FolderModel(QObject *parent = 0);

    virtual ~FolderModel();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Fetch data dynamically:
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;


    void setIconProvider(QFileIconProvider *provider);
    QFileIconProvider *iconProvider() const;

    void setNameFilters(const QStringList &filters);
    QStringList nameFilters() const;

    void setFilter(QDir::Filters filters);
    QDir::Filters filter() const;

    void setSorting(QDir::SortFlags sort);
    QDir::SortFlags sorting() const;

    void setResolveSymlinks(bool enable);
    bool resolveSymlinks() const;

    void setReadOnly(bool enable);
    bool isReadOnly() const;

    void setLazyChildCount(bool enable);
    bool lazyChildCount() const;

    QModelIndex index(const QString &path, int column = 0) const;

    bool isDir(const QModelIndex &index) const;
    QModelIndex mkdir(const QModelIndex &parent, const QString &name);
    bool rmdir(const QModelIndex &index);
    bool remove(const QModelIndex &index);

    QString filePath(const QModelIndex &index) const;
    QString fileName(const QModelIndex &index) const;
    QIcon fileIcon(const QModelIndex &index) const;
    QFileInfo fileInfo(const QModelIndex &index) const;
};

#endif

}           // namespace Farman

#endif // FOLDERMODEL_H
