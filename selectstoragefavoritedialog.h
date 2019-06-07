#ifndef SELECTSTORAGEFAVORITEDIALOG_H
#define SELECTSTORAGEFAVORITEDIALOG_H

#include <QDialog>

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

private:
    void initialize();

    Ui::SelectStorageFavoriteDialog *ui;
};

}           // namespace Farman

#endif // SELECTSTORAGEFAVORITEDIALOG_H
