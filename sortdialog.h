#ifndef SORTDIALOG_H
#define SORTDIALOG_H

#include <QDialog>
#include <qdir.h>

namespace Ui {
class SortDialog;
}

class SortDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SortDialog(QDir::SortFlags sortFlags, QWidget *parent = 0);
    ~SortDialog();

    QDir::SortFlags getSortFlags();

    void accept() Q_DECL_OVERRIDE;

private:
    Ui::SortDialog *ui;
    QDir::SortFlags m_sortFlags;
};

#endif // SORTDIALOG_H
