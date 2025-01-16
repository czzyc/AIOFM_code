#include "ogiparamdlg.h"
#include "ui_ogiparamdlg.h"
#include "globalmotor.h"
OgiParamDlg::OgiParamDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OgiParamDlg)
{
    ui->setupUi(this);
    ui->horizontalScrollBar->setRange(0,100);
    ui->horizontalScrollBar->setValue(50);
}

OgiParamDlg::~OgiParamDlg()
{
    delete ui;
}

void OgiParamDlg::on_pushButton_clicked()
{
    QString str = ui->edit_Thr->text();
    if(str!="")
        g_limit = str.toDouble();

    str = ui->cb_mode->currentText();
    if(str!="")
        g_mode = str.toDouble();
    emit messageOgiParam(g_limit,g_mode);
}


void OgiParamDlg::on_horizontalScrollBar_valueChanged(int value)
{
    ui->edit_Thr->setText(QString::number(value));
}


void OgiParamDlg::on_edit_Thr_returnPressed()
{
    QString value = ui->edit_Thr->text();
    if(value!=""){
        ui->horizontalScrollBar->setValue(value.toInt());
    }
}

