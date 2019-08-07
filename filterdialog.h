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
    explicit FilterDialog(AttrFilterFlags attrFilterFlags,
                          FileFolderFilterType fileFolderFilterType,
                          const QStringList& nameMaskFilters,
                          QWidget *parent = Q_NULLPTR);
    ~FilterDialog() Q_DECL_OVERRIDE;

    AttrFilterFlags getAttrFilterFlags() const;
    FileFolderFilterType getFileFolderFilterType() const;
    QStringList getNameMaskFilters() const;

private Q_SLOTS:
    void on_nameMaskFilterLineEdit_textChanged(const QString &arg1);

private:
    void accept() Q_DECL_OVERRIDE;
    bool valicationFilterString(const QString &nameMaskFilterString);

    Ui::FilterDialog *ui;
    AttrFilterFlags m_attrFilterFlags;
    FileFolderFilterType m_fileFolderFilterType;
    QStringList m_nameMaskFilters;
};

}           // namespace Farman

#endif // FILTERDIALOG_H
