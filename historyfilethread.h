#ifndef HISTORYFILETHREAD_H
#define HISTORYFILETHREAD_H
#include <QThread>
#include <vector>
#include <fstream>
#include <QObject>
#include <algorithm>
#include <QImage>
class HistoryFileThread : public QThread
{
    Q_OBJECT
public:
    // ~HistoryFileThread();
    explicit HistoryFileThread(QObject *parent = nullptr);
protected:
    void run() override;

public:
    bool copyVectorToArray(const std::vector<double>& sourceVector, double*& destArray);
    bool Runstate=true;
    void SetState(bool state);
    QString filepath="";
    void SetPath(QString path);
    std::vector<uint16_t> image;  // 替换原来的 uint16_t* image
    void dofiledata(const std::vector<uint16_t>& image, int imagecount);
    QImage convertY16ToQImage(short *frame_src_data);
    size_t readBinaryImage(const std::string& filename, int width, int height);
signals:
    void historyimg(QImage image);
    void historyEnd();
};

#endif // HISTORYFILETHREAD_H
