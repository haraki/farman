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
                        SortDirsType dirsType,
                        bool dotFirst,
                        Qt::CaseSensitivity caseSensitivity,
                        Qt::SortOrder order,
                        QWidget *parent = Q_NULLPTR);
    ~SortDialog() Q_DECL_OVERRIDE;

    SectionType getSortSectionType();
    SortDirsType getSortDirsType();
    bool getSortDotFirst();
    Qt::CaseSensitivity getSortCaseSensitivity();
    Qt::SortOrder getSortOrder();

private:
    void accept() Q_DECL_OVERRIDE;

    Ui::SortDialog *ui;

    SectionType m_sortSectionType;
    SortDirsType m_sortDirsType;
    bool m_sortDotFirst;
    Qt::CaseSensitivity m_sortCaseSensitivity;
    Qt::SortOrder m_sortOrder;
};

}           // namespace Farman

#endif // SORTDIALOG_H
