#include "forderform.h"
#include "ui_forderform.h"

ForderForm::ForderForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ForderForm)
{
    ui->setupUi(this);
}

ForderForm::~ForderForm()
{
    delete ui;
}
