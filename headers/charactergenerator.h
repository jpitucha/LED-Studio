#ifndef CHARACTERGENERATOR_H
#define CHARACTERGENERATOR_H

#include <QDialog>
#include "headers/clickablelabel.h"
#include <QPainter>
#include <QJsonDocument>
#include <QMap>
#include <QListWidgetItem>
#include <QBitArray>

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
    void loadMatrixView(int);
    void loadChar(QListWidgetItem*);
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
    enum CharsSource {User, Predefined};
    QBitArray mask5x7;
    QList<uint16_t> values;
    QList<QBitArray> values2;
    QList<ClickableLabel *> dots;
    QMap<QString, QStringList> userChars;
    QMap<QString, QStringList> predefinedChars;
    QJsonDocument readCharsFromFile(QString path, QIODevice::OpenMode mode);
    void saveUserCharsToFile();
    void decodeChars(CharsSource src);
    QJsonDocument encodeChars();
    void updateResult();
    void updateMatrixView();
    //void updateUserCharsList();
    QString bitArrayToString(QBitArray ba);
};

#endif // CHARACTERGENERATOR_H
