#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <qdir.h>

namespace Ui {
class FilterDialog;
}

namespace Farman
{

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(QDir::Filters filterFlags, QWidget *parent = 0);
    ~FilterDialog();

    QDir::Filters getFilterFlags();

private:
    Ui::FilterDialog *ui;

    QDir::Filters m_filterFlags;
};

}           // namespace Farman

#endif // FILTERDIALOG_H
