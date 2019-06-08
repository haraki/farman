#ifndef SELECTSTORAGEFAVORITEDIALOG_H
#define SELECTSTORAGEFAVORITEDIALOG_H

#include <QDialog>
#include "storagefavoriteinfomodel.h"

namespace Ui {
class SelectStorageFavoriteDialog;
}

namespace Farman
{

class SelectStorageFavoriteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectStorageFavoriteDialog(QWidget *parent = Q_NULLPTR);
    ~SelectStorageFavoriteDialog();

    const QString& getSelectedPath() { return m_selectedPath; }

    void accept();

private:
    void initialize();

    Ui::SelectStorageFavoriteDialog *ui;

    StorageFavoriteInfoModel *m_sfInfoModel;

    QString m_selectedPath;
};

}           // namespace Farman

#endif // SELECTSTORAGEFAVORITEDIALOG_H
