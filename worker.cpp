#include <QThread>
#include <QAtomicInt>
#include "worker.h"

namespace Farman
{

Worker::Worker(QObject *parent)
    : QObject(parent)
    , m_abort(0)
    , m_thread(new QThread())
{
//    connect(this, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(m_thread, SIGNAL(started()), this, SLOT(process()));
    connect(this, SIGNAL(finished(int)), m_thread, SLOT(quit()));
    connect(this, SIGNAL(finished(int)), this, SLOT(deleteLater()));
    connect(m_thread, SIGNAL(finished()), m_thread, SLOT(deleteLater()));
}

void Worker::exec()
{
    moveToThread(m_thread);
    m_thread->start();
}

void Worker::abort()
{
    m_abort = 1;
}

bool Worker::isAborted()
{
    return (m_abort != 0);
}

void Worker::emitPrepare(const QString& str)
{
    emit prepare(str);
}

void Worker::emitStart(int min, int max)
{
    emit start(min, max);
}

void Worker::emitProgress(int value)
{
    emit progress(value);
}

void Worker::emitFinished(int result)
{
    emit finished(result);
}

void Worker::emitError(const QString& err)
{
    emit error(err);
}

}           // Farman
