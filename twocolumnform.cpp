#include "twocolumnform.h"
#include "ui_twocolumnform.h"

TwoColumnForm::TwoColumnForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TwoColumnForm)
{
    ui->setupUi(this);
}

TwoColumnForm::~TwoColumnForm()
{
    delete ui;
}
