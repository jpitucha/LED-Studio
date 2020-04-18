#ifndef CHARACTERGENERATOR_H
#define CHARACTERGENERATOR_H

#include <QDialog>

namespace Ui {
class CharacterGenerator;
}

class CharacterGenerator : public QDialog
{
    Q_OBJECT

public:
    explicit CharacterGenerator(QWidget *parent = nullptr);
    ~CharacterGenerator();

public slots:
    void reject();
    void loadMatrix(int);

private slots:
    void on_addChar_clicked();
    void on_editChar_clicked();
    void on_deleteChar_clicked();

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
    void updateResult();
};

#endif // CHARACTERGENERATOR_H
