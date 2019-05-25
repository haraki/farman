#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <qdir.h>
#include "types.h"

namespace Ui {
class FilterDialog;
}

namespace Farman
{

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(FilterFlags filterFlags, QWidget *parent = Q_NULLPTR);
    ~FilterDialog() Q_DECL_OVERRIDE;

    FilterFlags getFilterFlags();

private:
    void accept() Q_DECL_OVERRIDE;

    Ui::FilterDialog *ui;
    FilterFlags m_filterFlags;
};

}           // namespace Farman

#endif // FILTERDIALOG_H
