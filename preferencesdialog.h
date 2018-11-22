#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QMap>
#include <QFont>
#include <QColor>
#include "types.h"

namespace Ui {
class PreferencesDialog;
}

namespace Farman
{

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(const QSize& mainWindowSize,
                               const QPoint& mainWindowPos,
                               const QString& leftDirPath,
                               const QString& rightDirPath,
                               QWidget *parent = Q_NULLPTR);
    ~PreferencesDialog() Q_DECL_OVERRIDE;

private slots:
    void on_sizeDefaultRadioButton_clicked();
    void on_sizeLastTimeRadioButton_clicked();
    void on_sizeFixedRadioButton_clicked();
    void on_positionDefaultRadioButton_clicked();
    void on_positionLastTimeRadioButton_clicked();
    void on_positionFixedRadioButton_clicked();
    void on_leftFolderDefaultRadioButton_clicked();
    void on_leftFolderLastTimeRadioButton_clicked();
    void on_leftFolderFixedRadioButton_clicked();
    void on_leftFolderSelectButton_clicked();
    void on_rightFolderDefaultRadioButton_clicked();
    void on_rightFolderLastTimeRadioButton_clicked();
    void on_rightFolderFixedRadioButton_clicked();
    void on_rightFolderSelectButton_clicked();

    void on_singlePaneFileSizeSIRadioButton_clicked();
    void on_singlePaneFileSizeIecRadioButton_clicked();
    void on_singlePaneFileSizeDetailRadioButton_clicked();
    void on_singlePaneFileSizeCommaCheckBox_stateChanged(int arg1);
    void on_singlePaneDateDefaultRadioButton_clicked();
    void on_singlePaneDateISORadioButton_clicked();
    void on_singlePaneDateOriginalRadioButton_clicked();
    void on_singlePaneDateOriginalLineEdit_textChanged(const QString &arg1);
    void on_dualPaneFileSizeSIRadioButton_clicked();
    void on_dualPaneFileSizeIecRadioButton_clicked();
    void on_dualPaneFileSizeDetailRadioButton_clicked();
    void on_dualPaneFileSizeCommaCheckBox_stateChanged(int arg1);
    void on_dualPaneDateDefaultRadioButton_clicked();
    void on_dualPaneDateISORadioButton_clicked();
    void on_dualPaneDateOriginalRadioButton_clicked();
    void on_dualPaneDateOriginalLineEdit_textChanged(const QString &arg1);

    void on_folderViewFontFontPushButton_clicked();

    void on_folderViewFontNormalColorPushButton_clicked();
    void on_folderViewFontNormalSelectedColorPushButton_clicked();
    void on_folderViewFontFolderColorPushButton_clicked();
    void on_folderViewFontFolderSelectedColorPushButton_clicked();
    void on_folderViewFontReadOnlyColorPushButton_clicked();
    void on_folderViewFontReadOnlySelectedColorPushButton_clicked();
    void on_folderViewFontHiddenColorPushButton_clicked();
    void on_folderViewFontHiddenSelectedColorPushButton_clicked();
    void on_folderViewFontSystemColorPushButton_clicked();
    void on_folderViewFontSystemSelectedColorPushButton_clicked();

    void on_folderViewFontBGColorPushButton_clicked();
    void on_folderViewFontSelectedBGColorPushButton_clicked();

    void on_folderViewCursorColorPushButton_clicked();
    void on_folderViewCursorInactiveColorPushButton_clicked();

    void on_folderPathFontPushButton_clicked();
    void on_folderPathColorPushButton_clicked();
    void on_folderPathBGColorPushButton_clicked();

    void on_consoleFontPushButton_clicked();
    void on_consoleColorPushButton_clicked();
    void on_consoleBGColorPushButton_clicked();

    void on_textViewerFontPushButton_clicked();
    void on_textViewerFontColorPushButton_clicked();
    void on_textViewerBGColorPushButton_clicked();
    void on_textViewerSelectedFontColorPushButton_clicked();
    void on_textViewerSelectedBGColorPushButton_clicked();
    void on_textViewerLineNumberFontColorPushButton_clicked();
    void on_textViewerLineNumberBGColorPushButton_clicked();
    void on_textViewerEncodeComboBox_activated(int index);

    void on_hexViewerFontPushButton_clicked();
    void on_hexViewerFontColorPushButton_clicked();
    void on_hexViewerBGColorPushButton_clicked();
    void on_hexViewerSelectedFontColorPushButton_clicked();
    void on_hexViewerSelectedBGColorPushButton_clicked();
    void on_hexViewerAddressFontColorPushButton_clicked();
    void on_hexViewerAddressBGColorPushButton_clicked();

    void on_imageViewerBGColorPushButton_clicked();

    void on_buttonBox_accepted();

private:
    void setFileSizeExample(FileSizeFormatType fileSizeFormatType, bool comma, QLineEdit* exampleLineEdit);
    void setDateExample(DateFormatType dataFormatType, const QString& orgFormat, QLineEdit* exampleLineEdit);

    void setAppearanceFontAndColorOption();
    void setViewerFontAndColorOption();

    void setFontColorSample(const QString& colorSettingType, const QString& bgSettingType, QWidget* widget);

    void chooseColor(const QString& colorSettingType, const QString& bgSettingType, QWidget* widget);

    bool showChooseColorDialog(const QColor& oldColor, QColor& newColor);
    bool showChooseFontDialog(const QFont& oldFont, QFont& newFont);

    QMap<QString, QFont> m_fontSettings;
    QMap<QString, QColor> m_colorSettings;

    QList<QString> m_textViewerEncodeList;

    Ui::PreferencesDialog *ui;
};

}           // namespace Farman

#endif // PREFERENCESDIALOG_H
