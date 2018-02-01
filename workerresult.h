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

    ErrorUnknown    = ErrorMask | 0x0001,   // 不明なエラー

    // ファイル操作系
    ErrorMakeDir    = ErrorMask | 0x0101,   // ディレクトリ作成失敗
    ErrorCopyFile   = ErrorMask | 0x0102,   // ファイルコピー失敗
    ErrorRemoveDir  = ErrorMask | 0x0103,   // ディレクトリ削除失敗
    ErrorRemoveFile = ErrorMask | 0x0104,   // ファイル削除失敗

    // ファイル読み込み系
    ErrorOpenFile   = ErrorMask | 0x0201,   // ファイルオープン失敗
    ErrorReadFile   = ErrorMask | 0x0202,   // ファイル読み込み失敗

    ErrorFatal = ErrorMask | 0xFFFF,        // 異常なエラー
};

}           // namespace Farman

#endif // WORKERRESULT_H
