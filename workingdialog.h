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
    explicit WorkingDialog(Worker* worker, bool autoClose, QWidget *parent = Q_NULLPTR);
    virtual ~WorkingDialog() Q_DECL_OVERRIDE;

    bool getAutoClose();

public Q_SLOTS:
    virtual int exec() Q_DECL_OVERRIDE;
    virtual void reject() Q_DECL_OVERRIDE;

protected Q_SLOTS:
    virtual void onStart(int min, int max);
    virtual void onProcess(const QString& description);
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
