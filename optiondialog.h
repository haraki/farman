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
    void on_generalSizeDefaultRadioButton_clicked();
    void on_generalSizeLastTimeRadioButton_clicked();
    void on_generalSizeFixedRadioButton_clicked();
    void on_generalPositionDefaultRadioButton_clicked();
    void on_generalPositionLastTimeRadioButton_clicked();
    void on_generalPositionFixedRadioButton_clicked();
    void on_generalLeftFolderDefaultRadioButton_clicked();
    void on_generalLeftFolderLastTimeRadioButton_clicked();
    void on_generalLeftFolderFixedRadioButton_clicked();
    void on_generalLeftFolderSelectButton_clicked();
    void on_generalRightFolderDefaultRadioButton_clicked();
    void on_generalRightFolderLastTimeRadioButton_clicked();
    void on_generalRightFolderFixedRadioButton_clicked();
    void on_generalRightFolderSelectButton_clicked();

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
