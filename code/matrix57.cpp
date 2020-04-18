#include "headers\matrix57.h"
#include "ui_matrix57.h"

Matrix57::Matrix57(QWidget *parent) : QWidget(parent), ui(new Ui::Matrix57) {
    ui->setupUi(this);
}

Matrix57::~Matrix57() {
    delete ui;
}
