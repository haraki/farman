#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

namespace Farman
{

class Settings : public QSettings
{
    Q_OBJECT

public:
    static void create();
    static Settings* getInstance();

private:
    Settings();

    static Settings* s_instance;
};

}

#endif // SETTINGS_H
