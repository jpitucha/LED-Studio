#ifndef CHARACTERGENERATOR_H
#define CHARACTERGENERATOR_H

#include <QDialog>
#include "headers/clickablelabel.h"
#include <QPainter>

namespace Ui {
class CharacterGenerator;
}

class CharacterGenerator : public QDialog
{
    Q_OBJECT

public:
    explicit CharacterGenerator(QWidget *parent = nullptr);
    virtual void paintEvent(QPaintEvent *event);
    ~CharacterGenerator();

public slots:
    void reject();
    void loadMatrix(int);
    void whenDotClicked(int);

private slots:
    void on_addChar_clicked();
    void on_editChar_clicked();
    void on_deleteChar_clicked();
    void on_moveLeftBtn_clicked();
    void on_moveRightBtn_clicked();
    void on_moveUpBtn_clicked();
    void on_moveDownBtn_clicked();
    void on_invertBtn_clicked();
    void on_clearBtn_clicked();

private:
    Ui::CharacterGenerator *ui;
    unsigned char b1 = 255;
    unsigned char b2 = 255;
    unsigned char b3 = 255;
    unsigned char b4 = 255;
    unsigned char b5 = 255;
    unsigned char b6 = 255;
    unsigned char b7 = 255;
    unsigned char b8 = 255;
    QList<uint16_t> values;
    QList<ClickableLabel *> dots;
    void updateResult();
    void updateMatrix();
};

#endif // CHARACTERGENERATOR_H
