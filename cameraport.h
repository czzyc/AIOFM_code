#ifndef CAMERAPORT_H
#define CAMERAPORT_H

#include <QWidget>

namespace Ui {
class CameraPort;
}

class CameraPort : public QWidget
{
    Q_OBJECT

public:
    explicit CameraPort(QWidget *parent = nullptr);
    ~CameraPort();
public:
    void on_pushButton_clicked();
private slots:
    //void on_pushButton_clicked();

private:
    Ui::CameraPort *ui;
};

#endif // CAMERAPORT_H
