// MotorCtrl.cpp
#include "MotorCtrl.h"
#include "global.h"
#include "qmessagebox.h"
#include "qdebug.h"
MotorCtrl::MotorCtrl() {
    serialPort = new QSerialPort();
    connect(serialPort, &QSerialPort::readyRead, this, &MotorCtrl::onReadyRead); // 连接信号和槽
}

MotorCtrl::~MotorCtrl() {
    closePort();
    delete serialPort;
}

bool MotorCtrl::openPort(const QString &portName) {
    serialPort->setPortName(portName);
    serialPort->setBaudRate(QSerialPort::Baud115200);
    serialPort->setDataBits(QSerialPort::Data8); // 数据位
    serialPort->setParity(QSerialPort::NoParity); // 校验位
    serialPort->setStopBits(QSerialPort::OneStop); // 停止位
    serialPort->setFlowControl(QSerialPort::NoFlowControl); // 流控制
    return serialPort->open(QIODevice::ReadWrite);
}

void MotorCtrl::closePort() {
    if (serialPort->isOpen()) {
        serialPort->close();
    }
}


bool MotorCtrl::on_send(QString str)
{


    QByteArray array;//=data.toLatin1();
    //Hex
    StringToHex(str,array);


    int a =  serialPort->write(array);
    // serialPort->waitForBytesWritten();
    // 发送字节计数并显示
    if(a > 0)
    {
        if (!serialPort->waitForBytesWritten(300)) {
            qDebug()<<QStringLiteral("发送数据超时!");
            return false;
        }

        return 1;

    }else{
        QMessageBox::information(NULL, QStringLiteral("警告"), QStringLiteral("发送FTS数据失败!"), QMessageBox::Yes);
        return -1;
    }
}

QString MotorCtrl::manual_serialPortReadyReadFts()
{
    QByteArray temp;
    while (1)
    {

        if (serialPort->waitForReadyRead(10))
        {
            qApp->processEvents();
            QByteArray info =  serialPort->readAll();
            temp.append(info);

        }else
            break;
    }
    QByteArray hexData = temp.toHex();
    QString string=QString(hexData);
    return string;
}


bool MotorCtrl::sendData(const QString &data) {
    QByteArray byteArray;
    for (QChar ch : data) {
        byteArray.append(ch.unicode());
    }
    return serialPort->write(byteArray) != -1;
}

QString MotorCtrl::hexToString(const QString &hexString) {
    QString result;
    for (int i = 0; i < hexString.length(); i += 2) {
        QString byteString = hexString.mid(i, 2);
        bool ok;
        char byte = static_cast<char>(byteString.toInt(&ok, 16));
        if (ok) {
            result.append(byte);
        }
    }
    return result;
}

void MotorCtrl::onReadyRead() {
    QByteArray receivedData = serialPort->readAll();


    while (serialPort->waitForReadyRead(100)) {
        receivedData += serialPort->readAll();
    }

    // 将接收到的十六进制数据转换为 QString
    QString hexString = receivedData.toHex().toUpper(); // 转换为大写十六进制字符串
    qDebug() << "Pos:" << hexString;

    emit dataReceived(hexString); // 发射信号，传递接收到的数据
}

void MotorCtrl::StringToHex(QString str, QByteArray &senddata)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        hstr = str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}

char MotorCtrl::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}
