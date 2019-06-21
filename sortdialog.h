#ifndef SORTDIALOG_H
#define SORTDIALOG_H

#include <QDialog>
#include <QDir>
#include "types.h"

namespace Ui {
class SortDialog;
}

namespace Farman
{

class SortDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SortDialog(SectionType sectionType,
                        SectionType sectionType2nd,
                        SortDirsType dirsType,
                        bool dotFirst,
                        Qt::CaseSensitivity caseSensitivity,
                        Qt::SortOrder order,
                        QWidget *parent = Q_NULLPTR);
    ~SortDialog() Q_DECL_OVERRIDE;

    SectionType getSortSectionType();
    SectionType getSortSectionType2nd();
    SortDirsType getSortDirsType();
    bool getSortDotFirst();
    Qt::CaseSensitivity getSortCaseSensitivity();
    Qt::SortOrder getSortOrder();

private slots:
    void on_sortNameRadioButton_clicked();
    void on_sortTypeRadioButton_clicked();
    void on_sortSizeRadioButton_clicked();
    void on_sortLastModifiedRadioButton_clicked();

private:
    void accept() Q_DECL_OVERRIDE;
    void setSort2ndEnabled(bool name, bool type, bool size, bool lastModified);

    Ui::SortDialog *ui;

    SectionType m_sortSectionType;
    SectionType m_sortSectionType2nd;
    SortDirsType m_sortDirsType;
    bool m_sortDotFirst;
    Qt::CaseSensitivity m_sortCaseSensitivity;
    Qt::SortOrder m_sortOrder;
};

}           // namespace Farman

#endif // SORTDIALOG_H
