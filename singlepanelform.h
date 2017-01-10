#ifndef SINGLEPANELFORM_H
#define SINGLEPANELFORM_H

#include <QWidget>
#include <qdir.h>

namespace Ui {
class SinglePanelForm;
}

class SinglePanelForm : public QWidget
{
    Q_OBJECT

public:
    explicit SinglePanelForm(QString& path, QDir::Filters filterFlags, QDir::SortFlags sortFlags, QWidget *parent/* = Q_NULLPTR*/);
    ~SinglePanelForm();

private:
    Ui::SinglePanelForm *ui;
};

#endif // SINGLEPANELFORM_H
