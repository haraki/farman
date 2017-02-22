#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class QString;
class QThread;

namespace Farman
{

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
    virtual ~Worker();
    void start();

Q_SIGNALS:
    void finished();
    void error(QString err);

public Q_SLOTS:
    virtual void process() = 0;

protected:
    void emitFinished();

private:
    QThread* m_thread;
};

}           // namespace Farman

#endif // WORKER_H
