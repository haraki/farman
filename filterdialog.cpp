#include "filterdialog.h"
#include "ui_filterdialog.h"

namespace Farman
{

FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

}
