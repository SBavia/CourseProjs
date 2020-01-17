
#ifndef OPTIONS_H
#define OPTIONS_H

#include <QWidget>
#include <QMessageBox>


namespace Ui {
class Options;
}


// Класс описывающий меню опций

class Options : public QWidget
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = 0);
    ~Options();
 QString mes;
 int difficulty;
private slots:

    void on_startButton_clicked();

    void on_easyButton_toggled(bool checked);

    void on_mediumButton_toggled(bool checked);

    void on_hardButton_toggled(bool checked);

private:
    Ui::Options *ui;
};

#endif // OPTIONS_H
