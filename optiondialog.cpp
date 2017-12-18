#include <QMainWindow>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
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

    m_fontSettings = Settings::getInstance()->getFontSettings();
    m_colorSettings = Settings::getInstance()->getColorSettings();

    setFontAndColorFolderViewOption();
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

void OptionDialog::on_fontAndColorFolderViewChooseFontPushButton_clicked()
{
    QFont newFont = QFont();

    if(showChooseFontDialog(m_fontSettings["folderView"], newFont))
    {
        m_fontSettings["folderView"] = newFont;

        setFontAndColorFolderViewOption();
    }
}

void OptionDialog::on_fontAndColorFolderViewChooseNormalColorPushButton_clicked()
{
    chooseColor("folderView_normal", "folderView_background", ui->fontAndColorFolderViewNormalLineEdit);
}

void OptionDialog::on_fontAndColorFolderViewChooseNormalSelectedColorPushButton_clicked()
{
    chooseColor("folderView_normal_selected", "folderView_selected_background", ui->fontAndColorFolderViewNormalSelectedLineEdit);
}

void OptionDialog::on_fontAndColorFolderViewChooseFolderColorPushButton_clicked()
{
    chooseColor("folderView_folder", "folderView_background", ui->fontAndColorFolderViewFolderLineEdit);
}

void OptionDialog::on_fontAndColorFolderViewChooseFolderSelectedColorPushButton_clicked()
{
    chooseColor("folderView_folder_selected", "folderView_selected_background", ui->fontAndColorFolderViewFolderSelectedLineEdit);
}

void OptionDialog::on_fontAndColorFolderViewChooseReadOnlyColorPushButton_clicked()
{
    chooseColor("folderView_readOnly", "folderView_background", ui->fontAndColorFolderViewReadOnlyLineEdit);
}

void OptionDialog::on_fontAndColorFolderViewChooseReadOnlySelectedColorPushButton_clicked()
{
    chooseColor("folderView_readOnly_selected", "folderView_selected_background", ui->fontAndColorFolderViewReadOnlySelectedLineEdit);
}

void OptionDialog::on_fontAndColorFolderViewChooseHiddenColorPushButton_clicked()
{
    chooseColor("folderView_hidden", "folderView_background", ui->fontAndColorFolderViewHiddenLineEdit);
}

void OptionDialog::on_fontAndColorFolderViewChooseHiddenSelectedColorPushButton_clicked()
{
    chooseColor("folderView_hidden_selected", "folderView_selected_background", ui->fontAndColorFolderViewHiddenSelectedLineEdit);
}

void OptionDialog::on_fontAndColorFolderViewChooseSystemColorPushButton_clicked()
{
    chooseColor("folderView_system", "folderView_background", ui->fontAndColorFolderViewSystemLineEdit);
}

void OptionDialog::on_fontAndColorFolderViewChooseSystemSelectedColorPushButton_clicked()
{
    chooseColor("folderView_system_selected", "folderView_selected_background", ui->fontAndColorFolderViewSystemSelectedLineEdit);
}

void OptionDialog::on_fontAndColorFolderViewChooseBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["folderView_background"], newColor))
    {
        m_colorSettings["folderView_background"] = newColor;

        setFontColorSample("folderView_normal",   "folderView_background", ui->fontAndColorFolderViewNormalLineEdit);
        setFontColorSample("folderView_folder",   "folderView_background", ui->fontAndColorFolderViewFolderLineEdit);
        setFontColorSample("folderView_readOnly", "folderView_background", ui->fontAndColorFolderViewReadOnlyLineEdit);
        setFontColorSample("folderView_hidden",   "folderView_background", ui->fontAndColorFolderViewHiddenLineEdit);
        setFontColorSample("folderView_system",   "folderView_background", ui->fontAndColorFolderViewSystemLineEdit);
    }
}

void OptionDialog::on_fontAndColorFolderViewChooseSelectedBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["folderView_selected_background"], newColor))
    {
        m_colorSettings["folderView_selected_background"] = newColor;

        setFontColorSample("folderView_normal_selected",   "folderView_selected_background", ui->fontAndColorFolderViewNormalSelectedLineEdit);
        setFontColorSample("folderView_folder_selected",   "folderView_selected_background", ui->fontAndColorFolderViewFolderSelectedLineEdit);
        setFontColorSample("folderView_readOnly_selected", "folderView_selected_background", ui->fontAndColorFolderViewReadOnlySelectedLineEdit);
        setFontColorSample("folderView_hidden_selected",   "folderView_selected_background", ui->fontAndColorFolderViewHiddenSelectedLineEdit);
        setFontColorSample("folderView_system_selected",   "folderView_selected_background", ui->fontAndColorFolderViewSystemSelectedLineEdit);
    }
}

void OptionDialog::setFontAndColorFolderViewOption()
{
    QFont font = m_fontSettings["folderView"];

    ui->fontAndColorFolderViewFontLabel->setText(QString("%1, %2 pt").arg(font.family()).arg(font.pointSize()));

    ui->fontAndColorFolderViewFontLabel->setFont(font);

    ui->fontAndColorFolderViewNormalLineEdit->setFont(font);
    ui->fontAndColorFolderViewNormalSelectedLineEdit->setFont(font);
    ui->fontAndColorFolderViewFolderLineEdit->setFont(font);
    ui->fontAndColorFolderViewFolderSelectedLineEdit->setFont(font);
    ui->fontAndColorFolderViewReadOnlyLineEdit->setFont(font);
    ui->fontAndColorFolderViewReadOnlySelectedLineEdit->setFont(font);
    ui->fontAndColorFolderViewHiddenLineEdit->setFont(font);
    ui->fontAndColorFolderViewHiddenSelectedLineEdit->setFont(font);
    ui->fontAndColorFolderViewSystemLineEdit->setFont(font);
    ui->fontAndColorFolderViewSystemSelectedLineEdit->setFont(font);

    setFontColorSample("folderView_normal",            "folderView_background",          ui->fontAndColorFolderViewNormalLineEdit);
    setFontColorSample("folderView_normal_selected",   "folderView_selected_background", ui->fontAndColorFolderViewNormalSelectedLineEdit);
    setFontColorSample("folderView_folder",            "folderView_background",          ui->fontAndColorFolderViewFolderLineEdit);
    setFontColorSample("folderView_folder_selected",   "folderView_selected_background", ui->fontAndColorFolderViewFolderSelectedLineEdit);
    setFontColorSample("folderView_readOnly",          "folderView_background",          ui->fontAndColorFolderViewReadOnlyLineEdit);
    setFontColorSample("folderView_readOnly_selected", "folderView_selected_background", ui->fontAndColorFolderViewReadOnlySelectedLineEdit);
    setFontColorSample("folderView_hidden",            "folderView_background",          ui->fontAndColorFolderViewHiddenLineEdit);
    setFontColorSample("folderView_hidden_selected",   "folderView_selected_background", ui->fontAndColorFolderViewHiddenSelectedLineEdit);
    setFontColorSample("folderView_system",            "folderView_background",          ui->fontAndColorFolderViewSystemLineEdit);
    setFontColorSample("folderView_system_selected",   "folderView_selected_background", ui->fontAndColorFolderViewSystemSelectedLineEdit);
}

void OptionDialog::setFontColorSample(const QString& colorSettingType, const QString& bgSettingType, QWidget* widget)
{
    QPalette pal = widget->palette();
    pal.setColor(QPalette::Text, m_colorSettings[colorSettingType]);
    pal.setColor(QPalette::Base, m_colorSettings[bgSettingType]);
    widget->setPalette(pal);
}

void OptionDialog::chooseColor(const QString& colorSettingType, const QString& bgSettingType, QWidget* widget)
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings[colorSettingType], newColor))
    {
        m_colorSettings[colorSettingType] = newColor;

        setFontColorSample(colorSettingType, bgSettingType, widget);
    }
}

bool OptionDialog::showChooseColorDialog(const QColor& oldColor, QColor& newColor)
{
    newColor = QColorDialog::getColor(oldColor, this, tr("Choose color"));

    return newColor.isValid();
}

bool OptionDialog::showChooseFontDialog(const QFont& oldFont, QFont& newFont)
{
    bool ok = false;
    newFont = QFontDialog::getFont(&ok, oldFont, this, tr("Choose font"));

    return ok;
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

    Settings::getInstance()->setFontSettings(m_fontSettings);
    Settings::getInstance()->setColorSettings(m_colorSettings);
}

}           // namespace Farman
