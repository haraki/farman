#include <QDebug>
#include "workingdialog.h"
#include "ui_workingdialog.h"
#include "worker.h"

namespace Farman
{

WorkingDialog::WorkingDialog(Worker* worker, QWidget *parent/*= Q_NULLPTR*/) :
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
}

WorkingDialog::~WorkingDialog()
{
    delete ui;
}

void WorkingDialog::onPrepare(const QString& str)
{
    qDebug() << "WorkingDialog::onPrepare(" << str << ");";

    ui->descriptionLabel->setText(str);
}

void WorkingDialog::onMinMax(int min, int max)
{
    qDebug() << "WorkingDialog::onMinMax(" << min << "," << max << ");";

    ui->progressBar->setMinimum(min);
    ui->progressBar->setMaximum(max);

    ui->progressLabel->setText(tr("( 0 / %1 )").arg(max));
    ui->progressLabel->setVisible(true);
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

    if(ui->autoCloseCheckBox->checkState() == Qt::Checked)
    {
        QDialog::accept();
    }

    m_finishedWork = true;
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

    connect(m_worker, SIGNAL(prepare(QString)), this, SLOT(onPrepare(QString)));
    connect(m_worker, SIGNAL(minMax(int,int)), this, SLOT(onMinMax(int,int)));
    connect(m_worker, SIGNAL(progress(int)), this, SLOT(onProgress(int)));
    connect(m_worker, SIGNAL(finished(int)), this, SLOT(onFinished(int)));
    connect(m_worker, SIGNAL(error(QString)), this, SLOT(onError(QString)));
//    connect(m_worker, SIGNAL(information(QString)), ui->label, SLOT(setText(QString)));

    m_worker->start();
    return QDialog::exec();
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
