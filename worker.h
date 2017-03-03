#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class QString;
class QThread;
class QAtomicInt;

namespace Farman
{

class Worker : public QObject
{
    Q_OBJECT
public:
    enum class Result : int
    {
        Success = 0,                        // 成功
        Abort = 1,                          // 中断

        Error = static_cast<int>(0x80000000),   // エラー

        ErrorUnknown = Error | 1,           // 不明なエラー
        ErrorMakeDir = Error | 2,           // ディレクトリ作成失敗
    };

    explicit Worker(QObject *parent = 0);
    virtual ~Worker();
    void start();

    void abort();
    bool isAborted();

Q_SIGNALS:
    void finished(int result);
    void error(const QString& err);

public Q_SLOTS:
    virtual void process() = 0;

protected:
    void emitFinished(int result);
    void emitError(const QString& err);

private:
    QThread* m_thread;
    QAtomicInt m_abort;
};

}           // namespace Farman

#endif // WORKER_H
