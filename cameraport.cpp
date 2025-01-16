#include "cameraport.h"
#include "ui_cameraport.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QSettings>
#include "qdebug.h"
CameraPort::CameraPort(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraPort)
{
    ui->setupUi(this);
    ui->cb_port->clear();

           // 获取可用的串口信息
           foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
               // 添加串口名称到 QComboBox
               ui->cb_port->addItem(info.portName());
           }
}

CameraPort::~CameraPort()
{
    delete ui;
}

void CameraPort::on_pushButton_clicked()
{
    QSettings settings(QCoreApplication::applicationDirPath()+"/SF6Config.ini",QSettings::IniFormat);
    QString port =  settings.value("Camera/Port", "").toString();
    if(port==""){
        port="COM2";
    }

    QSerialPort* serialPort = new QSerialPort(port, this);
    int baudi=QSerialPort::Baud9600;
    // 设置串口参数
    serialPort->setBaudRate(baudi);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    // 打开串口
    if (serialPort->open(QIODevice::ReadWrite)) {
    } else {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("打开红外相机串口失败:") + serialPort->errorString());
    }

}

