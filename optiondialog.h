#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

namespace Ui {
class OptionDialog;
}

namespace Farman
{

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = 0);
    ~OptionDialog();

private slots:
    void on_sizeDefaultRadioButton_clicked();

    void on_sizeLastTimeRadioButton_clicked();

    void on_sizeFixedRadioButton_clicked();

    void on_positionDefaultRadioButton_clicked();

    void on_positionLastTimeRadioButton_clicked();

    void on_positionFixedRadioButton_clicked();

private:
    Ui::OptionDialog *ui;
};

}           // namespace Farman

#endif // OPTIONDIALOG_H
