#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>

namespace Ui {
class FilterDialog;
}

namespace Farman
{

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(QWidget *parent = 0);
    ~FilterDialog();

private:
    Ui::FilterDialog *ui;
};

}           // namespace Farman

#endif // FILTERDIALOG_H
