#include "mainwindow.h"

#include <QApplication>

#include "light_sensor.h"
#include "temperature_humidity_sensor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.init();
    w.show();

    return a.exec();
}
