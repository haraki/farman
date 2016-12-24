#ifndef FOLDERFORM_H
#define FOLDERFORM_H

#include <QWidget>
#include <QDir>
#include <QModelIndex>

class FolderModel;

namespace Ui {
class FolderForm;
}
class QString;
class QResizeEvent;
class FolderModel;
class QItemSelectionModel;

class FolderForm : public QWidget
{
    Q_OBJECT

public:
    explicit FolderForm(QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent = Q_NULLPTR);
    ~FolderForm();

    void setFilterFlags(QDir::Filters filterFlags);
    void setSortFlags(QDir::SortFlags sortFlags);
    void setPath(const QString& dirPath, const QString& beforePath = QString());

    void onGoToParent();
protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

protected slots:
    void onColumnResized(int column, int oldWidth, int newWidth);
    void onOpen(const QModelIndex& index);
    void onCurrentChanged(const QModelIndex& newIndex, const QModelIndex& oldIndex);

private slots:
    void on_folderSelectButton_clicked();

private:
    bool eventFilter(QObject *watched, QEvent *e) Q_DECL_OVERRIDE;

    void onOpen();
    void onToggleCheck();

    int getTotalColumnWidth(int withOutColumn = -1);

private:
    Ui::FolderForm *ui;

    QDir::Filters m_filterFlags;
    QDir::SortFlags m_sortFlags;

    FolderModel *m_folderModel;
    QItemSelectionModel* m_folderSelectionModel;
};

#endif // FOLDERFORM_H
