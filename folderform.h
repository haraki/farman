#ifndef FOLDERFORM_H
#define FOLDERFORM_H

#include <QWidget>
#include <QDir>
#include <QModelIndex>

class FolderModel;

namespace Ui {
class FolderForm;
}
class QAbstractItemModel;
class QString;
class QResizeEvent;
class FolderModel;

class FolderForm : public QWidget
{
    Q_OBJECT

public:
    explicit FolderForm(QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent = 0);
    ~FolderForm();

    void setFilterFlags(QDir::Filters filterFlags);
    void setSortFlags(QDir::SortFlags sortFlags);
    void setPath(const QString& dirPath, const QString& beforePath = QString());

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

protected slots:
    void onColumnResized(int column, int oldWidth, int newWidth);
    void onOpen(const QModelIndex& index);

private:
    void onOpen();
    void onGoToParent();
    void onToggleCheck();
    int getTotalColumnWidth(int withOutColumn = -1);
    bool eventFilter(QObject *watched, QEvent *e) override;

private:
    Ui::FolderForm *ui;

    QDir::Filters m_filterFlags;
    QDir::SortFlags m_sortFlags;

    FolderModel *m_folderModel;
};

#endif // FOLDERFORM_H
