#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setStyle("fusion");
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Crack jopy");
    w.show();
    return a.exec();
}
