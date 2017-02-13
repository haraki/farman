#include "workingdialog.h"
#include "ui_workingdialog.h"

namespace Farman
{

WorkingDialog::WorkingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkingDialog)
{
    ui->setupUi(this);
}

WorkingDialog::~WorkingDialog()
{
    delete ui;
}

}           // namespace Farman
