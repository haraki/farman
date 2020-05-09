﻿#ifndef TYPES_H
#define TYPES_H

#include <qdir.h>

namespace Farman
{

// 起動時のウィンドウサイズ
enum class SizeAtStartup : int
{
    Default = 0,
    LastTime = 1,
    Fixed = 2,
};

// 起動時のウィンドウ位置
enum class PositionAtStartup : int
{
    Default = 0,
    LastTime = 1,
    Fixed = 2,
};

// 起動時のフォルダ位置
enum class FolderAtStartup : int
{
    Default = 0,
    LastTime = 1,
    Fixed = 2,
};

// ImageViewer の透過部分背景タイプ
enum class ImageViewerTransparentBGType : int
{
    Solid = 0,
    Checkered = 1,
};

enum class PaneMode : int
{
    Single = 0,
    Dual = 1,

    PaneModeNum,
};

enum class PaneType : int
{
    Left = 0,
    Right = 1,

    PaneTypeNum,
};

// 同名ファイルが存在する場合の処理タイプ
enum class OverwriteMethodType : int
{
    Overwrite = 0,
    OverwriteIfNewer = 1,
    Skip = 2,
    Rename = 3,

    OverwriteMethodTypeNum,
};

// Viewer 種別
enum class ViewerType : int
{
    Auto  = -1,
    Text  = 0,
    Hex   = 1,
    Image = 2,

    ViewerTypeNum,
};

// FolderView にドラッグ&ドロップした際の振る舞い
enum class DragAndDropBehaviorType : int
{
    Select = 0,
    Copy   = 1,
    Move   = 2,

    DragAndDropBehaviorTypeNum,
};

// 設定ダイアログのタブ
enum class PreferencesDialogTabPage : int
{
    General     = 0,
    Appearance  = 1,
    FolderView  = 2,
    Viewer      = 3,
    ExternalApp = 4,

    PreferencesDialogTabPageNum,
};

#ifdef Q_OS_WIN
// ファイルアトリビュートフラグ
enum class WinFileAttrFlag : int
{
    None = 0,

    ReadOnly = (1 << 0),          // 読み取り専用
    Hidden   = (1 << 1),          // 隠し
    Archive  = (1 << 2),          // アーカイブ
    System   = (1 << 3),          // システム
};
Q_DECLARE_FLAGS(WinFileAttrFlags, WinFileAttrFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(WinFileAttrFlags)
#endif

#ifdef Q_OS_WIN
#define LINEFEED_CODE       "\r\n"
#else
#define LINEFEED_CODE       "\n"
#endif

}

#endif // TYPES_H
