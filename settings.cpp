#include "settings.h"

namespace Farman
{

Settings* Settings::s_instance = Q_NULLPTR;

void Settings::create()
{
    s_instance = new Settings();

    s_instance->initialize();
}

Settings* Settings::getInstance()
{
    return s_instance;
}

Settings::Settings()
    : QSettings("settings.ini", QSettings::Format::IniFormat)
{
    this->setIniCodec("UTF-8");
}

void Settings::initialize()
{
    // ViewMode
    QString viewModeValue = Settings::getInstance()->value("main/viewMode", "double").toString();
    m_viewMode = (viewModeValue == "single") ? ViewMode::Single : ViewMode::Double;

    // Left side Sort settings
    m_leftSortSettings = getSortSettings("left");

    // Right side Sort settings
    m_rightSortSettings = getSortSettings("right");

    // Left side Filter settings
    m_leftFilterSettings = getFilterSettings("left");

    // Right side Filter settings
    m_rightFilterSettings = getFilterSettings("right");
}

void Settings::flush()
{
    // ViewMode
    QString viewModeValue = (m_viewMode == ViewMode::Single) ? "single"
                                                             : "double";
    Settings::getInstance()->setValue("main/viewMode", viewModeValue);

    // Left side Sort settings
    setSortSettings(m_leftSortSettings, "left");

    // Right side Sort settings
    setSortSettings(m_rightSortSettings, "right");

    // Left side Filter settings
    setFilterSettings(m_leftFilterSettings, "left");

    // Right side Filter settings
    setFilterSettings(m_rightFilterSettings, "right");
}

QDir::SortFlags Settings::getSortSettings(const QString& prefix)
{
    QDir::SortFlags ret = FIX_SORT_FLAGS;

    QString sortTypeValue = Settings::getInstance()->value("main/" + prefix + "SortType", "name").toString();
    ret |= (sortTypeValue == "lastModified") ? QDir::SortFlag::Time :
           (sortTypeValue == "size") ? QDir::SortFlag::Size :
           (sortTypeValue == "type") ? QDir::SortFlag::Type :
                                       QDir::SortFlag::Name;

    QString sortOrderValue = Settings::getInstance()->value("main/" + prefix + "SortOrder", "asc").toString();
    ret |= (sortOrderValue == "desc") ? QDir::SortFlag::Reversed :
                                        static_cast<QDir::SortFlags>(0);

    QString sortDirsValue = Settings::getInstance()->value("main/" + prefix + "SortDirs", "first").toString();
    ret |= (sortDirsValue == "first") ? QDir::SortFlag::DirsFirst :
           (sortDirsValue == "last") ? QDir::SortFlag::DirsLast :
                                       static_cast<QDir::SortFlags>(0);

    QString sortCaseValue = Settings::getInstance()->value("main/" + prefix + "SortCase", "sensitive").toString();
    ret |= (sortCaseValue == "insensitive") ? QDir::SortFlag::IgnoreCase :
                                              static_cast<QDir::SortFlags>(0);

    return ret;
}

void Settings::setSortSettings(QDir::SortFlags sortSettings, const QString& prefix)
{
    QString sortTypeValue = (sortSettings & QDir::SortFlag::Type) ? "type" :
                            ((sortSettings & QDir::SortFlag::SortByMask) == QDir::SortFlag::Time) ? "lastModified" :
                            ((sortSettings & QDir::SortFlag::SortByMask) == QDir::SortFlag::Size) ? "size" :
                                                                                                    "name";
    Settings::getInstance()->setValue("main/" + prefix + "SortType", sortTypeValue);

    QString sortOrderValue = (sortSettings & QDir::SortFlag::Reversed) ? "desc" :
                                                                         "asc";
    Settings::getInstance()->setValue("main/" + prefix + "SortOrder", sortOrderValue);

    QString sortDirsValue = (sortSettings & QDir::SortFlag::DirsFirst) ? "first" :
                            (sortSettings & QDir::SortFlag::DirsLast) ? "last" :
                                                                        "none";
    Settings::getInstance()->setValue("main/" + prefix + "SortDirs", sortDirsValue);

    QString sortCaseValue = (sortSettings & QDir::SortFlag::IgnoreCase) ? "insensitive" :
                                                                          "sensitive";
    Settings::getInstance()->setValue("main/" + prefix + "SortCase", sortCaseValue);
}

QDir::Filters Settings::getFilterSettings(const QString& prefix)
{
    QDir::Filters ret = FIX_FILTER_FLAGS;

    QString filterHiddenValue = Settings::getInstance()->value("main/" + prefix + "FilterHidden", "false").toString();
    ret |= (filterHiddenValue == "true") ? QDir::Filter::Hidden :
                                           static_cast<QDir::Filters>(0);

    QString filterSystemValue = Settings::getInstance()->value("main/" + prefix + "FilterSystem", "false").toString();
    ret |= (filterSystemValue == "true") ? QDir::Filter::System :
                                           static_cast<QDir::Filters>(0);

    return ret;
}

void Settings::setFilterSettings(QDir::Filters filterSettings, const QString& prefix)
{
    QString filterHiddenValue = (filterSettings & QDir::Filter::Hidden) ? "true" :
                                                                          "false";
    Settings::getInstance()->setValue("main/" + prefix + "FilterHidden", filterHiddenValue);

    QString filterSystemValue = (filterSettings & QDir::Filter::System) ? "true" :
                                                                          "false";
    Settings::getInstance()->setValue("main/" + prefix + "FilterSystem", filterSystemValue);
}

}
