#include <QStorageInfo>
#include "selectstoragebookmarkdialog.h"
#include "ui_selectstoragebookmarkdialog.h"

namespace Farman
{

SelectStorageBookmarkDialog::SelectStorageBookmarkDialog(QWidget *parent/* = Q_NULLPTR*/) :
    QDialog(parent),
    ui(new Ui::SelectStorageBookmarkDialog)
{
    ui->setupUi(this);

    m_sfInfoModel = new StorageBookmarkInfoModel();

    ui->storagesBookmarksTableView->setModel(m_sfInfoModel);
    ui->storagesBookmarksTableView->setCurrentIndex(m_sfInfoModel->index(0));

    ui->storagesBookmarksTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

SelectStorageBookmarkDialog::~SelectStorageBookmarkDialog()
{
    delete m_sfInfoModel;

    delete ui;
}

void SelectStorageBookmarkDialog::accept()
{
    m_selectedPath = m_sfInfoModel->data(ui->storagesBookmarksTableView->currentIndex(), StorageBookmarkInfoModel::PathRole).toString();

    QDialog::accept();
}

}           // namespace Farman
