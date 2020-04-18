#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "headers/charactergenerator.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_actionCharacter_generator_triggered() {
    CharacterGenerator *cg = new CharacterGenerator();
    cg->show();
}
