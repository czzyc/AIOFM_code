#include "frameshow.h"
#include "global.h"
#include "QDebug"
//FrameShow::FrameShow()
//{

//}
FrameShow::FrameShow(QObject *parent) : QThread(parent) {



}

bool FrameShow::get_frame(cv::Mat &frame)
{
    m_mutex.lock();
    if(m_vec_frame.size() < 1)
        return false;
    else
    {
        //从容器中取图像
        frame = m_frame;
        // frame = m_vec_frame.back();
        QImage img(frame.data, frame.cols, frame.rows, frame.step[0], QImage::Format_RGB888);
        emit frameReady(img.rgbSwapped()); // 发射信号
    }
    m_mutex.unlock();
    return true;
}

void FrameShow::run() {

    cv::Mat frame;
    while(videoThread)
    {
        qDebug()<<"videoThread"<<videoThread;
        bool ret = get_frame(frame);
        if(!ret && !frame.empty())
            imshow("windows",frame);

    }
}
