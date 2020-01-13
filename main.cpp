#include "mainwindow.h"
#include "yaml-cpp/yaml.h"
#include <QDir>
#include <fstream>
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    qDebug() << "CWD: " << QDir::currentPath();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
