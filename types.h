#ifndef TYPES_H
#define TYPES_H

namespace Farman
{

enum class ViewMode : int
{
    Single = 0,
    Double = 1,

    ViewModeNum
};

constexpr QDir::Filters DEFAULT_FILTER_FLAGS = QDir::AllEntries | QDir::NoSymLinks | QDir::AccessMask | QDir::NoDot;
constexpr QDir::SortFlags DEFAULT_SORT_FLAGS = QDir::DirsFirst | QDir::IgnoreCase | QDir::Name;

}

#endif // TYPES_H
