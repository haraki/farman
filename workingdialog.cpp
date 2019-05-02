#include <QDebug>
#include <QMessageBox>
#include "workingdialog.h"
#include "ui_workingdialog.h"
#include "worker.h"
#include "workerresult.h"

namespace Farman
{

WorkingDialog::WorkingDialog(Worker* worker, bool autoClose, QWidget *parent/*= Q_NULLPTR*/) :
    QDialog(parent),
    ui(new Ui::WorkingDialog),
    m_worker(worker),
    m_finishedWork(false)
{
    ui->setupUi(this);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(1);
    ui->progressBar->setValue(0);
    ui->progressLabel->setVisible(false);
    ui->closePushButton->setText(tr("Cancel"));
    ui->autoCloseCheckBox->setChecked(autoClose);
}

WorkingDialog::~WorkingDialog()
{
    delete ui;
}

bool WorkingDialog::getAutoClose()
{
    return ui->autoCloseCheckBox->isChecked();
}

void WorkingDialog::onStart(int min, int max)
{
    qDebug() << "WorkingDialog::onStart(" << min << "," << max << ");";

    ui->progressBar->setMinimum(min);
    ui->progressBar->setMaximum(max);
    ui->progressBar->setValue(min);

    ui->progressLabel->setText(tr("( %1 / %2 )").arg(min).arg(max));
    ui->progressLabel->setVisible(true);
}

void WorkingDialog::onProcess(const QString& description)
{
    qDebug() << "WorkingDialog::onProcess(" << description << ");";

    ui->descriptionLabel->setText(description);
}

void WorkingDialog::onProgress(int value)
{
    qDebug() << "WorkingDialog::onProgress(" << value << ");";

    ui->progressBar->setValue(value);

    ui->progressLabel->setText(tr("( %1 / %2 )").arg(value).arg(ui->progressBar->maximum()));
}

void WorkingDialog::onFinished(int result)
{
    qDebug() << "WorkingDialog::onFinished(" << result << ");";

    ui->closePushButton->setText(tr("Close"));

    m_finishedWork = true;

    if(result == static_cast<int>(WorkerResult::Success) && ui->autoCloseCheckBox->isChecked())
    {
        QDialog::accept();
    }
    else if(result == static_cast<int>(WorkerResult::Abort))
    {
        QMessageBox::warning(this->parentWidget(),
                             tr("Aborted"),
                             tr("Aborted by user."));

        QDialog::reject();
    }
}

void WorkingDialog::onError(const QString& err)
{
    qDebug() << "WorkingDialog::onError(" << err << ");";


}

int WorkingDialog::exec()
{
    if(m_worker == Q_NULLPTR)
    {
        return -1;
    }

    connect(m_worker, SIGNAL(start(int,int)), this, SLOT(onStart(int,int)));
    connect(m_worker, SIGNAL(process(QString)), this, SLOT(onProcess(QString)));
    connect(m_worker, SIGNAL(progress(int)), this, SLOT(onProgress(int)));
    connect(m_worker, SIGNAL(finished(int)), this, SLOT(onFinished(int)));
    connect(m_worker, SIGNAL(error(QString)), this, SLOT(onError(QString)));
//    connect(m_worker, SIGNAL(information(QString)), ui->label, SLOT(setText(QString)));

    m_worker->exec();
    return QDialog::exec();
}

void WorkingDialog::reject()
{
    if(m_finishedWork)
    {
        QDialog::reject();
    }
    else if(m_worker != Q_NULLPTR)
    {
        m_worker->abort();
    }
    else
    {
        QDialog::reject();
    }
}

void WorkingDialog::on_closePushButton_clicked()
{
    if(m_finishedWork)
    {
        QDialog::accept();
    }
    else if(m_worker != Q_NULLPTR)
    {
        m_worker->abort();
    }
    else
    {
        QDialog::accept();
    }
}

}           // namespace Farman
