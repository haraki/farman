﻿#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include "optiondialog.h"
#include "ui_optiondialog.h"
#include "settings.h"

namespace Farman
{

OptionDialog::OptionDialog(const QSize& mainWindowSize,
                           const QPoint& mainWindowPos,
                           const QString& leftDirPath,
                           const QString& rightDirPath,
                           QWidget *parent/* = Q_NULLPTR */) :
    QDialog(parent),
    ui(new Ui::OptionDialog)
{
    ui->setupUi(this);

    SizeAtStartup sizeAtStartupType = Settings::getInstance()->getSizeAtStartupType();
    if(sizeAtStartupType == SizeAtStartup::Fixed)
    {
        ui->sizeFixedRadioButton->setChecked(true);
        ui->sizeWidthLabel->setEnabled(true);
        ui->sizeWidthLineEdit->setEnabled(true);
        ui->sizeHeightLabel->setEnabled(true);
        ui->sizeHeightLineEdit->setEnabled(true);

        QSize size = Settings::getInstance()->getSizeAtStartup();

        ui->sizeWidthLineEdit->setText(QString::number(size.width()));
        ui->sizeHeightLineEdit->setText(QString::number(size.height()));
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

        ui->sizeWidthLineEdit->setText(QString::number(mainWindowSize.width()));
        ui->sizeHeightLineEdit->setText(QString::number(mainWindowSize.height()));
    }

    PositionAtStartup positionAtStartupType = Settings::getInstance()->getPositionAtStartupType();
    if(positionAtStartupType == PositionAtStartup::Fixed)
    {
        ui->positionFixedRadioButton->setChecked(true);
        ui->positionXLabel->setEnabled(true);
        ui->positionXLineEdit->setEnabled(true);
        ui->positionYLabel->setEnabled(true);
        ui->positionYLineEdit->setEnabled(true);

        QPoint pos = Settings::getInstance()->getPositionAtStartup();

        ui->positionXLineEdit->setText(QString::number(pos.x()));
        ui->positionYLineEdit->setText(QString::number(pos.y()));
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

        ui->positionXLineEdit->setText(QString::number(mainWindowPos.x()));
        ui->positionYLineEdit->setText(QString::number(mainWindowPos.y()));
    }

    FolderAtStartup leftFolderAtStartup = Settings::getInstance()->getLeftFolderAtStartup();
    if(leftFolderAtStartup == FolderAtStartup::Fixed)
    {
        ui->leftFolderFixedRadioButton->setChecked(true);
        ui->leftFolderSelectButton->setEnabled(true);
        ui->leftFolderPathLineEdit->setEnabled(true);

        ui->leftFolderPathLineEdit->setText(Settings::getInstance()->getLeftFolderPath());
    }
    else
    {
        if(leftFolderAtStartup == FolderAtStartup::LastTime)
        {
            ui->leftFolderLastTimeRadioButton->setChecked(true);
        }
        else
        {
            ui->leftFolderDefaultRadioButton->setChecked(true);
        }
        ui->leftFolderSelectButton->setEnabled(false);
        ui->leftFolderPathLineEdit->setEnabled(false);

        ui->leftFolderPathLineEdit->setText(leftDirPath);
    }

    FolderAtStartup rightFolderAtStartup = Settings::getInstance()->getRightFolderAtStartup();
    if(rightFolderAtStartup == FolderAtStartup::Fixed)
    {
        ui->rightFolderFixedRadioButton->setChecked(true);
        ui->rightFolderSelectButton->setEnabled(true);
        ui->rightFolderPathLineEdit->setEnabled(true);

        ui->rightFolderPathLineEdit->setText(Settings::getInstance()->getRightFolderPath());
    }
    else
    {
        if(rightFolderAtStartup == FolderAtStartup::LastTime)
        {
            ui->rightFolderLastTimeRadioButton->setChecked(true);
        }
        else
        {
            ui->rightFolderDefaultRadioButton->setChecked(true);
        }
        ui->rightFolderSelectButton->setEnabled(false);
        ui->rightFolderPathLineEdit->setEnabled(false);

        ui->rightFolderPathLineEdit->setText(rightDirPath);
    }

    ui->confirmQuitCheckBox->setChecked(Settings::getInstance()->getConfirmQuit());
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

void OptionDialog::on_leftFolderDefaultRadioButton_clicked()
{
    ui->leftFolderSelectButton->setEnabled(false);
    ui->leftFolderPathLineEdit->setEnabled(false);
}

void OptionDialog::on_leftFolderLastTimeRadioButton_clicked()
{
    ui->leftFolderSelectButton->setEnabled(false);
    ui->leftFolderPathLineEdit->setEnabled(false);
}

void OptionDialog::on_leftFolderFixedRadioButton_clicked()
{
    ui->leftFolderSelectButton->setEnabled(true);
    ui->leftFolderPathLineEdit->setEnabled(true);
}

void OptionDialog::on_rightFolderDefaultRadioButton_clicked()
{
    ui->rightFolderSelectButton->setEnabled(false);
    ui->rightFolderPathLineEdit->setEnabled(false);
}

void OptionDialog::on_rightFolderLastTimeRadioButton_clicked()
{
    ui->rightFolderSelectButton->setEnabled(false);
    ui->rightFolderPathLineEdit->setEnabled(false);
}

void OptionDialog::on_rightFolderFixedRadioButton_clicked()
{
    ui->rightFolderSelectButton->setEnabled(true);
    ui->rightFolderPathLineEdit->setEnabled(true);
}

void OptionDialog::on_leftFolderSelectButton_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this,
                                                        tr("Select folder."),
                                                        ui->leftFolderPathLineEdit->text(),
                                                        QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly);

    if(!dirPath.isEmpty())
    {
        ui->leftFolderPathLineEdit->setText(dirPath);
    }
}

void OptionDialog::on_rightFolderSelectButton_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this,
                                                        tr("Select folder."),
                                                        ui->rightFolderPathLineEdit->text(),
                                                        QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly);

    if(!dirPath.isEmpty())
    {
        ui->rightFolderPathLineEdit->setText(dirPath);
    }
}

void OptionDialog::on_buttonBox_accepted()
{
    if(ui->sizeFixedRadioButton->isChecked())
    {
        Settings::getInstance()->setSizeAtStartupType(SizeAtStartup::Fixed);

        QSize size = QSize(ui->sizeWidthLineEdit->text().toInt(), ui->sizeHeightLineEdit->text().toInt());
        Settings::getInstance()->setSizeAtStartup(size);
    }
    else if(ui->sizeLastTimeRadioButton->isChecked())
    {
        Settings::getInstance()->setSizeAtStartupType(SizeAtStartup::LastTime);
    }
    else
    {
        Settings::getInstance()->setSizeAtStartupType(SizeAtStartup::Default);
    }

    if(ui->positionFixedRadioButton->isChecked())
    {
        Settings::getInstance()->setPositionAtStartupType(PositionAtStartup::Fixed);

        QPoint pos = QPoint(ui->positionXLineEdit->text().toInt(), ui->positionYLineEdit->text().toInt());
        Settings::getInstance()->setPositionAtStartup(pos);
    }
    else if(ui->positionLastTimeRadioButton->isChecked())
    {
        Settings::getInstance()->setPositionAtStartupType(PositionAtStartup::LastTime);
    }
    else
    {
        Settings::getInstance()->setPositionAtStartupType(PositionAtStartup::Default);
    }

    if(ui->leftFolderFixedRadioButton->isChecked())
    {
        Settings::getInstance()->setLeftFolderAtStartup(FolderAtStartup::Fixed);

        QString dirPath = ui->leftFolderPathLineEdit->text();
        Settings::getInstance()->setLeftFolderPath(dirPath);
    }
    else if(ui->leftFolderLastTimeRadioButton->isChecked())
    {
        Settings::getInstance()->setLeftFolderAtStartup(FolderAtStartup::LastTime);
    }
    else
    {
        Settings::getInstance()->setLeftFolderAtStartup(FolderAtStartup::Default);
    }

    if(ui->rightFolderFixedRadioButton->isChecked())
    {
        Settings::getInstance()->setRightFolderAtStartup(FolderAtStartup::Fixed);

        QString dirPath = ui->rightFolderPathLineEdit->text();
        Settings::getInstance()->setRightFolderPath(dirPath);
    }
    else if(ui->rightFolderLastTimeRadioButton->isChecked())
    {
        Settings::getInstance()->setRightFolderAtStartup(FolderAtStartup::LastTime);
    }
    else
    {
        Settings::getInstance()->setRightFolderAtStartup(FolderAtStartup::Default);
    }

    Settings::getInstance()->setConfirmQuit(ui->confirmQuitCheckBox->isChecked());
}

}           // namespace Farman