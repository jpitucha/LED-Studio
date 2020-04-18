#include "headers\matrix88.h"
#include "ui_matrix88.h"

Matrix88::Matrix88(QWidget *parent) : QWidget(parent), ui(new Ui::Matrix88) {
    ui->setupUi(this);
}

Matrix88::~Matrix88() {
    delete ui;
}
