#include "mainwindow.h"
#include "qtopenglwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    string a1{"pointLights["};
//    string a2 = a1 + to_string(1) +"]";
//    qDebug() << QString::fromStdString(a2+".position");
    //QtOpenglWidget w1;
    MainWindow w;
    w.show();
   // w1.show();
    return a.exec();
}
