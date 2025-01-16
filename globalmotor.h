#ifndef GLOBALMOTOR_H
#define GLOBALMOTOR_H
#include "motorctrl.h"
#include "opencv2/opencv.hpp"
//using namespace cv;

//DLLEXPORT int msOGI_CLS(
//    _BASE* cubeIn,
//    _BASE* cuberef,
//    _BASE* forecubeIn,
//    size_t IMROW, size_t IMCOL, size_t FILTERNUM,
//    size_t GASNUM,
//    unsigned char* result, _BASE* CL,
//    size_t FrameNum,
//    size_t SPECDIM,
//    bool isClassfyGas,
//    bool isInitGMMPara,
//    int currentMode,
//    double Thr,
//    MatTriple& r0
//);
struct MatTriple {
    cv::Mat mat1;//温度图像
    cv::Mat mat2;//mask
    cv::Mat mat3 ;//叠加结果
};
extern MatTriple matstruct;
//typedef int(*msOGI_CLSC)(_BASE*, _BASE*,_BASE*,size_t,size_t,size_t,size_t,uchar*,_BASE*,size_t,size_t,bool,bool,int,double,MatTriple&);
//extern msOGI_CLSC msOGI_CLS;
extern MotorCtrl *motorCtrl; // MotorCtrl 的实例
extern int WheelPos;
extern double g_limit;//阈值
extern int g_mode;//模式
extern int countnum;
class GlobalMotor
{
public:
    GlobalMotor();
};

#endif // GLOBALMOTOR_H
