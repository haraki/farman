#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QStandardPaths>
#include <QtGlobal>
#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "settings.h"

namespace Farman
{

PreferencesDialog::PreferencesDialog(const QSize& mainWindowSize,
                                     const QPoint& mainWindowPos,
                                     const QString& leftDirPath,
                                     const QString& rightDirPath,
                                     PreferencesDialogTabPage page/* = PreferencesDialogTabPage::General*/,
                                     QWidget *parent/* = Q_NULLPTR */) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(static_cast<int>(page));

    initialize(mainWindowSize, mainWindowPos, leftDirPath, rightDirPath);
}

void PreferencesDialog::initialize(const QSize& mainWindowSize,
                                   const QPoint& mainWindowPos,
                                   const QString& leftDirPath,
                                   const QString& rightDirPath)
{
    // General

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

    {
        FolderAtStartup folderAtStartup = Settings::getInstance()->getFolderAtStartup(PaneType::Left);
        if(folderAtStartup == FolderAtStartup::Fixed)
        {
            ui->leftFolderFixedRadioButton->setChecked(true);
            ui->leftFolderSelectButton->setEnabled(true);
            ui->leftFolderPathLineEdit->setEnabled(true);

            ui->leftFolderPathLineEdit->setText(Settings::getInstance()->getFolderPath(PaneType::Left));
        }
        else
        {
            if(folderAtStartup == FolderAtStartup::LastTime)
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
    }

    {
        FolderAtStartup folderAtStartup = Settings::getInstance()->getFolderAtStartup(PaneType::Right);
        if(folderAtStartup == FolderAtStartup::Fixed)
        {
            ui->rightFolderFixedRadioButton->setChecked(true);
            ui->rightFolderSelectButton->setEnabled(true);
            ui->rightFolderPathLineEdit->setEnabled(true);

            ui->rightFolderPathLineEdit->setText(Settings::getInstance()->getFolderPath(PaneType::Right));
        }
        else
        {
            if(folderAtStartup == FolderAtStartup::LastTime)
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

    ui->moveCursorOpenViewerCheckBox->setChecked(Settings::getInstance()->getMoveCursorOpenViewer());

    ui->confirmQuitCheckBox->setChecked(Settings::getInstance()->getConfirmQuit());

    // Folder View

    FileSizeFormatType singlePaneFileSizeFormatType = Settings::getInstance()->getSinglePaneFileSizeFormatType();
    if(singlePaneFileSizeFormatType == FileSizeFormatType::IEC)
    {
        ui->singlePaneFileSizeIecRadioButton->setChecked(true);
        ui->singlePaneFileSizeCommaCheckBox->setEnabled(false);
    }
    else if(singlePaneFileSizeFormatType == FileSizeFormatType::Detail)
    {
        ui->singlePaneFileSizeDetailRadioButton->setChecked(true);
        ui->singlePaneFileSizeCommaCheckBox->setEnabled(true);
    }
    else
    {
        ui->singlePaneFileSizeSIRadioButton->setChecked(true);
        ui->singlePaneFileSizeCommaCheckBox->setEnabled(false);
    }

    bool singlePaneFileSizeDetailComma = Settings::getInstance()->getSinglePaneFileSizeDetailCommaEnable();
    ui->singlePaneFileSizeCommaCheckBox->setChecked(singlePaneFileSizeDetailComma);

    setFileSizeExample(singlePaneFileSizeFormatType, singlePaneFileSizeDetailComma, ui->singlePaneFileSizeExampleLineEdit);

    DateFormatType singlePaneDateFormatType = Settings::getInstance()->getSinglePaneDateFormatType();
    if(singlePaneDateFormatType == DateFormatType::ISO)
    {
        ui->singlePaneDateISORadioButton->setChecked(true);
        ui->singlePaneDateOriginalLineEdit->setEnabled(false);
    }
    else if(singlePaneDateFormatType == DateFormatType::Original)
    {
        ui->singlePaneDateOriginalRadioButton->setChecked(true);
        ui->singlePaneDateOriginalLineEdit->setEnabled(true);
    }
    else
    {
        ui->singlePaneDateDefaultRadioButton->setChecked(true);
        ui->singlePaneDateOriginalLineEdit->setEnabled(false);
    }

    QString singlePaneFormatOriginal = Settings::getInstance()->getSinglePaneDateFormatOriginalString();
    ui->singlePaneDateOriginalLineEdit->setText(singlePaneFormatOriginal);

    setDateExample(singlePaneDateFormatType, singlePaneFormatOriginal, ui->singlePaneDateExampleLineEdit);

    FileSizeFormatType dualPaneFileSizeFormatType = Settings::getInstance()->getDualPaneFileSizeFormatType();
    if(dualPaneFileSizeFormatType == FileSizeFormatType::IEC)
    {
        ui->dualPaneFileSizeIecRadioButton->setChecked(true);
        ui->dualPaneFileSizeCommaCheckBox->setEnabled(false);
    }
    else if(dualPaneFileSizeFormatType == FileSizeFormatType::Detail)
    {
        ui->dualPaneFileSizeDetailRadioButton->setChecked(true);
        ui->dualPaneFileSizeCommaCheckBox->setEnabled(true);
    }
    else
    {
        ui->dualPaneFileSizeSIRadioButton->setChecked(true);
        ui->dualPaneFileSizeCommaCheckBox->setEnabled(false);
    }

    bool dualPaneFileSizeDetailComma = Settings::getInstance()->getDualPaneFileSizeDetailCommaEnable();
    ui->dualPaneFileSizeCommaCheckBox->setChecked(dualPaneFileSizeDetailComma);

    setFileSizeExample(dualPaneFileSizeFormatType, dualPaneFileSizeDetailComma, ui->dualPaneFileSizeExampleLineEdit);

    DateFormatType dualPaneDateFormatType = Settings::getInstance()->getDualPaneDateFormatType();
    if(dualPaneDateFormatType == DateFormatType::ISO)
    {
        ui->dualPaneDateISORadioButton->setChecked(true);
        ui->dualPaneDateOriginalLineEdit->setEnabled(false);
    }
    else if(dualPaneDateFormatType == DateFormatType::Original)
    {
        ui->dualPaneDateOriginalRadioButton->setChecked(true);
        ui->dualPaneDateOriginalLineEdit->setEnabled(true);
    }
    else
    {
        ui->dualPaneDateDefaultRadioButton->setChecked(true);
        ui->dualPaneDateOriginalLineEdit->setEnabled(false);
    }

    QString dualPaneFormatOriginal = Settings::getInstance()->getDualPaneDateFormatOriginalString();
    ui->dualPaneDateOriginalLineEdit->setText(dualPaneFormatOriginal);

    setDateExample(dualPaneDateFormatType, dualPaneFormatOriginal, ui->dualPaneDateExampleLineEdit);

    // Appearance

    m_fontSettings = Settings::getInstance()->getFontSettings();
    m_colorSettings = Settings::getInstance()->getColorSettings();

    ui->folderViewInactiveFontGroupBox->setChecked(Settings::getInstance()->getEnableInactiveFontColor());

    ui->folderViewCursorWidthSpinBox->setValue(Settings::getInstance()->getCursorWidth());

    setAppearanceFontAndColorOption();

    // Viewer

    ui->imageViewerFitInViewCheckBox->setChecked(Settings::getInstance()->getImageViewerFitInView());

    ui->imageViewerTransparentBGTypeComboBox->addItems({tr("Solid"), tr("Checkered")});
    ui->imageViewerTransparentBGTypeComboBox->setCurrentIndex((Settings::getInstance()->getImageViewerTransparentBGType() == ImageViewerTransparentBGType::Solid) ? 0 : 1);

    m_textViewerEncodeList = Settings::getInstance()->getTextViewerEncodeList();
    ui->textViewerEncodeComboBox->addItems(m_textViewerEncodeList);

    ui->textViewerShowLineNumberCheckBox->setChecked(Settings::getInstance()->getTextViewerShowLineNumber());
    ui->textViewerWordWrapCheckBox->setChecked(Settings::getInstance()->getTextViewerWordWrap());

    // External application

    ui->textEditorPathLineEdit->setText(Settings::getInstance()->getTextEditorPath());
    ui->textEditorArgsLineEdit->setText(Settings::getInstance()->getTextEditorArgs());

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

    repaint();
}

void PreferencesDialog::on_sizeLastTimeRadioButton_clicked()
{
    ui->sizeWidthLabel->setEnabled(false);
    ui->sizeWidthLineEdit->setEnabled(false);
    ui->sizeHeightLabel->setEnabled(false);
    ui->sizeHeightLineEdit->setEnabled(false);

    repaint();
}

void PreferencesDialog::on_sizeFixedRadioButton_clicked()
{
    ui->sizeWidthLabel->setEnabled(true);
    ui->sizeWidthLineEdit->setEnabled(true);
    ui->sizeHeightLabel->setEnabled(true);
    ui->sizeHeightLineEdit->setEnabled(true);

    repaint();
}

void PreferencesDialog::on_positionDefaultRadioButton_clicked()
{
    ui->positionXLabel->setEnabled(false);
    ui->positionXLineEdit->setEnabled(false);
    ui->positionYLabel->setEnabled(false);
    ui->positionYLineEdit->setEnabled(false);

    repaint();
}

void PreferencesDialog::on_positionLastTimeRadioButton_clicked()
{
    ui->positionXLabel->setEnabled(false);
    ui->positionXLineEdit->setEnabled(false);
    ui->positionYLabel->setEnabled(false);
    ui->positionYLineEdit->setEnabled(false);

    repaint();
}

void PreferencesDialog::on_positionFixedRadioButton_clicked()
{
    ui->positionXLabel->setEnabled(true);
    ui->positionXLineEdit->setEnabled(true);
    ui->positionYLabel->setEnabled(true);
    ui->positionYLineEdit->setEnabled(true);

    repaint();
}

void PreferencesDialog::on_leftFolderDefaultRadioButton_clicked()
{
    ui->leftFolderSelectButton->setEnabled(false);
    ui->leftFolderPathLineEdit->setEnabled(false);

    repaint();
}

void PreferencesDialog::on_leftFolderLastTimeRadioButton_clicked()
{
    ui->leftFolderSelectButton->setEnabled(false);
    ui->leftFolderPathLineEdit->setEnabled(false);

    repaint();
}

void PreferencesDialog::on_leftFolderFixedRadioButton_clicked()
{
    ui->leftFolderSelectButton->setEnabled(true);
    ui->leftFolderPathLineEdit->setEnabled(true);

    repaint();
}

void PreferencesDialog::on_rightFolderDefaultRadioButton_clicked()
{
    ui->rightFolderSelectButton->setEnabled(false);
    ui->rightFolderPathLineEdit->setEnabled(false);

    repaint();
}

void PreferencesDialog::on_rightFolderLastTimeRadioButton_clicked()
{
    ui->rightFolderSelectButton->setEnabled(false);
    ui->rightFolderPathLineEdit->setEnabled(false);

    repaint();
}

void PreferencesDialog::on_rightFolderFixedRadioButton_clicked()
{
    ui->rightFolderSelectButton->setEnabled(true);
    ui->rightFolderPathLineEdit->setEnabled(true);

    repaint();
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

void PreferencesDialog::on_initializeSettingsPushButton_clicked()
{
    if(QMessageBox::question(this, tr("Confirm"), tr("initialize settings?")) != QMessageBox::Yes)
    {
        return;
    }

    Settings::getInstance()->clear();
    Settings::getInstance()->initialize();

    initialize(QSize(ui->sizeWidthLineEdit->text().toInt(), ui->sizeHeightLineEdit->text().toInt()),
               QPoint(ui->positionXLineEdit->text().toInt(), ui->positionYLineEdit->text().toInt()),
               ui->leftFolderPathLineEdit->text(), ui->rightFolderPathLineEdit->text());

    repaint();
}

void PreferencesDialog::on_singlePaneFileSizeSIRadioButton_clicked()
{
    ui->singlePaneFileSizeCommaCheckBox->setEnabled(false);
    setFileSizeExample(FileSizeFormatType::SI, ui->singlePaneFileSizeCommaCheckBox->isChecked(), ui->singlePaneFileSizeExampleLineEdit);
}

void PreferencesDialog::on_singlePaneFileSizeIecRadioButton_clicked()
{
    ui->singlePaneFileSizeCommaCheckBox->setEnabled(false);
    setFileSizeExample(FileSizeFormatType::IEC, ui->singlePaneFileSizeCommaCheckBox->isChecked(), ui->singlePaneFileSizeExampleLineEdit);
}

void PreferencesDialog::on_singlePaneFileSizeDetailRadioButton_clicked()
{
    ui->singlePaneFileSizeCommaCheckBox->setEnabled(true);
    setFileSizeExample(FileSizeFormatType::Detail, ui->singlePaneFileSizeCommaCheckBox->isChecked(), ui->singlePaneFileSizeExampleLineEdit);
}

void PreferencesDialog::on_singlePaneFileSizeCommaCheckBox_stateChanged(int arg1)
{
    Q_UNUSED(arg1);

    FileSizeFormatType fileSizeFormatType = ui->singlePaneFileSizeSIRadioButton->isChecked()     ? FileSizeFormatType::SI :
                                            ui->singlePaneFileSizeIecRadioButton->isChecked()    ? FileSizeFormatType::IEC :
                                            ui->singlePaneFileSizeDetailRadioButton->isChecked() ? FileSizeFormatType::Detail :
                                                                                                   DEFAULT_SINGLE_PANE_FILE_SIZE_FORMAT_TYPE;

    setFileSizeExample(fileSizeFormatType, ui->singlePaneFileSizeCommaCheckBox->isChecked(), ui->singlePaneFileSizeExampleLineEdit);
}

void PreferencesDialog::on_singlePaneDateDefaultRadioButton_clicked()
{
    ui->singlePaneDateOriginalLineEdit->setEnabled(false);
    setDateExample(DateFormatType::Default, ui->singlePaneDateOriginalLineEdit->text(), ui->singlePaneDateExampleLineEdit);
}

void PreferencesDialog::on_singlePaneDateISORadioButton_clicked()
{
    ui->singlePaneDateOriginalLineEdit->setEnabled(false);
    setDateExample(DateFormatType::ISO, ui->singlePaneDateOriginalLineEdit->text(), ui->singlePaneDateExampleLineEdit);
}

void PreferencesDialog::on_singlePaneDateOriginalRadioButton_clicked()
{
    ui->singlePaneDateOriginalLineEdit->setEnabled(true);
    setDateExample(DateFormatType::Original, ui->singlePaneDateOriginalLineEdit->text(), ui->singlePaneDateExampleLineEdit);
}

void PreferencesDialog::on_singlePaneDateOriginalLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);

    setDateExample(DateFormatType::Original, ui->singlePaneDateOriginalLineEdit->text(), ui->singlePaneDateExampleLineEdit);
}

void PreferencesDialog::on_dualPaneFileSizeSIRadioButton_clicked()
{
    ui->dualPaneFileSizeCommaCheckBox->setEnabled(false);
    setFileSizeExample(FileSizeFormatType::SI, ui->dualPaneFileSizeCommaCheckBox->isChecked(), ui->dualPaneFileSizeExampleLineEdit);
}

void PreferencesDialog::on_dualPaneFileSizeIecRadioButton_clicked()
{
    ui->dualPaneFileSizeCommaCheckBox->setEnabled(false);
    setFileSizeExample(FileSizeFormatType::IEC, ui->dualPaneFileSizeCommaCheckBox->isChecked(), ui->dualPaneFileSizeExampleLineEdit);
}

void PreferencesDialog::on_dualPaneFileSizeDetailRadioButton_clicked()
{
    ui->dualPaneFileSizeCommaCheckBox->setEnabled(true);
    setFileSizeExample(FileSizeFormatType::Detail, ui->dualPaneFileSizeCommaCheckBox->isChecked(), ui->dualPaneFileSizeExampleLineEdit);
}

void PreferencesDialog::on_dualPaneFileSizeCommaCheckBox_stateChanged(int arg1)
{
    Q_UNUSED(arg1);

    FileSizeFormatType fileSizeFormatType = ui->dualPaneFileSizeSIRadioButton->isChecked()     ? FileSizeFormatType::SI :
                                            ui->dualPaneFileSizeIecRadioButton->isChecked()    ? FileSizeFormatType::IEC :
                                            ui->dualPaneFileSizeDetailRadioButton->isChecked() ? FileSizeFormatType::Detail :
                                                                                                 DEFAULT_DUAL_PANE_FILE_SIZE_FORMAT_TYPE;

    setFileSizeExample(fileSizeFormatType, ui->dualPaneFileSizeCommaCheckBox->isChecked(), ui->dualPaneFileSizeExampleLineEdit);
}

void PreferencesDialog::on_dualPaneDateDefaultRadioButton_clicked()
{
    ui->dualPaneDateOriginalLineEdit->setEnabled(false);
    setDateExample(DateFormatType::Default, ui->dualPaneDateOriginalLineEdit->text(), ui->dualPaneDateExampleLineEdit);
}

void PreferencesDialog::on_dualPaneDateISORadioButton_clicked()
{
    ui->dualPaneDateOriginalLineEdit->setEnabled(false);
    setDateExample(DateFormatType::ISO, ui->dualPaneDateOriginalLineEdit->text(), ui->dualPaneDateExampleLineEdit);
}

void PreferencesDialog::on_dualPaneDateOriginalRadioButton_clicked()
{
    ui->dualPaneDateOriginalLineEdit->setEnabled(true);
    setDateExample(DateFormatType::Original, ui->dualPaneDateOriginalLineEdit->text(), ui->dualPaneDateExampleLineEdit);
}

void PreferencesDialog::on_dualPaneDateOriginalLineEdit_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);

    setDateExample(DateFormatType::Original, ui->dualPaneDateOriginalLineEdit->text(), ui->dualPaneDateExampleLineEdit);
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

void PreferencesDialog::on_folderViewInactiveFontColorPushButton_clicked()
{
    chooseColor("folderView_inactive", "folderView_inactive_background", ui->folderViewInactiveFontLineEdit);
}

void PreferencesDialog::on_folderViewInactiveFontSelectedColorPushButton_clicked()
{
    chooseColor("folderView_inactive_selected", "folderView_inactive_selected_background", ui->folderViewInactiveFontSelectedLineEdit);
}

void PreferencesDialog::on_folderViewInactiveFontBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["folderView_inactive_background"], newColor))
    {
        m_colorSettings["folderView_inactive_background"] = newColor;

        setFontColorSample("folderView_inactive", "folderView_inactive_background", ui->folderViewInactiveFontLineEdit);
    }
}

void PreferencesDialog::on_folderViewInactiveFontSelectedBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["folderView_inactive_selected_background"], newColor))
    {
        m_colorSettings["folderView_inactive_selected_background"] = newColor;

        setFontColorSample("folderView_inactive_selected", "folderView_inactive_selected_background", ui->folderViewInactiveFontSelectedLineEdit);
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

void PreferencesDialog::on_textViewerNormalFontColorPushButton_clicked()
{
    chooseColor("textViewer_text", "textViewer_background", ui->textViewerNormalSampleLineEdit);
}

void PreferencesDialog::on_textViewerNormalBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["textViewer_background"], newColor))
    {
        m_colorSettings["textViewer_background"] = newColor;

        setFontColorSample("textViewer_text", "textViewer_background", ui->textViewerNormalSampleLineEdit);
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

void PreferencesDialog::on_hexViewerNormalFontColorPushButton_clicked()
{
    chooseColor("hexViewer_text", "hexViewer_background", ui->hexViewerNormalSampleLineEdit);
}

void PreferencesDialog::on_hexViewerNormalBGColorPushButton_clicked()
{
    QColor newColor = QColor();

    if(showChooseColorDialog(m_colorSettings["hexViewer_background"], newColor))
    {
        m_colorSettings["hexViewer_background"] = newColor;

        setFontColorSample("hexViewer_text", "hexViewer_background", ui->hexViewerNormalSampleLineEdit);
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

void PreferencesDialog::on_imageViewerTransparentBGColorPushButton_clicked()
{
    QColor oldColor = ui->imageViewerTransparentBGColorSampleLineEdit->palette().base().color();
    QColor newColor = QColor();

    if(showChooseColorDialog(oldColor, newColor))
    {
        m_colorSettings["imageViewer_transparent_background"] = newColor;

        setFontColorSample("", "imageViewer_transparent_background", ui->imageViewerTransparentBGColorSampleLineEdit);
    }
}

void PreferencesDialog::on_textEditorSelectButton_clicked()
{
    QString appPath = ui->textEditorPathLineEdit->text();
    QString dirPath = QFileInfo(appPath).absolutePath();

    if(dirPath.isEmpty())
    {
#ifdef Q_OS_WIN
        // Program Files フォルダのパスを取得するには環境変数 PROGRAMFILES から取得するしかない
        // (QStandartPaths::standardLocations() では返されない)
        // 参考 : https://doc.qt.io/qt-5/qstandardpaths.html#StandardLocation-enum
        dirPath = qEnvironmentVariable("PROGRAMFILES", "C:\\Program Files");
#else
        dirPath = QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation).at(0);
#endif
    }

    appPath = QFileDialog::getOpenFileName(this,
                                           tr("Choose application."),
                                           dirPath,
#if defined(Q_OS_WIN)
                                           tr("Applications (*.exe *.com *.bat *.pif);;All files (*)"));
#elif defined(Q_OS_MAC)
                                           tr("Applications (*.app);;All files (*)"));
#else
                                           tr("All files (*)"));
#endif

    if(!appPath.isEmpty())
    {
        ui->textEditorPathLineEdit->setText(appPath);
    }
}

void PreferencesDialog::setFileSizeExample(FileSizeFormatType fileSizeFormatType, bool comma, QLineEdit* exampleLineEdit)
{
    Q_ASSERT(exampleLineEdit);

    const int exampleSize = 1234567;
    QString exampleStr = "";

    if(fileSizeFormatType == FileSizeFormatType::Detail)
    {
        exampleStr = (comma) ? QLocale(QLocale::English).toString(exampleSize) : QString::number(exampleSize);
    }
    else
    {
        exampleStr = QLocale().formattedDataSize(exampleSize, 2,
                                                 (fileSizeFormatType == FileSizeFormatType::IEC) ? QLocale::DataSizeIecFormat :
                                                                                                   QLocale::DataSizeSIFormat);
    }

    exampleLineEdit->setText(exampleStr);

    repaint();
}

void PreferencesDialog::setDateExample(DateFormatType dataFormatType, const QString& orgFormat, QLineEdit* exampleLineEdit)
{
    Q_ASSERT(exampleLineEdit);

    const QDateTime exampleDateTime = QDateTime::fromString("2001-01-23T12:34:56", Qt::ISODate);
    QString exampleStr = "";

    if(dataFormatType == DateFormatType::ISO)
    {
        exampleStr = exampleDateTime.toString(Qt::ISODate);
    }
    else if(dataFormatType == DateFormatType::Original)
    {
        exampleStr = exampleDateTime.toString(orgFormat);
    }
    else
    {
        exampleStr = exampleDateTime.toString(Qt::TextDate);
    }

    exampleLineEdit->setText(exampleStr);

    repaint();
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

    setFontColorSample("folderView_inactive",          "folderView_inactive_background",          ui->folderViewInactiveFontLineEdit);
    setFontColorSample("folderView_inactive_selected", "folderView_inactive_selected_background", ui->folderViewInactiveFontSelectedLineEdit);

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

    setFontColorSample("", "imageViewer_transparent_background", ui->imageViewerTransparentBGColorSampleLineEdit);

    // Text viewer

    font = m_fontSettings["textViewer"];

    ui->textViewerFontLabel->setText(QString("%1, %2 pt").arg(font.family()).arg(font.pointSize()));
    ui->textViewerFontLabel->setFont(font);

    ui->textViewerNormalSampleLineEdit->setFont(font);
    ui->textViewerSelectedSampleLineEdit->setFont(font);
    ui->textViewerLineNumberSampleLineEdit->setFont(font);

    setFontColorSample("textViewer_text",            "textViewer_background",            ui->textViewerNormalSampleLineEdit);
    setFontColorSample("textViewer_selected_text",   "textViewer_selected_background",   ui->textViewerSelectedSampleLineEdit);
    setFontColorSample("textViewer_lineNumber_text", "textViewer_lineNumber_background", ui->textViewerLineNumberSampleLineEdit);

    // Hex viewer

    font = m_fontSettings["hexViewer"];

    ui->hexViewerFontLabel->setText(QString("%1, %2 pt").arg(font.family()).arg(font.pointSize()));
    ui->hexViewerFontLabel->setFont(font);

    ui->hexViewerNormalSampleLineEdit->setFont(font);
    ui->hexViewerSelectedSampleLineEdit->setFont(font);
    ui->hexViewerAddressSampleLineEdit->setFont(font);

    setFontColorSample("hexViewer_text",          "hexViewer_background",          ui->hexViewerNormalSampleLineEdit);
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
    newFont = QFontDialog::getFont(&ok, oldFont, this, tr("Choose font"), QFontDialog::DontUseNativeDialog);    // macOS だと NativeDialog でフォント変更できない(Qt 5.11)

    return ok;
}

void PreferencesDialog::on_buttonBox_accepted()
{
    // General

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
        Settings::getInstance()->setFolderAtStartup(PaneType::Left, FolderAtStartup::Fixed);

        QString dirPath = ui->leftFolderPathLineEdit->text();
        Settings::getInstance()->setFolderPath(PaneType::Left, dirPath);
    }
    else if(ui->leftFolderLastTimeRadioButton->isChecked())
    {
        Settings::getInstance()->setFolderAtStartup(PaneType::Left, FolderAtStartup::LastTime);
    }
    else
    {
        Settings::getInstance()->setFolderAtStartup(PaneType::Left, FolderAtStartup::Default);
    }

    if(ui->rightFolderFixedRadioButton->isChecked())
    {
        Settings::getInstance()->setFolderAtStartup(PaneType::Right, FolderAtStartup::Fixed);

        QString dirPath = ui->rightFolderPathLineEdit->text();
        Settings::getInstance()->setFolderPath(PaneType::Right, dirPath);
    }
    else if(ui->rightFolderLastTimeRadioButton->isChecked())
    {
        Settings::getInstance()->setFolderAtStartup(PaneType::Right, FolderAtStartup::LastTime);
    }
    else
    {
        Settings::getInstance()->setFolderAtStartup(PaneType::Right, FolderAtStartup::Default);
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

    Settings::getInstance()->setMoveCursorOpenViewer(ui->moveCursorOpenViewerCheckBox->isChecked());

    Settings::getInstance()->setConfirmQuit(ui->confirmQuitCheckBox->isChecked());

    // FolderView

    if(ui->singlePaneFileSizeIecRadioButton->isChecked())
    {
        Settings::getInstance()->setSinglePaneFileSizeFormatType(FileSizeFormatType::IEC);
    }
    else if(ui->singlePaneFileSizeDetailRadioButton->isChecked())
    {
        Settings::getInstance()->setSinglePaneFileSizeFormatType(FileSizeFormatType::Detail);
    }
    else
    {
        Settings::getInstance()->setSinglePaneFileSizeFormatType(FileSizeFormatType::SI);
    }

    Settings::getInstance()->setSinglePaneFileSizeDetailCommaEnable(ui->singlePaneFileSizeCommaCheckBox->isChecked());

    if(ui->singlePaneDateISORadioButton->isChecked())
    {
        Settings::getInstance()->setSinglePaneDateFormatType(DateFormatType::ISO);
    }
    else if(ui->singlePaneDateOriginalRadioButton->isChecked())
    {
        Settings::getInstance()->setSinglePaneDateFormatType(DateFormatType::Original);
    }
    else
    {
        Settings::getInstance()->setSinglePaneDateFormatType(DateFormatType::Default);
    }

    Settings::getInstance()->setSinglePaneDateFormatOriginalString(ui->singlePaneDateOriginalLineEdit->text());

    if(ui->dualPaneFileSizeIecRadioButton->isChecked())
    {
        Settings::getInstance()->setDualPaneFileSizeFormatType(FileSizeFormatType::IEC);
    }
    else if(ui->dualPaneFileSizeDetailRadioButton->isChecked())
    {
        Settings::getInstance()->setDualPaneFileSizeFormatType(FileSizeFormatType::Detail);
    }
    else
    {
        Settings::getInstance()->setDualPaneFileSizeFormatType(FileSizeFormatType::SI);
    }

    Settings::getInstance()->setDualPaneFileSizeDetailCommaEnable(ui->dualPaneFileSizeCommaCheckBox->isChecked());

    if(ui->dualPaneDateISORadioButton->isChecked())
    {
        Settings::getInstance()->setDualPaneDateFormatType(DateFormatType::ISO);
    }
    else if(ui->dualPaneDateOriginalRadioButton->isChecked())
    {
        Settings::getInstance()->setDualPaneDateFormatType(DateFormatType::Original);
    }
    else
    {
        Settings::getInstance()->setDualPaneDateFormatType(DateFormatType::Default);
    }

    Settings::getInstance()->setDualPaneDateFormatOriginalString(ui->dualPaneDateOriginalLineEdit->text());

    // Appearance

    Settings::getInstance()->setFontSettings(m_fontSettings);
    Settings::getInstance()->setColorSettings(m_colorSettings);

    Settings::getInstance()->setEnableInactiveFontColor(ui->folderViewInactiveFontGroupBox->isChecked());

    Settings::getInstance()->setCursorWidth(ui->folderViewCursorWidthSpinBox->value());

    // Viewer

    Settings::getInstance()->setImageViewerFitInView(ui->imageViewerFitInViewCheckBox->isChecked());

    Settings::getInstance()->setImageViewerTransparentBGType((ui->imageViewerTransparentBGTypeComboBox->currentIndex() == 0) ? ImageViewerTransparentBGType::Solid
                                                                                                                             : ImageViewerTransparentBGType::Checkered);

    Settings::getInstance()->setTextViewerEncodeList(m_textViewerEncodeList);

    Settings::getInstance()->setTextViewerShowLineNumber(ui->textViewerShowLineNumberCheckBox->isChecked());
    Settings::getInstance()->setTextViewerWordWrap(ui->textViewerWordWrapCheckBox->isChecked());

    // External application

    Settings::getInstance()->setTextEditorPath(ui->textEditorPathLineEdit->text());
    Settings::getInstance()->setTextEditorArgs(ui->textEditorArgsLineEdit->text());
}

}           // namespace Farman
