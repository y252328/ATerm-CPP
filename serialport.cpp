#include "serialport.h"
#include <qDebug>

SerialPort::SerialPort()
{
    this->setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);
    this->setDataBits(QSerialPort::Data8);
    this->setFlowControl(QSerialPort::NoFlowControl);
    this->setStopBits(QSerialPort::OneStop);
    this->setParity(QSerialPort::NoParity);
    connect(this,SIGNAL(readyRead()),this,SLOT(slot_DataRec()));
}
