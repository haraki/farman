﻿#ifndef WORKER_H
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

        ErrorMask = static_cast<int>(0x80000000),   // エラー

        ErrorUnknown    = ErrorMask | 1,        // 不明なエラー
        ErrorMakeDir    = ErrorMask | 2,        // ディレクトリ作成失敗
        ErrorCopyFile   = ErrorMask | 3,        // ファイルコピー失敗
        ErrorRemoveDir  = ErrorMask | 4,        // ディレクトリ削除失敗
        ErrorRemoveFile = ErrorMask | 5,        // ファイル削除失敗

        ErrorFatal = ErrorMask | 0xFFFF,        // 異常なエラー
    };

    explicit Worker(QObject *parent = 0);
    virtual ~Worker() = default;
    void exec();

    void abort();
    bool isAborted();

    bool isError(int result);

Q_SIGNALS:
    void start(int min, int max);
    void process(const QString& description);
    void progress(int value);
    void outputConsole(const QString& consoleString);
    void finished(int result);
    void error(const QString& err);

public Q_SLOTS:
    virtual void run() = 0;

protected:
    void emitStart(int min, int max);
    void emitProcess(const QString& description);
    void emitProgress(int value);
    void emitOutputConsole(const QString& consoleString);
    void emitFinished(int result);
    void emitError(const QString& err);

private:
    QAtomicInt m_abort;

    QThread* m_thread;

};

}           // namespace Farman

#endif // WORKER_H
