#include "globalmotor.h"
MotorCtrl *motorCtrl; // MotorCtrl 的实例
//msOGI_CLSC msOGI_CLS;
int WheelPos=0;// 滤光轮位置
double g_limit=50.0;
int countnum=0;
MatTriple matstruct;
int g_mode=3;
GlobalMotor::GlobalMotor()
{

}
