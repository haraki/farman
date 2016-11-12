#ifndef FORDERFORM_H
#define FORDERFORM_H

#include <QWidget>

namespace Ui {
class ForderForm;
}

class ForderForm : public QWidget
{
    Q_OBJECT

public:
    explicit ForderForm(QWidget *parent = 0);
    ~ForderForm();

private:
    Ui::ForderForm *ui;
};

#endif // FORDERFORM_H
