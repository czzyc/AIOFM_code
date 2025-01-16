#pragma once

#define GUIDEUSBAPI __declspec(dllexport)

/* Function Return Value */
typedef enum
{
	ERROR_NO = 1,					   		        //1:no error
	ERROR_DEVICE_NOT_FOUND = -1,		//-1:can’t find device
	ERROR_POINT_NULL = -2,					//-2:null point
	ERROR_POINTS_TOO_LARGE = -3,		//-3:point too large点太大
	ERROR_POINTS_TOO_SMALL = -4,		//-4: point too small
	ERROR_MALLOC_FAILED = -5,				//-5:alloc memory failure分配内存故障
	ERROR_RESOLUTION = -6,				    //-6: set resolution error设置分辨率错误
	ERROR_UNKNOW = -999					//-999:unknow error
}guide_usb_code_e;

/*	Device Video Mode */
typedef enum
{
	X16 = 0,												//X16 
	X16_PARAM = 1,									//X16+paraline
	Y16 = 2,												//Y16
	Y16_PARAM = 3,									//Y16+paraline
	YUV = 4,												//YUV 
	YUV_PARAM = 5,									//YUV+paraline
	Y16_YUV = 6,										//Y16+YUV
	Y16_PARAM_YUV = 7							//Y16+paraline+YUV
}guide_usb_video_mode_e;

/* Device Connect Mode */
typedef enum
{
	DEVICE_CONNECT_OK = 1,					//connect ok
	DEVICE_DISCONNECT_OK = -1,			//disconnect
}guide_usb_device_status_e;

/* Device Video Info */
typedef struct
{
	int width;												//video width
	int height;												//video height
	guide_usb_video_mode_e video_mode;	//video mode
}guide_usb_device_info_t;

/* Image Frame  Info */
typedef struct
{
	int frame_width;								    //frame width
	int frame_height;									//frame height
	unsigned char *frame_rgb_data;			//rgb data stream
	int frame_rgb_data_length;					//rgb data length
	short *frame_src_data;							//X16/Y16 data stream
	int frame_src_data_length;					//X16/Y16 data length
	short *frame_yuv_data;							//yuv data stream
	int frame_yuv_data_length;					//yuv data length
	short *paraLine;									//paraline
	int paraLine_length;								//paraline length
}guide_usb_frame_data_t;

/* Device Info */
typedef struct
{
	int devID;												//device ID
	char devName[128];								//device name
}device_info;

/* Device List Info */
typedef struct
{
	int devCount;										//device count
	device_info devs[32];
}device_info_list;


/*	Video Stream Callback Function 视频流回调功能*/
typedef void(__stdcall *OnFrameDataReceivedCB)(const guide_usb_frame_data_t frame_data);

/* Connect Status  Callback Function连接状态回调函数 */
typedef void(__stdcall *OnDeviceConnectStatusCB)(const guide_usb_device_status_e device_status);

//******************************************************************************
//  Function    	: Initialize
//  Description 	: Initialize module, invoke once when the software boot up
//  Input       	: ------
//  Output       	: ------
//  Return      	: 1-success, <0-fail
//******************************************************************************
extern "C" GUIDEUSBAPI int Initialize();

//******************************************************************************
//  Function    	: Exit
//  Description 	: Uninitialize module, invoke once when the software exit.
//  Input       	: ------
//  Output       	: ------
//  Return      	: 1-success, <0-fail
//******************************************************************************
extern "C" GUIDEUSBAPI int Exit();

//******************************************************************************
//  Function    	: GetDeviceList
//  Description 	: Get usb device list info
//  Input       	: devInfos:include device number, device ID, device name
//描述：获取usb设备列表信息
//输入：devInfos：包括设备编号、设备ID、设备名称
//  Output       	: ------
//  Return      	: 1-success, <0-fail
//******************************************************************************
extern "C" GUIDEUSBAPI int GetDeviceList(device_info_list *devInfos);

//******************************************************************************
//  Function    	: OpenStream
//  Description 	: Open the video stream, the first device is default.
//  Input       	: deviceInfo: include width, height and video mode; frameRecvCB: Video stream callback; connectStatusCB: Connection status callback
//描述：打开视频流，第一个设备是默认的。
//输入：设备信息：包括宽度、高度和视频模式；frameRecvCB：视频流回调；connectStatusCB:连接状态回调
//  Output       	: ------
//  Return      	: 1-success, <0-fail
//******************************************************************************
extern "C" GUIDEUSBAPI int OpenStream(guide_usb_device_info_t* deviceInfo, OnFrameDataReceivedCB frameRecvCB, OnDeviceConnectStatusCB connectStatusCB);

//******************************************************************************
//  Function    	: OpenStream
//  Description 	: Open the video stream by device id
//  Input       	: devID: device ID from GetDeviceList; deviceInfo: include width, height and video mode; frameRecvCB: Video stream callback; connectStatusCB: Connection status callback
//描述：按设备id打开视频流
//输入：devID：GetDeviceList中的设备ID；设备信息：包括宽度、高度和视频模式；frameRecvCB：视频流回调；connectStatusCB:连接状态回调
//  Output       	: ------
//  Return      	: 1-success, <0-fail
//******************************************************************************
extern "C" GUIDEUSBAPI int OpenStreamByDevID(int devID, guide_usb_device_info_t* deviceInfo, OnFrameDataReceivedCB frameRecvCB, OnDeviceConnectStatusCB connectStatusCB);

//******************************************************************************
//  Function    	: CloseStream
//  Description 	: Close the video stream
//  Input       	: ------
//  Output       	: ------
//  Return      	: 1-success, <0-fail
//******************************************************************************
extern "C" GUIDEUSBAPI int CloseStream();

//******************************************************************************
//  Function    	: SetPalette
//  Description 	: Set the image Palette
//  Input       	: index: palette index, from 0 to 9, 10 color in total, 0 is default.
//函数：SetPalette
//描述：设置图像调色板
//输入：索引：调色板索引，从0到9，共10种颜色，默认为0。
//  Output       	: ------
//  Return      	: 1-success, <0-fail
//******************************************************************************
extern "C" GUIDEUSBAPI int SetPalette(int index);








