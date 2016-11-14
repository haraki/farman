#ifndef TWOCOLUMNFORM_H
#define TWOCOLUMNFORM_H

#include <QWidget>

namespace Ui {
class TwoColumnForm;
}

class TwoColumnForm : public QWidget
{
    Q_OBJECT

public:
    explicit TwoColumnForm(QWidget *parent = 0);
    ~TwoColumnForm();

private:
    Ui::TwoColumnForm *ui;
};

#endif // TWOCOLUMNFORM_H
