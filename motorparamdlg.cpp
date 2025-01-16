#include "motorparamdlg.h"
#include "ui_motorparamdlg.h"

motorParamdlg::motorParamdlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::motorParamdlg)
{
    ui->setupUi(this);
}

motorParamdlg::~motorParamdlg()
{
    delete ui;
}

void motorParamdlg::on_pushButton_clicked()
{

}

