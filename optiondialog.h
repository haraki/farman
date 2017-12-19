#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QFont>

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

    void on_buttonBox_accepted();

private:
    QFont chooseFont(const QFont& oldFont);

    Ui::OptionDialog *ui;
};

}           // namespace Farman

#endif // OPTIONDIALOG_H
