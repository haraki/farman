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
    m_viewMode = static_cast<ViewMode>(Settings::getInstance()->value(m_viewModeKey, static_cast<int>(ViewMode::Default)).toInt());
}

void Settings::flush()
{
    Settings::getInstance()->setValue(m_viewModeKey, static_cast<int>(m_viewMode));
}

}
