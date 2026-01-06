#include "game.h"
#include <QKeyEvent>
#include <QGraphicsTextItem>
#include <QIcon>
#include <QFont>
// 新增：引入颜色头文件（仅用于设置文字半透明，不修改其他逻辑）
#include <QColor>

Game::Game(QWidget* parent) : QGraphicsView(parent), score(0), gameState(0), countdownTimer(new QTimer(this)), countdownValue(0), countdownText(nullptr) {
    scene = new QGraphicsScene(this);
    setScene(scene);

    setWindowTitle("Ikun牌小鸟");

    QIcon icon(":/assets/images/bluebird-upflap.png");
    setWindowIcon(icon);

    bird = new Bird();
    // 先不添加到场景，等游戏开始再加
    bird->setPos(100, 300);

    // 定时器，控制游戏循环
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Game::gameLoop);

    setFixedSize(400, 600);
    scene->setSceneRect(0, 0, 400, 600);

    scene->setBackgroundBrush(QBrush(QImage(":/assets/images/background-day.png").scaled(400, 600)));

    // 取消滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 创建并显示分数文本项
    scoreText = new QGraphicsTextItem(QString("分数: %1").arg(score));
    //放在最前面
    scoreText->setZValue(1);
    scoreText->setDefaultTextColor(Qt::white);
    scoreText->setFont(QFont("Microsoft YaHei", 20));
    scoreText->setPos(10, 10);
    scene->addItem(scoreText);

    // 添加开始提示 - 修改为红色
    startText = new QGraphicsTextItem("按空格键开始游戏");
    startText->setDefaultTextColor(Qt::red);  // 改为红色
    startText->setFont(QFont("Microsoft YaHei", 16, QFont::Bold));
    startText->setPos(width() / 2 - startText->boundingRect().width() / 2,
        height() / 2 - 30);
    scene->addItem(startText);

    // ========== 仅新增：6个制作人名字竖向显示（不修改其他任何代码） ==========
    // 1. 定义6个制作人名字列表
    QStringList producerNames = {
        "史喆元",
        "黄骊文",
        "吴钟荣",
        "曹灿",
        "宋正阳",
        "熊圣洲"
    };

    // 2. 配置名字文字样式（适配中文、半透明）
    QFont producerFont("Microsoft YaHei", 18, QFont::Bold); // 微软雅黑+加粗
    QColor textColor(255, 255, 255, 200); // 白色半透明（不遮挡游戏元素）

    // 3. 计算排列参数（竖向排列、右对齐）
    int nameSpacing = 32; // 名字间距（适配18号字体，避免重叠）
    int startY = this->height() - 30; // 第一个名字的初始Y坐标（窗口底部）

    // 4. 循环创建每个名字的文本项
    for (int i = 0; i < producerNames.size(); ++i) {
        QGraphicsTextItem* producerText = new QGraphicsTextItem(producerNames[i]);

        // 设置样式
        producerText->setZValue(0); // 层级低于分数/小鸟，不遮挡核心元素
        producerText->setDefaultTextColor(textColor);
        producerText->setFont(producerFont);

        // 计算坐标：右对齐（X=窗口宽度-文字宽度-内边距），竖向排列（Y递减）
        qreal textX = this->width() - producerText->boundingRect().width() - 10; // 右内边距10px
        qreal textY = startY - (i * nameSpacing); // 向上排列

        // 添加到场景
        producerText->setPos(textX, textY);
        scene->addItem(producerText);
    }
    // ========== 制作人名字绘制结束 ==========

    // 初始状态为等待开始，定时器不启动
    timer->stop();
}

void Game::startGame() {
    gameState = 1;  // 切换到游戏中状态

    // 移除开始提示（只出现一次）
    if (startText) {
        scene->removeItem(startText);
        delete startText;
        startText = nullptr;  // 设置为空指针，确保只出现一次
    }

    // 将小鸟添加到场景
    if (!bird->scene()) {
        scene->addItem(bird);
    }

    // 启动游戏循环
    timer->start(20);
    bird->reset();

    connect(countdownTimer, &QTimer::timeout, this, &Game::updateCountdown);
}

void Game::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Space) {
        if (gameState == 0) {      // 等待开始状态
            startGame();
        }
        else if (gameState == 1 && !countdownTimer->isActive()) { // 游戏中状态
            bird->flap();
        }
        else if (gameState == 2) { // 游戏结束状态
            restartGame();
        }
    }
}

void Game::restartGame()
{
    // 清除场景中的管道和文本
    for (Pipe* pipe : pipes) {
        scene->removeItem(pipe);
        delete pipe;
    }
    pipes.clear();

    // 重置小鸟的位置和状态
    bird->setPos(100, 300);
    bird->reset();

    // 重置分数
    score = 0;
    scoreText->setPlainText(QString("分数: %1").arg(score));

    // 移除 Game Over 画面和重新开始提示
    QList<QGraphicsItem*> items = scene->items();
    QList<QGraphicsItem*> itemsToRemove;  // 先收集要删除的项

    for (QGraphicsItem* item : items) {
        if (QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(item))
        {
            if (pixmapItem->pixmap().cacheKey() == QPixmap(":/assets/images/gameover.png").cacheKey())
            {
                itemsToRemove.append(pixmapItem);
            }
        }
        if (QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item)) {
            if (textItem->toPlainText() == "按空格键重新开始") {
                itemsToRemove.append(textItem);
            }
        }
    }

    // 安全删除收集的项目
    for (QGraphicsItem* item : itemsToRemove) {
        scene->removeItem(item);
        delete item;
    }

    // 重置游戏状态为游戏中，直接开始游戏
    gameState = 1;

    // 注意：这里不再添加开始提示，确保只出现一次

    // 启动定时器，直接开始游戏
    //timer->start(20);
    countdownValue = 3;  // 倒计时初始值设为3
    // 创建/更新倒计时文本，显示在屏幕正中间
    if (countdownText) {
        scene->removeItem(countdownText);
        delete countdownText;
    }
    countdownText = new QGraphicsTextItem(QString::number(countdownValue));
    countdownText->setDefaultTextColor(Qt::red);  // 红色字体更醒目
    countdownText->setFont(QFont("Arial", 48, QFont::Bold));  // 大号粗体
    // 居中显示
    qreal x = (this->width() - countdownText->boundingRect().width()) / 2;
    qreal y = (this->height() - countdownText->boundingRect().height()) / 2;
    countdownText->setPos(x, y);
    countdownText->setZValue(2);  // 显示在最上层
    scene->addItem(countdownText);

    // 启动倒计时定时器（1秒触发一次）
    countdownTimer->start(1000);
}

// 新增：倒计时更新槽函数
void Game::updateCountdown() {
    countdownValue--;
    if (countdownValue > 0) {
        // 更新倒计时数字
        countdownText->setPlainText(QString::number(countdownValue));
        // 重新居中（数字宽度变化）
        qreal x = (this->width() - countdownText->boundingRect().width()) / 2;
        qreal y = (this->height() - countdownText->boundingRect().height()) / 2;
        countdownText->setPos(x, y);
    }
    else {
        // 倒计时结束：移除倒计时文本，启动游戏主循环
        scene->removeItem(countdownText);
        delete countdownText;
        countdownText = nullptr;
        countdownTimer->stop();  // 停止倒计时定时器
        timer->start(20);        // 启动游戏循环
    }
}

void Game::gameLoop() {
    // 只在游戏中状态运行
    if (gameState != 1) {
        return;
    }

    bird->updatePosition();

    // 生成新的管道
    if (pipes.isEmpty() || pipes.last()->x() < 200) {
        Pipe* pipe = new Pipe();
        pipes.append(pipe);
        scene->addItem(pipe);
    }

    // 管道移动与检测碰撞
    auto it = pipes.begin();
    while (it != pipes.end()) {
        Pipe* pipe = *it;
        pipe->movePipe();

        // 检测与小鸟的碰撞
        if (bird->collidesWithItem(pipe)) {
            timer->stop();
            gameState = 2;  // 设置为游戏结束状态

            QGraphicsPixmapItem* gameOverItem = scene->addPixmap(QPixmap(":/assets/images/gameover.png"));
            // 将 Game Over 画面放在中间位置
            gameOverItem->setPos(this->width() / 2 - gameOverItem->pixmap().width() / 2,
                this->height() / 2 - gameOverItem->pixmap().height() / 2);

            // 提示按空格重新游戏 - 字体改小，颜色改黑
            QGraphicsTextItem* restartText = new QGraphicsTextItem("按空格键重新开始");
            restartText->setDefaultTextColor(Qt::black);  // 黑色
            restartText->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));  // 字体改小为10号
            // 放在中间
            restartText->setPos(this->width() / 2 - restartText->boundingRect().width() / 2,
                this->height() / 2 + gameOverItem->pixmap().height() / 2 + 5);  // 位置微调

            scene->addItem(restartText);
            return;
        }

        // 如果小鸟通过了管道（即小鸟的x坐标刚好超过管道的x坐标）
        if (pipe->x() + pipe->boundingRect().width() < bird->x() && !pipe->isPassed) {
            // 增加分数
            score++;
            pipe->isPassed = true;  // 确保不会重复加分

            // 更新分数显示
            scoreText->setPlainText(QString("分数: %1").arg(score));
        }

        // 如果管道移出了屏幕，将其从场景和列表中删除
        if (pipe->x() < -60) {
            scene->removeItem(pipe);
            delete pipe;
            it = pipes.erase(it);  // 从列表中安全移除管道
        }
        else {
            ++it;  // 继续遍历
        }
    }
}

