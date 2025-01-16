#pragma once
//#define TESTDLLEXPORT 1
#include "global.h"
#include "globalmotor.h"
//#include "opencv2/opencv.hpp"
//using namespace cv;

#ifdef TESTDLLEXPORT
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif
//struct MatTriple {
//	cv::Mat mat1;//温度图像
//	cv::Mat mat2;//mask
//	cv::Mat mat3 ;//叠加结果
//};
//导出函数
#ifdef __cplusplus
extern "C" {
#endif
#define _BASE double

	DLLEXPORT int setGasThresh(_BASE* threshPer, size_t gasNum);

	DLLEXPORT int readGasModels(const std::string& jsonFilePath);

	
	DLLEXPORT void INIT_GLOBAL_PARAMS_INDOOR(size_t SPECDIM, size_t IMROW, size_t IMCOL, int FrameNum, _BASE* cubeIn);
		
	DLLEXPORT int msOGI_CLS(
		_BASE* cubeIn,
		_BASE* cuberef,
		_BASE* forecubeIn,
		size_t IMROW, size_t IMCOL, size_t FILTERNUM,
		size_t GASNUM,
		unsigned char* result, _BASE* CL,
		size_t FrameNum,
		size_t SPECDIM,
		bool isClassfyGas,
		bool isInitGMMPara,
        int currentMode,//自定义 mode  1，2，3，4
        double Thr,//自定义 阈值
		MatTriple& r0
	);

	
	

#ifdef __cplusplus
}
#endif
