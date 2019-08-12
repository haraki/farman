#include "mainwindow.h"
#include "bookmarkmanager.h"
#include "settings.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("MASHSOFT");
    QCoreApplication::setApplicationVersion(APP_VERSION);

    int ret = 0;

    QApplication a(argc, argv);

    Farman::BookmarkManager::create();
    Farman::Settings::create();

    Farman::MainWindow w;
    w.show();

    ret = a.exec();

    Farman::Settings::destroy();

    return ret;
}
