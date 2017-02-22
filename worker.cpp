#include <QThread>
#include "worker.h"

namespace Farman
{

Worker::Worker(QObject *parent)
    : QObject(parent)
    , m_thread(new QThread())
{
//    connect(this, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(m_thread, SIGNAL(started()), this, SLOT(process()));
    connect(this, SIGNAL(finished(int)), m_thread, SLOT(quit()));
    connect(this, SIGNAL(finished(int)), this, SLOT(deleteLater()));
    connect(m_thread, SIGNAL(finished()), m_thread, SLOT(deleteLater()));
}

Worker::~Worker()
{
    delete m_thread;
}

void Worker::start()
{
    moveToThread(m_thread);
    m_thread->start();
}

void Worker::emitFinished()
{
    emit finished();
}

}           // Farman
