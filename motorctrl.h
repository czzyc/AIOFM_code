// MotorCtrl.h
#ifndef MOTORCTRL_H
#define MOTORCTRL_H

#include "global.h"
#include <QString>
#include <QObject>
#include <QApplication>
class MotorCtrl : public QObject {
    Q_OBJECT

public:
    MotorCtrl();
    ~MotorCtrl();
    QString hexToString(const QString &hexString);
    char ConvertHexChar(char ch);
    bool on_send(QString str);
    void StringToHex(QString str, QByteArray &senddata);
    bool openPort(const QString &portName);
    void closePort();
    bool sendData(const QString &data);
    QString manual_serialPortReadyReadFts();
signals:
    void dataReceived(const QString &data); // 新增信号，用于发送接收到的数据

private slots:
    void onReadyRead(); // 新增槽函数，用于处理接收数据

private:
    //    QSerialPort *serialPort;
};

#endif // MOTORCTRL_H
