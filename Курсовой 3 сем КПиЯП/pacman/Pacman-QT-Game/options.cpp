
#include "options.h"
#include "ui_options.h"
#include "pacmanwindow.h"
#include <exception>
#include <QDebug>


Options::Options(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Options)
{
    QLabel *image = new QLabel();
       image->setPixmap( QPixmap( "paclogo.jpg" ) );
       image->show();
    ui->setupUi(this);
    difficulty = 0;

}

Options::~Options()
{
    delete ui;
}

// обработка нажатия клавиши старт
void Options::on_startButton_clicked()
{
    QString nickname = ui->nameText->toPlainText();
    bool good_start = true;
    try
    {
        if(nickname.length() < 1 || nickname.length() > 6) throw QString("Ник не должен быть пустым, а также его длинна не должна превышать 6 символов");
        if(difficulty == 0) throw QString("Вы не выбрали уровень сложности");
    }
    catch(QString &message)
    {
        QMessageBox::warning(this,"Warning", message);
        good_start = false;
    }

if(good_start)
{
Pacmanwindow *w = new Pacmanwindow(ui->nameText->toPlainText());

QString nametemp = ui->nameText->toPlainText();

w->parseMessage(nametemp, difficulty);

    w->show();
    this->close();
}
}

void Options::on_easyButton_toggled(bool )
{
    difficulty = 1;
}

void Options::on_mediumButton_toggled(bool )
{
    difficulty = 2;
}

void Options::on_hardButton_toggled(bool )
{
    difficulty = 3;
}
