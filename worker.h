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
        Skip = 2,                           // スキップ

        Error = static_cast<int>(0x80000000),   // エラー

        ErrorUnknown    = Error | 1,        // 不明なエラー
        ErrorMakeDir    = Error | 2,        // ディレクトリ作成失敗
        ErrorCopyFile   = Error | 3,        // ファイルコピー失敗
        ErrorRemoveDir  = Error | 4,        // ディレクトリ削除失敗
        ErrorRemoveFile = Error | 5,        // ファイル削除失敗

        ErrorFatal = Error | 0xFFFF,        // 異常なエラー
    };

    explicit Worker(QObject *parent = 0);
    virtual ~Worker() = default;
    void start();

    void abort();
    bool isAborted();

Q_SIGNALS:
    void prepare(const QString& str);
    void minMax(int min, int max);
    void progress(int value);
    void finished(int result);
    void error(const QString& err);

public Q_SLOTS:
    virtual void process() = 0;

protected:
    void emitPrepare(const QString& str);
    void emitMinMax(int min, int max);
    void emitProgress(int value);
    void emitFinished(int result);
    void emitError(const QString& err);

private:
    QAtomicInt m_abort;

    QThread* m_thread;

};

}           // namespace Farman

#endif // WORKER_H
