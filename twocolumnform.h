#ifndef TWOCOLUMNFORM_H
#define TWOCOLUMNFORM_H

#include <QWidget>
#include <qdir.h>

namespace Ui {
class TwoColumnForm;
}
class QString;

class TwoColumnForm : public QWidget
{
    Q_OBJECT

public:
    explicit TwoColumnForm(QString& path, QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent = 0);
    ~TwoColumnForm();

private:
    Ui::TwoColumnForm *ui;
};

#endif // TWOCOLUMNFORM_H
