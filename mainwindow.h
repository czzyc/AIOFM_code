#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <windows.h>
#include <QMainWindow>
//#define _BASE double
#include "detectiongas.h"
#include "videowidget.h"
#include "global.h"
//#include "MotorCtrl.h"
#include <QTimer>
#include "motor.h"
#include "frameshow.h"

#include "motorparamdlg.h"//电机设置界面
#include "ogiparamdlg.h"//算法参数设置界面

#include <QMenu>
#include <QStyle>
#include <QIcon>

#include "historyfilelist.h"

#include <thread>

#include <vector>
#include <fstream>
#include <algorithm>
#include "historyfilethread.h"

#include "debugoutput.h"//自定义状态栏
#include <QTextEdit>

#include "form.h"
//extern "C"
//{
//#include "libavcodec/avcodec.h"
//#include "libavdevice/avdevice.h"
//#include "libavformat/avformat.h"
//#include "libavfilter/avfilter.h"
//#include "libavutil/imgutils.h"
//#include "libavutil/avutil.h"
//#include "libswscale/swscale.h"
//}
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    //  bool eventFilter(QObject *watched, QEvent *event) override;  // 添加这行
    void changeEvent(QEvent *event) override;
private slots:
    // 电机
    void handleDataReceived(const QString &data);

    // 历史文件处理函数
    void onHistoryFileClicked();
    // 参数设置处理函数
    void onSettingsClicked();
    //电机设置
    void motorParamSet();
    //算法检测参数设置
    void ogiParamSet();
private:
    // 初始化工具按钮菜单
    void initToolButtonMenu();
    // 保存菜单指针
    QMenu *toolButtonMenu;
    // MotorCtrl *motorCtrl; // MotorCtrl 的实例
public:
    void showpic();
public:
    bool isOgi=false;
    QString SeleteFilePath="";
    int TabcurrentIndex;
    QImage matToQImaget(const cv::Mat& mat,int type) ;
    int test =1;
    bool a=1;

    Form *fors=nullptr ;//叠加画面
    Motor *motor;//电机控制
    OgiParamDlg *ogidlg;//算法参数
    motorParamdlg *motordlg;//电机参数
    std::deque<cv::Mat> masks;
    // uint16_t* image;
    std::vector<uint16_t> image;  // 替换原来的 uint16_t* image
    cv::Mat processMasks(cv::Mat r0);
    HINSTANCE hDLL;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    VideoWidget * videowidget;

    FrameShow *frameshow;

    DetectionGas *detection;

    HistoryFileList *historyfilelist;
    HistoryFileThread *historyfilethread;

public:

    bool hasOne(const cv::Mat& mask);
    bool ResetMotor();
    void GetHistoryFilePath(QString path,QString filename);
    void on_pushButton_clicked();
    void on_btn_file_clicked();
    QImage convertY16ToQImage(short *frame_src_data);

public slots:
    void followLabel(QImage image);//叠加显示
    void handleMessageGasEnd(bool end);
    void handleMessageResetName(QString filename);
    void handleMessage(QImage image);
    void handleMessageResult(QImage image);
    void handleMessageGas(/*cv::Mat mask*/);
    void handleHistoryEnd();
private slots:
    void updateFrame(const QImage& frame); // 视频流
private slots:


    void on_btn_file_3_clicked();

    void on_btn_start_clicked();

    void on_checkBox_clicked(bool checked);

    //    void on_pushButton_clicked();

    //    void on_btn_file_clicked();

    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;
    bool isLinkSeeCamera=false;
    QTextEdit *debugTextEdit;
    DebugOutput *debugOutput;
public:
    void StatusInquiry();
public:
    QString newFileName="";
    bool isLeakage=false;//是否存在泄露
    bool isHistoryfile=false;
    bool isHistoryEnd=false;
    cv::Mat g_showMat;
    //    void dofiledata(uint16_t*image,int imagecount);
    //    size_t MainWindow::readBinaryImage(const std::string& filename,  int width=640, int height=512);
    void dofiledata(const std::vector<uint16_t>& image, int imagecount);
    size_t readBinaryImage(const std::string& filename, int width, int height);



    void setupDebugOutput();
    bool ResetFileName(QString oldFileName);
};
#endif // MAINWINDOW_H
