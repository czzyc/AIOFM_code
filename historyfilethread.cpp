#include "historyfilethread.h"
#include <QDebug>
#include "opencv2/opencv.hpp"
#include <qimage.h>
#include "ogiworker.h"
#include "ogiglo.h"
HistoryFileThread::HistoryFileThread(QObject *parent)
    : QThread(parent)
{

}

void HistoryFileThread::SetPath(QString path){
    filepath=path;
}

void HistoryFileThread::SetState(bool state)
{
    Runstate=state;
}
void HistoryFileThread::run()
{

    int imagecount = readBinaryImage(filepath.toStdString(),640,512);
    dofiledata(image,imagecount);
}

size_t HistoryFileThread::readBinaryImage(const std::string& filename, int width, int height) {
    // 计算单个图像的总大小
    size_t totalSize = static_cast<size_t>(width) * height;

    // 打开二进制文件
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        qDebug() << QStringLiteral("无法打开文件: ") << filename.c_str();
        return 0;
    }

    // 获取文件大小
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // 计算可以读取的图像数量
    if (fileSize % (totalSize * sizeof(uint16_t)) != 0) {
        qDebug() << "Warning: File size is not aligned with image size";
    }
    size_t imageCount = fileSize / (totalSize * sizeof(uint16_t));

    if (imageCount == 0) {
        qDebug() << "File is empty or too small for one image";
        return 0;
    }

    try {
        // 调整vector大小
        image.resize(totalSize * imageCount);

        // 读取数据
        if (!file.read(reinterpret_cast<char*>(image.data()),
                       totalSize * imageCount * sizeof(uint16_t))) {
            qDebug() << "Error reading file, only" << file.gcount() << "bytes read";
            image.clear();
            return 0;
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Error occurred:" << e.what();
        image.clear();
        return 0;
    }

    file.close();
    return imageCount;
}

void HistoryFileThread::dofiledata(const std::vector<uint16_t>& image, int imagecount) {
    if (imagecount < 8) {
        qDebug() << QStringLiteral("没有完整的八张滤光轮图！");
        return;
    }

    // 使用vector替代原始数组
    std::vector<short> Seedata1(640 * 512);
    std::vector<double> Seedata(640 * 512 * 8);
    std::vector<unsigned char> result(640 * 512);
    std::vector<double> CL(640 * 512 * 2);

    int count = 0;
    bool isinit = true;
    int currentFrame = 0;
    int q = 1;
    while (currentFrame < imagecount && Runstate) {
        if(needPara){
            qDebug()<<"currentFrame"<<currentFrame;
            msleep(300);
            continue;
        }
        size_t frameOffset = currentFrame * 640 * 512;

        if (q >= 9) {
            q = 1;
            std::fill(Seedata.begin(), Seedata.end(), 0.0);  // 重置数据而不是重新分配
        }

        // 确保不会越界
        if (frameOffset + 640 * 512 <= image.size()) {
            for (int i = 0; i < 640 * 512; i++) {
                if (q != 0) {
                    Seedata[i + (q-1) * 640 * 512] = static_cast<double>(image[frameOffset + i]);
                }

                if (i == 640 * 512 - 1) {
                    Seedata1[i] = image[frameOffset + i - 1];
                } else {
                    Seedata1[i] = image[frameOffset + i];
                }
            }
        }



        QImage qimage = convertY16ToQImage(Seedata1.data());
        emit historyimg(qimage);

        if(q==8 && !needPara){
            if(doubleData){
                delete[] doubleData;
                doubleData = nullptr;  // 将指针设为空，避免悬空指针
            }
            doubleData = new double[640 * 512 * 8];
            //            for(int i=0;i<640 * 512 * 8;i++){
            //                doubleData[i]=Seedata[i];
            //            }
            std::memcpy(doubleData, Seedata.data(), Seedata.size() * sizeof(double));

            needPara = 1;
        }
        currentFrame++;
        q++;
        //        // 调用算法处理
        //        msOGI_CLS(Seedata.data(), Seedata.data(), Seedata.data(),
        //                  512, 640, 8, 2,
        //                  result.data(), CL.data(),
        //                  count++, 7, 0, isinit, 3, 8.0, matstruct);

        //        if (isinit) {
        //            isinit = false;
        //        }

        //        if (!matstruct.mat3.empty()) {
        //            QImage resultimage = matToQImaget(matstruct.mat3, 0);
        //            handleMessageResult(resultimage);
        //        }
    }
    emit historyEnd();//结束
}


bool HistoryFileThread::copyVectorToArray(const std::vector<double>& sourceVector, double*& destArray) {
    try {
        // 确保目标指针为空
        if(destArray != nullptr) {
            delete[] destArray;
            destArray = nullptr;
        }

        // 分配新内存
        destArray = new double[sourceVector.size()];

        // 使用 memcpy 复制数据
        std::memcpy(destArray, sourceVector.data(),
                    sourceVector.size() * sizeof(double));

        return true;
    } catch(const std::exception& e) {
        if(destArray != nullptr) {
            delete[] destArray;
            destArray = nullptr;
        }
        qDebug() << QStringLiteral("复制数据失败:") << e.what();
        return false;
    }
}



QImage HistoryFileThread::convertY16ToQImage(short *frame_src_data) {

    // 创建一个 cv::Mat 对象，表示 16 位图像
    cv::Mat img_pic(512, 640, CV_16U, frame_src_data);

    // 归一化处理，将 16 位图像转换为 8 位图像
    cv::Mat normalize_mat;
    cv::normalize(img_pic, normalize_mat, 0, 255, cv::NORM_MINMAX);

    // 转换为 8 位灰度图像
    cv::Mat gray_image;
    normalize_mat.convertTo(gray_image, CV_8UC1); // 转换为单通道 8 位图像

    //      cv::imshow("Converted Image", gray_image);
    //      cv::waitKey(10); // 等待按键，0 表示无限等待
    // 将 cv::Mat 转换为 QImage
    QImage qImage(gray_image.data, gray_image.cols, gray_image.rows, gray_image.step[0], QImage::Format_Grayscale8);

    // 需要确保 QImage 在 cv::Mat 的生命周期内有效
    qImage = qImage.copy(); // 复制数据，确保 QImage 的数据独立于 cv::Mat

    return qImage;
}
