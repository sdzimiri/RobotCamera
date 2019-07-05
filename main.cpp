#include "mainwindow.h"
#include <windows.h>
#include <QApplication>
#include "SDK\Include\lucamapi.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
