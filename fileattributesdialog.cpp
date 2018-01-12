#include "fileattributesdialog.h"
#include "ui_fileattributesdialog.h"

FileAttributesDialog::FileAttributesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileAttributesDialog)
{
    ui->setupUi(this);
}

FileAttributesDialog::~FileAttributesDialog()
{
    delete ui;
}
