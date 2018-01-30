#ifndef WORKERRESULT_H
#define WORKERRESULT_H

namespace Farman
{

enum class WorkerResult : int
{
    Success = 0,                        // 成功
    Abort = 1,                          // 中断
    Skip = 2,                           // スキップ

    ErrorMask = static_cast<int>(0x80000000),   // エラー

    ErrorUnknown    = ErrorMask | 0x01,     // 不明なエラー
    ErrorMakeDir    = ErrorMask | 0x02,     // ディレクトリ作成失敗
    ErrorCopyFile   = ErrorMask | 0x03,     // ファイルコピー失敗
    ErrorRemoveDir  = ErrorMask | 0x04,     // ディレクトリ削除失敗
    ErrorRemoveFile = ErrorMask | 0x05,     // ファイル削除失敗

    ErrorFatal = ErrorMask | 0xFFFF,        // 異常なエラー
};

}           // namespace Farman

#endif // WORKERRESULT_H
