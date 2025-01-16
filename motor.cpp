#include "motor.h"
#include <QTimer>
#include <QElapsedTimer>
#include "QDebug"
#include "global.h"
#include "globalmotor.h"
Motor::Motor(QObject *parent) : QThread(parent) {


}

//void Motor::handleDataReceived(const QString &data) //电机接收数据
//{
//    // 处理接收到的数据
//    qDebug() << "Received data:" << data;
////    QByteArray temp;
////    while (1)
////    {

////        if (serialPort->waitForReadyRead(10))
////        {
////            qApp->processEvents();
////            QByteArray info =  serialPort->readAll();
////            temp.append(info);

////        }else
////            break;
////    }
////    QByteArray hexData = temp.toHex();
////    QString string=QString(hexData);
////    if(data=="")
////    {
////        isCancatch=true;
////    }
//    // 这里可以更新 UI 或执行其他操作
//}

void Motor::run() {
    QString data="AA 02 00 FF";
    if(1){
        while(StartCatch)
        {
            if(!isCancatch/*1*/){
                bool recv=motorCtrl->on_send(data);
                isCancatch=true;
                if(recv)
                {
                    QThread::msleep(1500);  // 休眠100毫秒
                }
            }
        }
    }

}

