#include "mainwindow.h"
#include "settings.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Farman::Settings::create();
    Farman::MainWindow::create();

    return a.exec();
}
