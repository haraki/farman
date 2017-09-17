#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
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

    ViewMode getViewMode() { return m_viewMode; }
    void setViewMode(ViewMode viewMode) { m_viewMode = viewMode; }

private:
    Settings();

    void initialize();

    static Settings* s_instance;

    ViewMode m_viewMode = ViewMode::Double;
    const QString m_viewModeKey = "main/viewMode";

};

}

#endif // SETTINGS_H
