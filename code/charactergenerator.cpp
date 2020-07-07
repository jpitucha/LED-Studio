#include "headers/charactergenerator.h"
#include "ui_charactergenerator.h"
#include <QInputDialog>
#include <QMessageBox>
#include "headers/clickablelabel.h"
#include <QSettings>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

CharacterGenerator::CharacterGenerator(QWidget *parent) : QDialog(parent), ui(new Ui::CharacterGenerator) {
    ui->setupUi(this);
    for (int i = 0; i < 16; i++) { values.append(0); }
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
    loadMatrix(0);
    updateResult();
    this->ui->comboBox->addItems(QString("5x7;8x8").split(';'));
    connect(this->ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(loadMatrix(int)));
    parsePredefinedChars();
    fillInListWidget();
}

void CharacterGenerator::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if (ui->comboBox->currentText() == "5x7") {
        painter.drawRect(5, 45, 139, 197);
    } else {
        painter.drawRect(5, 45, 221, 226);
    }
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

QJsonDocument CharacterGenerator::readPredefinedChars() {
    QFile chars(":/assets/predefined.json");
    if (!chars.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QJsonDocument();
    }
    QByteArray contents;
    contents = chars.readAll();
    chars.close();
    return QJsonDocument::fromJson(contents);
}

void CharacterGenerator::parsePredefinedChars() {
    QJsonDocument document = readPredefinedChars();
    if (!document.isNull()) {
        QJsonObject mainObject = document.object();
        QStringList mainKeys = mainObject.keys();
        for (int i = 0; i < mainObject.length(); i++) { //iterate over chars
            QMap<QString, QStringList> character;
            QJsonObject currentObject = mainObject.value(mainKeys.at(i)).toObject();
            QStringList currentObjectKeys = currentObject.keys();
            bool protectedValue = true;
            for (int j = 0; j < currentObjectKeys.length(); j++) { //iterate over char properties
                QStringList row;
                if (currentObject.value(currentObjectKeys.at(j)).isBool()) {
                    protectedValue = currentObject.value(currentObjectKeys.at(j)).toBool();
                } else if (currentObject.value(currentObjectKeys.at(j)).isArray()) {
                    QJsonArray values = currentObject.value(currentObjectKeys.at(j)).toArray();
                    for (int i = 0; i < values.count(); i++) { //iterate over rows
                        row.append(QString::number(values.at(i).toDouble()));
                    }
                    row.append(protectedValue == true ? "editable: no" : "editable: yes");
                    row.append("name: " + mainKeys.at(i));
                    character.insert(currentObjectKeys.at(j), row);
                }
            }
            predefinedChars.append(character);
        }
    }
}

void CharacterGenerator::fillInListWidget() {
    QString editable = "";
    QString name = "";
    for (int i = 0; i < predefinedChars.length(); i++) {
        QStringList keys = predefinedChars.at(i).keys(); //keys of one character
        for (int j = 0; j < keys.length(); j++) {
            if (predefinedChars.at(i).value(keys.at(j)).startsWith("editable:")) {
                editable = predefinedChars.at(i).value(keys.at(j)).at(j).split(" ").at(1);
            } else if (predefinedChars.at(i).value(keys.at(j)).startsWith("name:")) {
                name = predefinedChars.at(i).value(keys.at(j)).at(j).split(" ").at(1);
            }
        }
        QString title = name + " " +  keys.at(i) + " (" + editable == "yes" ? "rw" : "ro";
        qDebug() << title;
    }
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
        for (int i = 0; i < values.size(); i++) {
            uint16_t tmp = values.at(i);
            tmp &= 0b1111111111000000;
            values.replace(i, tmp);
            values.replace(14, 0);
            values.replace(15, 0);
        }
        updateMatrix();
        update();
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
        update();
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
        uint16_t tmp = values.at(i);
        tmp = tmp >> 1;
        if (ui->comboBox->currentText() == "5x7") {
            tmp &= 0b1111111111000000;
        }
        values.replace(i, tmp);
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
    if (ui->comboBox->currentText() == "5x7") {
        values.replace(14, 0);
        values.replace(15, 0);
    }
    updateMatrix();
}

void CharacterGenerator::on_invertBtn_clicked() {
    for (int i = 0; i < values.size(); i++) {
        values.replace(i, ~values.at(i));
    }
    updateMatrix();
    //add clearing extra data when selected 5x7 matrix type
}

void CharacterGenerator::on_clearBtn_clicked() {
    for (int i = 0; i < values.size(); i++) {
        values.replace(i, 0);
    }
    updateMatrix();
}
