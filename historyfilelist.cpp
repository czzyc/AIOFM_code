#include "historyfilelist.h"
#include "ui_historyfilelist.h"
#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>
#include "global.h"
#include <QFile>
#include <QDir>
#include <QDateTimeEdit>
#include <QDateTime>
#include <QScrollBar>
#include <QStyle>
#include <QIcon>
#include <QApplication>
#include <QScreen>
#define WarningT "警告"
#define NormalT "正常"
HistoryFileList::HistoryFileList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryFileList)
{
    ui->setupUi(this);
    setupListView();
    //      setupDateTimeEdit();
    // 获取当前日期和时间
    QDateTime currentDateTime = QDateTime::currentDateTime();
    // 设置 QDateTimeEdit 控件的值为当前时间
    ui->date_Start->setDateTime(currentDateTime);
    ui->date_End->setDateTime(currentDateTime);

    ui->comboBox->setCurrentIndex(0);//初始化加载所有的
    on_pushButton_2_clicked();
    // 测试添加数据

}

HistoryFileList::~HistoryFileList()
{
    delete ui;
}


void HistoryFileList::setupDateTimeEdit()
{
    // 获取系统默认样式
    QStyle *style = QApplication::style();

    // 获取默认的上箭头和下箭头图标
    const QIcon upIcon = style->standardIcon(QStyle::SP_ArrowUp);
    const QIcon downIcon = style->standardIcon(QStyle::SP_ArrowDown);

    // 获取 QDateTimeEdit 控件
    QDateTimeEdit *dateTimeEditStart = ui->date_Start;
    QDateTimeEdit *dateTimeEditEnd = ui->date_End;
    // 设置上下箭头图标
    QAbstractSpinBox::ButtonSymbols buttonSymbols = QAbstractSpinBox::UpDownArrows;
    dateTimeEditStart->setButtonSymbols(buttonSymbols);  // 设置为上下箭头按钮
    dateTimeEditEnd->setButtonSymbols(buttonSymbols);  // 设置为上下箭头按钮
    // 通过样式表自定义箭头的显示
    QString styleSheet = QString(
                "QDateTimeEdit::down-button {"
                "    image: url(:/down.png);"
                "}"
                "QDateTimeEdit::up-button {"
                "    image: url(:/up.png);"
                "}"
                ).arg(upIcon.name(), downIcon.name());

    dateTimeEditStart->setStyleSheet(styleSheet);
    dateTimeEditEnd->setStyleSheet(styleSheet);
}

void HistoryFileList::on_pushButton_clicked()
{
    QWidget::close();
}

// 设置代码
//void HistoryFileList::setupListView()
//{
//    // 创建模型
//        model = new QStandardItemModel(this);
//        model->setHorizontalHeaderLabels({QStringLiteral("名称"), QStringLiteral("路径")});

//        // 设置模型
//        ui->treeView->setModel(model);

//        // 隐藏树状图标
//        ui->treeView->setRootIsDecorated(false);

//        // 设置列宽比例 1:3
//        ui->treeView->header()->setSectionResizeMode(0, QHeaderView::Fixed);
//        ui->treeView->header()->setSectionResizeMode(1, QHeaderView::Stretch);
//        ui->treeView->setColumnWidth(0, ui->treeView->width() / 4);  // 设置为总宽度的1/4

//        // 设置选择模式
//        ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
//        ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
//}

void HistoryFileList::setupListView()
{
    // 启用列点击排序
    ui->treeView->setSortingEnabled(true);
    // 设置列宽比例大约为 3:6:1
    int totalWidth = ui->treeView->width();
    ui->treeView->setColumnWidth(0, totalWidth * 0.4);  // 时间列
    ui->treeView->setColumnWidth(1, totalWidth * 0.5);  // 路径列
    ui->treeView->setColumnWidth(2, totalWidth * 0.1);  // 是否报警列
    // 创建模型
    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({QStringLiteral("时间"), QStringLiteral("路径"),QStringLiteral("气体")});

    // 设置模型
    ui->treeView->setModel(model);

    // 设置字体
    QFont font = ui->treeView->font();
    font.setPointSize(20);  // 设置字体大小
    ui->treeView->setFont(font);

    // 设置表头字体
    QFont headerFont = ui->treeView->header()->font();
    headerFont.setPointSize(20);
    headerFont.setBold(true);
    ui->treeView->header()->setFont(headerFont);

    // 设置样式表

    // 启用交替行颜色
    ui->treeView->setAlternatingRowColors(true);
    //        ui->treeView->setStyleSheet(
    //            "QTreeView {"
    //            "    background-color: white;"  // 默认背景为白色
    //            "}"

    //            "QTreeView::item {"
    //            "    background-color: white;"  // 默认每项背景为白色
    //            "}"

    //            "QTreeView::item:selected {"
    //            "    background-color: blue;"  // 选中项背景为蓝色
    //            "    color: white;"             // 选中项文字为白色
    //            "}"

    //            "QTreeView::item:alternate {"
    //            "    background-color: lightblue;"  // 交替行背景色为淡蓝色
    //            "}"
    //        );
    ui->treeView->setStyleSheet(
                "QTreeView {"
                "    background-color: white;"  // 默认背景为白色
                "}"

                "QTreeView::item {"
                "    background-color: white;"  // 默认每项背景为白色
                "}"

                // 确保选中项的背景色为蓝色，且文字为白色
                "QTreeView::item:selected {"
                "    background-color: blue;"  // 选中项背景为蓝色
                "    color: white;"             // 选中项文字为白色
                "}"

                // 设置交替行背景色
                "QTreeView::item:alternate {"
                "    background-color: lightblue;"  // 交替行背景色为淡蓝色
                "} "

                // 确保表头背景色和字体大小设置
                "QHeaderView::section {"
                "    background-color: #E6F3FF;"  // 表头背景色为淡蓝色
                "    padding: 4px;"
                "}"

                );

    // 获取 QTreeView 的垂直滚动条
    QScrollBar* verticalScrollBar = ui->treeView->verticalScrollBar();
    verticalScrollBar->setStyleSheet(
                "QScrollBar:vertical {"
                "    width: 50px;"  // 设置垂直滚动条宽度
                "    background: #f1f1f1;"  // 背景颜色
                "    border: 1px solid #d3d3d3;"  // 边框颜色
                "    border-radius: 5px;"  // 边框圆角
                "}"
                "QScrollBar::handle:vertical {"
                "    background: #1E90FF;"  // 滑块背景颜色 (深蓝色)
                "    border-radius: 5px;"  // 圆角滑块
                "}"
                "QScrollBar::handle:vertical:hover {"
                "    background: #4682B4;"  // 滑块悬浮时的背景颜色 (浅蓝色)
                "}"
                "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
                "    background: none;"  // 不显示增减按钮的背景
                "}"
                "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
                "    border: none;"  // 去掉箭头
                "}"
                );

    // 隐藏树状图标
    ui->treeView->setRootIsDecorated(false);
    // 先设置所有列为Interactive模式
    ui->treeView->header()->setSectionResizeMode(QHeaderView::Interactive);



    // 设置最小列宽
    // ui->treeView->header()->setMinimumSectionSize(100);

    // 路径列设置为自动扩展
    ui->treeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->treeView->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->treeView->header()->setSectionResizeMode(2, QHeaderView::Stretch);
    // 设置选择模式
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 连接数据变化信号以更新背景色
    connect(model, &QStandardItemModel::itemChanged, this, [this](QStandardItem *item) {
        // 获取 QTreeView 的选择模型
        QItemSelectionModel *selectionModel = ui->treeView->selectionModel();

        // 判断当前项是否被选中
        QModelIndex index = model->indexFromItem(item);
        if (selectionModel->isSelected(index)) {
            item->setBackground(QColor("blue"));  // 选中时背景设置为蓝色
            item->setForeground(Qt::white);       // 选中时字体设置为白色
        } else {
            item->setBackground(QColor("#E6F3FF"));  // 非选中项设置淡蓝色背景
        }
    });

    // 确保列宽设置生效
    ui->treeView->header()->setStretchLastSection(false);
}

void HistoryFileList::addRow(const QString& name, const QString& path, const QString& warning)
{
    if (!existingItems.contains(name)) {
        QList<QStandardItem*> items;
        items << new QStandardItem(name);
        items << new QStandardItem(path);
        items << new QStandardItem(warning);
        model->appendRow(items);
        // 设置时间列为可排序类型
        model->setSortRole(Qt::DisplayRole);  // 按显示内容进行排序
        ui->treeView->header()->setSortIndicator(0, Qt::AscendingOrder); // 初始排序为按时间升序

        // 启动排序
        ui->treeView->sortByColumn(0, Qt::AscendingOrder);
    }
}


int HistoryFileList::getDirectoryFiles(const QString& relativePath,
                                       std::vector<QString>& absolutePaths,
                                       std::vector<QString>& fileNames) {
    // 清空输入的vectors
    absolutePaths.clear();
    fileNames.clear();

    // 获取应用程序当前目录
    QDir currentDir(QDir::current());

    // 转到相对路径目录
    if (!currentDir.cd(relativePath)) {
        qDebug() << QStringLiteral("无法访问目录:") << relativePath;
        return 0;
    }

    // 设置过滤器，只获取文件（不包括文件夹）
    QFileInfoList fileList = currentDir.entryInfoList(
                QDir::Files | QDir::NoDotAndDotDot,  // 只要文件，不要.和..
                QDir::Name  // 按名称排序
                );

    // 遍历所有文件
    for (const QFileInfo& fileInfo : fileList) {
        absolutePaths.push_back(fileInfo.absoluteFilePath());
        fileNames.push_back(fileInfo.fileName());
    }

    return fileList.size();
}

void HistoryFileList::on_treeView_doubleClicked(const QModelIndex &index)
{
    // 获取同行第二列的索引
    int curindex = ui->comboBox->currentIndex();

    QModelIndexList selectedIndexes = ui->treeView->selectionModel()->selectedIndexes();
    QString text_jb;

    QModelIndex secondColumnIndex = index.sibling(index.row(), 1);
    QString text_path = secondColumnIndex.data().toString();
    secondColumnIndex = index.sibling(index.row(), 0);
    QString text_name = secondColumnIndex.data().toString();

    secondColumnIndex = index.sibling(index.row(), 2);
    text_jb = secondColumnIndex.data().toString();
    if(curindex==1){
        if (text_jb==QStringLiteral(WarningT) || text_jb==QStringLiteral(NormalT)) {
        this->hide();
        emit FilePath(text_path,text_name);
        QWidget::close();
        }
    }
}

static bool isValidRange(const QDateTime& startDateTime, const QDateTime& endDateTime)
{
    // 检查日期时间是否有效，并且确保开始时间早于结束时间
    return startDateTime.isValid() && endDateTime.isValid() && startDateTime <= endDateTime;
}

// 检查日期时间是否在指定的范围内
static bool isDateInRange(const QDateTime& dateTime, const QDateTime& startDateTime, const QDateTime& endDateTime)
{
    // 如果时间范围无效，则直接返回 false
    if (!isValidRange(startDateTime, endDateTime)) {
        return false;
    }

    // 检查给定的日期时间是否在开始时间和结束时间之间
    return (dateTime >= startDateTime) && (dateTime <= endDateTime);
}

static QDateTime toDate(const QString& dateStr)
{
    // 去除可能的 ".png" 后缀
    QString processedDateStr = dateStr;
    if (processedDateStr.endsWith(".png")) {
        processedDateStr.chop(4);  // 去掉最后的 ".png"
    }
    if (processedDateStr.endsWith("Y")) {
        processedDateStr.chop(1);  // 去掉最后的 "Y"
    }
    return QDateTime::fromString(processedDateStr, "yyyy_MM_dd_HH_mm_ss");
}

void HistoryFileList::FindRepeat()//去重
{
    // 清空 QSet
    existingItems.clear();
    // 遍历模型中的所有项，获取第一列的内容
    for (int row = 0; row < model->rowCount(); ++row) {
        QStandardItem *item = model->item(row, 0);  // 获取第一列的项
        if (item) {
            existingItems.insert(item->text());  // 将第一列的文本加入到集合中
        }
    }
}

void HistoryFileList::on_pushButton_2_clicked()
{

    if (model) {
        // 清空模型中的所有项
        model->removeRows(0, model->rowCount());
    }
    //去重
    FindRepeat();

    QDateTime date_Start = ui->date_Start->dateTime();
    QDateTime date_End = ui->date_End->dateTime();

    int FindSF6index = ui->comboBox->currentIndex();//0-警报图片 1-原始数据
    //bool isFindSF6 = ui->FindSF6->isChecked();

    // 读取 "images" 子目录下的所有文件
    int fileCount=0;
    if(FindSF6index==1)
        fileCount = getDirectoryFiles("PicData", absolutePaths, fileNames); // 原始数据读取
    else
        fileCount = getDirectoryFiles("GasImg", absolutePaths, fileNames);//报警数据图片
    QDateTime date1;
    if (fileCount > 0) {
        for(int j=0;j<fileCount;j++){
            date1=toDate(fileNames[j]);
            if (isValidRange(date_Start, date_End)) {
                if (isDateInRange(date1, date_Start, date_End) ) {
                    if(FindSF6index==0){
                        addRow(fileNames[j],absolutePaths[j],QStringLiteral("未知气体"));
                    }else if(FindSF6index==1){
                        if(fileNames[j].right(1)=="Y"){
                            addRow(fileNames[j],absolutePaths[j],QStringLiteral(WarningT));
                        }else{
                            addRow(fileNames[j],absolutePaths[j],QStringLiteral(NormalT));
                        }
                    }
                }
            }

        }
    } else {
        qDebug() << QStringLiteral("目录为空或不存在");
    }

}


void HistoryFileList::on_pushButton_3_clicked()
{
    int curindex = ui->comboBox->currentIndex();

    QModelIndexList selectedIndexes = ui->treeView->selectionModel()->selectedIndexes();
    QString text_path;
    if (!selectedIndexes.isEmpty()) {
        // 获取第一个选中项的索引
        QModelIndex index = selectedIndexes.first();
        // 确保当前索引所在的列是第三列（索引从0开始，第三列是2）
        QModelIndex secondColumnIndex = index.sibling(index.row(), 2);
        text_path = secondColumnIndex.data().toString();

    }

    if(curindex==1){
        if (text_path==QStringLiteral(WarningT) || text_path==QStringLiteral(NormalT)) {


            QModelIndex currentIndex = ui->treeView->currentIndex();
            QString text_path,text_name;
            if (currentIndex.isValid()) {
                text_path = currentIndex.sibling(currentIndex.row(), 1).data().toString();
                text_name = currentIndex.sibling(currentIndex.row(), 0).data().toString();
            }
            this->hide();
            emit FilePath(text_path,text_name);
            QWidget::close();
        }
    }else{
        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("请查询原始数据信息，在进行复检！"));
    }
}


void HistoryFileList::on_treeView_clicked(const QModelIndex &index)
{
    if(ui->comboBox->currentIndex()!=0)
        return;
        // 获取屏幕大小
        QList<QScreen *> screens = QGuiApplication::screens();
            QScreen *primaryScreen = screens.first(); // 获取主屏幕
            QRect screenGeometry = primaryScreen->geometry(); // 获取屏幕的几何信息
            int screenWidth = screenGeometry.width();
            int screenHeight =screenGeometry.height();
            // 获取OpenCV窗口的大小
            int windowWidth = 640;
            int windowHeight = 512;

            // 计算窗口左上角坐标，使其居中
               int x = (screenWidth - windowWidth) / 2;
               int y = (screenHeight - windowHeight) / 2;

    // 获取点击行的第一列内容
    QModelIndex firstColumnIndex = model->index(index.row(), 1);  // 1 表示第二列
    QString firstColumnPath = model->data(firstColumnIndex).toString();

    // 图片路径
    std::string imagePath = firstColumnPath.toStdString();  // 替换为你的图片路径

    // 读取图片
    cv::Mat img = cv::imread(imagePath);

    if (img.empty()) {
        qDebug()<< "Failed to load image!" ;
        return;
    }

    // 使用 imshow 显示图片
    cv::imshow("Pic", img);
    cv::moveWindow("Pic",x,y);
    // 等待按键事件，如果按下任意键或者点击关闭按钮，则退出
    cv::waitKey(1);  // 等待直到用户按下一个键
    // 获取窗口句柄（使用 FindWindow 和窗口标题）
    HWND hwnd = FindWindowA(NULL, "Pic");

    if (hwnd != NULL) {
        // 将窗口置顶
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        qDebug() << "Window is set to topmost." ;
    } else {
        qDebug() << "Failed to get window handle!";

    }
}

// 重载鼠标点击事件
//void HistoryFileList::mousePressEvent(QMouseEvent* event)
//{
//    // 当窗口被点击时，关闭OpenCV显示的图片窗口
//   // std::cout << "Window clicked, closing OpenCV window." << std::endl;

//    // 使用 OpenCV 关闭显示的窗口
//    cv::destroyWindow("Pic");


//}

void HistoryFileList::on_pushButton_4_clicked()
{

    QMessageBox msgBox;
    msgBox.setWindowTitle(QStringLiteral("提示"));
    msgBox.setText(QStringLiteral("是否继续删除操作？"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    int ret = msgBox.exec();
    if (ret == QMessageBox::Yes) {
        qDebug() << QStringLiteral("正在删除。。。");
        QSet<QString> exisdelete;

        // 获取 QTreeView 的选择模型
        QItemSelectionModel *selectionModel = ui->treeView->selectionModel();

        // 获取当前选中的索引列表
        QModelIndexList selectedIndexes = selectionModel->selectedIndexes();

        // 遍历选中的索引
        for (const QModelIndex &index : selectedIndexes) {
            // 只获取第二列（路径列）的值
            if (index.column() == 1) {  // 假设路径列在第二列，列索引为 1
                QStandardItem *item = model->itemFromIndex(index);
                if (item) {
                    QString filePath = item->text();  // 获取路径
                    exisdelete.insert(filePath);  // 将路径插入 QSet
                }
            }
        }


        deleteFilesFromSet(exisdelete);
        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("删除操作结完成！"));
    }

}

void HistoryFileList::deleteFilesFromSet(QSet<QString> &exisdelete) {
    // 遍历 QSet 中的所有路径
    for (const QString &filePath : exisdelete) {
        QFile file(filePath);  // 创建 QFile 对象
        if (file.exists()) {
            // 如果文件存在，则删除文件
            if (file.remove()) {
                qDebug() << QStringLiteral("成功删除文件：") << filePath;
            } else {
                qDebug() << QStringLiteral("无法删除文件：") << filePath;
            }
        } else {
            qDebug() << QStringLiteral("文件不存在：") << filePath;
        }
    }

    // 清空 QSet
    exisdelete.clear();
    on_pushButton_2_clicked();
}

void HistoryFileList::on_delete_all_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(QStringLiteral("提示"));
    msgBox.setText(QStringLiteral("是否删除当前显示所有文件？"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    int ret = msgBox.exec();
    if (ret == QMessageBox::Yes) {
        qDebug() << QStringLiteral("正在删除。。。");
        QSet<QString> exisdelete;
        for (int row = 0; row < model->rowCount(); ++row) {
            QStandardItem *item = model->item(row, 1);  // 获取第二列的项
            if (item) {
                exisdelete.insert(item->text());  // 将第一列的文本加入到集合中
            }
        }

        deleteFilesFromSet(exisdelete);//将此删除方法改成treeview选择的所有项删除

        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("删除操作结完成！"));
    }
}

