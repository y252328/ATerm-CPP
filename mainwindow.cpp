#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QFileDialog>
#include <QFile>
#include <string>
#include <fstream>
#include <QKeyEvent>
#include <qDebug>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ser(this)
    , default_setting_str("---\npriority: []\nbaud: {}\ncustom_baud: []\n")
{
    ui->setupUi(this);
    connect(&ser, &QSerialPort::readyRead, this, &MainWindow::ser_data_ready);
    connect(ui->inputLineEdit, &QLineEdit::returnPressed, this, &MainWindow::on_sendBtn_clicked);
    connect(&ser, &QSerialPort::errorOccurred, this, &MainWindow::on_serial_errorOccurred);
    QStringList baud_list = {"9600", "115200"};
    ui->baudComboBox->addItems(baud_list);
    load_setting();
    on_refreshBtn_clicked();
    ui->outputTextBrowser->installEventFilter(this);

}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->outputTextBrowser && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        auto key = keyEvent->key();
        if (ser.isOpen() && (key == Qt::Key_Enter || key == Qt::Key_Return)) {
            on_sendBtn_clicked();
        } else {
            QString txt = ui->inputLineEdit->text();
            if (key == Qt::Key_Backspace) {
                txt = txt.mid(0, txt.size()-1);
            } else if(keyEvent->text() != ""){
                txt = txt + keyEvent->text();
            }
            ui->inputLineEdit->setText(txt);
        }
    }
    return QMainWindow::eventFilter(object, event);
}

MainWindow::~MainWindow()
{
    ser.close();
    delete ui;
}


void MainWindow::on_clearOutputBtn_clicked()
{
    ui->outputTextBrowser->clear();
}

void MainWindow::on_refreshBtn_clicked()
{
    ui->portComboBox->clear();
    QStringList port_list;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QString str =  info.portName() + ": " + info.description();
        port_list << str;
    }
    port_list.sort();
    ui->portComboBox->addItems(port_list);
}

void MainWindow::on_connectBtn_clicked(bool force)
{
    if ( (ui->connectBtn->text() == "Connect") && !force ) {
        auto txt = ui->portComboBox->currentText();
        auto txts = txt.split(":");
        if (txts[0] != "") {
           auto port_name = txts[0];
           auto baud_rate = ui->baudComboBox->currentText().toUInt();

           ser.setPortName(port_name);
           ser.setBaudRate(baud_rate, QSerialPort::AllDirections);
           ser.setDataBits(QSerialPort::Data8);
           ser.setFlowControl(QSerialPort::NoFlowControl);
           ser.setStopBits(QSerialPort::OneStop);
           ser.setParity(QSerialPort::NoParity);
           auto suc = ser.open(QIODevice::ReadWrite);
           if (suc) {
               ui->connectBtn->setText("Disconnect");
               ui->sendBtn->setEnabled(true);
               ui->sendFileBtn->setEnabled(true);
               ui->portComboBox->setEnabled(false);
               ui->baudComboBox->setEnabled(false);
               ui->refreshBtn->setEnabled(false);
           }
        }
    } else {
        if (ser.isOpen())
            ser.close();
        ui->connectBtn->setText("Connect");
        ui->sendBtn->setEnabled(false);
        ui->sendFileBtn->setEnabled(false);
        ui->portComboBox->setEnabled(true);
        ui->baudComboBox->setEnabled(true);
        ui->refreshBtn->setEnabled(true);
    }
}

void MainWindow::append(const QString &msg) {
//    auto text = ui->outputTextBrowser->toPlainText();
    ui->outputTextBrowser->moveCursor (QTextCursor::End);
    ui->outputTextBrowser->insertPlainText (msg);
    ui->outputTextBrowser->moveCursor (QTextCursor::End);

    if (ui->autoScrollCheckBox->isChecked()){
        auto scroll_bar = ui->outputTextBrowser->verticalScrollBar();
    }
}

void MainWindow::ser_data_ready() {
    QString text(ser.readAll());
    append(text);
}

void MainWindow::on_sendBtn_clicked()
{
    if (ser.isOpen()) {
        QString eol;
        if (ui->EOLComboBox->currentText() == "LF")
            eol = "\n";
        if (ui->EOLComboBox->currentText() == "CR;LF")
            eol = "\r\n";
        if (ui->EOLComboBox->currentText() == "CR")
            eol = "\r";

        auto msg = ui->inputLineEdit->text() + eol;
        auto bytes = msg.toLatin1();
        ser.write(bytes);
        ui->inputLineEdit->clear();
    }
}

void MainWindow::on_serial_errorOccurred(QSerialPort::SerialPortError error) {
    if (error == QSerialPort::NoError) return;
    on_connectBtn_clicked(true);
    qDebug() << error;
}

void MainWindow::on_sendFileBtn_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    qDebug() << fileName;
    if (fileName != ""){
        QFile file(fileName);
        if (file.open(QFile::ReadOnly)) {
            QByteArray bytes = file.readAll();
            ser.write(bytes);
        }
    }
}

void MainWindow::load_setting() {
    auto default_setting = YAML::Load(default_setting_str);
    std::fstream file("../setting.yaml", std::fstream::in);
    setting = YAML::Load(file);
    file.close();
    for ( const auto & pair : default_setting) {
        const auto & key = pair.first.as<std::string>();
        if (!setting[key].IsDefined()){
            setting[key] = pair.second;
        }
    }
}

void MainWindow::save_setting() {
    std::fstream fout("../setting.yaml", std::fstream::out);
    fout << setting;
    fout.close();
}
