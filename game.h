#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include "bird.h"
#include "pipe.h"

class Game : public QGraphicsView {
    Q_OBJECT
public:
    Game(QWidget* parent = nullptr);
    void keyPressEvent(QKeyEvent* event);
    void restartGame();
private slots:
    void gameLoop();
    void updateCountdown();

private:
    QGraphicsScene* scene;
    QGraphicsTextItem* scoreText;
    QTimer* countdownTimer;
    int countdownValue;
    QGraphicsTextItem* countdownText;
    Bird* bird;
    QTimer* timer;
    QGraphicsTextItem* startText;
    int gameState;
    QList<Pipe*> pipes;
    int score;
    bool isGameOver;
    void startGame();
};

#endif // GAME_H