#ifndef OGIPARAMDLG_H
#define OGIPARAMDLG_H

#include <QWidget>

namespace Ui {
class OgiParamDlg;
}

class OgiParamDlg : public QWidget
{
    Q_OBJECT

public:
    explicit OgiParamDlg(QWidget *parent = nullptr);
    ~OgiParamDlg();
signals:
    void messageOgiParam(double Thr,int Mode);//检测结束
private slots:
    void on_pushButton_clicked();

    void on_horizontalScrollBar_valueChanged(int value);

    void on_edit_Thr_returnPressed();

private:
    Ui::OgiParamDlg *ui;
};

#endif // OGIPARAMDLG_H
