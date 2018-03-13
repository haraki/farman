#ifndef TYPES_H
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

// ImageViewer の背景タイプ
enum class ImageViewerBGType : int
{
    Solid = 0,
    Checkered = 1,
};

enum class ViewMode : int
{
    Single = 0,
    Double = 1,

    ViewModeNum,

    Default = ViewMode::Double,
};

const QDir::Filters FIX_FILTER_FLAGS = QDir::AllEntries | QDir::AccessMask | QDir::AllDirs | QDir::NoDot;
const QDir::Filters DEFAULT_FILTER_FLAGS = FIX_FILTER_FLAGS;
const QDir::SortFlags FIX_SORT_FLAGS = static_cast<QDir::SortFlags>(0);
const QDir::SortFlags DEFAULT_SORT_FLAGS = FIX_SORT_FLAGS | QDir::DirsFirst | QDir::IgnoreCase | QDir::Name;

const int DEFAULT_CURSOR_WIDTH = 1;

// 同名ファイルが存在する場合の処理タイプ
enum class OverwriteMethodType : int
{
    Overwrite = 0,
    OverwriteIfNewer = 1,
    Skip = 2,
    Rename = 3,

    OverwriteMethodTypeNum,

    Default = OverwriteMethodType::Overwrite,
};

// Viewer 種別
enum class ViewerType : int
{
    Text = 0,
    Binary = 1,
    Image = 2,

    ViewerTypeNum,
};

}

#endif // TYPES_H
