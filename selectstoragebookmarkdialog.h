#ifndef SELECTSTORAGEBOOKMARKDIALOG_H
#define SELECTSTORAGEBOOKMARKDIALOG_H

#include <QDialog>
#include "storagebookmarkinfomodel.h"

namespace Ui {
class SelectStorageBookmarkDialog;
}

namespace Farman
{

class SelectStorageBookmarkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectStorageBookmarkDialog(QWidget *parent = Q_NULLPTR);
    ~SelectStorageBookmarkDialog();

    const QString& getSelectedPath() { return m_selectedPath; }

    void accept();

private:
    Ui::SelectStorageBookmarkDialog *ui;

    StorageBookmarkInfoModel *m_sfInfoModel;

    QString m_selectedPath;
};

}           // namespace Farman

#endif // SELECTSTORAGEBOOKMARKDIALOG_H
