#include <QStorageInfo>
#include "selectstoragefavoritedialog.h"
#include "ui_selectstoragefavoritedialog.h"

namespace Farman
{

SelectStorageFavoriteDialog::SelectStorageFavoriteDialog(QWidget *parent/* = Q_NULLPTR*/) :
    QDialog(parent),
    ui(new Ui::SelectStorageFavoriteDialog)
{
    ui->setupUi(this);
}

SelectStorageFavoriteDialog::~SelectStorageFavoriteDialog()
{
    delete ui;
}

}           // namespace Farman
