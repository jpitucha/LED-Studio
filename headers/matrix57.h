#ifndef MATRIX57_H
#define MATRIX57_H

#include <QWidget>

namespace Ui {
class Matrix57;
}

class Matrix57 : public QWidget
{
    Q_OBJECT

public:
    explicit Matrix57(QWidget *parent = nullptr);
    ~Matrix57();

private:
    Ui::Matrix57 *ui;
};

#endif // MATRIX57_H
