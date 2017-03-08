#ifndef TYPES_H
#define TYPES_H

#include <qdir.h>

namespace Farman
{

enum class ViewMode : int
{
    Single = 0,
    Double = 1,

    ViewModeNum
};

const QDir::Filters DEFAULT_FILTER_FLAGS = QDir::AllEntries | QDir::AccessMask | QDir::AllDirs | QDir::NoDot;
const QDir::SortFlags DEFAULT_SORT_FLAGS = QDir::DirsFirst | QDir::IgnoreCase | QDir::Name;

// 同名ファイルが存在する場合の処理タイプ
enum class OverwriteMethodType : int
{
    Overwrite = 0,
    OverwriteIfNewer = 1,
    Skip = 2,
    Rename = 3,

    Default = OverwriteMethodType::Overwrite,

    ConflictMethodTypeNum
};

}

#endif // TYPES_H
