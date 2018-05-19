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
    explicit FolderForm(QDir::Filters filterFlags,
                        SectionType sortSectionType,
                        SortDirsType sortDirsType,
                        bool sortDotFirst,
                        Qt::CaseSensitivity sortCaseSensitivity,
                        Qt::SortOrder sortOrder,
                        QWidget *parent = Q_NULLPTR);
    ~FolderForm();

    void updateSettings();

    void setFilterFlags(QDir::Filters filterFlags);
    QDir::Filters getFilterFlags() const;

    void setSortSettings(SectionType sectionType, SortDirsType dirsType, bool dotFirst, Qt::CaseSensitivity caseSensitivity, Qt::SortOrder order);
    SectionType getSortSectionType() const;
    int getSortColumn() const;
    SortDirsType getSortDirsType() const;
    bool getSortDotFirst() const;
    Qt::CaseSensitivity getSortCaseSensitivity() const;
    Qt::SortOrder getSortOrder() const;

    int setPath(const QString& dirPath, const QString& beforePath = QString());

    QString getCurrentDirPath();
    QString getCurrentFileName();
    QFileInfo getCurrentFileInfo();

    QList<QFileInfo> getSelectedFileInfoList();

    void setCursor(const QString& fileName);

    int onGoToChildDir();
    int onGoToParentDir();
    void refresh();

Q_SIGNALS:
    void currentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void focusChanged(bool inFocus);

protected Q_SLOTS:
    void onCurrentChanged(const QModelIndex& newIndex, const QModelIndex& oldIndex);
    void onDirectoryLoaded(const QString &path);

private Q_SLOTS:
    void on_folderSelectButton_clicked();

    void emitCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void emitFocusChanged(bool inFocus);

private:
    void initFont();
    void initPalette();
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;

    int getTotalColumnWidth(int withOutColumn = -1);

private:
    Ui::FolderForm *ui;

    FolderModel *m_folderModel;

    QString m_beforePath;
    bool m_isSettingPath;
};

}           // namespace Farman

#endif // FOLDERFORM_H
