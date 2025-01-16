#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    void SetNewImg(QImage img);//更新图
    explicit Form(QWidget *parent = nullptr);
    ~Form();
private:
    Ui::Form *ui;
protected:
    void paintEvent(QPaintEvent *event) override;  // 重写 paintEvent 方法

private:
    QPixmap image;  // 用来存储加载的图片
};

#endif // FORM_H
