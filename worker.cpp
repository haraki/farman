#include <QThread>
#include <QAtomicInt>
#include "worker.h"
#include "workerresult.h"

namespace Farman
{

Worker::Worker(QObject *parent)
    : QObject(parent)
    , m_timer()
    , m_abort(0)
    , m_thread(new QThread())
{
//    connect(this, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(m_thread, SIGNAL(started()), this, SLOT(run()));
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

bool Worker::isError(int result)
{
    return (static_cast<int>(result) & static_cast<int>(WorkerResult::ErrorMask));
}

void Worker::emitStart(int min, int max)
{
    emit start(min, max);
}

void Worker::emitProcess(const QString& description)
{
    emit process(description);
}

void Worker::emitProgress(int value)
{
    emit progress(value);
}

void Worker::emitOutputConsole(const QString& consoleString)
{
    emit outputConsole(consoleString);
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
