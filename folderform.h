#ifndef FOLDERFORM_H
#define FOLDERFORM_H

#include <QWidget>
#include <QDir>
#include <QModelIndex>

namespace Ui {
class FolderForm;
}
class QString;
class QResizeEvent;
class QFileSystemWatcher;

namespace Farman
{
class FolderModel;

class FolderForm : public QWidget
{
    Q_OBJECT

public:
    explicit FolderForm(QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent = Q_NULLPTR);
    ~FolderForm();

    void updateSettings();

    void setFilterFlags(QDir::Filters filterFlags);
    void setSortFlags(QDir::SortFlags sortFlags);
    void setPath(const QString& dirPath, const QString& beforePath = QString());

    QDir::Filters getFilterFlags();
    QDir::SortFlags getSortFlags();

    QString getCurrentDirPath();
    QString getCurrentFileName();
    QFileInfo getCurrentFileInfo();

    QList<QFileInfo> getSelectedFileInfoList();

    void setCursor(const QString& fileName);

    void onGoToChildDir();
    void onGoToParentDir();
    void refresh();

Q_SIGNALS:
    void currentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void focusChanged(bool inFocus);

protected Q_SLOTS:
    void onCurrentChanged(const QModelIndex& newIndex, const QModelIndex& oldIndex);
    void onDirectoryChanged(const QString& path);

private Q_SLOTS:
    void on_folderSelectButton_clicked();

    void emitCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void emitFocusChanged(bool inFocus);

private:
    void initFont();
    void initPalette();
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;

    void onSelect();

    int getTotalColumnWidth(int withOutColumn = -1);

private:
    Ui::FolderForm *ui;

    FolderModel *m_folderModel;

    QFileSystemWatcher* m_folderWatcher;
};

}           // namespace Farman

#endif // FOLDERFORM_H
