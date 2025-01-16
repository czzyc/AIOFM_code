#ifndef GLOBAL_H
#define GLOBAL_H

#include <QMutex>
#include <QString>
#include <QImage>
#include <QSerialPort>
#include <QSerialPortInfo>
#define NOMINMAX
#include <windows.h>
#include "opencv2/opencv.hpp"

#include <iostream>
using namespace std;
/*using namespace cv;

extern cv::Mat Showmat;*/
extern  bool videoThread;
extern  QString RevState;
extern uint16_t *Revdata;
extern short *shortRevdata;
extern int atestcolor;
extern bool iscandata;
extern QImage g_image;
extern bool issavedata;
extern QSerialPort *serialPort;
extern bool isCancatch;
extern bool StartCatch;
extern bool isShow;
//缓存容器
extern cv::Mat m_frame;
extern vector<cv::Mat> m_vec_frame;
//互斥锁
extern QMutex m_mutex;
extern bool isopenCamera;
#define _BASE double
class Global
{
public:
    Global();
};

#endif // GLOBAL_H
