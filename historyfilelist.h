#ifndef HISTORYFILELIST_H
#define HISTORYFILELIST_H

#include <QWidget>
#include <QDir>
#include <QFileInfo>
#include <vector>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QSet>
#include <QMouseEvent>
#include <QApplication>
#include <QWidget>
namespace Ui {
class HistoryFileList;
}

class HistoryFileList : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryFileList(QWidget *parent = nullptr);
    ~HistoryFileList();

public:
    void HistoryFileList::setupDateTimeEdit();//datetimeedit
    void deleteFilesFromSet(QSet<QString> &exisdelete);
    void FindRepeat();
    void setupListView();
    void addRow(const QString& name, const QString& path, const QString& warning);
private slots:
    void on_pushButton_clicked();
    int getDirectoryFiles(const QString& relativePath,
                          std::vector<QString>& absolutePaths,
                          std::vector<QString>& fileNames);

    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_treeView_clicked(const QModelIndex &index);

    void on_pushButton_4_clicked();

    void on_delete_all_clicked();

private:
    Ui::HistoryFileList *ui;
    // 添加模型成员变量
    QStandardItemModel* model;
    std::vector<QString> absolutePaths;
    std::vector<QString> fileNames;
    // 存储所有第一列的内容（假设是名称或路径）
    QSet<QString> existingItems;
signals:
    void FilePath(QString path,QString name); // 信号，用于通知气体检测结果

protected:
    // 重载鼠标点击事件
    //  void mousePressEvent(QMouseEvent* event) override;
};

#endif // HISTORYFILELIST_H
