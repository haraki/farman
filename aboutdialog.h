#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

namespace Farman
{

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog();

private Q_SLOTS:
    void on_aboutQtPushButton_clicked();
    void on_licensePushButton_clicked();

private:
    Ui::AboutDialog *ui;
};

}           // namespace Farman

#endif // ABOUTDIALOG_H
