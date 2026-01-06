// flybird/mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent> // 新增：引入绘图事件头文件
#include <QPainter>    // 新增：引入绘图类
#include <QFont>       // 新增：引入字体类

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // 新增：重写paintEvent绘图函数
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
