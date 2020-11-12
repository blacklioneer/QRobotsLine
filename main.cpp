#include "mainwindow.h"
#include "qtopenglwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QtOpenglWidget w1;
    MainWindow w;
    w.show();
   // w1.show();
    return a.exec();
}
