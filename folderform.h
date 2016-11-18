#ifndef FOLDERFORM_H
#define FOLDERFORM_H

#include <QWidget>

class FolderModel;

namespace Ui {
class FolderForm;
}
class QAbstractItemModel;
class QString;
class FolderModel;

class FolderForm : public QWidget
{
    Q_OBJECT

public:
    explicit FolderForm(QWidget *parent = 0);
    ~FolderForm();

    void setModel(FolderModel *model);
    void setPath(QString& path);

private:
    Ui::FolderForm *ui;
    FolderModel *m_folderModel;
};

#endif // FOLDERFORM_H
