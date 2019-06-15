#include <QStorageInfo>
#include "selectbookmarkdialog.h"
#include "ui_selectbookmarkdialog.h"

namespace Farman
{

SelectBookmarkDialog::SelectBookmarkDialog(QWidget *parent/* = Q_NULLPTR*/) :
    QDialog(parent),
    ui(new Ui::SelectBookmarkDialog),
    m_bookmarkInfoModel(new BookmarkInfoModel())
{
    ui->setupUi(this);

    ui->bookmarksTableView->setModel(m_bookmarkInfoModel);
    ui->bookmarksTableView->setCurrentIndex(m_bookmarkInfoModel->index(0, 0));

    ui->bookmarksTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

SelectBookmarkDialog::~SelectBookmarkDialog()
{
    delete m_bookmarkInfoModel;

    delete ui;
}

void SelectBookmarkDialog::accept()
{
    m_selectedPath = m_bookmarkInfoModel->data(ui->bookmarksTableView->currentIndex(), BookmarkInfoModel::PathRole).toString();

    QDialog::accept();
}

}           // namespace Farman
