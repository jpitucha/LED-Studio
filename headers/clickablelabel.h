#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget* parent = Q_NULLPTR, int id = 0, Qt::WindowFlags f = Qt::WindowFlags());
    void setOn();
    void setOff();
    ~ClickableLabel();

signals:
    void clicked(int);

protected:    
    void mousePressEvent(QMouseEvent* event);

private:
    bool enabled = false;
    int id;
};

#endif // CLICKABLELABEL_H
