#include "headers/clickablelabel.h"
#include <QDebug>

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f) : QLabel(parent) {

}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    if (enabled) {
        setPixmap(QPixmap("://assets/icons/gray_dot.png"));
        enabled = false;
    } else {
        setPixmap(QPixmap("://assets/icons/blue_dot.png"));
        enabled = true;
    }
    emit clicked();
}
