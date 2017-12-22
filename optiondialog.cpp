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
        ui->generalSizeFixedRadioButton->setChecked(true);
        ui->generalSizeWidthLabel->setEnabled(true);
        ui->generalSizeWidthLineEdit->setEnabled(true);
        ui->generalSizeHeightLabel->setEnabled(true);
        ui->generalSizeHeightLineEdit->setEnabled(true);

        QSize size = Settings::getInstance()->getSizeAtStartup();

        ui->generalSizeWidthLineEdit->setText(QString::number(size.width()));
        ui->generalSizeHeightLineEdit->setText(QString::number(size.height()));
    }
    else
    {
        if(sizeAtStartupType == SizeAtStartup::LastTime)
        {
            ui->generalSizeLastTimeRadioButton->setChecked(true);
        }
        else
        {
            ui->generalSizeDefaultRadioButton->setChecked(true);
        }

        ui->generalSizeWidthLabel->setEnabled(false);
        ui->generalSizeWidthLineEdit->setEnabled(false);
        ui->generalSizeHeightLabel->setEnabled(false);
        ui->generalSizeHeightLineEdit->setEnabled(false);

        ui->generalSizeWidthLineEdit->setText(QString::number(mainWindowSize.width()));
        ui->generalSizeHeightLineEdit->setText(QString::number(mainWindowSize.height()));
    }

    PositionAtStartup positionAtStartupType = Settings::getInstance()->getPositionAtStartupType();
    if(positionAtStartupType == PositionAtStartup::Fixed)
    {
        ui->generalPositionFixedRadioButton->setChecked(true);
        ui->generalPositionXLabel->setEnabled(true);
        ui->generalPositionXLineEdit->setEnabled(true);
        ui->generalPositionYLabel->setEnabled(true);
        ui->generalPositionYLineEdit->setEnabled(true);

        QPoint pos = Settings::getInstance()->getPositionAtStartup();

        ui->generalPositionXLineEdit->setText(QString::number(pos.x()));
        ui->generalPositionYLineEdit->setText(QString::number(pos.y()));
    }
    else
    {
        if(positionAtStartupType == PositionAtStartup::LastTime)
        {
            ui->generalPositionLastTimeRadioButton->setChecked(true);
        }
        else
        {
            ui->generalPositionDefaultRadioButton->setChecked(true);
        }

        ui->generalPositionXLabel->setEnabled(false);
        ui->generalPositionXLineEdit->setEnabled(false);
        ui->generalPositionYLabel->setEnabled(false);
        ui->generalPositionYLineEdit->setEnabled(false);

        ui->generalPositionXLineEdit->setText(QString::number(mainWindowPos.x()));
        ui->generalPositionYLineEdit->setText(QString::number(mainWindowPos.y()));
    }

    FolderAtStartup leftFolderAtStartup = Settings::getInstance()->getLeftFolderAtStartup();
    if(leftFolderAtStartup == FolderAtStartup::Fixed)
    {
        ui->generalLeftFolderFixedRadioButton->setChecked(true);
        ui->generalLeftFolderSelectButton->setEnabled(true);
        ui->generalLeftFolderPathLineEdit->setEnabled(true);

        ui->generalLeftFolderPathLineEdit->setText(Settings::getInstance()->getLeftFolderPath());
    }
    else
    {
        if(leftFolderAtStartup == FolderAtStartup::LastTime)
        {
            ui->generalLeftFolderLastTimeRadioButton->setChecked(true);
        }
        else
        {
            ui->generalLeftFolderDefaultRadioButton->setChecked(true);
        }
        ui->generalLeftFolderSelectButton->setEnabled(false);
        ui->generalLeftFolderPathLineEdit->setEnabled(false);

        ui->generalLeftFolderPathLineEdit->setText(leftDirPath);
    }

    FolderAtStartup rightFolderAtStartup = Settings::getInstance()->getRightFolderAtStartup();
    if(rightFolderAtStartup == FolderAtStartup::Fixed)
    {
        ui->generalRightFolderFixedRadioButton->setChecked(true);
        ui->generalRightFolderSelectButton->setEnabled(true);
        ui->generalRightFolderPathLineEdit->setEnabled(true);

        ui->generalRightFolderPathLineEdit->setText(Settings::getInstance()->getRightFolderPath());
    }
    else
    {
        if(rightFolderAtStartup == FolderAtStartup::LastTime)
        {
            ui->generalRightFolderLastTimeRadioButton->setChecked(true);
        }
        else
        {
            ui->generalRightFolderDefaultRadioButton->setChecked(true);
        }
        ui->generalRightFolderSelectButton->setEnabled(false);
        ui->generalRightFolderPathLineEdit->setEnabled(false);

        ui->generalRightFolderPathLineEdit->setText(rightDirPath);
    }

    ui->generalConfirmQuitCheckBox->setChecked(Settings::getInstance()->getConfirmQuit());

    m_fontSettings = Settings::getInstance()->getFontSettings();
    m_colorSettings = Settings::getInstance()->getColorSettings();

    ui->appearanceFolderViewCursorWidthSpinBox->setValue(Settings::getInstance()->getCursorWidth());

    setAppearanceFontAndColorOption();
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::on_generalSizeDefaultRadioButton_clicked()
{
    ui->generalSizeWidthLabel->setEnabled(false);
    ui->generalSizeWidthLineEdit->setEnabled(false);
    ui->generalSizeHeightLabel->setEnabled(false);
    ui->generalSizeHeightLineEdit->setEnabled(false);
}

void OptionDialog::on_generalSizeLastTimeRadioButton_clicked()
{
    ui->generalSizeWidthLabel->setEnabled(false);
    ui->generalSizeWidthLineEdit->setEnabled(false);
    ui->generalSizeHeightLabel->setEnabled(false);
    ui->generalSizeHeightLineEdit->setEnabled(false);
}

void OptionDialog::on_generalSizeFixedRadioButton_clicked()
{
    ui->generalSizeWidthLabel->setEnabled(true);
    ui->generalSizeWidthLineEdit->setEnabled(true);
    ui->generalSizeHeightLabel->setEnabled(true);
    ui->generalSizeHeightLineEdit->setEnabled(true);
}

void OptionDialog::on_generalPositionDefaultRadioButton_clicked()
{
    ui->generalPositionXLabel->setEnabled(false);
    ui->generalPositionXLineEdit->setEnabled(false);
    ui->generalPositionYLabel->setEnabled(false);
    ui->generalPositionYLineEdit->setEnabled(false);
}

void OptionDialog::on_generalPositionLastTimeRadioButton_clicked()
{
    ui->generalPositionXLabel->setEnabled(false);
    ui->generalPositionXLineEdit->setEnabled(false);
    ui->generalPositionYLabel->setEnabled(false);
    ui->generalPositionYLineEdit->setEnabled(false);
}

void OptionDialog::on_generalPositionFixedRadioButton_clicked()
{
    ui->generalPositionXLabel->setEnabled(true);
    ui->generalPositionXLineEdit->setEnabled(true);
    ui->generalPositionYLabel->setEnabled(true);
    ui->generalPositionYLineEdit->setEnabled(true);
}

void OptionDialog::on_generalLeftFolderDefaultRadioButton_clicked()
{
    ui->generalLeftFolderSelectButton->setEnabled(false);
    ui->generalLeftFolderPathLineEdit->setEnabled(false);
}

void OptionDialog::on_generalLeftFolderLastTimeRadioButton_clicked()
{
    ui->generalLeftFolderSelectButton->setEnabled(false);
    ui->generalLeftFolderPathLineEdit->setEnabled(false);
}

void OptionDialog::on_generalLeftFolderFixedRadioButton_clicked()
{
    ui->generalLeftFolderSelectButton->setEnabled(true);
    ui->generalLeftFolderPathLineEdit->setEnabled(true);
}

void OptionDialog::on_generalRightFolderDefaultRadioButton_clicked()
{
    ui->generalRightFolderSelectButton->setEnabled(false);
    ui->generalRightFolderPathLineEdit->setEnabled(false);
}

void OptionDialog::on_generalRightFolderLastTimeRadioButton_clicked()
{
    ui->generalRightFolderSelectButton->setEnabled(false);
    ui->generalRightFolderPathLineEdit->setEnabled(false);
}

void OptionDialog::on_generalRightFolderFixedRadioButton_clicked()
{
    ui->generalRightFolderSelectButton->setEnabled(true);
    ui->generalRightFolderPathLineEdit->setEnabled(true);
}

void OptionDialog::on_generalLeftFolderSelectButton_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this,
                                                        tr("Select folder."),
                                                        ui->generalLeftFolderPathLineEdit->text(),
                                                        QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly);

    if(!dirPath.isEmpty())
    {
        ui->generalLeftFolderPathLineEdit->setText(dirPath);
    }
}

void OptionDialog::on_generalRightFolderSelectButton_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this,
                                                        tr("Select folder."),
                                                        ui->generalRightFolderPathLineEdit->text(),
                                                        QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly);

    if(!dirPath.isEmpty())
    {
        ui->generalRightFolderPathLineEdit->setText(dirPath);
    }
}

void OptionDialog::on_appearanceFolderViewChooseFontPushButton_clicked()
{
    QFont newFont = QFont();

    if(showChooseFontDialog(m_fontSettings["folderView"], newFont))
    {
        m_fontSettings["folderView"] = newFont;

        setAppearanceFontAndColorOption();
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

void OptionDialog::on_appearanceFolderPathChooseFontPushButton_clicked()
{
    QFont newFont = QFont();

    if(showChooseFontDialog(m_fontSettings["folderPath"], newFont))
    {
        m_fontSettings["folderPath"] = newFont;

        setAppearanceFontAndColorOption();
    }
}

void OptionDialog::on_appearanceFolderPathChooseColorPushButton_clicked()
{
    chooseColor("folderPath_text", "folderPath_background", ui->appearanceFolderPathLineEdit);
}

void OptionDialog::on_appearanceFolderPathChooseBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["folderPath_background"], newColor))
    {
        m_colorSettings["folderPath_background"] = newColor;

        setFontColorSample("folderPath_text", "folderPath_background", ui->appearanceFolderPathLineEdit);
    }
}

void OptionDialog::on_appearanceConsoleChooseFontPushButton_clicked()
{
    QFont newFont = QFont();

    if(showChooseFontDialog(m_fontSettings["console"], newFont))
    {
        m_fontSettings["console"] = newFont;

        setAppearanceFontAndColorOption();
    }
}

void OptionDialog::on_appearanceConsoleChooseColorPushButton_clicked()
{
    chooseColor("console_text", "console_background", ui->appearanceConsoleLineEdit);
}

void OptionDialog::on_appearanceConsoleChooseBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["console_background"], newColor))
    {
        m_colorSettings["console_background"] = newColor;

        setFontColorSample("console_text", "console_background", ui->appearanceConsoleLineEdit);
    }
}

void OptionDialog::setAppearanceFontAndColorOption()
{
    QFont font;

    // Folder View

    font = m_fontSettings["folderView"];

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

    // Folder Path

    font = m_fontSettings["folderPath"];

    ui->appearanceFolderPathFontLabel->setText(QString("%1, %2 pt").arg(font.family()).arg(font.pointSize()));
    ui->appearanceFolderPathFontLabel->setFont(font);

    ui->appearanceFolderPathLineEdit->setFont(font);

    setFontColorSample("folderPath_text", "folderPath_background", ui->appearanceFolderPathLineEdit);

    // Console

    font = m_fontSettings["console"];

    ui->appearanceConsoleFontLabel->setText(QString("%1, %2 pt").arg(font.family()).arg(font.pointSize()));
    ui->appearanceConsoleFontLabel->setFont(font);

    ui->appearanceConsoleLineEdit->setFont(font);

    setFontColorSample("console_text", "console_background", ui->appearanceConsoleLineEdit);
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
    if(ui->generalSizeFixedRadioButton->isChecked())
    {
        Settings::getInstance()->setSizeAtStartupType(SizeAtStartup::Fixed);

        QSize size = QSize(ui->generalSizeWidthLineEdit->text().toInt(), ui->generalSizeHeightLineEdit->text().toInt());
        Settings::getInstance()->setSizeAtStartup(size);
    }
    else if(ui->generalSizeLastTimeRadioButton->isChecked())
    {
        Settings::getInstance()->setSizeAtStartupType(SizeAtStartup::LastTime);
    }
    else
    {
        Settings::getInstance()->setSizeAtStartupType(SizeAtStartup::Default);
    }

    if(ui->generalPositionFixedRadioButton->isChecked())
    {
        Settings::getInstance()->setPositionAtStartupType(PositionAtStartup::Fixed);

        QPoint pos = QPoint(ui->generalPositionXLineEdit->text().toInt(), ui->generalPositionYLineEdit->text().toInt());
        Settings::getInstance()->setPositionAtStartup(pos);
    }
    else if(ui->generalPositionLastTimeRadioButton->isChecked())
    {
        Settings::getInstance()->setPositionAtStartupType(PositionAtStartup::LastTime);
    }
    else
    {
        Settings::getInstance()->setPositionAtStartupType(PositionAtStartup::Default);
    }

    if(ui->generalLeftFolderFixedRadioButton->isChecked())
    {
        Settings::getInstance()->setLeftFolderAtStartup(FolderAtStartup::Fixed);

        QString dirPath = ui->generalLeftFolderPathLineEdit->text();
        Settings::getInstance()->setLeftFolderPath(dirPath);
    }
    else if(ui->generalLeftFolderLastTimeRadioButton->isChecked())
    {
        Settings::getInstance()->setLeftFolderAtStartup(FolderAtStartup::LastTime);
    }
    else
    {
        Settings::getInstance()->setLeftFolderAtStartup(FolderAtStartup::Default);
    }

    if(ui->generalRightFolderFixedRadioButton->isChecked())
    {
        Settings::getInstance()->setRightFolderAtStartup(FolderAtStartup::Fixed);

        QString dirPath = ui->generalRightFolderPathLineEdit->text();
        Settings::getInstance()->setRightFolderPath(dirPath);
    }
    else if(ui->generalRightFolderLastTimeRadioButton->isChecked())
    {
        Settings::getInstance()->setRightFolderAtStartup(FolderAtStartup::LastTime);
    }
    else
    {
        Settings::getInstance()->setRightFolderAtStartup(FolderAtStartup::Default);
    }

    Settings::getInstance()->setConfirmQuit(ui->generalConfirmQuitCheckBox->isChecked());

    Settings::getInstance()->setFontSettings(m_fontSettings);
    Settings::getInstance()->setColorSettings(m_colorSettings);

    Settings::getInstance()->setCursorWidth(ui->appearanceFolderViewCursorWidthSpinBox->value());
}

}           // namespace Farman
