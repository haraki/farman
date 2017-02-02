#ifndef SORTDIALOG_H
#define SORTDIALOG_H

#include <QDialog>
#include <qdir.h>

namespace Ui {
class SortDialog;
}

namespace Farman
{

class SortDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SortDialog(QDir::SortFlags sortFlags, QWidget *parent = 0);
    ~SortDialog();

    QDir::SortFlags getSortFlags();

private:
    void accept() Q_DECL_OVERRIDE;

    Ui::SortDialog *ui;
    QDir::SortFlags m_sortFlags;
};

}           // namespace Farman

#endif // SORTDIALOG_H
