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

    ui->appearanceFolderViewCursorWidthSpinBox->setValue(Settings::getInstance()->getCursorWidth());

    setAppearanceFolderViewOption();
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

void OptionDialog::on_appearanceFolderViewChooseFontPushButton_clicked()
{
    QFont newFont = QFont();

    if(showChooseFontDialog(m_fontSettings["folderView"], newFont))
    {
        m_fontSettings["folderView"] = newFont;

        setAppearanceFolderViewOption();
    }
}

void OptionDialog::on_appearanceFolderViewChooseNormalColorPushButton_clicked()
{
    chooseColor("folderView_normal", "folderView_background", ui->appearanceFolderViewNormalLineEdit);
}

void OptionDialog::on_appearanceFolderViewChooseNormalSelectedColorPushButton_clicked()
{
    chooseColor("folderView_normal_selected", "folderView_selected_background", ui->appearanceFolderViewNormalSelectedLineEdit);
}

void OptionDialog::on_appearanceFolderViewChooseFolderColorPushButton_clicked()
{
    chooseColor("folderView_folder", "folderView_background", ui->appearanceFolderViewFolderLineEdit);
}

void OptionDialog::on_appearanceFolderViewChooseFolderSelectedColorPushButton_clicked()
{
    chooseColor("folderView_folder_selected", "folderView_selected_background", ui->appearanceFolderViewFolderSelectedLineEdit);
}

void OptionDialog::on_appearanceFolderViewChooseReadOnlyColorPushButton_clicked()
{
    chooseColor("folderView_readOnly", "folderView_background", ui->appearanceFolderViewReadOnlyLineEdit);
}

void OptionDialog::on_appearanceFolderViewChooseReadOnlySelectedColorPushButton_clicked()
{
    chooseColor("folderView_readOnly_selected", "folderView_selected_background", ui->appearanceFolderViewReadOnlySelectedLineEdit);
}

void OptionDialog::on_appearanceFolderViewChooseHiddenColorPushButton_clicked()
{
    chooseColor("folderView_hidden", "folderView_background", ui->appearanceFolderViewHiddenLineEdit);
}

void OptionDialog::on_appearanceFolderViewChooseHiddenSelectedColorPushButton_clicked()
{
    chooseColor("folderView_hidden_selected", "folderView_selected_background", ui->appearanceFolderViewHiddenSelectedLineEdit);
}

void OptionDialog::on_appearanceFolderViewChooseSystemColorPushButton_clicked()
{
    chooseColor("folderView_system", "folderView_background", ui->appearanceFolderViewSystemLineEdit);
}

void OptionDialog::on_appearanceFolderViewChooseSystemSelectedColorPushButton_clicked()
{
    chooseColor("folderView_system_selected", "folderView_selected_background", ui->appearanceFolderViewSystemSelectedLineEdit);
}

void OptionDialog::on_appearanceFolderViewChooseBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["folderView_background"], newColor))
    {
        m_colorSettings["folderView_background"] = newColor;

        setFontColorSample("folderView_normal",   "folderView_background", ui->appearanceFolderViewNormalLineEdit);
        setFontColorSample("folderView_folder",   "folderView_background", ui->appearanceFolderViewFolderLineEdit);
        setFontColorSample("folderView_readOnly", "folderView_background", ui->appearanceFolderViewReadOnlyLineEdit);
        setFontColorSample("folderView_hidden",   "folderView_background", ui->appearanceFolderViewHiddenLineEdit);
        setFontColorSample("folderView_system",   "folderView_background", ui->appearanceFolderViewSystemLineEdit);
    }
}

void OptionDialog::on_appearanceFolderViewChooseSelectedBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["folderView_selected_background"], newColor))
    {
        m_colorSettings["folderView_selected_background"] = newColor;

        setFontColorSample("folderView_normal_selected",   "folderView_selected_background", ui->appearanceFolderViewNormalSelectedLineEdit);
        setFontColorSample("folderView_folder_selected",   "folderView_selected_background", ui->appearanceFolderViewFolderSelectedLineEdit);
        setFontColorSample("folderView_readOnly_selected", "folderView_selected_background", ui->appearanceFolderViewReadOnlySelectedLineEdit);
        setFontColorSample("folderView_hidden_selected",   "folderView_selected_background", ui->appearanceFolderViewHiddenSelectedLineEdit);
        setFontColorSample("folderView_system_selected",   "folderView_selected_background", ui->appearanceFolderViewSystemSelectedLineEdit);
    }
}

void OptionDialog::on_appearanceFolderViewChooseCursorColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["folderView_cursor"], newColor))
    {
        m_colorSettings["folderView_cursor"] = newColor;

        setFontColorSample("folderView_cursor", "folderView_cursor", ui->appearanceFolderViewCursorLineEdit);
    }
}

void OptionDialog::on_appearanceFolderViewChooseCursorInactiveColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["folderView_cursor_inactive"], newColor))
    {
        m_colorSettings["folderView_cursor_inactive"] = newColor;

        setFontColorSample("folderView_cursor_inactive", "folderView_cursor_inactive", ui->appearanceFolderViewCursorInactiveLineEdit);
    }
}

void OptionDialog::setAppearanceFolderViewOption()
{
    QFont font = m_fontSettings["folderView"];

    ui->appearanceFolderViewFontLabel->setText(QString("%1, %2 pt").arg(font.family()).arg(font.pointSize()));

    ui->appearanceFolderViewFontLabel->setFont(font);

    ui->appearanceFolderViewNormalLineEdit->setFont(font);
    ui->appearanceFolderViewNormalSelectedLineEdit->setFont(font);
    ui->appearanceFolderViewFolderLineEdit->setFont(font);
    ui->appearanceFolderViewFolderSelectedLineEdit->setFont(font);
    ui->appearanceFolderViewReadOnlyLineEdit->setFont(font);
    ui->appearanceFolderViewReadOnlySelectedLineEdit->setFont(font);
    ui->appearanceFolderViewHiddenLineEdit->setFont(font);
    ui->appearanceFolderViewHiddenSelectedLineEdit->setFont(font);
    ui->appearanceFolderViewSystemLineEdit->setFont(font);
    ui->appearanceFolderViewSystemSelectedLineEdit->setFont(font);

    setFontColorSample("folderView_normal",            "folderView_background",          ui->appearanceFolderViewNormalLineEdit);
    setFontColorSample("folderView_normal_selected",   "folderView_selected_background", ui->appearanceFolderViewNormalSelectedLineEdit);
    setFontColorSample("folderView_folder",            "folderView_background",          ui->appearanceFolderViewFolderLineEdit);
    setFontColorSample("folderView_folder_selected",   "folderView_selected_background", ui->appearanceFolderViewFolderSelectedLineEdit);
    setFontColorSample("folderView_readOnly",          "folderView_background",          ui->appearanceFolderViewReadOnlyLineEdit);
    setFontColorSample("folderView_readOnly_selected", "folderView_selected_background", ui->appearanceFolderViewReadOnlySelectedLineEdit);
    setFontColorSample("folderView_hidden",            "folderView_background",          ui->appearanceFolderViewHiddenLineEdit);
    setFontColorSample("folderView_hidden_selected",   "folderView_selected_background", ui->appearanceFolderViewHiddenSelectedLineEdit);
    setFontColorSample("folderView_system",            "folderView_background",          ui->appearanceFolderViewSystemLineEdit);
    setFontColorSample("folderView_system_selected",   "folderView_selected_background", ui->appearanceFolderViewSystemSelectedLineEdit);
    setFontColorSample("folderView_cursor",            "folderView_cursor",              ui->appearanceFolderViewCursorLineEdit);
    setFontColorSample("folderView_cursor_inactive",   "folderView_cursor_inactive",     ui->appearanceFolderViewCursorInactiveLineEdit);
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

    Settings::getInstance()->setCursorWidth(ui->appearanceFolderViewCursorWidthSpinBox->value());
}

}           // namespace Farman
