#include <QMainWindow>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QDebug>
#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "settings.h"

namespace Farman
{

PreferencesDialog::PreferencesDialog(const QSize& mainWindowSize,
                                     const QPoint& mainWindowPos,
                                     const QString& leftDirPath,
                                     const QString& rightDirPath,
                                     QWidget *parent/* = Q_NULLPTR */) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
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

    DragAndDropBehaviorType behaviorType = Settings::getInstance()->getDragAndDropBehaviorType();
    if(behaviorType == DragAndDropBehaviorType::Copy)
    {
        ui->dragAndDropBehaviorCopyRadioButton->setChecked(true);
    }
    else if(behaviorType == DragAndDropBehaviorType::Move)
    {
        ui->dragAndDropBehaviorMoveRadioButton->setChecked(true);
    }
    else
    {
        ui->dragAndDropBehaviorSelectRadioButton->setChecked(true);
    }

    ui->autoDialogCloseCopyCheckBox->setChecked(Settings::getInstance()->getAutoDialogCloseCopy());
    ui->autoDialogCloseMoveCheckBox->setChecked(Settings::getInstance()->getAutoDialogCloseMove());
    ui->autoDialogCloseRemoveCheckBox->setChecked(Settings::getInstance()->getAutoDialogCloseRemove());

    ui->allowCursorAroundCheckBox->setChecked(Settings::getInstance()->getAllowCursorAround());

    ui->confirmQuitCheckBox->setChecked(Settings::getInstance()->getConfirmQuit());

    m_fontSettings = Settings::getInstance()->getFontSettings();
    m_colorSettings = Settings::getInstance()->getColorSettings();

    ui->folderViewCursorWidthSpinBox->setValue(Settings::getInstance()->getCursorWidth());

    setAppearanceFontAndColorOption();

    ui->imageViewerFitInViewCheckBox->setChecked(Settings::getInstance()->getImageViewerFitInView());

    ui->imageViewerBGTypeComboBox->addItems({tr("Solid"), tr("Checkered")});
    ui->imageViewerBGTypeComboBox->setCurrentIndex((Settings::getInstance()->getImageViewerBGType() == ImageViewerBGType::Solid) ? 0 : 1);

    m_textViewerEncodeList = Settings::getInstance()->getTextViewerEncodeList();
    ui->textViewerEncodeComboBox->addItems(m_textViewerEncodeList);

    ui->textViewerShowLineNumberCheckBox->setChecked(Settings::getInstance()->getTextViewerShowLineNumber());
    ui->textViewerWordWrapCheckBox->setChecked(Settings::getInstance()->getTextViewerWordWrap());

    setViewerFontAndColorOption();
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::on_sizeDefaultRadioButton_clicked()
{
    ui->sizeWidthLabel->setEnabled(false);
    ui->sizeWidthLineEdit->setEnabled(false);
    ui->sizeHeightLabel->setEnabled(false);
    ui->sizeHeightLineEdit->setEnabled(false);
}

void PreferencesDialog::on_sizeLastTimeRadioButton_clicked()
{
    ui->sizeWidthLabel->setEnabled(false);
    ui->sizeWidthLineEdit->setEnabled(false);
    ui->sizeHeightLabel->setEnabled(false);
    ui->sizeHeightLineEdit->setEnabled(false);
}

void PreferencesDialog::on_sizeFixedRadioButton_clicked()
{
    ui->sizeWidthLabel->setEnabled(true);
    ui->sizeWidthLineEdit->setEnabled(true);
    ui->sizeHeightLabel->setEnabled(true);
    ui->sizeHeightLineEdit->setEnabled(true);
}

void PreferencesDialog::on_positionDefaultRadioButton_clicked()
{
    ui->positionXLabel->setEnabled(false);
    ui->positionXLineEdit->setEnabled(false);
    ui->positionYLabel->setEnabled(false);
    ui->positionYLineEdit->setEnabled(false);
}

void PreferencesDialog::on_positionLastTimeRadioButton_clicked()
{
    ui->positionXLabel->setEnabled(false);
    ui->positionXLineEdit->setEnabled(false);
    ui->positionYLabel->setEnabled(false);
    ui->positionYLineEdit->setEnabled(false);
}

void PreferencesDialog::on_positionFixedRadioButton_clicked()
{
    ui->positionXLabel->setEnabled(true);
    ui->positionXLineEdit->setEnabled(true);
    ui->positionYLabel->setEnabled(true);
    ui->positionYLineEdit->setEnabled(true);
}

void PreferencesDialog::on_leftFolderDefaultRadioButton_clicked()
{
    ui->leftFolderSelectButton->setEnabled(false);
    ui->leftFolderPathLineEdit->setEnabled(false);
}

void PreferencesDialog::on_leftFolderLastTimeRadioButton_clicked()
{
    ui->leftFolderSelectButton->setEnabled(false);
    ui->leftFolderPathLineEdit->setEnabled(false);
}

void PreferencesDialog::on_leftFolderFixedRadioButton_clicked()
{
    ui->leftFolderSelectButton->setEnabled(true);
    ui->leftFolderPathLineEdit->setEnabled(true);
}

void PreferencesDialog::on_rightFolderDefaultRadioButton_clicked()
{
    ui->rightFolderSelectButton->setEnabled(false);
    ui->rightFolderPathLineEdit->setEnabled(false);
}

void PreferencesDialog::on_rightFolderLastTimeRadioButton_clicked()
{
    ui->rightFolderSelectButton->setEnabled(false);
    ui->rightFolderPathLineEdit->setEnabled(false);
}

void PreferencesDialog::on_rightFolderFixedRadioButton_clicked()
{
    ui->rightFolderSelectButton->setEnabled(true);
    ui->rightFolderPathLineEdit->setEnabled(true);
}

void PreferencesDialog::on_leftFolderSelectButton_clicked()
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

void PreferencesDialog::on_rightFolderSelectButton_clicked()
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

void PreferencesDialog::on_folderViewFontFontPushButton_clicked()
{
    QFont newFont = QFont();

    if(showChooseFontDialog(m_fontSettings["folderView"], newFont))
    {
        m_fontSettings["folderView"] = newFont;

        setAppearanceFontAndColorOption();
    }
}

void PreferencesDialog::on_folderViewFontNormalColorPushButton_clicked()
{
    chooseColor("folderView_normal", "folderView_background", ui->folderViewFontNormalLineEdit);
}

void PreferencesDialog::on_folderViewFontNormalSelectedColorPushButton_clicked()
{
    chooseColor("folderView_normal_selected", "folderView_selected_background", ui->folderViewFontNormalSelectedLineEdit);
}

void PreferencesDialog::on_folderViewFontFolderColorPushButton_clicked()
{
    chooseColor("folderView_folder", "folderView_background", ui->folderViewFontFolderLineEdit);
}

void PreferencesDialog::on_folderViewFontFolderSelectedColorPushButton_clicked()
{
    chooseColor("folderView_folder_selected", "folderView_selected_background", ui->folderViewFontFolderSelectedLineEdit);
}

void PreferencesDialog::on_folderViewFontReadOnlyColorPushButton_clicked()
{
    chooseColor("folderView_readOnly", "folderView_background", ui->folderViewFontReadOnlyLineEdit);
}

void PreferencesDialog::on_folderViewFontReadOnlySelectedColorPushButton_clicked()
{
    chooseColor("folderView_readOnly_selected", "folderView_selected_background", ui->folderViewFontReadOnlySelectedLineEdit);
}

void PreferencesDialog::on_folderViewFontHiddenColorPushButton_clicked()
{
    chooseColor("folderView_hidden", "folderView_background", ui->folderViewFontHiddenLineEdit);
}

void PreferencesDialog::on_folderViewFontHiddenSelectedColorPushButton_clicked()
{
    chooseColor("folderView_hidden_selected", "folderView_selected_background", ui->folderViewFontHiddenSelectedLineEdit);
}

void PreferencesDialog::on_folderViewFontSystemColorPushButton_clicked()
{
    chooseColor("folderView_system", "folderView_background", ui->folderViewFontSystemLineEdit);
}

void PreferencesDialog::on_folderViewFontSystemSelectedColorPushButton_clicked()
{
    chooseColor("folderView_system_selected", "folderView_selected_background", ui->folderViewFontSystemSelectedLineEdit);
}

void PreferencesDialog::on_folderViewFontBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["folderView_background"], newColor))
    {
        m_colorSettings["folderView_background"] = newColor;

        setFontColorSample("folderView_normal",   "folderView_background", ui->folderViewFontNormalLineEdit);
        setFontColorSample("folderView_folder",   "folderView_background", ui->folderViewFontFolderLineEdit);
        setFontColorSample("folderView_readOnly", "folderView_background", ui->folderViewFontReadOnlyLineEdit);
        setFontColorSample("folderView_hidden",   "folderView_background", ui->folderViewFontHiddenLineEdit);
        setFontColorSample("folderView_system",   "folderView_background", ui->folderViewFontSystemLineEdit);
    }
}

void PreferencesDialog::on_folderViewFontSelectedBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["folderView_selected_background"], newColor))
    {
        m_colorSettings["folderView_selected_background"] = newColor;

        setFontColorSample("folderView_normal_selected",   "folderView_selected_background", ui->folderViewFontNormalSelectedLineEdit);
        setFontColorSample("folderView_folder_selected",   "folderView_selected_background", ui->folderViewFontFolderSelectedLineEdit);
        setFontColorSample("folderView_readOnly_selected", "folderView_selected_background", ui->folderViewFontReadOnlySelectedLineEdit);
        setFontColorSample("folderView_hidden_selected",   "folderView_selected_background", ui->folderViewFontHiddenSelectedLineEdit);
        setFontColorSample("folderView_system_selected",   "folderView_selected_background", ui->folderViewFontSystemSelectedLineEdit);
    }
}

void PreferencesDialog::on_folderViewCursorColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["folderView_cursor"], newColor))
    {
        m_colorSettings["folderView_cursor"] = newColor;

        setFontColorSample("folderView_cursor", "folderView_cursor", ui->folderViewCursorLineEdit);
    }
}

void PreferencesDialog::on_folderViewCursorInactiveColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["folderView_cursor_inactive"], newColor))
    {
        m_colorSettings["folderView_cursor_inactive"] = newColor;

        setFontColorSample("folderView_cursor_inactive", "folderView_cursor_inactive", ui->folderViewCursorInactiveLineEdit);
    }
}

void PreferencesDialog::on_folderPathFontPushButton_clicked()
{
    QFont newFont = QFont();

    if(showChooseFontDialog(m_fontSettings["folderPath"], newFont))
    {
        m_fontSettings["folderPath"] = newFont;

        setAppearanceFontAndColorOption();
    }
}

void PreferencesDialog::on_folderPathColorPushButton_clicked()
{
    chooseColor("folderPath_text", "folderPath_background", ui->folderPathLineEdit);
}

void PreferencesDialog::on_folderPathBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["folderPath_background"], newColor))
    {
        m_colorSettings["folderPath_background"] = newColor;

        setFontColorSample("folderPath_text", "folderPath_background", ui->folderPathLineEdit);
    }
}

void PreferencesDialog::on_consoleFontPushButton_clicked()
{
    QFont newFont = QFont();

    if(showChooseFontDialog(m_fontSettings["console"], newFont))
    {
        m_fontSettings["console"] = newFont;

        setAppearanceFontAndColorOption();
    }
}

void PreferencesDialog::on_consoleColorPushButton_clicked()
{
    chooseColor("console_text", "console_background", ui->consoleLineEdit);
}

void PreferencesDialog::on_consoleBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["console_background"], newColor))
    {
        m_colorSettings["console_background"] = newColor;

        setFontColorSample("console_text", "console_background", ui->consoleLineEdit);
    }
}

void PreferencesDialog::on_textViewerFontPushButton_clicked()
{
    QFont newFont = QFont();

    if(showChooseFontDialog(m_fontSettings["textViewer"], newFont))
    {
        m_fontSettings["textViewer"] = newFont;

        setViewerFontAndColorOption();
    }
}

void PreferencesDialog::on_textViewerFontColorPushButton_clicked()
{
    chooseColor("textViewer_text", "textViewer_background", ui->textViewerSampleLineEdit);
}

void PreferencesDialog::on_textViewerBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["textViewer_background"], newColor))
    {
        m_colorSettings["textViewer_background"] = newColor;

        setFontColorSample("textViewer_text", "textViewer_background", ui->textViewerSampleLineEdit);
    }
}

void PreferencesDialog::on_textViewerSelectedFontColorPushButton_clicked()
{
    chooseColor("textViewer_selected_text", "textViewer_selected_background", ui->textViewerSelectedSampleLineEdit);
}

void PreferencesDialog::on_textViewerSelectedBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["textViewer_selected_background"], newColor))
    {
        m_colorSettings["textViewer_selected_background"] = newColor;

        setFontColorSample("textViewer_selected_text", "textViewer_selected_background", ui->textViewerSelectedSampleLineEdit);
    }
}

void PreferencesDialog::on_textViewerLineNumberFontColorPushButton_clicked()
{
    chooseColor("textViewer_lineNumber_text", "textViewer_lineNumber_background", ui->textViewerLineNumberSampleLineEdit);
}

void PreferencesDialog::on_textViewerLineNumberBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["textViewer_lineNumber_background"], newColor))
    {
        m_colorSettings["textViewer_lineNumber_background"] = newColor;

        setFontColorSample("textViewer_lineNumber_text", "textViewer_lineNumber_background", ui->textViewerLineNumberSampleLineEdit);
    }
}

void PreferencesDialog::on_textViewerEncodeComboBox_activated(int index)
{
    qDebug() << "PreferencesDialog::on_textViewerEncodeComboBox_activated(" << index << ")";

    // 選択されたエンコードをリストの先頭に移動する
    QString encode = m_textViewerEncodeList[index];
    m_textViewerEncodeList.removeAt(index);
    m_textViewerEncodeList.insert(0, encode);

    ui->textViewerEncodeComboBox->blockSignals(true);
    ui->textViewerEncodeComboBox->clear();
    ui->textViewerEncodeComboBox->addItems(m_textViewerEncodeList);
    ui->textViewerEncodeComboBox->setCurrentIndex(0);
    ui->textViewerEncodeComboBox->blockSignals(false);
}

void PreferencesDialog::on_hexViewerFontPushButton_clicked()
{
    QFont newFont = QFont();

    if(showChooseFontDialog(m_fontSettings["hexViewer"], newFont))
    {
        m_fontSettings["hexViewer"] = newFont;

        setViewerFontAndColorOption();
    }
}

void PreferencesDialog::on_hexViewerFontColorPushButton_clicked()
{
    chooseColor("hexViewer_text", "hexViewer_background", ui->hexViewerSampleLineEdit);
}

void PreferencesDialog::on_hexViewerBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["hexViewer_background"], newColor))
    {
        m_colorSettings["hexViewer_background"] = newColor;

        setFontColorSample("hexViewer_text", "hexViewer_background", ui->hexViewerSampleLineEdit);
    }
}

void PreferencesDialog::on_hexViewerSelectedFontColorPushButton_clicked()
{
    chooseColor("hexViewer_selected_text", "hexViewer_selected_background", ui->hexViewerSelectedSampleLineEdit);
}

void PreferencesDialog::on_hexViewerSelectedBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["hexViewer_selected_background"], newColor))
    {
        m_colorSettings["hexViewer_selected_background"] = newColor;

        setFontColorSample("hexViewer_selected_text", "hexViewer_selected_background", ui->hexViewerSelectedSampleLineEdit);
    }
}

void PreferencesDialog::on_hexViewerAddressFontColorPushButton_clicked()
{
    chooseColor("hexViewer_address_text", "hexViewer_address_background", ui->hexViewerAddressSampleLineEdit);
}

void PreferencesDialog::on_hexViewerAddressBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["hexViewer_address_background"], newColor))
    {
        m_colorSettings["hexViewer_address_background"] = newColor;

        setFontColorSample("hexViewer_address_text", "hexViewer_address_background", ui->hexViewerAddressSampleLineEdit);
    }
}

void PreferencesDialog::on_imageViewerBGColorPushButton_clicked()
{
    QColor oldColor = ui->imageViewerBGColorSampleLineEdit->palette().base().color();
    QColor newColor = QColor();

    if(showChooseColorDialog(oldColor, newColor))
    {
        m_colorSettings["imageViewer_background"] = newColor;

        setFontColorSample("", "imageViewer_background", ui->imageViewerBGColorSampleLineEdit);
    }
}

void PreferencesDialog::setAppearanceFontAndColorOption()
{
    QFont font;

    // Folder View

    font = m_fontSettings["folderView"];

    ui->folderViewFontFontLabel->setText(QString("%1, %2 pt").arg(font.family()).arg(font.pointSize()));
    ui->folderViewFontFontLabel->setFont(font);

    ui->folderViewFontNormalLineEdit->setFont(font);
    ui->folderViewFontNormalSelectedLineEdit->setFont(font);
    ui->folderViewFontFolderLineEdit->setFont(font);
    ui->folderViewFontFolderSelectedLineEdit->setFont(font);
    ui->folderViewFontReadOnlyLineEdit->setFont(font);
    ui->folderViewFontReadOnlySelectedLineEdit->setFont(font);
    ui->folderViewFontHiddenLineEdit->setFont(font);
    ui->folderViewFontHiddenSelectedLineEdit->setFont(font);
    ui->folderViewFontSystemLineEdit->setFont(font);
    ui->folderViewFontSystemSelectedLineEdit->setFont(font);

    setFontColorSample("folderView_normal",            "folderView_background",          ui->folderViewFontNormalLineEdit);
    setFontColorSample("folderView_normal_selected",   "folderView_selected_background", ui->folderViewFontNormalSelectedLineEdit);
    setFontColorSample("folderView_folder",            "folderView_background",          ui->folderViewFontFolderLineEdit);
    setFontColorSample("folderView_folder_selected",   "folderView_selected_background", ui->folderViewFontFolderSelectedLineEdit);
    setFontColorSample("folderView_readOnly",          "folderView_background",          ui->folderViewFontReadOnlyLineEdit);
    setFontColorSample("folderView_readOnly_selected", "folderView_selected_background", ui->folderViewFontReadOnlySelectedLineEdit);
    setFontColorSample("folderView_hidden",            "folderView_background",          ui->folderViewFontHiddenLineEdit);
    setFontColorSample("folderView_hidden_selected",   "folderView_selected_background", ui->folderViewFontHiddenSelectedLineEdit);
    setFontColorSample("folderView_system",            "folderView_background",          ui->folderViewFontSystemLineEdit);
    setFontColorSample("folderView_system_selected",   "folderView_selected_background", ui->folderViewFontSystemSelectedLineEdit);
    setFontColorSample("folderView_cursor",            "folderView_cursor",              ui->folderViewCursorLineEdit);
    setFontColorSample("folderView_cursor_inactive",   "folderView_cursor_inactive",     ui->folderViewCursorInactiveLineEdit);

    // Folder Path

    font = m_fontSettings["folderPath"];

    ui->folderPathFontLabel->setText(QString("%1, %2 pt").arg(font.family()).arg(font.pointSize()));
    ui->folderPathFontLabel->setFont(font);

    ui->folderPathLineEdit->setFont(font);

    setFontColorSample("folderPath_text", "folderPath_background", ui->folderPathLineEdit);

    // Console

    font = m_fontSettings["console"];

    ui->consoleFontLabel->setText(QString("%1, %2 pt").arg(font.family()).arg(font.pointSize()));
    ui->consoleFontLabel->setFont(font);

    ui->consoleLineEdit->setFont(font);

    setFontColorSample("console_text", "console_background", ui->consoleLineEdit);
}

void PreferencesDialog::setViewerFontAndColorOption()
{
    QFont font;

    // Image viewer

    setFontColorSample("", "imageViewer_background", ui->imageViewerBGColorSampleLineEdit);

    // Text viewer

    font = m_fontSettings["textViewer"];

    ui->textViewerFontLabel->setText(QString("%1, %2 pt").arg(font.family()).arg(font.pointSize()));
    ui->textViewerFontLabel->setFont(font);

    setFontColorSample("textViewer_text",            "textViewer_background",            ui->textViewerSampleLineEdit);
    setFontColorSample("textViewer_selected_text",   "textViewer_selected_background",   ui->textViewerSelectedSampleLineEdit);
    setFontColorSample("textViewer_lineNumber_text", "textViewer_lineNumber_background", ui->textViewerLineNumberSampleLineEdit);

    // Hex viewer

    font = m_fontSettings["hexViewer"];

    ui->hexViewerFontLabel->setText(QString("%1, %2 pt").arg(font.family()).arg(font.pointSize()));
    ui->hexViewerFontLabel->setFont(font);

    setFontColorSample("hexViewer_text",          "hexViewer_background",          ui->hexViewerSampleLineEdit);
    setFontColorSample("hexViewer_selected_text", "hexViewer_selected_background", ui->hexViewerSelectedSampleLineEdit);
    setFontColorSample("hexViewer_address_text",  "hexViewer_address_background",  ui->hexViewerAddressSampleLineEdit);
}

void PreferencesDialog::setFontColorSample(const QString& colorSettingType, const QString& bgSettingType, QWidget* widget)
{
    QPalette pal = widget->palette();
    if(colorSettingType != "")
    {
        pal.setColor(QPalette::Text, m_colorSettings[colorSettingType]);
    }
    if(bgSettingType != "")
    {
        pal.setColor(QPalette::Base, m_colorSettings[bgSettingType]);
    }
    widget->setPalette(pal);
}

void PreferencesDialog::chooseColor(const QString& colorSettingType, const QString& bgSettingType, QWidget* widget)
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings[colorSettingType], newColor))
    {
        m_colorSettings[colorSettingType] = newColor;

        setFontColorSample(colorSettingType, bgSettingType, widget);
    }
}

bool PreferencesDialog::showChooseColorDialog(const QColor& oldColor, QColor& newColor)
{
    newColor = QColorDialog::getColor(oldColor, this, tr("Choose color"));

    return newColor.isValid();
}

bool PreferencesDialog::showChooseFontDialog(const QFont& oldFont, QFont& newFont)
{
    bool ok = false;
    newFont = QFontDialog::getFont(&ok, oldFont, this, tr("Choose font"));

    return ok;
}

void PreferencesDialog::on_buttonBox_accepted()
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

    if(ui->dragAndDropBehaviorCopyRadioButton->isChecked())
    {
        Settings::getInstance()->setDragAndDropBehaviorType(DragAndDropBehaviorType::Copy);
    }
    else if(ui->dragAndDropBehaviorMoveRadioButton->isChecked())
    {
        Settings::getInstance()->setDragAndDropBehaviorType(DragAndDropBehaviorType::Move);
    }
    else
    {
        Settings::getInstance()->setDragAndDropBehaviorType(DragAndDropBehaviorType::Select);
    }

    Settings::getInstance()->setAutoDialogCloseCopy(ui->autoDialogCloseCopyCheckBox->isChecked());
    Settings::getInstance()->setAutoDialogCloseMove(ui->autoDialogCloseMoveCheckBox->isChecked());
    Settings::getInstance()->setAutoDialogCloseRemove(ui->autoDialogCloseRemoveCheckBox->isChecked());

    Settings::getInstance()->setAllowCursorAround(ui->allowCursorAroundCheckBox->isChecked());

    Settings::getInstance()->setConfirmQuit(ui->confirmQuitCheckBox->isChecked());

    Settings::getInstance()->setFontSettings(m_fontSettings);
    Settings::getInstance()->setColorSettings(m_colorSettings);

    Settings::getInstance()->setCursorWidth(ui->folderViewCursorWidthSpinBox->value());

    Settings::getInstance()->setImageViewerFitInView(ui->imageViewerFitInViewCheckBox->isChecked());

    Settings::getInstance()->setImageViewerBGType((ui->imageViewerBGTypeComboBox->currentIndex() == 0) ? ImageViewerBGType::Solid
                                                                                                       : ImageViewerBGType::Checkered);

    Settings::getInstance()->setTextViewerEncodeList(m_textViewerEncodeList);

    Settings::getInstance()->setTextViewerShowLineNumber(ui->textViewerShowLineNumberCheckBox->isChecked());
    Settings::getInstance()->setTextViewerWordWrap(ui->textViewerWordWrapCheckBox->isChecked());
}

}           // namespace Farman
