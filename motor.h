#ifndef MOTOR_H
#define MOTOR_H

#include <QThread>
//#include "motorctrl.h"
class Motor: public QThread
{
    Q_OBJECT
public:
    explicit Motor(QObject *parent = nullptr);
    void run() override; // 重写 run() 方法
    //    MotorCtrl *motorCtrl; // MotorCtrl 的实例
private slots:
    //void handleDataReceived(const QString &data); // 电机
    //public:
    //    Motor();
};

#endif // MOTOR_H
