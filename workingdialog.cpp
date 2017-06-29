#include <QDebug>
#include "workingdialog.h"
#include "ui_workingdialog.h"
#include "worker.h"

namespace Farman
{

WorkingDialog::WorkingDialog(Worker* worker, QWidget *parent/*= Q_NULLPTR*/) :
    QDialog(parent),
    ui(new Ui::WorkingDialog),
    m_worker(worker)
{
    ui->setupUi(this);
}

WorkingDialog::~WorkingDialog()
{
    delete ui;
}


void WorkingDialog::onMinMax(int min, int max)
{
    qDebug() << "WorkingDialog::onMinMax(" << min << "," << max << ");";

    ui->progressBar->setMinimum(min);
    ui->progressBar->setMaximum(max);
}

void WorkingDialog::onProgress(int value)
{
    qDebug() << "WorkingDialog::onProgress(" << value << ");";

    ui->progressBar->setValue(value);
}

void WorkingDialog::onFinished(int result)
{
    qDebug() << "WorkingDialog::onFinished(" << result << ");";

    QDialog::accept();
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

    connect(m_worker, SIGNAL(minMax(int,int)), this, SLOT(onMinMax(int,int)));
    connect(m_worker, SIGNAL(progress(int)), this, SLOT(onProgress(int)));
    connect(m_worker, SIGNAL(finished(int)), this, SLOT(onFinished(int)));
    connect(m_worker, SIGNAL(error(QString)), this, SLOT(onError(QString)));
//    connect(m_worker, SIGNAL(information(QString)), ui->label, SLOT(setText(QString)));

    m_worker->start();
    return QDialog::exec();
}

void WorkingDialog::on_cancelPushButton_clicked()
{
    if(m_worker != Q_NULLPTR)
    {
        m_worker->abort();
    }
}

}           // namespace Farman
