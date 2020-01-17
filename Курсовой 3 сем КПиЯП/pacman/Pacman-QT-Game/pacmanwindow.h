#ifndef PACMANWINDOW_H
#define PACMANWINDOW_H


#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QSound>
#include <QSoundEffect>
#include "pacman.h"
#include "pacmanboard.h"
#include "ball.h"
#include "ghost.h"
#include "powerball.h"
#include "textdrawing.h"
#include <QShortcut>
#include <fstream>
#include "list.h"


namespace Ui {
class Pacmanwindow;
}

// класс, описывающий поведение игрового процесса

class Pacmanwindow : public QDialog
{
    Q_OBJECT

public:

    explicit Pacmanwindow(QString, QWidget *parent = 0);
    void pacman_move();
    void ghostsmove();
    void ghostsmove1();
    void ghostsmove2();
    void ghostsmove3();
    void moveghostsinrect3();
    void moveghostsinrect2();
    void moveghostsinrect1();
    void moveghostsinrect();
    void checklost();
    void delay();
    void start_Game();
    void end_Game();
    void pause();
    void retry();
    void easy();
    void medium();
    void hard();
    void parseMessage(QString dataIn, int);


    Pacman *pacman;
    Ghost *ghost;
    Ghost *ghost1;
    Ghost *ghost2;
    Ghost *ghost3;


    PacmanBoard *pac_map;
    Ball *ball;
    PowerBall *powerball;
    TextDrawing *text;
    QString name, difficultyText; // текст, используемый для названия и сложности на экране
    QString user;
    // переменные отвечающие за расположение всех элементов игры
    int pacx,pacy,direction,nextdirection;
    int gosx,gosy,ghostdir,nextghostdir;
    int gosx1,gosy1,ghostdir1,nextghostdir1;
    int gosx2,gosy2,ghostdir2,nextghostdir2;
    int gosx3,gosy3,ghostdir3,nextghostdir3;
    // в этих переменыых лежат состояния игры
    bool moving,ghostmoving,ghostmoving1,ghostmoving2,ghostmoving3;
    bool scared,scared1,scared2,scared3;
    int score,hiscore,state3,state2,state1,state,lives,level,difficulty;
    int starting;
    bool start;
    bool ghoststart,ghoststart1,ghoststart2,ghoststart3;
    bool playing;
    bool isPaused;
    QVector<QPoint> ballpoints; // вектор содержащий все шары, которые нужно собрать
    QVector<QPoint> Powerballpoints; // вектор, содержащий, все усиляющие шары
    QSound *waka;
    ~Pacmanwindow();
public slots:
    void updater();
    void ghostupdater();
protected:
    void keyPressEvent(QKeyEvent *event);



private slots:

    int timeoutTime() { return 1000 / (2); }
    void on_pauseButton_clicked();
    void on_controlsButton_clicked();


private:
    Ui::Pacmanwindow *ui;
    QGraphicsScene *scene;
    QTimer *timer;
    QTimer *ghoststimer;
    };

#endif // PACMANWINDOW_H
