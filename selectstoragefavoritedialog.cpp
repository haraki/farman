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

    m_sfInfoModel = new StorageFavoriteInfoModel();

    ui->storagesFavoritesListView->setModel(m_sfInfoModel);
    ui->storagesFavoritesListView->setCurrentIndex(m_sfInfoModel->index(0));
}

SelectStorageFavoriteDialog::~SelectStorageFavoriteDialog()
{
    delete m_sfInfoModel;

    delete ui;
}

void SelectStorageFavoriteDialog::accept()
{
    m_selectedPath = m_sfInfoModel->data(ui->storagesFavoritesListView->currentIndex(), StorageFavoriteInfoModel::PathRole).toString();

    QDialog::accept();
}

}           // namespace Farman
