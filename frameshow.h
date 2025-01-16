#ifndef FRAMESHOW_H
#define FRAMESHOW_H
#include <opencv2/opencv.hpp> // 使用OpenCV库来处理视频流
#include <QThread>
//using namespace cv;
class FrameShow: public QThread
{
    Q_OBJECT
public:
    bool get_frame(cv::Mat &frame);
    void run() override; // 重写 run() 方法
    explicit FrameShow(QObject *parent = nullptr);
signals:
    void frameReady(const QImage& frame);
    //public:
    //    FrameShow();
};

#endif // FRAMESHOW_H
