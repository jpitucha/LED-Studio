#include "headers/charactergenerator.h"
#include "ui_charactergenerator.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include "headers/clickablelabel.h"

CharacterGenerator::CharacterGenerator(QWidget *parent) : QDialog(parent), ui(new Ui::CharacterGenerator) {
    ui->setupUi(this);
    updateResult();
    QStringList matrixes;
    matrixes.append("5x7");
    matrixes.append("8x8");
    this->ui->comboBox->addItems(matrixes);
    int row = 1;
    int col = 1;
    for(int i = 0; i < 256; i++) {
        this->ui->gridLayout->addWidget(new ClickableLabel(this, i), row, col);
        if (col < 16) {
            col++;
        } else {
            col = 1;
            row++;
        }
    }
    connect(this->ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(loadMatrix(int)));
    this->ui->charsList->addItem(new QListWidgetItem(QIcon(QPixmap("://assets/icons/lock.png")), "Test"));
}

CharacterGenerator::~CharacterGenerator() {
    delete ui;
}

void CharacterGenerator::on_addChar_clicked() {
    QString name = QInputDialog::getText(this, "New Item Name", "Please enter name for new char");
}

void CharacterGenerator::on_editChar_clicked() {
    QString name = QInputDialog::getText(this, "New Item Name", "Please enter name for new char");
}

void CharacterGenerator::on_deleteChar_clicked() {
    QMessageBox::question(this, "Remove char?", "Are you sure to delete this char?");
}

void CharacterGenerator::reject() {
    this->deleteLater();
    QDialog::reject();
}

void CharacterGenerator::updateResult() {
    this->ui->result->setText("result = { " + QString::number(b1) + ", "
                                            + QString::number(b2) + ", "
                                            + QString::number(b3) + ", "
                                            + QString::number(b4) + ", "
                                            + QString::number(b5) + ", "
                                            + QString::number(b6) + ", "
                                            + QString::number(b7) + ", "
                                            + QString::number(b8) + " }");
    this->repaint();
}

void CharacterGenerator::loadMatrix(int matrix) {
    if (matrix == 0) {
        //ui->matrix88->setDisabled(true);
        //ui->matrix57->setEnabled(true);
    } else {
        //ui->matrix57->setDisabled(true);
        //ui->matrix88->setEnabled(true);
    }
}
