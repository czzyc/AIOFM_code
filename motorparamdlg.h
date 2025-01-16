#ifndef MOTORPARAMDLG_H
#define MOTORPARAMDLG_H

#include <QWidget>

namespace Ui {
class motorParamdlg;
}

class motorParamdlg : public QWidget
{
    Q_OBJECT

public:
    explicit motorParamdlg(QWidget *parent = nullptr);
    ~motorParamdlg();

private slots:
    void on_pushButton_clicked();

private:
    Ui::motorParamdlg *ui;
};

#endif // MOTORPARAMDLG_H
