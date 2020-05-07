#include "headers/charactergenerator.h"
#include "ui_charactergenerator.h"
#include <QInputDialog>
#include <QMessageBox>
#include "headers/clickablelabel.h"

CharacterGenerator::CharacterGenerator(QWidget *parent) : QDialog(parent), ui(new Ui::CharacterGenerator) {
    ui->setupUi(this);
    for (int i = 0; i < 16; i++) {
        values.append(0);
    }
    int row = 1;
    int col = 1;
    for(int i = 0; i < 256; i++) {
        dots.append(new ClickableLabel(this, i));
        connect(dots.last(), SIGNAL(clicked(int)), this, SLOT(whenDotClicked(int)));
        this->ui->gridLayout->addWidget(dots.last(), row, col);
        if (col < 16) {
            col++;
        } else {
            col = 1;
            row++;
        }
    }
    updateMatrix();
    updateResult();
    QStringList matrixes;
    matrixes.append("5x7");
    matrixes.append("8x8");
    this->ui->comboBox->addItems(matrixes);
    connect(this->ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(loadMatrix(int)));
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
    if (matrix == 0) { //5x7
        for (int i = 10; i <= 15; i++) dots.at(i)->setEnabled(false);
        for (int i = 26; i <= 31; i++) dots.at(i)->setEnabled(false);
        for (int i = 42; i <= 47; i++) dots.at(i)->setEnabled(false);
        for (int i = 58; i <= 63; i++) dots.at(i)->setEnabled(false);
        for (int i = 74; i <= 79; i++) dots.at(i)->setEnabled(false);
        for (int i = 90; i <= 95; i++) dots.at(i)->setEnabled(false);
        for (int i = 106; i <= 111; i++) dots.at(i)->setEnabled(false);
        for (int i = 122; i <= 127; i++) dots.at(i)->setEnabled(false);
        for (int i = 138; i <= 143; i++) dots.at(i)->setEnabled(false);
        for (int i = 154; i <= 159; i++) dots.at(i)->setEnabled(false);
        for (int i = 170; i <= 175; i++) dots.at(i)->setEnabled(false);
        for (int i = 186; i <= 191; i++) dots.at(i)->setEnabled(false);
        for (int i = 202; i <= 207; i++) dots.at(i)->setEnabled(false);
        for (int i = 218; i <= 255; i++) dots.at(i)->setEnabled(false);
        //TO DO: clear the data
    } else { //8x8
        for (int i = 10; i <= 15; i++) dots.at(i)->setEnabled(true);
        for (int i = 26; i <= 31; i++) dots.at(i)->setEnabled(true);
        for (int i = 42; i <= 47; i++) dots.at(i)->setEnabled(true);
        for (int i = 58; i <= 63; i++) dots.at(i)->setEnabled(true);
        for (int i = 74; i <= 79; i++) dots.at(i)->setEnabled(true);
        for (int i = 90; i <= 95; i++) dots.at(i)->setEnabled(true);
        for (int i = 106; i <= 111; i++) dots.at(i)->setEnabled(true);
        for (int i = 122; i <= 127; i++) dots.at(i)->setEnabled(true);
        for (int i = 138; i <= 143; i++) dots.at(i)->setEnabled(true);
        for (int i = 154; i <= 159; i++) dots.at(i)->setEnabled(true);
        for (int i = 170; i <= 175; i++) dots.at(i)->setEnabled(true);
        for (int i = 186; i <= 191; i++) dots.at(i)->setEnabled(true);
        for (int i = 202; i <= 207; i++) dots.at(i)->setEnabled(true);
        for (int i = 218; i <= 255; i++) dots.at(i)->setEnabled(true);
    }
}

void CharacterGenerator::whenDotClicked(int id) {
    int row = id / 16;
    int col = id % 16;
    values.replace(row, values.at(row) ^ (1 << (15 - col)));
    updateMatrix();
}

void CharacterGenerator::updateMatrix() {
    int index = 0;
    for (int i = 0; i < 16; i++) {
        for (int y = 15; y >= 0; y--) {
            if ((values.at(i) >> y) & 1) {
                dots.at(index)->setOn();
            } else {
                dots.at(index)->setOff();
            }
            index++;
        }
    }
}

void CharacterGenerator::on_moveLeftBtn_clicked() {
    for (int i = 0; i < values.size(); i++) {
        values.replace(i, values.at(i) << 1);
    }
    updateMatrix();
}

void CharacterGenerator::on_moveRightBtn_clicked() {
    for (int i = 0; i < values.size(); i++) {
        values.replace(i, values.at(i) >> 1);
    }
    updateMatrix();
}

void CharacterGenerator::on_moveUpBtn_clicked() {
    values.removeFirst();
    values.append(0);
    updateMatrix();

}

void CharacterGenerator::on_moveDownBtn_clicked() {
    values.prepend(0);
    values.removeLast();
    updateMatrix();
}

void CharacterGenerator::on_invertBtn_clicked() {
    for (int i = 0; i < values.size(); i++) {
        values.replace(i, ~values.at(i));
    }
    updateMatrix();
}
