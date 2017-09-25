#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QPoint>
#include <QSize>
#include "types.h"

namespace Farman
{

class Settings : public QSettings
{
    Q_OBJECT

public:
    static void create();
    static Settings* getInstance();

    void flush();

    SizeAtStartup getSizeAtStartupType() { return m_sizeAtStartupType; }
    void setSizeAtStartupType(SizeAtStartup sizeAtStartupType) { m_sizeAtStartupType = sizeAtStartupType; }

    QSize getSizeAtStartup() { return m_sizeAtStartup; }
    void setSizeAtStartup(const QSize& sizeAtStartup) { m_sizeAtStartup = sizeAtStartup; }

    PositionAtStartup getPositionAtStartupType() { return m_positionAtStartupType; }
    void setPositionAtStartupType(PositionAtStartup positionAtStartupType) { m_positionAtStartupType = positionAtStartupType; }

    QPoint getPositionAtStartup() { return m_positionAtStartup; }
    void setPositionAtStartup(const QPoint& positionAtStartup) { m_positionAtStartup = positionAtStartup; }

    ViewMode getViewMode() { return m_viewMode; }
    void setViewMode(ViewMode viewMode) { m_viewMode = viewMode; }

    QDir::SortFlags getLeftSortSettings() { return m_leftSortSettings; }
    void setLeftSortSettings(QDir::SortFlags leftSortSettings) { m_leftSortSettings = leftSortSettings; }

    QDir::SortFlags getRightSortSettings() { return m_rightSortSettings; }
    void setRightSortSettings(QDir::SortFlags rightSortSettings) { m_rightSortSettings = rightSortSettings; }

    QDir::Filters getLeftFilterSettings() { return m_leftFilterSettings; }
    void setLeftFilterSettings(QDir::Filters leftFilterSettings) { m_leftFilterSettings = leftFilterSettings; }

    QDir::Filters getRightFilterSettings() { return m_rightFilterSettings; }
    void setRightFilterSettings(QDir::Filters rightFilterSettings) { m_rightFilterSettings = rightFilterSettings; }

private:
    Settings();

    void initialize();

    QDir::SortFlags getSortSettings(const QString& prefix);
    void setSortSettings(QDir::SortFlags sortSettings, const QString& prefix);

    QDir::Filters getFilterSettings(const QString& prefix);
    void setFilterSettings(QDir::Filters filterSettings, const QString& prefix);

    static Settings* s_instance;

    SizeAtStartup m_sizeAtStartupType = SizeAtStartup::Default;
    QSize m_sizeAtStartup = QSize(0, 0);

    PositionAtStartup m_positionAtStartupType = PositionAtStartup::Default;
    QPoint m_positionAtStartup = QPoint(0, 0);

    ViewMode m_viewMode = ViewMode::Double;

    QDir::SortFlags m_leftSortSettings = DEFAULT_SORT_FLAGS;
    QDir::SortFlags m_rightSortSettings = DEFAULT_SORT_FLAGS;

    QDir::Filters m_leftFilterSettings = DEFAULT_FILTER_FLAGS;
    QDir::Filters m_rightFilterSettings = DEFAULT_FILTER_FLAGS;
};

}

#endif // SETTINGS_H
