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
    QString viewModeValue = Settings::getInstance()->value("main/viewMode", "double").toString();
    m_viewMode = (viewModeValue == "single") ? ViewMode::Single
                                             : ViewMode::Double;
}

void Settings::flush()
{
    QString viewModeValue = (m_viewMode == ViewMode::Single) ? "single"
                                                             : "double";
    Settings::getInstance()->setValue("main/viewMode", viewModeValue);
}

}
