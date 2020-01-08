#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

class SerialPort : public QSerialPort
{
    Q_OBJECT
public:
    explicit SerialPort();

signals:

};

#endif // SERIALPORT_H
