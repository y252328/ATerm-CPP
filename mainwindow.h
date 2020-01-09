#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QMainWindow>
#include <string>
#include "yaml-cpp/yaml.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_clearOutputBtn_clicked();

    void on_refreshBtn_clicked();

    void on_connectBtn_clicked(bool force=false);

    void ser_data_ready();

    void on_sendBtn_clicked();

    void on_serial_errorOccurred(QSerialPort::SerialPortError error);

    void on_sendFileBtn_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort ser;
    YAML::Node setting;
    const std::string default_setting_str;
    void load_setting();
    void save_setting();
    void append(const QString &);

};
#endif // MAINWINDOW_H
