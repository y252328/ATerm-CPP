#include "mainwindow.h"
#include "yaml-cpp/yaml.h"
#include <QDir>
#include <fstream>
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    qDebug() << "CWD: " << QDir::currentPath();
//    std::fstream file("../setting.yaml", std::fstream::in);
//    YAML::Node setting = YAML::Load(file);
//    file.close();
//    auto b = setting["123"];
//    for (std::size_t i=0;i<primes["custom_baud"].size();i++) {
//      qDebug() << primes["custom_baud"][i].as<int>() << "\n";
//    }
//    return 0;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
