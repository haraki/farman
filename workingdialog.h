#ifndef WORKINGDIALOG_H
#define WORKINGDIALOG_H

#include <QDialog>

namespace Ui {
class WorkingDialog;
}

namespace Farman
{

class WorkingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WorkingDialog(QWidget *parent = 0);
    virtual ~WorkingDialog();

private:
    Ui::WorkingDialog *ui;
};

}           // namespace Farman

#endif // WORKINGDIALOG_H
