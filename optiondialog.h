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

    void on_appearanceFolderViewChooseFontPushButton_clicked();

    void on_appearanceFolderViewChooseNormalColorPushButton_clicked();
    void on_appearanceFolderViewChooseNormalSelectedColorPushButton_clicked();
    void on_appearanceFolderViewChooseFolderColorPushButton_clicked();
    void on_appearanceFolderViewChooseFolderSelectedColorPushButton_clicked();
    void on_appearanceFolderViewChooseReadOnlyColorPushButton_clicked();
    void on_appearanceFolderViewChooseReadOnlySelectedColorPushButton_clicked();
    void on_appearanceFolderViewChooseHiddenColorPushButton_clicked();
    void on_appearanceFolderViewChooseHiddenSelectedColorPushButton_clicked();
    void on_appearanceFolderViewChooseSystemColorPushButton_clicked();
    void on_appearanceFolderViewChooseSystemSelectedColorPushButton_clicked();

    void on_appearanceFolderViewChooseBGColorPushButton_clicked();
    void on_appearanceFolderViewChooseSelectedBGColorPushButton_clicked();

    void on_appearanceFolderViewChooseCursorColorPushButton_clicked();
    void on_appearanceFolderViewChooseCursorInactiveColorPushButton_clicked();

    void on_buttonBox_accepted();

private:
    void setAppearanceFolderViewOption();
    void setAppearanceFolderPathOption();
    void setAppearanceConsoleOption();

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
