// flybird/mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 可选：设置窗口大小固定（匹配游戏背景）
    this->setFixedSize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 新增：实现paintEvent绘图函数
void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // ========== 1. 绘制游戏背景（优先用项目已有背景图） ==========
    // 方式1：如果项目有背景图（assets/images/下），加载并绘制
    QPixmap bgImg(":/assets/images/background.png"); // 替换为实际背景图路径
    if (bgImg.isNull()) { // 若背景图不存在，用纯色填充背景
        painter.fillRect(this->rect(), QColor(135, 206, 235)); // 天蓝色背景
    } else {
        painter.drawPixmap(0, 0, this->width(), this->height(), bgImg);
    }

    // ========== 2. 绘制制作人名字（核心新增逻辑） ==========
    // 设置字体样式
    QFont nameFont;
    nameFont.setFamily("Microsoft YaHei"); // 字体（兼容Windows）
    nameFont.setPixelSize(24);            // 字体大小
    nameFont.setBold(true);               // 加粗
    painter.setFont(nameFont);

    // 设置文字颜色（白色半透明，避免遮挡背景）
    painter.setPen(QColor(255, 255, 255, 200));

    // 开启文字抗锯齿（可选，让文字更清晰）
    painter.setRenderHint(QPainter::TextAntialiasing);

    // 绘制文字（右下角位置，适配窗口大小）
    int textX = this->width() - 200;  // 右偏移200px
    int textY = this->height() - 40;  // 下偏移40px
    painter.drawText(textX, textY, "制作人：XXX"); // 替换为你的名字

    // ========== 3. 若需要叠加原有游戏逻辑（如小鸟、管道） ==========
    // 可调用game.cpp中的绘制函数，或把game.cpp的绘图逻辑移到这里
}
