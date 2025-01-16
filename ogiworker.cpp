#include "ogiworker.h"
#include "MSOGIDLL.h"
#include "ogiglo.h"
#include "QDebug"
#include "qdatetime.h"
#include "qdir.h"
bool needPara=false;
OGIWorker::OGIWorker(QObject *parent)
    : QThread(parent)
{

}

void OGIWorker::setParameters(double *data)
{
    //    // 释放旧数据
    //    if(!needPara){
    //       if (m_data != nullptr) {
    //           delete[] m_data;
    //       }

    //       // 分配新内存并复制数据
    //       m_data = new double[512 * 640 * 8];
    //       m_muteogi.lock();
    //       std::memcpy(m_data, doubleData, 512 * 640 * 8 * sizeof(double));
    //       m_muteogi.unlock();

    //       needPara=true;
    //    }
    // 修复版本
    if (!needPara) {
        try {
            QMutexLocker locker(&m_muteogi);

            // 使用智能指针来确保异常安全
            std::unique_ptr<double[]> newData(new double[512 * 640 * 8]);

            // 复制数据
            std::memcpy(newData.get(), data, 512 * 640 * 8 * sizeof(double));

            // 交换并删除旧数据
            double* oldData = m_data;
            m_data = newData.release(); // 释放智能指针的所有权
            delete[] oldData;

            needPara = true;
        }
        catch (const std::bad_alloc& e) {
            qDebug() << QStringLiteral("内存分配失败:") << e.what();
        }
    }
}

OGIWorker::~OGIWorker()
{
    //    if (m_data != nullptr) {
    //        delete[] m_data;
    //        m_data = nullptr;
    //    }
}


void  OGIWorker::setStart(bool run)
{
    m_running=run;
}
void OGIWorker::run()
{
    countnum=0;
    bool isinit=1;
    m_running = true;
    int numFrames = 8;
    unsigned char* result = new  unsigned char[640*512];
    double * CL=new double[640*512*2];

    while (m_running) {
        //qDebug()<<"1111111111111"<<countnum;
        if(needPara){
            // m_muteogi.lock();
            //  qDebug()<<"fenxi222222"<<countnum;
            //  QMutexLocker locker(&m_muteogi);  // 使用 Qt 的互斥锁
            int ret=msOGI_CLS(doubleData,doubleData,doubleData,512,640,8,2,result,CL,countnum++,7,0,isinit,g_mode,g_limit,/*g_showMat*/matstruct);
            qDebug()<<QStringLiteral("分析完成：")<<countnum;

            if(doubleData){
                delete[] doubleData;
                doubleData = nullptr;  // 将指针设为空，避免悬空指针
            }
            // m_muteogi.unlock();
            cv::Mat maat3=matstruct.mat3;
            //                if(!matstruct.mat3.empty()){
            //                      cv::imshow("Result1",maat3);
            //                   //   cv::waitKey(10); // 等待按键，0 表示无限等待
            //                }

            QImage resultimage=matToQImaget(/*g_showMat*/matstruct.mat3,0);
            emit gasResult(resultimage);

            if(isWarning(matstruct.mat2))
            {
                saveMatAsImage(matstruct.mat3);
            }
            emit isExist(/*matstruct.mat2*/);
            needPara=false;
            if(isinit)
                isinit=false;
        }
        // Optional: Add some delay to prevent CPU overload
        //msleep(100);
    }

    emit isExistEnd(1);

    // 释放数组内存
    delete[] result;
    result = nullptr;  // 将指针设为空，避免悬空指针
    delete[] CL;
    CL = nullptr;  // 将指针设为空，避免 悬空指针
}


void OGIWorker::printMat(const cv::Mat& mat) {
    // 单通道
    for(int i = 0; i < mat.rows; i++) {
        for(int j = 0; j < mat.cols; j++) {
            std::cout << mat.at<float>(i,j) << " ";
        }
        std::cout << std::endl;
    }
}

std::string getMatType(const cv::Mat& mat) {
    int type = mat.type();
    std::string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar channels = 1 + (type >> CV_CN_SHIFT);

    switch (depth) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
    }

    r += "C";
    r += (channels + '0');

    return r;
}

QImage OGIWorker::matToQImaget(const cv::Mat& mat,int type) {

    //  qDebug()<<QString::fromStdString(getMatType(mat));
    // cv::imshow("Result",mat);
    // 确保Mat是8位无符号的，如果不是（例如16位图像），需要先转换
    int chanel = mat.channels();
    if(type==0){//灰度图
        if (mat.type() == CV_8UC1) {
            // 直接创建 QImage
            return QImage((const uchar*)mat.data, mat.cols, mat.rows, mat.step[0], QImage::Format_Grayscale8);
        } else if (mat.type() == CV_16UC1) {
            // 如果是 16 位图像，先转换为 8 位图像
            cv::Mat img8;
            mat.convertTo(img8, CV_8UC1, 255.0 / 65535.0); // 将 16 位图像缩放到 8 位范围
            return QImage((const uchar*)img8.data, img8.cols, img8.rows, img8.step[0], QImage::Format_Grayscale8);
        }else if(mat.type() == CV_8UC3)
        {
            cv::Mat rgb;
            cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);

            // 创建QImage，共享内存方式
            QImage image(rgb.data, rgb.cols, rgb.rows,
                         rgb.step,           // 每行字节数
                         QImage::Format_RGB888);

            // 深拷贝，确保数据独立
            return image.copy();
        }
        else {
            // 如果不是灰度图像，返回一个空的 QImage
            //return QImage((const uchar*)mat.data, mat.cols, mat.rows, mat.step[0], QImage::Format_Grayscale8);
            return QImage();
        }
        //     QImage image((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        //     return image;
    }
    else if(type==1)
    {


        //        // 将 8 位灰度图像转换为彩色图像
        // 可选：进行直方图均衡化以增强对比度
        // 判断是否需要均衡化
        //        if (shouldEqualizeHistogram(mat)) {
        //            // 进行均衡化处理
        //            cv::equalizeHist(mat, mat);
        //        }

        cv::Mat colorImage;
        if(atestcolor==0)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_JET);
        if(atestcolor==1)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_AUTUMN);
        if(atestcolor==2)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_BONE);
        if(atestcolor==3)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_COOL);
        if(atestcolor==4)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_HSV);
        if(atestcolor==5)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_HOT);
        if(atestcolor==6)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_OCEAN);
        if(atestcolor==7)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_PARULA);
        if(atestcolor==8)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_PINK);
        if(atestcolor==9)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_RAINBOW);
        if(atestcolor==10)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_SPRING);
        if(atestcolor==11)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_SPRING);
        if(atestcolor==12)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_SPRING);
        //atestcolor+=1;
        if(atestcolor>=12)
            atestcolor=0;
        // 显示彩色图像
        QImage qImage(colorImage.data, colorImage.cols, colorImage.rows, colorImage.step, QImage::Format_RGB888);
        return qImage.rgbSwapped();

    }


}

void OGIWorker::saveMatAsImage(const cv::Mat &mat)
{
    // 获取当前时间
    QString currentTime = QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss");

    // 创建目录 GasImg（如果不存在的话）
    QDir dir;
    if (!dir.exists("GasImg")) {
        dir.mkpath("GasImg");
    }

    // 设置图片保存路径
    QString filePath = QString("GasImg/%1.png").arg(currentTime);

    // 将cv::Mat保存为图片
    bool success = cv::imwrite(filePath.toStdString(), mat);

    if (success) {
        qDebug() << "Image saved to: " << filePath;
    } else {
        qDebug() << "Failed to save the image!";
    }
}

bool OGIWorker::isWarning(const cv::Mat& mask) {
    for(int i = 0; i < mask.rows; i++) {
        for(int j = 0; j < mask.cols; j++) {
            // qDebug()<<mask.at<uchar>(i,j);
            if(mask.at<uchar>(i,j) == 255) {
                return true;
            }
        }
    }
    return false;
}
