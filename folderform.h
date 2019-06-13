#ifndef FOLDERFORM_H
#define FOLDERFORM_H

#include <QWidget>
#include <QDir>
#include <QModelIndex>
#include "types.h"

namespace Ui {
class FolderForm;
}
class QString;
class QResizeEvent;

namespace Farman
{
class FolderModel;

class FolderForm : public QWidget
{
    Q_OBJECT

public:
    explicit FolderForm(FilterFlags filterFlags,
                        const QStringList& nameMaskFilters,
                        SectionType sortSectionType,
                        SortDirsType sortDirsType,
                        bool sortDotFirst,
                        Qt::CaseSensitivity sortCaseSensitivity,
                        Qt::SortOrder sortOrder,
                        QWidget *parent = Q_NULLPTR);
    ~FolderForm() Q_DECL_OVERRIDE;

    void setAppearance(const QFont& viewFont,
                       const QFont& pathFont,
                       const QMap<ColorRoleType, QColor>& colors,
                       const QColor& pathColor,
                       const QColor& pathBgColor);

    void setFilterFlags(FilterFlags filterFlags);
    FilterFlags getFilterFlags() const;
    void setNameMaskFilters(const QStringList& nameMaskFilters);
    QStringList getNameMaskFilters() const;

    void setSortSettings(SectionType sectionType,
                         SortDirsType dirsType,
                         bool dotFirst,
                         Qt::CaseSensitivity caseSensitivity,
                         Qt::SortOrder order);
    SectionType getSortSectionType() const;
    int getSortColumn() const;
    SortDirsType getSortDirsType() const;
    bool getSortDotFirst() const;
    Qt::CaseSensitivity getSortCaseSensitivity() const;
    Qt::SortOrder getSortOrder() const;

    int setPath(const QString& dirPath);

    QString getCurrentDirPath();
    QString getCurrentFileName();
    QFileInfo getCurrentFileInfo();

    QList<QFileInfo> getSelectedFileInfoList();

    void setCursor(const QString& fileName);

    int onGoToChildDir();
    int onGoToParentDir();
    int onSelectDir();
    int onBookmarkDir(bool marked);

    void refresh(bool clearSelected = false);

Q_SIGNALS:
    void currentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void focusChanged(bool inFocus);

protected Q_SLOTS:
    void onCurrentChanged(const QModelIndex& newIndex, const QModelIndex& oldIndex);
    void onDirectoryLoaded(const QString &path);
    void onLayoutChanged(const QList<QPersistentModelIndex>& parents = QList<QPersistentModelIndex>(), QAbstractItemModel::LayoutChangeHint hint = QAbstractItemModel::NoLayoutChangeHint);
    void onLayoutAboutToBeChanged(const QList<QPersistentModelIndex>& parents = QList<QPersistentModelIndex>(), QAbstractItemModel::LayoutChangeHint hint = QAbstractItemModel::NoLayoutChangeHint);

private Q_SLOTS:
    void on_selectFolderButton_clicked();
    void on_bookmarkToolButton_toggled(bool checked);

    void emitCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void emitFocusChanged(bool inFocus);

private:
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;

    int getTotalColumnWidth(int withOutColumn = -1);

private:
    Ui::FolderForm *ui;

    FolderModel *m_folderModel;

//    bool m_isSettingPath;
};

}           // namespace Farman

#endif // FOLDERFORM_H
