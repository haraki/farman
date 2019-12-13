#ifndef FOLDERFORM_H
#define FOLDERFORM_H

#include <QWidget>
#include <QDir>
#include <QModelIndex>
#include <QItemSelectionModel>
#include "historymanager.h"
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
    explicit FolderForm(PaneType pane,
                        AttrFilterFlags attrFilterFlags,
                        SectionType sortSectionType,
                        SectionType sortSectionType2nd,
                        SortDirsType sortDirsType,
                        bool sortDotFirst,
                        Qt::CaseSensitivity sortCaseSensitivity,
                        Qt::SortOrder sortOrder,
                        FileSizeFormatType fileSizeformatType,
                        bool fileSizeComma,
                        DateFormatType dateFormatType,
                        QString dateOrgString,
                        QWidget *parent = Q_NULLPTR);
    ~FolderForm() Q_DECL_OVERRIDE;

    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;

    PaneType getPaneType() const { return m_paneType; }

    void setAppearance(const QFont& viewFont,
                       qreal viewRowHeight,
                       const QFont& pathFont,
                       const QColor& pathColor,
                       const QColor& pathBgColor,
                       int cursorWidth,
                       const QColor cursorActiveColor,
                       const QColor cursorInactiveColor,
                       bool loopMove,
                       bool moveOpenViewer,
                       DragAndDropBehaviorType behaviorType);
    void setAppearanceFolderViewColors(const QMap<FolderViewColorRoleType, QColor>& folderViewColors, bool folderColorTopPrio);

    void setFileSizeFormatType(FileSizeFormatType formatType);
    FileSizeFormatType getFileSizeFormatType() const;

    void setFileSizeComma(bool comma);
    bool getFileSizeComma() const;

    void setDateFormatType(DateFormatType formatType);
    DateFormatType getDateFormatType() const;

    void setDateFormatOriginalString(const QString& orgString);
    QString getDateFormatOriginalString() const;

    void setAttrFilterFlags(AttrFilterFlags filterFlags);
    AttrFilterFlags getAttrFilterFlags() const;
    void setFileFolderFilterType(FileFolderFilterType fileFolderFilterType);
    FileFolderFilterType getFileFolderFilterType() const;
    void setNameMaskFilters(const QStringList& nameMaskFilters);
    QStringList getNameMaskFilters() const;

    void setSortSettings(SectionType sectionType,
                         SectionType sectionType2nd,
                         SortDirsType dirsType,
                         bool dotFirst,
                         Qt::CaseSensitivity caseSensitivity,
                         Qt::SortOrder order);
    SectionType getSortSectionType() const;
    SectionType getSortSectionType2nd() const;
    int getSortColumn() const;
    SortDirsType getSortDirsType() const;
    bool getSortDotFirst() const;
    Qt::CaseSensitivity getSortCaseSensitivity() const;
    Qt::SortOrder getSortOrder() const;

    int setPath(const QString& dirPath, bool addHistory = true);

    QString getCurrentDirPath();
    QString getCurrentFileName();
    QFileInfo getCurrentFileInfo();

    QString getPreviousDirPath();
    QString getNextDirPath();
    const HistoryManager* getHistoryManager();

    QList<QFileInfo> getSelectedFileInfoList();

    void setCursor(const QString& fileName);

    int onGoToChildDir();
    int onGoToParentDir();
    int onPreviousDir();
    int onNextDir();
    int onChangeDir();
    void onBookmarkDir(bool marked);
    void onSelectAll();
    void onDeselectAll();

    void checkBookmark();

    void updateMarkedLabel();
    void updateFilterLabel();

    void refresh(bool clearSelected = false);

Q_SIGNALS:
    void currentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void directoryLoaded(const QString &path);
    void focusChanged(bool inFocus);
    void directoryBookmarked(const QString &path, bool marked);

protected Q_SLOTS:
    void onSelectedFile(const QString& path, QItemSelectionModel::SelectionFlag selectionFlag);
    void onCurrentChanged(const QModelIndex& newIndex, const QModelIndex& oldIndex);
    void onDirectoryLoaded(const QString &path);
    void onLayoutChanged(const QList<QPersistentModelIndex>& parents = QList<QPersistentModelIndex>(), QAbstractItemModel::LayoutChangeHint hint = QAbstractItemModel::NoLayoutChangeHint);
    void onLayoutAboutToBeChanged(const QList<QPersistentModelIndex>& parents = QList<QPersistentModelIndex>(), QAbstractItemModel::LayoutChangeHint hint = QAbstractItemModel::NoLayoutChangeHint);

private Q_SLOTS:
    void on_goToFolderToolButton_clicked();
    void on_bookmarkToolButton_toggled(bool checked);

private:
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;

    int getTotalColumnWidth(int withOutColumn = -1);

    void emitCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void emitDirectoryLoaded(const QString &path);
    void emitFocusChanged(bool inFocus);
    void emitDirectoryBookmarked(const QString &path, bool marked);

private:
    Ui::FolderForm *ui;

    const PaneType m_paneType;
    FolderModel* m_folderModel;

    HistoryManager* m_historyManager;
};

}           // namespace Farman

#endif // FOLDERFORM_H
