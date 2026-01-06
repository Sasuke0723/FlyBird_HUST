#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include "bird.h"
#include "pipe.h"
#include <QElapsedTimer>
#include <QGraphicsTextItem>
#include <QElapsedTimer>

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
    Bird* bird;
    QTimer* timer;
    QGraphicsTextItem* startText;
    int score;
    int gameState;
    QTimer* countdownTimer;
    int countdownValue;
    QGraphicsTextItem* countdownText;
    QList<Pipe*> pipes;
    bool isGameOver;
    void startGame();
    QElapsedTimer fpsTimer;
    int frameCount = 0;
    QGraphicsTextItem* fpsText = nullptr;
};

#endif // GAME_H

