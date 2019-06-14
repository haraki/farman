#include "bookmarkmanagerdialog.h"
#include "ui_bookmarkmanagerdialog.h"

namespace Farman
{

BookmarkManagerDialog::BookmarkManagerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookmarkManagerDialog)
{
    ui->setupUi(this);
}

BookmarkManagerDialog::~BookmarkManagerDialog()
{
    delete ui;
}

}           // namespace Farman
