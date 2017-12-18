#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QMap>
#include <QFont>
#include <QColor>

namespace Ui {
class OptionDialog;
}

namespace Farman
{

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(const QSize& mainWindowSize,
                          const QPoint& mainWindowPos,
                          const QString& leftDirPath,
                          const QString& rightDirPath,
                          QWidget *parent = Q_NULLPTR);
    ~OptionDialog();

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

    void on_fontAndColorFolderViewChooseFontPushButton_clicked();

    void on_fontAndColorFolderViewChooseNormalColorPushButton_clicked();
    void on_fontAndColorFolderViewChooseNormalSelectedColorPushButton_clicked();
    void on_fontAndColorFolderViewChooseFolderColorPushButton_clicked();
    void on_fontAndColorFolderViewChooseFolderSelectedColorPushButton_clicked();
    void on_fontAndColorFolderViewChooseReadOnlyColorPushButton_clicked();
    void on_fontAndColorFolderViewChooseReadOnlySelectedColorPushButton_clicked();
    void on_fontAndColorFolderViewChooseHiddenColorPushButton_clicked();
    void on_fontAndColorFolderViewChooseHiddenSelectedColorPushButton_clicked();
    void on_fontAndColorFolderViewChooseSystemColorPushButton_clicked();
    void on_fontAndColorFolderViewChooseSystemSelectedColorPushButton_clicked();

    void on_fontAndColorFolderViewChooseBGColorPushButton_clicked();
    void on_fontAndColorFolderViewChooseSelectedBGColorPushButton_clicked();

    void on_buttonBox_accepted();

private:
    void setFontAndColorFolderViewOption();
    void setFontAndColorFolderPathOption();
    void setFontAndColorConsoleOption();

    void setFontColorSample(const QString& colorSettingType, const QString& bgSettingType, QWidget* widget);

    void chooseColor(const QString& colorSettingType, const QString& bgSettingType, QWidget* widget);

    bool showChooseColorDialog(const QColor& oldColor, QColor& newColor);
    bool showChooseFontDialog(const QFont& oldFont, QFont& newFont);

    QMap<QString, QFont> m_fontSettings;
    QMap<QString, QColor> m_colorSettings;

    Ui::OptionDialog *ui;
};

}           // namespace Farman

#endif // OPTIONDIALOG_H
