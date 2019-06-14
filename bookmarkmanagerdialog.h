#ifndef BOOKMARKMANAGERDIALOG_H
#define BOOKMARKMANAGERDIALOG_H

#include <QDialog>

namespace Ui {
class BookmarkManagerDialog;
}

namespace Farman
{

class BookmarkManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookmarkManagerDialog(QWidget *parent = nullptr);
    ~BookmarkManagerDialog();

private:
    Ui::BookmarkManagerDialog *ui;
};

}           // namespace Farman

#endif // BOOKMARKMANAGERDIALOG_H
