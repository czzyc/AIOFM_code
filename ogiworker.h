#ifndef OGIWORKER_H
#define OGIWORKER_H
#include "global.h"
#include "globalmotor.h"
#include "qthread.h"
extern bool needPara;
class OGIWorker : public QThread
{
    Q_OBJECT
public:

    ~OGIWorker();
    explicit OGIWorker(QObject *parent = nullptr);
    void setParameters(double *data);
    void  setStart(bool run);
    QImage matToQImaget(const cv::Mat& mat,int type);
    void printMat(const cv::Mat& mat);
    void saveMatAsImage(const cv::Mat &mat);//存储报警图片
    bool isWarning(const cv::Mat& mask);//气体判断
protected:
    void run() override;

private:
    static constexpr size_t DATA_SIZE = 512 * 640 * 8;

    bool m_running;
    double* m_data;
signals:
    void gasResult(QImage image); // 信号，用于通知气体检测结果
    void isExist(/*cv::Mat mask*/); // 信号，用于判断是否有泄露
    void isExistEnd(bool end);//检测结束
};

#endif // OGIWORKER_H
