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

enum class PaneMode : int
{
    Single = 0,
    Dual = 1,

    PaneModeNum,

    Default = PaneMode::Dual,
};

enum class PaneType : int
{
    Left = 0,
    Right = 1,

    PaneTypeNum,

    Default = PaneType::Left,
};

enum class SectionType : int
{
    Unknown = -1,

    FileName = 0,
    FileType,
    FileSize,
    LastModified,

    SectionTypeNum
};

enum class SortDirsType : int
{
    First = 0,
    Last = 1,
    NoSpecify = 2,

    SortDirsTypeNum
};

enum class ColorRoleType : int
{
    Unknown = -1,

    Normal,
    Normal_Selected,
    Folder,
    Folder_Selected,
    ReadOnly,
    ReadOnly_Selected,
    Hidden,
    Hidden_Selected,
    System,
    System_Selected,

    Background,
    Selected_Background,

    ColorRoleTypeNum
};

const QDir::Filters FIX_FILTER_FLAGS = QDir::AllEntries | QDir::AccessMask | QDir::AllDirs | QDir::NoDot;
const QDir::Filters DEFAULT_FILTER_FLAGS = FIX_FILTER_FLAGS;

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

    Default = DragAndDropBehaviorType::Select,
};

}

#endif // TYPES_H
