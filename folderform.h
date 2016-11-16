#ifndef FOLDERFORM_H
#define FOLDERFORM_H

#include <QWidget>

namespace Ui {
class FolderForm;
}
class QAbstractItemModel;

class FolderForm : public QWidget
{
    Q_OBJECT

public:
    explicit FolderForm(QWidget *parent = 0);
    ~FolderForm();

    void setModel(QAbstractItemModel *model);

private:
    Ui::FolderForm *ui;
};

#endif // FOLDERFORM_H
