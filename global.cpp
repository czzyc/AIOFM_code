#include "global.h"
QString RevState;
uint16_t *Revdata;
short *shortRevdata;
//cv::Mat Showmat;
int atestcolor=0;
bool iscandata=true;
QImage g_image;
bool issavedata=false;
QSerialPort *serialPort;
bool isCancatch=false;
bool StartCatch=false;
bool isShow=false;
vector<cv::Mat> m_vec_frame;
cv::Mat m_frame;
bool videoThread=false;
//互斥锁
QMutex m_mutex;


bool isopenCamera=false;
Global::Global()
{

}
