#ifndef FOLDERFORM_H
#define FOLDERFORM_H

#include <QWidget>
#include <QDir>

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
    explicit FolderForm(QWidget *parent = 0);
    ~FolderForm();

    void setFilterFlags(QDir::Filters filterFlags);
    void setSortFlags(QDir::SortFlags sortFlags);
    void setPath(QString& path);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

protected slots:
    void columnResized(int column, int oldWidth, int newWidth);

private:
    int getTotalColumnWidth(int withOutColumn = -1);

private:
    Ui::FolderForm *ui;

    QDir::Filters m_filterFlags;
    QDir::SortFlags m_sortFlags;

    FolderModel *m_folderModel;
};

#endif // FOLDERFORM_H
