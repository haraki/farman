#include <QDebug>
#include <QMessageBox>
#include "workingdialog.h"
#include "ui_workingdialog.h"
#include "worker.h"
#include "workerresult.h"

namespace Farman
{

static constexpr qint64 subProgressBarMax = 1000;

WorkingDialog::WorkingDialog(Worker* worker,
                             bool autoClose,
                             bool subProgress/* = false*/,
                             QWidget *parent/* = Q_NULLPTR*/) :
    QDialog(parent),
    ui(new Ui::WorkingDialog),
    m_worker(worker),
    m_finishedWork(false),
    m_enabledSubProgress(subProgress),
    m_mainProgressLabelFormat("%1 / %2"),
    m_subProgressLabelFormat("%1 / %2")
{
    ui->setupUi(this);

    ui->mainProgressBar->setMinimum(0);
    ui->mainProgressBar->setMaximum(1);
    ui->mainProgressBar->setValue(0);
    ui->mainProgressLabel->setText("");
    ui->subProgressBar->setVisible(subProgress);
    ui->subProgressLabel->setVisible(subProgress);
    if(subProgress)
    {
        ui->subProgressBar->setMinimum(0);
        ui->subProgressBar->setMaximum(1);
        ui->subProgressBar->setValue(0);
        ui->subProgressLabel->setText("");
    }
    ui->closePushButton->setText(tr("Cancel"));
    ui->autoCloseCheckBox->setChecked(autoClose);
}

WorkingDialog::~WorkingDialog()
{
    delete ui;
}

void WorkingDialog::setMainProgressLabelFormat(const QString& formatString)
{
    m_mainProgressLabelFormat = formatString;
}

void WorkingDialog::setSubProgressLabelFormat(const QString& formatString)
{
    m_subProgressLabelFormat = formatString;
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

    ui->mainProgressLabel->setText(m_mainProgressLabelFormat.arg(min).arg(max));
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

    ui->mainProgressLabel->setText(m_mainProgressLabelFormat.arg(value).arg(ui->mainProgressBar->maximum()));
}

void WorkingDialog::onStartSub(qint64 min, qint64 max)
{
    qDebug() << "WorkingDialog::onStartSub(" << min << "," << max << ");";

    m_subMax = max;

    int subProgressBarMin = static_cast<int>(min * subProgressBarMax / max);

    ui->subProgressBar->setMinimum(subProgressBarMin);
    ui->subProgressBar->setMaximum(subProgressBarMax);
    ui->subProgressBar->setValue(subProgressBarMin);

    ui->subProgressLabel->setText(m_subProgressLabelFormat.arg(min).arg(max));
}

void WorkingDialog::onProgressSub(qint64 value)
{
//    qDebug() << "WorkingDialog::onProgressSub(" << value << ");";

    int subProgressBarValue = static_cast<int>(value * subProgressBarMax / m_subMax);

    ui->subProgressBar->setValue(subProgressBarValue);

    ui->subProgressLabel->setText(m_subProgressLabelFormat.arg(value).arg(m_subMax));
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
        connect(m_worker, SIGNAL(startSub(qint64,qint64)), this, SLOT(onStartSub(qint64,qint64)));
        connect(m_worker, SIGNAL(progressSub(qint64)), this, SLOT(onProgressSub(qint64)));
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
