#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include <QMessageBox>

#include <iostream>
#include <fstream>  // 确保包含这个头文件
#include <vector>
#include <cstdint>  // 包含uint16_t定义
#include <QDebug>
#include <QFileDialog>
#include "cameraport.h"
#include "globalmotor.h"
#include <QSettings>
#include <stdio.h>

#include "Windows.h"
#include "ogiglo.h"



#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QFileDialog>
#include <QDateTime>
#include <QSplitter>
#include <QQueue>
#include <QMutex>
static QMutex messageMutex;
static QQueue<QPair<QtMsgType, QString>> messageQueue;
static const int MAX_QUEUE_SIZE = 1000;  // 限制队列大小


#include "MSOGIDLL.h"

// 全局消息处理器
static DebugOutput *globalDebugOutput = nullptr;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (globalDebugOutput) {
        QMutexLocker locker(&messageMutex);
        globalDebugOutput->handleMessage(type, msg);
    }
}

//void MainWindow::setupDebugOutput()
//{
//    // 创建状态栏的容器widget
//    QWidget *debugWidget = new QWidget(this);
//    QVBoxLayout *debugLayout = new QVBoxLayout(debugWidget);
//    debugLayout->setContentsMargins(0, 0, 0, 0);

//    // 创建文本编辑区
//    debugTextEdit = new QTextEdit(this);
//    debugTextEdit->setReadOnly(true);
//    debugTextEdit->setMaximumHeight(100);
//    debugTextEdit->setFont(QFont("Consolas", 9));  // 使用等宽字体

//    // 创建按钮容器
//    QWidget *buttonWidget = new QWidget(this);
//    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);
//    buttonLayout->setContentsMargins(0, 0, 0, 0);

//    // 创建清除按钮
//    QPushButton *clearButton = new QPushButton(QStringLiteral("清除"), this);
//    connect(clearButton, &QPushButton::clicked, debugTextEdit, &QTextEdit::clear);

////    // 创建保存按钮
////    QPushButton *saveButton = new QPushButton(QStringLiteral("另存为"), this);
////    connect(saveButton, &QPushButton::clicked, this, [this]() {
////        QString fileName = QFileDialog::getSaveFileName(this,
////            "保存日志",
////            QString("log_%1.txt").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
////            "Text Files (*.txt);;All Files (*)");

////        if (!fileName.isEmpty()) {
////            QFile file(fileName);
////            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
////                QTextStream stream(&file);
////                stream << debugTextEdit->toPlainText();
////                file.close();
////                qDebug() << QStringLiteral("日志已保存到:") << fileName;
////            }
////        }
////    });

//    // 添加按钮到布局
//    buttonLayout->addWidget(clearButton);
//   // buttonLayout->addWidget(saveButton);
//    buttonLayout->addStretch();

//    // 将所有元素添加到主布局
//    debugLayout->addWidget(debugTextEdit);
//    debugLayout->addWidget(buttonWidget);

//    // 将容器添加到状态栏
//    statusBar()->addWidget(debugWidget, 1);  // 1表示占用所有可用空间

//    // 创建调试输出处理器
//    debugOutput = new DebugOutput(debugTextEdit, this);

//    // 在 setupDebugOutput 方法中，创建文本编辑区后添加：
//    QSplitter *splitter = new QSplitter(Qt::Vertical, this);
//    splitter->addWidget(debugTextEdit);
//    splitter->addWidget(buttonWidget);
//    debugLayout->addWidget(splitter);
//}

void MainWindow::setupDebugOutput()
{
    // 创建状态栏的容器widget
    QWidget *debugWidget = new QWidget(this);
    QVBoxLayout *debugLayout = new QVBoxLayout(debugWidget);
    debugLayout->setContentsMargins(0, 0, 0, 0);

    // 创建文本编辑区
    debugTextEdit = new QTextEdit(this);
    debugTextEdit->setReadOnly(true);
    debugTextEdit->setMaximumHeight(100);  // 最大高度
    debugTextEdit->setFont(QFont("Consolas", 9));  // 使用等宽字体
    debugTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);  // 确保文本框宽度可扩展，高度固定

    // 创建清除按钮
    QPushButton *clearButton = new QPushButton(QStringLiteral("清除"), this);
    connect(clearButton, &QPushButton::clicked, debugTextEdit, &QTextEdit::clear);

    // 将清除按钮和文本框放在同一行
    QHBoxLayout *textButtonLayout = new QHBoxLayout();  // 创建一个水平布局
    textButtonLayout->addWidget(debugTextEdit);         // 添加文本编辑区
    textButtonLayout->addWidget(clearButton);           // 添加清除按钮

    // 创建容器来包含文本编辑区和按钮
    QWidget *textButtonContainer = new QWidget(this);   // 创建一个容器
    textButtonContainer->setLayout(textButtonLayout);    // 设置水平布局

    // 添加文本和按钮容器到主布局
    debugLayout->addWidget(textButtonContainer);

    // 将容器添加到状态栏
    statusBar()->addWidget(debugWidget, 1);  // 1表示占用所有可用空间

    // 创建调试输出处理器
    debugOutput = new DebugOutput(debugTextEdit, this);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , debugTextEdit(nullptr)
    , debugOutput(nullptr)
{
    TabcurrentIndex=1;
    // avformat_version();
    ui->setupUi(this);
    // 创建调试输出区域
    setupDebugOutput();

    // 安装消息处理器
    globalDebugOutput = debugOutput;
    qInstallMessageHandler(messageHandler);

    //qDebug()<<"version:"<<QString::fromStdString(av_version_info());
    initToolButtonMenu();//菜单


    motorCtrl=new MotorCtrl;
    connect(motorCtrl, &MotorCtrl::dataReceived, this, &MainWindow::handleDataReceived);

    motor = new Motor();
    QSettings settings(QCoreApplication::applicationDirPath()+"/SF6Config.ini",QSettings::IniFormat);
    QString port =  settings.value("Motor/Port","").toString();
    qDebug()<<"port:"<<port<<QCoreApplication::applicationDirPath();
    if(port==""){
        port="COM1";
    }
    if(!motorCtrl->openPort(port)){
        qDebug()<<QStringLiteral("电机串口打开失败！")<<GetLastError();
        isLinkSeeCamera=false;
        //return;
    }else{
        isLinkSeeCamera=true;
        StatusInquiry();
        bool recv=ResetMotor();
        qDebug()<<QStringLiteral("寻零:")<<recv;

    }

    QLibrary myLibrary("LIB/GuideUSB3LiveStream"); // 不需要加 .dll 后缀
    if (!myLibrary.load()) {
        qDebug() << QStringLiteral("加载红外成像采集配置:") << myLibrary.errorString();
        return ;
    }
    // 获取函数指针
    Initialize1 = (InitializeC)myLibrary.resolve("Initialize");
    if (!Initialize1()) {
        qDebug() << QStringLiteral("采集初始化:") << myLibrary.errorString();
        return ;
    }







    detection = new DetectionGas();
    connect(detection, &DetectionGas::gasDetected, this, &MainWindow::handleMessage); // 连接信号和槽
    connect(detection, &DetectionGas::saveFileName, this, &MainWindow::handleMessageResetName);
    // connect(detection, &DetectionGas::gasResult, this, &MainWindow::handleMessageResult); // 连接信号和槽

    CameraPort *cameraport = new CameraPort;
    cameraport->on_pushButton_clicked();


    ogidlg = new OgiParamDlg();//算法参数
    motordlg = new motorParamdlg();//电机参数

    //  cameraport->show();
    //  MainWindow mainWindow;
    //  QMetaObject::invokeMethod(&mainWindow,"showpic",Qt::QueuedConnection);
    // 使用 QTimer::singleShot 在界面显示后调用 myFunction
    QTimer::singleShot(3, this, &MainWindow::showpic);
    //  showpic();
}

MainWindow::~MainWindow()
{
    // 在析构函数中清理
    QMutexLocker locker(&messageMutex);
    globalDebugOutput = nullptr;
    qInstallMessageHandler(nullptr);
    messageQueue.clear();


    motorCtrl->closePort();
    if (isopenCamera)
        Exit1();
    delete ui;
}

void MainWindow::followLabel(QImage image)//叠加显示
{
    isOgi=true;
    if(TabcurrentIndex==1){
        // 获取 QLabel 在屏幕上的位置
        QPoint labelPos = ui->label_2->mapToGlobal(QPoint(0, 0));
        fors->SetNewImg(image);
        // 将 followWidget 移动到 QLabel 的位置
        fors->move(labelPos); // +10 是为了留出一点间距
        fors->show();
    }
    isOgi=false;
}

void MainWindow::showpic()
{

    detection->start();//红外相机


    videoThread=false;//可见相机
    videowidget = new VideoWidget();
    QObject::connect(videowidget, &VideoWidget::frameReady, this, &MainWindow::updateFrame); // 连接信号到槽
    videowidget->start(); // 启动线程


    TabcurrentIndex=ui->tabWidget->currentIndex();//获取当前标签

//    fors = new Form();
//    fors->show();//透明测试

}

void MainWindow::handleDataReceived(const QString &data) //电机接收数据
{
    // 处理接收到的数据
    if(StartCatch){
        if(data=="01" && (WheelPos==0 ||WheelPos==8)){
            WheelPos=1;
        }
        if(data=="02" && WheelPos==1){
            WheelPos=2;
        }
        if(data=="03"&& WheelPos==2){
            WheelPos=3;
        }
        if(data=="04"&& WheelPos==3){
            WheelPos=4;
        }
        if(data=="05"&& WheelPos==4){
            WheelPos=5;
        }
        if(data=="06"&& WheelPos==5){
            WheelPos=6;
        }
        if(data=="07"&& WheelPos==6){
            WheelPos=7;
        }
        if(data=="08"&& WheelPos==7){
            WheelPos=8;
        }
    }

}

void MainWindow::handleMessageResetName(QString filename)
{
    newFileName=filename;
}

void MainWindow::handleMessage(QImage image){
    if(TabcurrentIndex==1 ){//红外相机
        ui->label_2->clear();

        if(image.isNull())
            return;
        image = image.mirrored(false, true);
        QPixmap pixmap = QPixmap::fromImage(image);

        // 获取 QLabel 的大小
        QSize labelSize = ui->label_2->size();

        // 自适应调整 QPixmap 的大小
        QPixmap scaledPixmap = QPixmap::fromImage(image).scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // 设置 QPixmap 到 QLabel
        ui->label_2->setPixmap(scaledPixmap);
        // 调整 QLabel 的大小以适应新的图像
        ui->label_2->adjustSize();

        if(isHistoryfile)
            repaint();

    }

}

//bool MainWindow::hasOne(const cv::Mat& mask) {
//    cv::Mat ones;
//    cv::compare(mask, 1, ones, cv::CMP_EQ);
//    return cv::countNonZero(ones) > 0;
//}

bool MainWindow::hasOne(const cv::Mat& mask) {
    for(int i = 0; i < mask.rows; i++) {
        for(int j = 0; j < mask.cols; j++) {
            // qDebug()<<mask.at<uchar>(i,j);
            if(mask.at<uchar>(i,j) == 255) {
                return true;
            }
        }
    }
    return false;
}

void MainWindow::handleMessageGasEnd(bool end){
    if(isLeakage){
        ResetFileName(newFileName);
        newFileName="";//刷新重命名依据
    }
    ResetMotor();//检测结束需要点击复位
}

void MainWindow::handleMessageGas(/*cv::Mat mask*/)
{
    if(!isLeakage){
        if(hasOne(matstruct.mat2))
        {
            isLeakage=true;
        }
    }
}

void MainWindow::handleMessageResult(QImage image){
    if(TabcurrentIndex==2 ){//分析结果
        ui->label_3->clear();

        if(image.isNull())
            return;
        image = image.mirrored(false, true);
        QPixmap pixmap = QPixmap::fromImage(image);

        // 获取 QLabel 的大小
        QSize labelSize = ui->label_3->size();

        // 自适应调整 QPixmap 的大小
        QPixmap scaledPixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // 设置 QPixmap 到 QLabel
        ui->label_3->setPixmap(scaledPixmap);
        // 调整 QLabel 的大小以适应新的图像
        if(isHistoryfile)
            repaint();
    }
}


void MainWindow::on_btn_file_3_clicked()
{
    if(!isopenCamera){
        QMessageBox::information(nullptr, QStringLiteral("提示"),
                                 QStringLiteral("请先连接红外图像设备！"));
        qDebug()<< QStringLiteral("请先连接红外图像设备！");
        return;
    }
    if(isHistoryfile)
        historyfilethread->SetState(0);
    isHistoryfile=false;
    isHistoryEnd=true;

    StartCatch=false;

    detection->FileClose();
    CloseStream1();

    if(isLeakage)
        ResetFileName(newFileName);

    //    ogiworker->setStart(0);



    //    delete [] ogiworker;
    //    ogiworker = nullptr;
    //    int ret = Exit1();//退出
    //    if (ret == 1)
    //    {
    //        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("退出成功"));
    //    }else{
    //        QMessageBox::information(NULL,QStringLiteral("警告"),QStringLiteral("退出失败"));
    //    }
}

bool MainWindow::ResetMotor(){
    QString data="AA 03 00 FF";
    bool recv=motorCtrl->on_send(data);
    return recv;

}

void MainWindow::on_btn_start_clicked()
{
    if(!isopenCamera){
        QMessageBox::information(nullptr, QStringLiteral("提示"),
                                 QStringLiteral("请先连接红外图像设备！"));
        qDebug()<< QStringLiteral("请先连接红外图像设备！");
        return;
    }
    isLeakage=false;//刷新泄露判断依据
    StartCatch=true;

    if (fors!=nullptr) {
        delete fors;
        fors = nullptr;  // 防止悬挂指针
    }
    fors =new Form();//叠加图像显示



    //测试无需算法
    if(!isHistoryfile){
        motor->start();//电机启动

        ogiworker = new  OGIWorker();//算法线程
        connect(ogiworker, &OGIWorker::gasResult, this, &MainWindow::handleMessageResult); // 分析结果图
        connect(ogiworker, &OGIWorker::gasResult, this, &MainWindow::followLabel); // 连接信号和槽叠加显示图
        connect(ogiworker, &OGIWorker::isExist, this, &MainWindow::handleMessageGas);//是否存在泄露
        connect(ogiworker, &OGIWorker::isExistEnd, this, &MainWindow::handleMessageGasEnd);//检测结束
        ogiworker->setStart(1);
        ogiworker->start();
    }else{
        historyfilethread->start();

        ogiworker = new  OGIWorker();//算法线程
        connect(ogiworker, &OGIWorker::gasResult, this, &MainWindow::handleMessageResult); // 连接信号和槽
        connect(ogiworker, &OGIWorker::isExist, this, &MainWindow::handleMessageGas);//是否存在泄露
        connect(ogiworker, &OGIWorker::isExistEnd, this, &MainWindow::handleMessageGasEnd);//检测结束
        ogiworker->setStart(1);
        ogiworker->start();
    }

}


void MainWindow::on_checkBox_clicked(bool checked)
{
    if(isopenCamera){
        if(issavedata){
            detection->FileClose();
        }
        issavedata = checked;
        if(issavedata)
        {
            detection->initSaveImg();
        }
    }else{
        QMessageBox::information(nullptr, QStringLiteral("提示"),
                                 QStringLiteral("请先连接红外图像设备！"));
        qDebug()<< QStringLiteral("请先连接红外图像设备！");
    }

}


void MainWindow::on_pushButton_clicked()
{

    videoThread=false;
    videowidget = new VideoWidget();
    videowidget->start(); // 启动线程
    QObject::connect(videowidget, &VideoWidget::frameReady, this, &MainWindow::updateFrame); // 连接信号到槽

    //       frameshow = new FrameShow();
    //       QObject::connect(frameshow, &FrameShow::frameReady, this, &MainWindow::updateFrame); // 连接信号到槽

    //       videoThread=true;
    //       frameshow->start();

}

void MainWindow::updateFrame(const QImage& frame) {
    if(TabcurrentIndex==0){//可见相机
        // 同时水平和垂直翻转图像
        QImage flippedImage = frame.mirrored(false, true);
        ui->label->clear();

        QPixmap pixmap = QPixmap::fromImage(flippedImage);

        // 获取 QLabel 的大小
        QSize labelSize = ui->label->size();

        // 自适应调整 QPixmap 的大小
        QPixmap scaledPixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->label->adjustSize();


        ui->label->setPixmap(scaledPixmap); // 更新QLabel
        update();
    }
}




size_t MainWindow::readBinaryImage(const std::string& filename, int width, int height) {
    // 计算单个图像的总大小
    size_t totalSize = static_cast<size_t>(width) * height;

    // 打开二进制文件
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        qDebug() << QStringLiteral("无法打开文件: ") << filename.c_str();
        return 0;
    }

    // 获取文件大小
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // 计算可以读取的图像数量
    if (fileSize % (totalSize * sizeof(uint16_t)) != 0) {
        qDebug() << "Warning: File size is not aligned with image size";
    }
    size_t imageCount = fileSize / (totalSize * sizeof(uint16_t));

    if (imageCount == 0) {
        qDebug() << "File is empty or too small for one image";
        return 0;
    }

    try {
        // 调整vector大小
        image.resize(totalSize * imageCount);

        // 读取数据
        if (!file.read(reinterpret_cast<char*>(image.data()),
                       totalSize * imageCount * sizeof(uint16_t))) {
            qDebug() << "Error reading file, only" << file.gcount() << "bytes read";
            image.clear();
            return 0;
        }
    }
    catch (const std::exception& e) {
        qDebug() << "Error occurred:" << e.what();
        image.clear();
        return 0;
    }

    file.close();
    return imageCount;
}

void MainWindow::dofiledata(const std::vector<uint16_t>& image, int imagecount) {
    if (imagecount < 8) {
        QMessageBox::information(nullptr, QStringLiteral("警告"),
                                 QStringLiteral("没有完整的八张滤光轮图！"));
        qDebug() << QStringLiteral("没有完整的八张滤光轮图！");
        return;
    }

    // 使用vector替代原始数组
    std::vector<short> Seedata1(640 * 512);
    std::vector<double> Seedata(640 * 512 * 8);
    std::vector<unsigned char> result(640 * 512);
    std::vector<double> CL(640 * 512 * 2);

    int count = 0;
    bool isinit = true;
    int currentFrame = 0;
    int q = 1;

    while (currentFrame < /*imagecount*/1 && isHistoryfile) {//只放第一张展示图
        size_t frameOffset = currentFrame * 640 * 512;

        if (q >= 9) {
            q = 1;
            std::fill(Seedata.begin(), Seedata.end(), 0.0);  // 重置数据而不是重新分配
        }

        // 确保不会越界
        if (frameOffset + 640 * 512 <= image.size()) {
            for (int i = 0; i < 640 * 512; i++) {
                if (q != 0) {
                    Seedata[i + (q-1) * 640 * 512] = static_cast<double>(image[frameOffset + i]);
                }

                if (i == 640 * 512 - 1) {
                    Seedata1[i] = image[frameOffset + i - 1];
                } else {
                    Seedata1[i] = image[frameOffset + i];
                }
            }
        }

        currentFrame++;
        q++;

        QImage qimage = convertY16ToQImage(Seedata1.data());
        handleMessage(qimage);

        //        // 调用算法处理 测试部分
        //        msOGI_CLS(Seedata.data(), Seedata.data(), Seedata.data(),
        //                  512, 640, 8, 2,
        //                  result.data(), CL.data(),
        //                  count++, 7, 0, isinit, 3, 8.0, matstruct);

        //        if (isinit) {
        //            isinit = false;
        //        }

        //        if (!matstruct.mat3.empty()) {
        //            QImage resultimage = matToQImaget(matstruct.mat3, 0);
        //            handleMessageResult(resultimage);
        //        }
    }
}

void MainWindow::GetHistoryFilePath(QString path,QString filename){
    newFileName = filename;
    SeleteFilePath = path;
    historyfilethread->SetPath(SeleteFilePath);
    if(SeleteFilePath!="")
    {
        detection->FileClose();
        if(isopenCamera)
            CloseStream1();
        isHistoryEnd=false;

        isHistoryfile=true;
        StartCatch=false;//关闭原始的
        int imagecount = readBinaryImage(SeleteFilePath.toStdString(),640,512);
        dofiledata(image,imagecount);
    }
}

void MainWindow::handleHistoryEnd(){
    isHistoryEnd=true;
    ogiworker->setStart(0);
}

void MainWindow::on_btn_file_clicked()
{
    historyfilelist = new HistoryFileList();
    historyfilethread = new HistoryFileThread();
    QObject::connect(historyfilelist, &HistoryFileList::FilePath, this, &MainWindow::GetHistoryFilePath); // 连接信号到槽
    QObject::connect(historyfilethread, &HistoryFileThread::historyimg, this, &MainWindow::handleMessage); // 连接信号到槽
    QObject::connect(historyfilethread, &HistoryFileThread::historyEnd, this, &MainWindow::handleHistoryEnd); // 连接信号到槽

    historyfilelist->showMaximized();
    historyfilelist->show();
    return;


    detection->FileClose();
    if(isopenCamera)
        CloseStream1();
    isHistoryEnd=false;
    QString filename = QFileDialog::getOpenFileName(this,QStringLiteral( "选择文件"), "", "All Files (*)");
    if (!filename.isEmpty()) {
        isHistoryfile=true;

        StartCatch=false;//关闭原始的



        int imagecount = readBinaryImage(filename.toStdString(),640,512);
        dofiledata(image,imagecount);
        isHistoryEnd=true;
    }else{
        //  QMessageBox::information(NULL,QStringLiteral("警告"),QStringLiteral("大开文件失败！"));
        qDebug()<<QStringLiteral("未打开文件！");
    }
}


//void MainWindow::dofiledata(uint16_t*image,int imagecount)
//{
//    short* Seedata1;
//    double* Seedata;
////    double* Seedata2;
//   // (_BASE*, _BASE*,_BASE*,size_t,size_t,size_t,size_t,uchar*,_BASE*,size_t,size_t,bool,bool,int,cv::Mat&);
//      //(Seedata1,Seedata,Seedata2,256,320,8,2,result,CL,count++,7,0,isinit,1,g_showMat);

//    Seedata1 = new short[640*512];
//    Seedata = new double[640*512*8];
//    //Seedata2 = new double[640*512*8];
//    if(imagecount<8){
//        QMessageBox::information(NULL,QStringLiteral("警告"),QStringLiteral("没有完整的八张滤光轮图！"));
//         qDebug()<<QStringLiteral("没有完整的八张滤光轮图！");
//        return;
//    }
//    unsigned char* result = new  unsigned char[640*512];
//    double * CL=new double[640*512*2];
//    int count=0;
//    bool isinit=1;
//    int currentFrame = 0;
//    // 计算当前帧的起始位置
//    size_t frameOffset = currentFrame * 640 * 512;
//    int q=1;
//    while(currentFrame<imagecount && isHistoryfile){

//        if(q>=9){
//            q=1;
//            delete [] Seedata;
//            Seedata = nullptr;
//            Seedata = new double[640*512*8];
//        }
//            for (int i = 0; i < 640 * 512; i++) {
//                  if(q!=0)
//                     Seedata[i+(q-1)*640 * 512] = static_cast<uint16_t>(image[frameOffset+i]);
//               if(640 * 512-1==i)
//                    Seedata1[i] =image[i-1];
//               else
//                    Seedata1[i] =image[i];
//            }
//            currentFrame++;
//            frameOffset = currentFrame * 640 * 512;
//            q++;

//        QImage image = convertY16ToQImage(Seedata1);
//        handleMessage(image);
//        delete [] Seedata1;
//        Seedata1 = nullptr;
//        Seedata1 = new short[640*512];
////        for(int i=0;i<8;i++){
////            for(int j=0 ;j<640*512;j++){
////               Seedata1[i*j+j]= image[j];
////            }
////        }



//          msOGI_CLS(Seedata,Seedata,Seedata,512,640,8,2,result,CL,count++,7,0,isinit,3,8.0,/*g_showMat*/matstruct);
////          if(!matstruct.mat3.empty()){
////           g_showMat =  processMasks(g_showMat);
////           if(!g_showMat.empty()){
////              cv::imshow("1",g_showMat);
////              // 处理窗口事件，等待10毫秒
////                     if (cv::waitKey(10) == 27) {  // 按下ESC键退出
////                         break;
////                     }
////           }
////          }
////          if(!matstruct.mat1.empty()){
////                cv::imshow("Temperature", matstruct.mat1);
////                  cv::waitKey(1);
////          }
////          if(!matstruct.mat2.empty()){
////                cv::imshow("Mask", matstruct.mat2);
////          }
////          if(!matstruct.mat3.empty()){
////                cv::imshow("Result", matstruct.mat3);
////          }
//          if(isinit)
//              isinit=0;
//         QImage resultimage=matToQImaget(/*g_showMat*/matstruct.mat3,0);
//         handleMessageResult(resultimage);
//    }
//    if(Seedata1)
//    {
//        delete [] Seedata1;
//        Seedata1=nullptr;
//    }
//    if(Seedata)
//    {
//        delete [] Seedata;
//        Seedata=nullptr;
//    }

//}

QImage MainWindow::convertY16ToQImage(short *frame_src_data) {

    // 创建一个 cv::Mat 对象，表示 16 位图像
    cv::Mat img_pic(512, 640, CV_16U, frame_src_data);

    // 归一化处理，将 16 位图像转换为 8 位图像
    cv::Mat normalize_mat;
    cv::normalize(img_pic, normalize_mat, 0, 255, cv::NORM_MINMAX);

    // 转换为 8 位灰度图像
    cv::Mat gray_image;
    normalize_mat.convertTo(gray_image, CV_8UC1); // 转换为单通道 8 位图像

    // cv::imshow("Converted Image", gray_image);
    // cv::waitKey(10); // 等待按键，0 表示无限等待
    // 将 cv::Mat 转换为 QImage
    QImage qImage(gray_image.data, gray_image.cols, gray_image.rows, gray_image.step[0], QImage::Format_Grayscale8);

    // 需要确保 QImage 在 cv::Mat 的生命周期内有效
    qImage = qImage.copy(); // 复制数据，确保 QImage 的数据独立于 cv::Mat

    return qImage;
}


QImage MainWindow::matToQImaget(const cv::Mat& mat,int type) {

    // 确保Mat是8位无符号的，如果不是（例如16位图像），需要先转换
    int chanel = mat.channels();
    if(type==0){//灰度图
        if (mat.type() == CV_8UC1) {
            // 直接创建 QImage
            return QImage((const uchar*)mat.data, mat.cols, mat.rows, mat.step[0], QImage::Format_Grayscale8);
        } else if (mat.type() == CV_16UC1) {
            // 如果是 16 位图像，先转换为 8 位图像
            cv::Mat img8;
            mat.convertTo(img8, CV_8UC1, 255.0 / 65535.0); // 将 16 位图像缩放到 8 位范围
            return QImage((const uchar*)img8.data, img8.cols, img8.rows, img8.step[0], QImage::Format_Grayscale8);
        } else {
            // 如果不是灰度图像，返回一个空的 QImage
            return QImage();
        }

    }
    else if(type==1)
    {


        //        // 将 8 位灰度图像转换为彩色图像
        // 可选：进行直方图均衡化以增强对比度
        // 判断是否需要均衡化
        //        if (shouldEqualizeHistogram(mat)) {
        //            // 进行均衡化处理
        //            cv::equalizeHist(mat, mat);
        //        }

        cv::Mat colorImage;
        if(atestcolor==0)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_JET);
        if(atestcolor==1)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_AUTUMN);
        if(atestcolor==2)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_BONE);
        if(atestcolor==3)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_COOL);
        if(atestcolor==4)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_HSV);
        if(atestcolor==5)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_HOT);
        if(atestcolor==6)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_OCEAN);
        if(atestcolor==7)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_PARULA);
        if(atestcolor==8)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_PINK);
        if(atestcolor==9)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_RAINBOW);
        if(atestcolor==10)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_SPRING);
        if(atestcolor==11)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_SPRING);
        if(atestcolor==12)
            cv::applyColorMap(mat, colorImage, cv::COLORMAP_SPRING);
        //atestcolor+=1;
        if(atestcolor>=12)
            atestcolor=0;
        // 显示彩色图像
        QImage qImage(colorImage.data, colorImage.cols, colorImage.rows, colorImage.step, QImage::Format_RGB888);
        return qImage.rgbSwapped();

    }


}


cv::Mat MainWindow::processMasks(cv::Mat r0) {
    // 添加新的掩码（示例）

    // cv::circle(r0, cv::Point(320, 256), 100, cv::Scalar(255), -1); // 添加新的白色圆形
    masks.push_back(r0.clone());

    if (masks.size() > 3) {
        masks.pop_front(); // 保持队列大小为3
    }
    cv::Mat smoothedMask ;
    smoothedMask=cv::Mat();
    if (masks.size() >= 3) {
        // 检查 masks[0] 和 masks[1] 的尺寸和类型是否匹配
        if (masks[0].size() == masks[1].size() && masks[0].type() == masks[1].type()) {
            // 使用逻辑与操作
            cv::bitwise_and(masks[0], masks[1], smoothedMask);

            // 检查 smoothedMask 和 masks[2] 的尺寸和类型是否匹配
            if (smoothedMask.size() == masks[2].size() && smoothedMask.type() == masks[2].type()) {
                cv::bitwise_and(smoothedMask, masks[2], smoothedMask);
            } else {
                qDebug() << "smoothedMask 和 masks[2] 的尺寸或类型不匹配";
            }
        } else {
            qDebug() << "masks[0] 和 masks[1] 的尺寸或类型不匹配";
        }
    } else {
        qDebug() << QStringLiteral("队列中帧数不足，无法进行平滑处理");
    }

    return smoothedMask;
}


//void MainWindow::on()//遮光片到相机
//{
//    QString data="AA 04 00 FF";
//    QString recv=motorCtrl->on_send(data);
//}


//void MainWindow::on_d()//遮光片离开相机
//{
//    QString data="AA 05 00 FF";
//    QString recv=motorCtrl->on_send(data);
//}

void MainWindow::StatusInquiry()//状态查询
{
    QString data="AA 01 00 FF";
    bool recv=motorCtrl->on_send(data);
    qDebug()<<"Status:"<<recv;
}




void MainWindow::on_tabWidget_currentChanged(int index)
{
    TabcurrentIndex=index;//获取当前标签
    if(fors!=nullptr){
        if(TabcurrentIndex!=1){
            fors->hide();
        }else{
            if(isOgi){
                fors->show();
            }
        }
    }
    if(TabcurrentIndex==1 && isHistoryfile==false && isHistoryEnd){

        if (!Initialize1()) {
            qDebug() << "Could not resolve function!" ;
            return ;
        }

        detection->start();
    }
}

void MainWindow::initToolButtonMenu()
{

    // 设置图标大小

    QMenu *menu = ui->menubar->findChild<QMenu*>("menu_SF6");  // menu是菜单的objectName

    ui->action->setIcon(this->style()->standardIcon(QStyle::SP_FileLinkIcon  ));
    ui->action_2->setIcon(this->style()->standardIcon(QStyle::SP_DialogApplyButton   ));
    // 连接信号和槽
    connect(ui->action, &QAction::triggered, this, &MainWindow::onHistoryFileClicked);
    connect(ui->action_2, &QAction::triggered, this, &MainWindow::onSettingsClicked);


    QMenu *menuSet = ui->menubar->findChild<QMenu*>("menu_Set");  // menu是菜单的objectName

    QIcon motorIcon("Pic//motor.png");
    QIcon ogiIcon("Pic//ogi.png");

    // 设置图标的大小
    int iconSize = 30; // 根据需要调整大小
    ui->action_motor->setIcon(motorIcon.pixmap(iconSize, iconSize));
    ui->action_ogi->setIcon(ogiIcon.pixmap(iconSize, iconSize));
    //    ui->action_motor->setIcon(QIcon("Pic//motor.png"));
    //    ui->action_ogi->setIcon(QIcon("Pic//ogi.png"));

    // 连接信号和槽
    connect(ui->action_motor, &QAction::triggered, this, &MainWindow::motorParamSet);
    connect(ui->action_ogi, &QAction::triggered, this, &MainWindow::ogiParamSet);
}

//电机设置
void MainWindow::motorParamSet(){
    qDebug() << QStringLiteral("电机设置");
    motordlg->show();
}
//算法检测参数设置
void MainWindow::ogiParamSet()
{
    qDebug() << QStringLiteral("检测参数设置");
    ogidlg->show();
}

void MainWindow::onHistoryFileClicked()
{
    // 处理历史文件选项
    qDebug() << QStringLiteral("历史文件");
    on_btn_file_clicked();
    // 在这里添加你的代码
    // 例如：打开历史文件对话框
}

void MainWindow::onSettingsClicked()
{
    // 处理参数设置选项
    qDebug() << QStringLiteral("初始化滤光轮");
    // QString data="AA 03 00 FF";
    // bool recv=motorCtrl->on_send(data);
    bool recv = ResetMotor();
    qDebug()<<"to zero:"<<recv;
    // 在这里添加你的代码
    // 例如：打开设置对话框
}

void  MainWindow::changeEvent(QEvent *event)  {
    if (event->type() == QEvent::WindowStateChange) {
        if (isMaximized()) {

            // ui->label->setFixedSize(1280, 800);
            // ui->label_2->setFixedSize(1280, 800);
            // ui->label_3->setFixedSize(1280, 800);
        } else if (isMinimized()) {
            qDebug() << "Window is minimized";
        } else {

            // ui->label->setFixedSize(640, 512);
            // ui->label_2->setFixedSize(640, 512);
            // ui->label_3->setFixedSize(640, 512);
        }
    }
    QMainWindow::changeEvent(event);  // 调用基类的处理方法
}

bool MainWindow::ResetFileName(QString oldFileName){
    QFileInfo fileInfo(oldFileName);
    QString dirPath = "PicData//";

    QString newFileName;
    if (fileInfo.suffix().toLower() == "txt") {
        if(fileInfo.baseName().right(1)=="Y")
            return 1;//已经标记
        // 如果是 .txt 文件，保留扩展名
        newFileName = fileInfo.baseName() + "Y." + fileInfo.suffix();
    } else {
        // 如果没有扩展名或其他扩展名，直接加Y
        if(oldFileName.right(1)=="Y")
            return 1;//已经标记
        newFileName = oldFileName + "Y";
    }

    QString oldFilePath = dirPath + fileInfo.fileName();
    QString newFilePath = dirPath + newFileName;

    QFile file(oldFilePath);
    if (file.rename(newFilePath)) {
        qDebug() << QStringLiteral("文件重命名成功:") << newFilePath;
        return true;
    } else {
        qDebug() << QStringLiteral("文件重命名失败:") << file.errorString();
        return false;
    }
}
