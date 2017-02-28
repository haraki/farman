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

namespace Farman
{
class FolderModel;

class FolderForm : public QWidget
{
    Q_OBJECT

public:
    explicit FolderForm(QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent = Q_NULLPTR);
    ~FolderForm();

    void setFilterFlags(QDir::Filters filterFlags);
    void setSortFlags(QDir::SortFlags sortFlags);
    void setPath(const QString& dirPath, const QString& beforePath = QString());

    QDir::Filters getFilterFlags();
    QDir::SortFlags getSortFlags();

    QString getCurrentDirPath();
    QString getCurrentFileName();
    QFileInfo getCurrentFileInfo();

    QList<QFileInfo> getSelectedFileInfoList();

    void onGoToParent();

Q_SIGNALS:
    void currentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void focusChanged(bool inFocus);

protected slots:
    void onOpen(const QModelIndex& index);
    void onCurrentChanged(const QModelIndex& newIndex, const QModelIndex& oldIndex);

private slots:
    void on_folderSelectButton_clicked();

    void emitCurrentChanged(const QFileInfo& newFileInfo, const QFileInfo& oldFileInfo);
    void emitFocusChanged(bool inFocus);

private:
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;

    void onOpen();
    void onSelect();

    int getTotalColumnWidth(int withOutColumn = -1);

private:
    Ui::FolderForm *ui;

    FolderModel *m_folderModel;
};

}           // namespace Farman

#endif // FOLDERFORM_H
