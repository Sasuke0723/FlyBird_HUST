#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include "bird.h"
#include "pipe.h"
// 新增：引入文字项头文件（Qt图形视图的文字组件）
#include <QGraphicsTextItem>

class Game : public QGraphicsView {
    Q_OBJECT
public:
    Game(QWidget* parent = nullptr);
    void keyPressEvent(QKeyEvent* event);
    void restartGame();
private slots:
    void gameLoop();

private:
    QGraphicsScene* scene;
    QGraphicsTextItem* scoreText;
    // 新增：制作人名字文本项（作为成员变量，方便控制）
    QGraphicsTextItem* producerText;
    Bird* bird;
    QTimer* timer;
    QList<Pipe*> pipes;
    int score;
    bool isGameOver;
};

#endif // GAME_H
