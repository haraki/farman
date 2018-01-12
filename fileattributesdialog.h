#ifndef FILEATTRIBUTESDIALOG_H
#define FILEATTRIBUTESDIALOG_H

#include <QDialog>

namespace Ui {
class FileAttributesDialog;
}

class FileAttributesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileAttributesDialog(QWidget *parent = 0);
    ~FileAttributesDialog();

private:
    Ui::FileAttributesDialog *ui;
};

#endif // FILEATTRIBUTESDIALOG_H
