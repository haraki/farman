#include "filterdialog.h"
#include "ui_filterdialog.h"

namespace Farman
{

FilterDialog::FilterDialog(QDir::Filters filterFlags, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog),
    m_filterFlags(filterFlags)
{
    ui->setupUi(this);
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

QDir::Filters FilterDialog::getFilterFlags()
{
    return m_filterFlags;
}

}
