#include <QDebug>
#include <QMessageBox>
#include "workingdialog.h"
#include "ui_workingdialog.h"
#include "worker.h"
#include "workerresult.h"

namespace Farman
{

WorkingDialog::WorkingDialog(Worker* worker, bool autoClose, bool subProgress/*= false */, QWidget *parent/*= Q_NULLPTR*/) :
    QDialog(parent),
    ui(new Ui::WorkingDialog),
    m_worker(worker),
    m_finishedWork(false),
    m_enabledSubProgress(subProgress)
{
    ui->setupUi(this);

    ui->mainProgressBar->setMinimum(0);
    ui->mainProgressBar->setMaximum(1);
    ui->mainProgressBar->setValue(0);
    ui->subProgressBar->setVisible(subProgress);
    ui->subProgressLabel->setVisible(subProgress);
    if(subProgress)
    {
        ui->subProgressBar->setMinimum(0);
        ui->subProgressBar->setMaximum(1);
        ui->subProgressBar->setValue(0);
    }
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

    ui->mainProgressBar->setMinimum(min);
    ui->mainProgressBar->setMaximum(max);
    ui->mainProgressBar->setValue(min);

    ui->mainProgressLabel->setText(tr("%1 / %2").arg(min).arg(max));
}

void WorkingDialog::onProcess(const QString& description)
{
    qDebug() << "WorkingDialog::onProcess(" << description << ");";

    ui->descriptionLabel->setText(description);
}

void WorkingDialog::onProgress(int value)
{
    qDebug() << "WorkingDialog::onProgress(" << value << ");";

    ui->mainProgressBar->setValue(value);

    ui->mainProgressLabel->setText(tr("%1 / %2").arg(value).arg(ui->mainProgressBar->maximum()));
}

void WorkingDialog::onStartSub(int min, int max)
{
    qDebug() << "WorkingDialog::onStartSub(" << min << "," << max << ");";

    ui->subProgressBar->setMinimum(min);
    ui->subProgressBar->setMaximum(max);
    ui->subProgressBar->setValue(min);

    ui->subProgressLabel->setText(tr("%1 / %2").arg(min).arg(max));
}

void WorkingDialog::onProgressSub(int value)
{
//    qDebug() << "WorkingDialog::onProgressSub(" << value << ");";

    ui->subProgressBar->setValue(value);

    ui->subProgressLabel->setText(tr("%1 / %2").arg(value).arg(ui->subProgressBar->maximum()));
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
    if(m_enabledSubProgress)
    {
        connect(m_worker, SIGNAL(startSub(int,int)), this, SLOT(onStartSub(int,int)));
        connect(m_worker, SIGNAL(progressSub(int)), this, SLOT(onProgressSub(int)));
    }

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
