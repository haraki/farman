#ifndef FOLDERMODEL_H
#define FOLDERMODEL_H

#include <QSortFilterProxyModel>
#include <QDir>
#include <QFont>
#include <QBrush>
#include <QItemSelectionModel>
#include "types.h"

class QFileSystemModel;
class QFileIconProvider;

namespace Farman
{

class FolderModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit FolderModel(QObject *parent = Q_NULLPTR);
    ~FolderModel() Q_DECL_OVERRIDE;

    void setFont(const QFont& font);
    void initBrushes(const QMap<FolderViewColorRoleType, QColor>& colors, bool folderColorTopPrio);

    using QSortFilterProxyModel::index;
    QModelIndex index(const QString &path, int column = 0) const;

    bool isDirectoryLoading() { return m_isDirectoryLoading; }

    void setSortSectionType(SectionType sectionType);
    SectionType sortSectionType() const;
    void setSortSectionType2nd(SectionType sectionType2nd);
    SectionType sortSectionType2nd() const;

    void setSortColumn(int column);
    int sortColumn() const;
    void setSortColumn2nd(int column);
    int sortColumn2nd() const;

    void setSortDirsType(SortDirsType dirsType);
    SortDirsType sortDirsType() const;

    void setSortDotFirst(bool dotFirst);
    bool sortDotFirst() const;

    void setSortOrder(Qt::SortOrder order);
    Qt::SortOrder sortOrder() const;

    void setAttrFilterFlags(AttrFilterFlags attrFilterFlags);
    AttrFilterFlags getAttrFilterFlags() const;
    void setFileFolderFilterType(FileFolderFilterType fileFolderFilterType);
    FileFolderFilterType getFileFolderFilterType() const;

    void setFileSizeFormatType(FileSizeFormatType formatType);
    FileSizeFormatType getFileSizeFormatType() const;

    void setFileSizeComma(bool comma);
    bool getFileSizeComma() const;

    void setDateFormatType(DateFormatType formatType);
    DateFormatType getDateFormatType() const;

    void setDateFormatOriginalString(const QString& orgString);
    QString getDateFormatOriginalString() const;

    int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &modelIndex, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) Q_DECL_OVERRIDE;

    void refresh();

    QItemSelectionModel* getSelectionModel();
    void setSelect(int row, QItemSelectionModel::SelectionFlags selectionFlags, const QModelIndex &parentIndex);
    QModelIndexList getSelectedIndexList();
    void setSelectAll();
    void clearSelected();

    // カレントディレクトリ内のファイル・ディレクトリ数を返す
    int getFileNum();               // ファイル数を返す(ディレクトリは含まない)
    int getDirNum();                // ディレクトリ数を返す(".." は除外)
    int getFileDirNum();            // getFileNum() + getDirNum()

    // QFileSystemModel specific API
    QModelIndex setRootPath(const QString &path);
    QString rootPath() const;
    QDir rootDirectory() const;

    void setIconProvider(QFileIconProvider *provider);
    QFileIconProvider *iconProvider() const;

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

Q_SIGNALS:
    void rootPathChanged(const QString &newPath);
    void fileRenamed(const QString &path, const QString &oldName, const QString &newName);
    void directoryLoaded(const QString &path);

private Q_SLOTS:
    void onRootPathChanged(const QString &newPath);
    void onFileRenamed(const QString &path, const QString &oldName, const QString &newName);
    void onDirectoryLoaded(const QString &path);

private:
    int getFileDirNum(QDir::Filters filters);

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;

    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const Q_DECL_OVERRIDE;
    bool sectionTypeLessThan(QFileInfo& l_info, QFileInfo& r_info, SectionType sectionType, SectionType sectionType2nd, Qt::CaseSensitivity caseSensitivity) const;

    bool isSelected(const QModelIndex& index) const;

    SectionType getSectionTypeFromColumn(int column) const;
    int getColumnFromSectionType(SectionType sectionType) const;

    QBrush getTextBrush(const QModelIndex& index) const;
    QBrush getBackgroundBrush(const QModelIndex& index) const;
    QBrush getBrush(FolderViewColorRoleType colorRole) const;

    void emitRootPathChanged(const QString &newPath);
    void emitFileRenamed(const QString &path, const QString &oldName, const QString &newName);
    void emitDirectoryLoaded(const QString &path);

    bool m_isDirectoryLoading;

    AttrFilterFlags m_attrFilterFlags;
    FileFolderFilterType m_fileFolderFilterType;

    int m_sortColumn;
    int m_sortColumn2nd;
    SortDirsType m_sortDirsType;
    bool m_sortDotFirst;
    Qt::SortOrder m_sortOrder;

    FileSizeFormatType m_fileSizeFormatType;
    bool m_fileSizeComma;

    DateFormatType m_dateFormatType;
    QString m_dateFormatOriginalString;

    QFont m_font;
    QMap<FolderViewColorRoleType, QBrush> m_brushes;

    int m_iconSize;

    bool m_folderColorTopPriority;

    QItemSelectionModel* m_selectionModel;
};

}           // namespace Farman

#endif // FOLDERMODEL_H
