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
    explicit FilterDialog(FilterFlags filterFlags, const QStringList& nameMaskFilters, QWidget *parent = Q_NULLPTR);
    ~FilterDialog() Q_DECL_OVERRIDE;

    FilterFlags getFilterFlags();
    QStringList getNameMaskFilters();

private Q_SLOTS:
    void on_nameMaskFilterLineEdit_textChanged(const QString &arg1);

private:
    void accept() Q_DECL_OVERRIDE;
    bool valicationFilterString(const QString &nameMaskFilterString);

    Ui::FilterDialog *ui;
    FilterFlags m_filterFlags;
    QStringList m_nameMaskFilters;
};

}           // namespace Farman

#endif // FILTERDIALOG_H
