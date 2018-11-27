#include "mainwindow.h"
#include "settings.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("MASHSOFT");

    QApplication a(argc, argv);

    Farman::Settings::create();

    Farman::MainWindow w;
    w.show();

    return a.exec();
}
