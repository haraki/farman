#ifndef WORKINGDIALOG_H
#define WORKINGDIALOG_H

#include <QDialog>

namespace Ui {
class WorkingDialog;
}

namespace Farman
{

class Worker;

class WorkingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WorkingDialog(Worker* worker, QWidget *parent = 0);
    virtual ~WorkingDialog();

public Q_SLOTS:
    virtual int exec() Q_DECL_OVERRIDE;

protected Q_SLOTS:
    virtual void onPrepare(const QString& str);
    virtual void onStart(int min, int max);
    virtual void onProgress(int value);
    virtual void onFinished(int result);
    virtual void onError(const QString& err);

private slots:
    void on_closePushButton_clicked();

private:
    Ui::WorkingDialog *ui;
    Worker* m_worker;

    bool m_finishedWork;
};

}           // namespace Farman

#endif // WORKINGDIALOG_H
