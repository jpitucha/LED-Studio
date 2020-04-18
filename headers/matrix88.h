#ifndef MATRIX88_H
#define MATRIX88_H

#include <QWidget>

namespace Ui {
class Matrix88;
}

class Matrix88 : public QWidget
{
    Q_OBJECT

public:
    explicit Matrix88(QWidget *parent = nullptr);
    ~Matrix88();

private:
    Ui::Matrix88 *ui;
};

#endif // MATRIX88_H
