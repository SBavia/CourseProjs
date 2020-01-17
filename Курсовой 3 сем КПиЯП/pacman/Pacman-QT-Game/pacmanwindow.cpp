#include "Pacmanwindow.h"
#include "ui_Pacmanwindow.h"
#include <QThread>
#include <QtWidgets>
#include "options.h"
#include "list.h"
#include "list.cpp"


// метод, достающий самый выскоий результат из файла
int get_high_score()
{

    List<int> scores; // используем собственный контейнер
                      // для хранения очков, считанных из файла
    ifstream file;
    file.open("users.txt", ios::in); // файл, в котором хранятся результаты
    // формат файла <ник> <кол-во набранных очков>
    if(file)
    {
    while(!file.eof())
    {
        string user;
        int score;

        if(!file.eof()) file >> user >> score;
        scores.push_front(score); // добавляем каждый результат в конец
    }
    file.close();
    scores.sort(); // сортируем
    return *(scores.end()); // последний самый большой
    }
    else return 0;
}

// класс, отвечающий за отрисовку окна, в котром проходит игровой процесс
Pacmanwindow::Pacmanwindow(QString nickname, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pacmanwindow)
{
    user = nickname; // имя юзера, нужно для записи статистики в файл

    score = 0; // счётчик очков

    hiscore = get_high_score(); // получаем лучший результат

    level = 0; // уровень сложности
    lives = 3;  // количество жизней
    starting = 0;

    // координаты призраков
    gosx=450/2;
    gosy=480/2;
    gosx1=450/2;
    gosy1=480/2;
    gosx2=450/2;
    gosy2=480/2;
    gosx3=450/2;
    gosy3=480/2;
    pacx=410/2;
    pacy=410;
    ghostmoving=false;
    ghostmoving1=false;
    ghostmoving2=false;
    ghostmoving3=false;
    scared=false;
    scared1=false;
    scared2=false;
    scared3=false;
    start=false;
    direction=0;
    moving=false;
    playing=false;
    isPaused = false;


    // настройка графической формы
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->scoreLabel->setText(name);// adds the name tp score label

    scene->setSceneRect(0,0,640,480);
    ui->graphicsView->setSceneRect(scene->sceneRect());
    // обработка нажатия esc для выхода из игры
    connect(ui->exitButton,SIGNAL(clicked()),qApp,SLOT(closeAllWindows()));
    ui->exitButton->setShortcut(QKeySequence("Escape"));


    text=new TextDrawing;
    text->over=false;
    text->paused=false;
    scene->addItem(text);
    text->setZValue(7);





}

void Pacmanwindow::parseMessage(QString name, int temp){

    this->name = name;
    ui->scoreLabel->setText("Score");
    difficulty = temp;
}


// метод, вызов которого начинает игры
void Pacmanwindow::start_Game()
{
    level = 0;
    direction = 0;
    playing = true;

    // установка координат призраков
    gosx=450/2;
    gosy=480/2;
    gosx1=450/2;
    gosy1=480/2;
    gosx2=450/2;
    gosy2=480/2;
    gosx3=450/2;
    gosy3=480/2;
    pacx=400/2;
    pacy=360;

    pacman=new Pacman;
    pac_map=new PacmanBoard;
    ghost=new Ghost;
    ghost1=new Ghost;
    ghost2=new Ghost;
    ghost3=new Ghost;

    // Установка цвета призраков
    ghost1->setColor("orange");
    ghost2->setColor("red");
    ghost3->setColor("blue");

    ball=new Ball;
    powerball=new PowerBall;
    ballpoints=pac_map->getballpoints();
    Powerballpoints=pac_map->getpowerballpoints();

    // установка всех игровых элементов на форму
    scene->removeItem(text);
    scene->addItem(pac_map);
    scene->addItem(pacman);
    scene->addItem(ghost);
    scene->addItem(ghost1);
    scene->addItem(ghost2);
    scene->addItem(ghost3);
    scene->addItem(text);

    text->playing=true;
    ghostmoving=false;
    ghostmoving1=false;
    ghostmoving2=false;
    ghostmoving3=false;
    moving=false;
    ghoststart=false;
    ghoststart1=false;
    ghoststart2=false;
    ghoststart3=false;

    state=0;
    state1=0;
    state2=0;
    state3=0;


    // проигрывание мелодии при начале

    waka->play(":/Sound/pacman_beginning.wav");


    // настройка сложности в зависимости от выбора в начальном меню
    if (starting==0)
    {
        if (difficulty == 1){
            easy();
            difficultyText = "easy";
        }
        if (difficulty == 2){
            medium();
            difficultyText = "medium";
        }
        if (difficulty == 3){
            hard();
            difficultyText = "hard";
        }

    }
}


// настройка игры при выборе сложности easy
void Pacmanwindow::easy(){

    // таймеры для плавной анимации
    timer = new QTimer(this);
    ghoststimer=new QTimer(this);

    // каждое срабатывание таймера отвечает за передвижение пакмана и призраков по карте
    connect(timer, SIGNAL(timeout()), this,SLOT(updater()));
    connect(ghoststimer, SIGNAL(timeout()), this,SLOT(ghostupdater()));
    this->show();
    scene->update();
    this->update();

    lives = 3;
    timer->start();
    timer->start(35);
    ghoststimer->start(45);
    this->setFocus();


}



void Pacmanwindow::medium(){
    lives =2;

    timer = new QTimer(this);
    ghoststimer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this,SLOT(updater()));
    connect(ghoststimer, SIGNAL(timeout()), this,SLOT(ghostupdater()));
    this->show();
    scene->update();
    this->update();


    timer->start();
    timer->start(50);
    ghoststimer->start(50);
    this->setFocus();


}
void Pacmanwindow::hard(){

    lives = 0;
    timer = new QTimer(this);
    ghoststimer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this,SLOT(updater()));
    connect(ghoststimer, SIGNAL(timeout()), this,SLOT(ghostupdater()));
    this->show();
    scene->update();
    this->update();

    timer->start();
    timer->start(40);
    ghoststimer->start(35);
    this->setFocus();


}

// Данный метод вызывается в случае проийгрыша, когда число жизней станет меньше 0
void Pacmanwindow::end_Game()
{
    starting = 0;

    // очистка графической сцены
    scene->removeItem(pacman);
    scene->removeItem(ghost);
    scene->removeItem(ghost1);
    scene->removeItem(ghost2);
    scene->removeItem(ghost3);
    scene->removeItem(pac_map);

    text->playing=false;
    start = false;
    playing=false;
    scene->update();
    timer->stop();
    ghoststimer->stop();

    // Запись текущего результата в файл
    ofstream file;
    file.open("users.txt", std::ofstream::out | std::ofstream::app); // открывается файл для до записи

    file << this->user.toStdString() << " " << score << endl;
    file.close();
    score = 0; // обнуляем счетчик очков
}



// метод запуска повторной попытки

void Pacmanwindow::retry(){

    gosx=450/2;
    gosy=480/2;
    gosx1=450/2;
    gosy1=480/2;
    gosx2=450/2;
    gosy2=480/2;
    gosx3=450/2;
    gosy3=480/2;
    pacx=400/2;
    pacy=360;
    direction=4;
    start = true;
    ghoststart=false;
    ghoststart1=false;
    ghoststart2=false;
    ghoststart3=false;
    ghostmoving=false;
    ghostmoving1=false;
    ghostmoving2=false;
    ghostmoving3=false;
    moving=false;
    state=0;
    state1=0;
    state2=0;
    state3=0;

}


// метод отвечающий за передвижения пакмана по карте
void Pacmanwindow::pacman_move()
{
    QPoint p;

    if(nextdirection!=direction){
        switch(nextdirection)
        {
        case 1:
            p.setX(pacx-5);
            p.setY(pacy);

            if(pac_map->canmove(p)){
                direction=nextdirection;
                nextdirection=0;

            }

            break;

        case 4:
            p.setX(pacx+5);
            p.setY(pacy);
            if(pac_map->canmove(p)){
                direction=nextdirection;
                nextdirection=0;

            }

            break;
        case 3:
            p.setX(pacx);
            p.setY(pacy+5);
            if(pac_map->canmove(p)){
                direction=nextdirection;
                nextdirection=0;

            }
            break;
        case 2:
            p.setX(pacx);
            p.setY(pacy-5);
            if(pac_map->canmove(p)){
                direction=nextdirection;
                nextdirection=0;

            }

            break;
        }

    }
    switch(direction)
    {
    case 1:
        p.setX(pacx-5);
        p.setY(pacy);
        pacman->setDirection(direction);

        if(pac_map->canmove(p)){
            pacx-=5;


            moving=true;

        }else{
            moving=false;
        }

        break;

    case 4:
        pacman->setDirection(direction);
        p.setX(pacx+5);
        p.setY(pacy);
        if(pac_map->canmove(p)){
            pacx+=5;

            moving=true;

        }else{
            moving=false;
        }

        break;
    case 3:
        pacman->setDirection(direction);
        p.setX(pacx);
        p.setY(pacy+5);
        if(pac_map->canmove(p)){
            pacy+=5;
            moving=true;

        }else{
            moving=false;
        }

        break;
    case 2:
        pacman->setDirection(direction);
        p.setX(pacx);
        p.setY(pacy-5);
        if(pac_map->canmove(p)){
            pacy-=5;
            moving=true;

        }else{
            moving=false;
        }

        break;
    }

    // Телепортирование пакмана через боковые границы
    if(pacx<=0){
        pacx=450;
        pacy=230;
    }else if(pacx>=450){
        pacx=0;
        pacy=230;
    }


    pacman->setpacx(pacx);
    pacman->setpacy(pacy);


}


// передвижение призраков
void Pacmanwindow::ghostsmove()
{
    QPoint p;

    if(!ghostmoving){
        ghostdir=(qrand()%4)+1;
    }else{

        if(ghostdir==4 && gosy<pacy || ghostdir==1 && gosy>pacy){
            if(ghostdir==1 && gosy>pacy){
                nextghostdir=2;
            }else if(ghostdir==4 &&gosy<pacy){
                nextghostdir=3;
            }

        }else if(ghostdir==3 && gosx<pacx || ghostdir==2 && gosx>pacx){
            if(ghostdir==2 && gosx>pacx){
                nextghostdir=1;
            }else if(ghostdir==3 && gosx<pacx){
                nextghostdir=4;
            }
        }

    }

    if(nextghostdir!=ghostdir){
        switch(nextghostdir)
        {
        case 1:
            p.setX(gosx-5);
            p.setY(gosy);

            if(pac_map->canmove(p)){
                ghostdir=nextghostdir;
                nextghostdir=0;

            }

            break;

        case 4:
            p.setX(gosx+5);
            p.setY(gosy);
            if(pac_map->canmove(p)){
                ghostdir=nextghostdir;
                nextghostdir=0;

            }

            break;
        case 3:
            p.setX(gosx);
            p.setY(gosy+5);
            if(pac_map->canmove(p)){
                ghostdir=nextghostdir;
                nextghostdir=0;

            }

            break;
        case 2:
            p.setX(gosx);
            p.setY(gosy-5);
            if(pac_map->canmove(p)){
                ghostdir=nextghostdir;
                nextghostdir=0;

            }

            break;
        }

    }
    switch(ghostdir)
    {
    case 1:
        p.setX(gosx-5);
        p.setY(gosy);
        ghost->setDirection(ghostdir);

        if(pac_map->canmove(p)){
            gosx-=5;


            ghostmoving=true;

        }else{
            ghostmoving=false;
        }

        break;

    case 4:
        ghost->setDirection(ghostdir);
        p.setX(gosx+5);
        p.setY(gosy);
        if(pac_map->canmove(p)){
            gosx+=5;

            ghostmoving=true;

        }else{
            ghostmoving=false;
        }

        break;
    case 3:
        ghost->setDirection(ghostdir);
        p.setX(gosx);
        p.setY(gosy+5);
        if(pac_map->canmove(p)){
            gosy+=5;
            ghostmoving=true;

        }else{
            ghostmoving=false;
        }

        break;
    case 2:
        ghost->setDirection(ghostdir);
        p.setX(gosx);
        p.setY(gosy-5);
        if(pac_map->canmove(p)){
            gosy-=5;
            ghostmoving=true;

        }else{
            ghostmoving=false;
        }

        break;
    }


    if(gosx<=0){
        gosx=450;
        gosy=230;
    }else if(gosx>=450){
        gosx=0;
        gosy=230;
    }
    ghost->setgosx(gosx);
    ghost->setgosy(gosy);

}



void Pacmanwindow::ghostsmove1()
{
    QPoint p;



    if(!ghostmoving1){
        ghostdir1=(qrand()%4)+1;
    }else{
        if(ghostdir1==4 && gosy1<pacy || ghostdir1==1 && gosy1>pacy){
            if(ghostdir1==1 && gosy>pacy){
                nextghostdir1=2;
            }else if(ghostdir1==4 &&gosy1<pacy){
                nextghostdir1=3;
            }

        }else if(ghostdir1==3 && gosx1<pacx || ghostdir1==2 && gosx1>pacx){
            if(ghostdir1==2 && gosx1>pacx){
                nextghostdir1=1;
            }else if(ghostdir1==3 && gosx1<pacx){
                nextghostdir1=4;
            }
        }

    }


    if(nextghostdir1!=ghostdir1){
        switch(nextghostdir1)
        {
        case 1:
            p.setX(gosx1-5);
            p.setY(gosy1);

            if(pac_map->canmove(p)){
                ghostdir1=nextghostdir1;
                nextghostdir1=0;

            }

            break;

        case 4:
            p.setX(gosx1+5);
            p.setY(gosy1);
            if(pac_map->canmove(p)){
                ghostdir1=nextghostdir1;
                nextghostdir1=0;

            }

            break;
        case 3:
            p.setX(gosx1);
            p.setY(gosy1+5);
            if(pac_map->canmove(p)){
                ghostdir1=nextghostdir1;
                nextghostdir1=0;

            }

            break;
        case 2:
            p.setX(gosx1);
            p.setY(gosy1-5);
            if(pac_map->canmove(p)){
                ghostdir1=nextghostdir1;
                nextghostdir1=0;

            }

            break;
        }

    }
    switch(ghostdir1)
    {
    case 1:
        p.setX(gosx1-5);
        p.setY(gosy1);
        ghost1->setDirection(ghostdir1);

        if(pac_map->canmove(p)){
            gosx1-=5;


            ghostmoving1=true;

        }else{
            ghostmoving1=false;
        }

        break;

    case 4:
        ghost1->setDirection(ghostdir1);
        p.setX(gosx1+5);
        p.setY(gosy1);
        if(pac_map->canmove(p)){
            gosx1+=5;

            ghostmoving1=true;

        }else{
            ghostmoving1=false;
        }

        break;
    case 3:
        ghost1->setDirection(ghostdir1);
        p.setX(gosx1);
        p.setY(gosy1+5);
        if(pac_map->canmove(p)){
            gosy1+=5;
            ghostmoving1=true;

        }else{
            ghostmoving1=false;
        }

        break;
    case 2:
        ghost1->setDirection(ghostdir1);
        p.setX(gosx1);
        p.setY(gosy1-5);
        if(pac_map->canmove(p)){
            gosy1-=5;
            ghostmoving1=true;

        }else{
            ghostmoving1=false;
        }

        break;
    }


    if(gosx1<=0){
        gosx1=450;
        gosy1=230;
    }else if(gosx1>=450){
        gosx1=0;
        gosy1=230;
    }
    ghost1->setgosx(gosx1);
    ghost1->setgosy(gosy1);

}

void Pacmanwindow::ghostsmove2()
{
    QPoint p;

    if(!ghostmoving2){
        ghostdir2=(qrand()%4)+1;
    }else{
        if(ghostdir2==4 &&gosy2<pacy || ghostdir2==1 && gosy2>pacy){
            if(ghostdir2==1 && gosy2>pacy){
                nextghostdir2=2;
            }else if(ghostdir2==4 &&gosy2<pacy){
                nextghostdir2=3;
            }

        }else if(ghostdir2==3 && gosx2<pacx || ghostdir2==2 && gosx2>pacx){
            if(ghostdir2==2 && gosx2>pacx){
                nextghostdir2=1;
            }else if(ghostdir2==3 && gosx2<pacx){
                nextghostdir2=4;
            }
        }

    }


    if(nextghostdir2!=ghostdir2){
        switch(nextghostdir2)
        {
        case 1:
            p.setX(gosx2-5);
            p.setY(gosy2);

            if(pac_map->canmove(p)){
                ghostdir2=nextghostdir2;
                nextghostdir2=0;

            }

            break;

        case 4:
            p.setX(gosx2+5);
            p.setY(gosy2);
            if(pac_map->canmove(p)){
                ghostdir2=nextghostdir2;
                nextghostdir2=0;

            }

            break;
        case 3:
            p.setX(gosx2);
            p.setY(gosy2+5);
            if(pac_map->canmove(p)){
                ghostdir2=nextghostdir2;
                nextghostdir2=0;

            }

            break;
        case 2:
            p.setX(gosx2);
            p.setY(gosy2-5);
            if(pac_map->canmove(p)){
                ghostdir2=nextghostdir2;
                nextghostdir2=0;

            }

            break;
        }

    }
    switch(ghostdir2)
    {
    case 1:
        p.setX(gosx2-5);
        p.setY(gosy2);
        ghost2->setDirection(ghostdir2);

        if(pac_map->canmove(p)){
            gosx2-=5;


            ghostmoving2=true;

        }else{
            ghostmoving2=false;
        }

        break;

    case 4:
        ghost2->setDirection(ghostdir2);
        p.setX(gosx2+5);
        p.setY(gosy2);
        if(pac_map->canmove(p)){
            gosx2+=5;

            ghostmoving2=true;

        }else{
            ghostmoving2=false;
        }

        break;
    case 3:
        ghost2->setDirection(ghostdir2);
        p.setX(gosx2);
        p.setY(gosy2+5);
        if(pac_map->canmove(p)){
            gosy2+=5;
            ghostmoving2=true;

        }else{
            ghostmoving2=false;
        }

        break;
    case 2:
        ghost2->setDirection(ghostdir2);
        p.setX(gosx2);
        p.setY(gosy2-5);
        if(pac_map->canmove(p)){
            gosy2-=5;
            ghostmoving2=true;

        }else{
            ghostmoving2=false;
        }

        break;
    }


    if(gosx2<=0){
        gosx2=450;
        gosy2=230;
    }else if(gosx2>=450){
        gosx2=0;
        gosy2=230;
    }
    ghost2->setgosx(gosx2);
    ghost2->setgosy(gosy2);

}

void Pacmanwindow::ghostsmove3()
{
    QPoint p,g;

    if(!ghostmoving3){
        ghostdir3=(qrand()%4)+1;
    }else{
        if(ghostdir3==4 &&gosy3<pacy || ghostdir3==1 && gosy3>pacy){
            if(ghostdir3==1 && gosy3>pacy){
                nextghostdir3=2;
            }else if(ghostdir3==4 &&gosy3<pacy){
                nextghostdir3=3;
            }

        }else if(ghostdir3==3 && gosx3<pacx || ghostdir3==2 && gosx3>pacx){
            if(ghostdir3==2 && gosx3>pacx){
                nextghostdir3=1;
            }else if(ghostdir3==3 && gosx3<pacx){
                nextghostdir3=4;
            }
        }

    }


    if(nextghostdir3!=ghostdir3){
        switch(nextghostdir3)
        {
        case 1:
            p.setX(gosx3-5);
            p.setY(gosy3);

            if(pac_map->canmove(p)){
                ghostdir3=nextghostdir3;
                nextghostdir3=0;

            }

            break;

        case 4:
            p.setX(gosx3+5);
            p.setY(gosy3);
            if(pac_map->canmove(p)){
                ghostdir3=nextghostdir3;
                nextghostdir3=0;

            }

            break;
        case 3:
            p.setX(gosx3);
            p.setY(gosy3+5);
            if(pac_map->canmove(p)){
                ghostdir3=nextghostdir3;
                nextghostdir3=0;

            }

            break;
        case 2:
            p.setX(gosx3);
            p.setY(gosy3-5);
            if(pac_map->canmove(p)){
                ghostdir3=nextghostdir3;
                nextghostdir3=0;

            }

            break;
        }

    }
    switch(ghostdir3)
    {
    case 1:
        p.setX(gosx3-5);
        p.setY(gosy3);
        ghost3->setDirection(ghostdir3);

        if(pac_map->canmove(p)){
            gosx3-=5;


            ghostmoving3=true;

        }else{
            ghostmoving3=false;
        }

        break;

    case 4:
        ghost3->setDirection(ghostdir3);
        p.setX(gosx3+5);
        p.setY(gosy3);
        if(pac_map->canmove(p)){
            gosx3+=5;

            ghostmoving3=true;

        }else{
            ghostmoving3=false;
        }

        break;
    case 3:
        ghost3->setDirection(ghostdir3);
        p.setX(gosx3);
        p.setY(gosy3+5);
        if(pac_map->canmove(p)){
            gosy3+=5;
            ghostmoving3=true;

        }else{
            ghostmoving3=false;
        }

        break;
    case 2:
        ghost3->setDirection(ghostdir3);
        p.setX(gosx3);
        p.setY(gosy3-5);
        if(pac_map->canmove(p)){
            gosy3-=5;
            ghostmoving3=true;

        }else{
            ghostmoving3=false;
        }

        break;
    }


    if(gosx3<=0){
        gosx3=450;
        gosy3=230;
    }else if(gosx3>=450){
        gosx3=0;
        gosy3=230;
    }
    ghost3->setgosx(gosx3);
    ghost3->setgosy(gosy3);

}

///// Keep ghosts in rectangles
void Pacmanwindow::moveghostsinrect3()
{
    if(gosx3==450/2+40 || gosx3==450/2-40){
        if(ghostdir3==4){
            ghostdir3=1;
        }else{
            ghostdir3=4;
        }
    }
    if(ghostdir3==4){

        gosx3-=2;

    }else{

        gosx3+=2;
    }




    ghost3->setgosx(gosx3);
    ghost3->setgosy(gosy3);
}
void Pacmanwindow::moveghostsinrect2()
{
    if(gosx2==450/2+40 || gosx2==450/2-40){
        if(ghostdir2==4){
            ghostdir2=1;
        }else{
            ghostdir2=4;
        }
    }
    if(ghostdir2==4){

        gosx2+=5;

    }else{

        gosx2-=5;
    }




    ghost2->setgosx(gosx2);
    ghost2->setgosy(gosy2);
}
void Pacmanwindow::moveghostsinrect1()
{
    if(gosx1==450/2+40 || gosx1==450/2-40){
        if(ghostdir1==4){
            ghostdir1=1;
        }else{
            ghostdir1=4;
        }
    }
    if(ghostdir1==4){

        gosx1+=5;

    }else{

        gosx1-=5;
    }




    ghost1->setgosx(gosx1);
    ghost1->setgosy(gosy1);
}
void Pacmanwindow::moveghostsinrect()
{
    if(gosx==450/2+40 || gosx==450/2-40){
        if(ghostdir==4){
            ghostdir=1;
        }else{
            ghostdir=4;
        }
    }
    if(ghostdir==4){

        gosx+=5;

    }else{

        gosx-=5;
    }




    ghost->setgosx(gosx);
    ghost->setgosy(gosy);
}


void Pacmanwindow::checklost()
{

    /// When all the points have been eaten start new game
    if(ballpoints.isEmpty() ){


        level += 1;
        starting += 1;

        start_Game();

        int temp = hiscore;
        hiscore = temp;
    }

    if(pacman->collidesWithItem(ghost) ||
            pacman->collidesWithItem(ghost1) ||
            pacman->collidesWithItem(ghost2) ||
            pacman->collidesWithItem(ghost3)){



        if(scared){
            // за каждого съеденого призрака по сотке
            score+=100;
            if(pacman->collidesWithItem(ghost)){
                gosx=450/2;
                gosy=450/2;
                ghoststart=false;
            }
            if(pacman->collidesWithItem(ghost1)){
                gosx1=450/2;
                gosy1=450/2;
                ghoststart1=false;
            }
            if(pacman->collidesWithItem(ghost2)){
                gosx2=450/2;
                gosy2=450/2;
                ghoststart2=false;
            }
            if(pacman->collidesWithItem(ghost3)){
                gosx3=450/2;
                gosy3=450/2;
                ghoststart3=false;
            }
            waka->play(":/Sound/pacman_eatghost.wav");

        }else{

            if (lives > 0){


          waka->play(":/Sound/pacman_death.wav");

            lives -= 1;
            retry();}
            else{
                scene->addItem(text);

                end_Game();

            }
        }


    }


}

// обработка нажатия клавиши

void Pacmanwindow::keyPressEvent(QKeyEvent *event)
{

    // если пробел то начинаем игру
    if(event->key()==Qt::Key_Space)
    {
        if(!playing){
            start_Game();
            playing=true;
        }else{

        }
    }


    if(gosx3==450/2){
        state++;


    }
    if(state==4){

        start=true;
        state=0;
    }
    if(!start){


    } else {

        switch(event->key())
        {

        case Qt::Key_Left:

            if(!moving){


                direction=1;

            }else{
                nextdirection=1;
            }

            break;
        case Qt::Key_Right:

            if(!moving){


                direction=4;

            }else{

                nextdirection=4;
            }

            break;
        case Qt::Key_Down:
            if(!moving){
                direction=3;
            }else{
                nextdirection=3;
            }



            break;
        case Qt::Key_Up:
            if(!moving){
                direction=2;
            }else{
                nextdirection=2;
            }
            break;


        case Qt::Key_A:

            if(!moving){


                direction=1;

            }else{
                nextdirection=1;
            }

            break;
        case Qt::Key_D:

            if(!moving){


                direction=4;

            }else{

                nextdirection=4;
            }

            break;
        case Qt::Key_S:
            if(!moving){
                direction=3;
            }else{
                nextdirection=3;
            }



            break;
        case Qt::Key_W:
            if(!moving){
                direction=2;
            }else{
                nextdirection=2;
            }
            break;

            break;

        case Qt::Key_P:
            pause();
            break;

        case Qt::Key_C:
            on_controlsButton_clicked();
            break;

        case Qt::Key_Escape:
            break;


        default:
            break;

        }
    }

}



int scarestate=0;
int soundstate=0;
int timeGame;


// метод обновления карты и табло
void Pacmanwindow::updater()
{
    ui->livesLcdNumber_2->display(lives);
    ui->levelLcdNumber_2->display(level);
    checklost();
    pacman_move();

    for(int i=0;i<ballpoints.size();i++){
        if(pacman->pacx==ballpoints[i].x() && pacman->pacy==ballpoints[i].y()){
            ballpoints.remove(i);
            score++;
            if(score > hiscore)
                hiscore = score;
            text->score = score;
            ui->highScroreLcdNumber_2->display(hiscore);
            ui->scoreLcdNumber_2->display(score);

            if(soundstate==0){waka->play(":/Sound/pacman_chomp.wav");soundstate++;}
            else{soundstate++;if(soundstate==6){soundstate=0;}}


        }
    }

    for(int i=0;i<Powerballpoints.size();i++){
        if(pacman->pacx==Powerballpoints[i].x() && pacman->pacy==Powerballpoints[i].y()){
            Powerballpoints.remove(i);
            if(scared){scarestate=0;}
            scared=true;
            waka->play(":/Sound/pacman_extrapac.wav");

        }
    }
    if(scared){
        ghost->is_Scared=true;
        ghost1->is_Scared=true;
        ghost2->is_Scared=true;
        ghost3->is_Scared=true;
        scarestate+=1;
        if(scarestate==1){ghoststimer->setInterval(150);}
        if(scarestate==100){
            ghost->whiteb=true;
            ghost1->whiteb=true;
            ghost2->whiteb=true;
            ghost3->whiteb=true;

        }
        if(scarestate==150){
            scared=false; scarestate=0; ghoststimer->setInterval(75);
            ghost->whiteb=false;
            ghost1->whiteb=false;
            ghost2->whiteb=false;
            ghost3->whiteb=false;
        }
    }else{
        ghost->is_Scared=false;
        ghost1->is_Scared=false;
        ghost2->is_Scared=false;
        ghost3->is_Scared=false;
    }


    if (timeGame > -1)
    {
        timeGame++;

        ui->timeLcdNumber_2->display(timeGame/30);
    }

    text->timeElapsed = timeGame/30;

    text->name = this->name;
    text->difficulty = difficultyText;

    ball->setpoints(ballpoints);
    powerball->setpoints(Powerballpoints);
    pac_map->setballpoints(ballpoints);
    pac_map->setpowerballpoints(Powerballpoints);
    pac_map->fillpacpoints(pacx,pacy);
    ghost->advance();
    ghost1->advance();
    ghost2->advance();
    ghost3->advance();


    this->setFocus();
    scene->update(pac_map->boundingRect());

pacman->advance();


}

void Pacmanwindow::pause(){

    if (!playing)
        return;
    isPaused = !isPaused;
    if (isPaused) {

        timer->stop();
        ghoststimer->stop();


        text->paused=true;
        scene->addItem(text);
        text->setZValue(6);

    } else {
        scene->removeItem(text);
        timer->start();
        ghoststimer->start();
    }

    update();



}

void Pacmanwindow::ghostupdater()
{
    if(ghoststart || ghoststart1 || ghoststart2 || ghoststart3){
        if(ghoststart){ghostsmove();}
        if(ghoststart1){ghostsmove1();}
        if(ghoststart2){ghostsmove2();}
        if(ghoststart3){ghostsmove3();}
    }
    if(gosx3==450/2){
        state++;
    }
    if(state==4 || 0){
        start=true;

        state=0;
    }
    if(!start){
        moveghostsinrect3();
        moveghostsinrect2();
        moveghostsinrect1();
        moveghostsinrect();

    }

    else if(!ghoststart3 || !ghoststart2 || !ghoststart1 || !ghoststart){
        QPoint p,p1,p2,p3;
        if(!ghoststart3){
            gosy3-=5;
            ghost3->setgosx(gosx3);
            ghost3->setgosy(gosy3);
            p3.setX(gosx3);
            p3.setY(gosy3);
            if(pac_map->pacpoints.contains(p3)){ghoststart3=true;}
        }

        if(!ghoststart2){
            gosy2-=5;
            ghost2->setgosx(gosx2);
            ghost2->setgosy(gosy2);
            p2.setX(gosx2);
            p2.setY(gosy2);
            if(pac_map->pacpoints.contains(p2)){ghoststart2=true;}
        }
        if(!ghoststart1){
            gosy1-=5;
            ghost1->setgosx(gosx1);
            ghost1->setgosy(gosy1);
            p1.setX(gosx1);
            p1.setY(gosy1);
            if(pac_map->pacpoints.contains(p1)){ghoststart1=true;}
        }
        if(!ghoststart){
            gosy-=5;
            ghost->setgosx(gosx);
            ghost->setgosy(gosy);
            p.setX(gosx);
            p.setY(gosy);
            if(pac_map->pacpoints.contains(p)){ghoststart=true;}
        }

    }
}


Pacmanwindow::~Pacmanwindow()
{
    delete ui;
}


void Pacmanwindow::on_pauseButton_clicked()
{
    pause();
}

void Pacmanwindow::on_controlsButton_clicked()
{

    if(isPaused == false){
        pause();

        QPalette palette;
        palette.setColor(QPalette::Background, Qt::black);
        QMessageBox *controls = new QMessageBox;
        controls->setPalette(palette);
        controls->about(this,
                        tr("Controls"),
                        tr("<p align='center'><font><h1>Controls</h1><br>Press direction keys and aswd keys to move pacman across the screen.<br><br><br>Press ESC to exit application.<br><br><br>Press P to pause the game</font></p>"));




    }
}




