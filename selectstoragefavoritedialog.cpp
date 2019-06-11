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

    ui->storagesFavoritesTableView->setModel(m_sfInfoModel);
    ui->storagesFavoritesTableView->setCurrentIndex(m_sfInfoModel->index(0));

    ui->storagesFavoritesTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

SelectStorageFavoriteDialog::~SelectStorageFavoriteDialog()
{
    delete m_sfInfoModel;

    delete ui;
}

void SelectStorageFavoriteDialog::accept()
{
    m_selectedPath = m_sfInfoModel->data(ui->storagesFavoritesTableView->currentIndex(), StorageFavoriteInfoModel::PathRole).toString();

    QDialog::accept();
}

}           // namespace Farman
