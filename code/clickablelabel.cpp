#include "headers/clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget* parent, int id, Qt::WindowFlags f) : QLabel(parent) {
    setPixmap(QPixmap("://assets/icons/gray_dot.png"));
    setAlignment(Qt::AlignCenter);
    setMinimumWidth(23);
    setMinimumHeight(23);
    this->id = id;
}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    if (enabled) {
        setOff();
    } else {
        setOn();
    }
    emit clicked(id);
}

void ClickableLabel::setOn() {
    setPixmap(QPixmap("://assets/icons/blue_dot.png"));
    enabled = true;
}

void ClickableLabel::setOff() {
    setPixmap(QPixmap("://assets/icons/gray_dot.png"));
    enabled = false;
}
