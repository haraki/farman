﻿#include <QMainWindow>
#include "optiondialog.h"
#include "ui_optiondialog.h"
#include "settings.h"
#include <QDebug>

namespace Farman
{

OptionDialog::OptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionDialog)
{
    ui->setupUi(this);

    QSize size = QSize(0, 0);
    if(parent != Q_NULLPTR)
    {
        size = parent->size();
    }

    SizeAtStartup sizeAtStartupType = Settings::getInstance()->getSizeAtStartupType();
    if(sizeAtStartupType == SizeAtStartup::Fixed)
    {
        ui->sizeFixedRadioButton->setChecked(true);
        ui->sizeWidthLabel->setEnabled(true);
        ui->sizeWidthLineEdit->setEnabled(true);
        ui->sizeHeightLabel->setEnabled(true);
        ui->sizeHeightLineEdit->setEnabled(true);

        size = Settings::getInstance()->getSizeAtStartup();
    }
    else
    {
        if(sizeAtStartupType == SizeAtStartup::LastTime)
        {
            ui->sizeLastTimeRadioButton->setChecked(true);
        }
        else
        {
            ui->sizeDefaultRadioButton->setChecked(true);
        }

        ui->sizeWidthLabel->setEnabled(false);
        ui->sizeWidthLineEdit->setEnabled(false);
        ui->sizeHeightLabel->setEnabled(false);
        ui->sizeHeightLineEdit->setEnabled(false);
    }

    ui->sizeWidthLineEdit->setText(QString::number(size.width()));
    ui->sizeHeightLineEdit->setText(QString::number(size.height()));

    QPoint position = QPoint(0, 0);
    if(parent != Q_NULLPTR)
    {
        position = parent->pos();
    }

    PositionAtStartup positionAtStartupType = Settings::getInstance()->getPositionAtStartupType();
    if(positionAtStartupType == PositionAtStartup::Fixed)
    {
        ui->positionFixedRadioButton->setChecked(true);
        ui->positionXLabel->setEnabled(true);
        ui->positionXLineEdit->setEnabled(true);
        ui->positionYLabel->setEnabled(true);
        ui->positionYLineEdit->setEnabled(true);

        position = Settings::getInstance()->getPositionAtStartup();
    }
    else
    {
        if(positionAtStartupType == PositionAtStartup::LastTime)
        {
            ui->positionLastTimeRadioButton->setChecked(true);
        }
        else
        {
            ui->positionDefaultRadioButton->setChecked(true);
        }

        ui->positionXLabel->setEnabled(false);
        ui->positionXLineEdit->setEnabled(false);
        ui->positionYLabel->setEnabled(false);
        ui->positionYLineEdit->setEnabled(false);
    }

    ui->positionXLineEdit->setText(QString::number(position.x()));
    ui->positionYLineEdit->setText(QString::number(position.y()));
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::on_sizeDefaultRadioButton_clicked()
{
    ui->sizeWidthLabel->setEnabled(false);
    ui->sizeWidthLineEdit->setEnabled(false);
    ui->sizeHeightLabel->setEnabled(false);
    ui->sizeHeightLineEdit->setEnabled(false);
}

void OptionDialog::on_sizeLastTimeRadioButton_clicked()
{
    ui->sizeWidthLabel->setEnabled(false);
    ui->sizeWidthLineEdit->setEnabled(false);
    ui->sizeHeightLabel->setEnabled(false);
    ui->sizeHeightLineEdit->setEnabled(false);
}

void OptionDialog::on_sizeFixedRadioButton_clicked()
{
    ui->sizeWidthLabel->setEnabled(true);
    ui->sizeWidthLineEdit->setEnabled(true);
    ui->sizeHeightLabel->setEnabled(true);
    ui->sizeHeightLineEdit->setEnabled(true);
}

void OptionDialog::on_positionDefaultRadioButton_clicked()
{
    ui->positionXLabel->setEnabled(false);
    ui->positionXLineEdit->setEnabled(false);
    ui->positionYLabel->setEnabled(false);
    ui->positionYLineEdit->setEnabled(false);
}

void OptionDialog::on_positionLastTimeRadioButton_clicked()
{
    ui->positionXLabel->setEnabled(false);
    ui->positionXLineEdit->setEnabled(false);
    ui->positionYLabel->setEnabled(false);
    ui->positionYLineEdit->setEnabled(false);
}

void OptionDialog::on_positionFixedRadioButton_clicked()
{
    ui->positionXLabel->setEnabled(true);
    ui->positionXLineEdit->setEnabled(true);
    ui->positionYLabel->setEnabled(true);
    ui->positionYLineEdit->setEnabled(true);
}

}           // namespace Farman
