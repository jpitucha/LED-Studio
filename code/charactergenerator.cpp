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
#include <bitset>
#include <math.h>

CharacterGenerator::CharacterGenerator(QWidget *parent) : QDialog(parent), ui(new Ui::CharacterGenerator) {
    ui->setupUi(this);
    mask5x7.resize(16);
    for (int i = 0; i < 10; i++) mask5x7.setBit(i, true);
    for (int i = 0; i < 16; i++) values2.append(QBitArray(16));

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
    loadMatrixView(0);
    updateResult();
    this->ui->comboBox->addItems(QString("5x7;8x8").split(';'));
    connect(this->ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(loadMatrixView(int)));

    decodeChars(CharsSource::User);
    decodeChars(CharsSource::Predefined);
    ui->charsList->addItems(userChars.keys());
    ui->preMadeCharsList->addItems(predefinedChars.keys());
    connect(this->ui->charsList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(loadChar(QListWidgetItem*)));
    connect(this->ui->preMadeCharsList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(loadChar(QListWidgetItem*)));
}

void CharacterGenerator::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if (ui->comboBox->currentText() == "5x7") {
        painter.drawRect(5, 45, 139, 197);
    } else {
        painter.drawRect(5, 45, 221, 226);
    }
}

void CharacterGenerator::loadChar(QListWidgetItem *item) {
    if (userChars.keys().contains(item->text())) {
        qDebug() << "user";
    } else if (predefinedChars.keys().contains(item->text())) {
        qDebug() << "predef";
    }
    if (item->text().split(" ").at(1).startsWith("5x7")) {
        ui->comboBox->setCurrentIndex(0);
    } else {
        ui->comboBox->setCurrentIndex(1);
    }
}

QString CharacterGenerator::bitArrayToString(QBitArray ba) {
    int tmp = 0;
    for (int i = 0; i < ba.size(); i++) {
        tmp = 0;
        if (ba.testBit(i)) {
            tmp += int(pow(2, i));
        }
    }
    return QString::number(tmp);
}

QJsonDocument CharacterGenerator::readCharsFromFile(QString path, QIODevice::OpenMode mode) {
    QFile chars(path);
    if (!chars.open(mode)) {
        return QJsonDocument();
    }
    QByteArray contents;
    contents = chars.readAll();
    chars.close();
    return QJsonDocument::fromJson(contents);
}

void CharacterGenerator::saveUserCharsToFile() {
    QFile chars(QDir::currentPath() + "/user_chars.json");
    if (chars.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "file opened";
        QJsonDocument doc = encodeChars();
        if (!doc.isEmpty()) {
            chars.resize(0);
            chars.write(doc.toJson());
        }
        chars.close();
    }
}

void CharacterGenerator::decodeChars(CharsSource src) {
    QJsonDocument document;
    if (src == CharsSource::User) {
        document = readCharsFromFile(QDir::currentPath() + "/user_chars.json",
                             QIODevice::ReadWrite | QIODevice::Text);
    } else {
        document = readCharsFromFile(":/assets/predefined.json",
                             QIODevice::ReadOnly | QIODevice::Text);
    }
    if (!document.isNull()) {
        QJsonObject mainObject = document.object();
        QStringList letters = mainObject.keys();
        for (int i = 0; i < mainObject.length(); i++) { //iterate over chars
            QJsonObject currentLetter = mainObject.value(letters.at(i)).toObject();
            QStringList currentLetterKeys = currentLetter.keys();
            for (int j = 0; j < currentLetterKeys.length(); j++) { //iterate over char properties
                QStringList row;
                QJsonArray values = currentLetter.value(currentLetterKeys.at(j)).toArray();
                for (int i = 0; i < values.count(); i++) { //iterate over rows
                    row.append(QString::number(values.at(i).toDouble()));
                }
                if (src == CharsSource::User) {
                    userChars.insert(letters.at(i) + " " + currentLetterKeys.at(j), row);
                } else {
                    predefinedChars.insert(letters.at(i) + " " + currentLetterKeys.at(j), row);
                }
            }
        }
    }
}

QJsonDocument CharacterGenerator::encodeChars() {
    QJsonObject mainObject;
    QStringList uniqueKeys;
    if (userChars.size() == 1) {
        uniqueKeys.append(userChars.keys().at(0));
    } else if (userChars.size() > 1) {
        for (int i = 1; i < userChars.size(); i++) {
            if (!uniqueKeys.contains(userChars.keys().at(i))) {
                uniqueKeys.append(userChars.keys().at(i));
            }
        }
    }

    if (uniqueKeys.length() > 0) {
        QJsonObject primary;
        for (int i = 0; i < uniqueKeys.size(); i++) {
            QJsonObject pattern;
            QJsonArray array;
            for (int j = 0; j < 16; j++) {
                array.append(userChars.value(uniqueKeys.at(i)).at(j));
            }
            pattern.insert(uniqueKeys.at(i).split(" ").at(1), array);
            if (userChars.size() > uniqueKeys.length()) { //if there are duplicates
                for (int k = 0; k < userChars.size(); k++) {
                    if (!uniqueKeys.contains(userChars.keys().at(k))) {
                        QJsonArray duplicateArray;
                        for (int l = 0; l < 16; l++) {
                            duplicateArray.append(userChars.value(userChars.keys().at(k)).at(l));
                        }
                        pattern.insert(userChars.keys().at(k).split(" ").at(1), duplicateArray);
                    }
                }
            }
            primary.insert(uniqueKeys.at(i).split(" ").at(0), pattern);
        }
        return QJsonDocument(primary);
    } else {
        return QJsonDocument();
    }
}

void CharacterGenerator::loadMatrixView(int matrix) {
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
        for (int i = 0; i < values2.size() - 2; i++) {
            QBitArray tmp = values2.at(i);
            tmp &= mask5x7;
            values2.replace(i, tmp);
        }
        values2.replace(14, QBitArray(16));
        values2.replace(15, QBitArray(16));
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

void CharacterGenerator::updateMatrixView() {
    int index = 0;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            values2.at(i).testBit(j) ? dots.at(index)->setOn() : dots.at(index)->setOff();
            index++;
        }
    }
}

void CharacterGenerator::whenDotClicked(int id) {
    int row = id / 16;
    int col = id % 16;
    QBitArray tmp = values2.at(row);
    tmp.toggleBit(col);
    values2.replace(row, tmp);
    updateMatrixView();
}

void CharacterGenerator::on_addChar_clicked() {
    QString name = QInputDialog::getText(this, "New", "Please enter name for new char");
    if (!name.isEmpty()) {
        if (userChars.keys().contains(name)) { //this dont catches anything
            if (QMessageBox::question(this, "Name exists", "That name already exists. Override?") == QMessageBox::No) {
                return;
            }
        }
        // to do - use bit array to string method
        QStringList x;
        //double d = 0;
        for(int i = 0; i < 16; i++) {
//            d = 0.0;
//            for(int j = 0; j < 16; j++) {
//                if (values2.at(i).testBit(j)) {
//                    d += pow(2, j);
//                }
//            }
//            x.append(QString::number(int(d)));
            x.append(bitArrayToString(values2.at(i)));
            qDebug() << bitArrayToString(values2.at(i));
        }
        userChars.insert(name + " " + ui->comboBox->currentText(), x);
        ui->charsList->addItem(name + " " + ui->comboBox->currentText());
        saveUserCharsToFile();
        qDebug() << "saving user chars to file";
        qDebug() << userChars;
    }
}

void CharacterGenerator::on_editChar_clicked() {
    QString name = QInputDialog::getText(this, "Edit", "Please enter new name for this char");
    if (!name.isEmpty() && ui->charsList->selectedItems().length() > 0) {
        if (userChars.keys().contains(name)) {
            QMessageBox::critical(this, "Char exists!", "Char with that name already exists. Choose another name");
        } else {
            QString currentKey = ui->charsList->selectedItems().at(0)->text();
            QStringList c = userChars.value(currentKey);
            userChars.remove(currentKey);
            userChars.insert(name + " " + ui->comboBox->currentText(), c);
            ui->charsList->clear();
            ui->charsList->addItems(userChars.keys());
            saveUserCharsToFile();
        }
    }
}

void CharacterGenerator::on_deleteChar_clicked() {
    if (ui->charsList->selectedItems().length() > 0) {
        QMessageBox::question(this, "Remove", "Are you sure to delete this char?");
        QString name = ui->charsList->selectedItems().at(0)->text();
        userChars.remove(name);
        ui->charsList->clear();
        ui->charsList->addItems(userChars.keys());
        saveUserCharsToFile();
    }
}

void CharacterGenerator::on_moveLeftBtn_clicked() {
    for (int i = 0; i < values2.size(); i++) {
        QBitArray line = values2.at(i);
        for (int j = 0; j < 15; j++) {
            line.setBit(j, line.testBit(j + 1));
        }
        line.setBit(15, false);
        values2.replace(i, line);
    }
    updateMatrixView();
}

void CharacterGenerator::on_moveRightBtn_clicked() {
    for (int i = 0; i < values2.size(); i++) {
        QBitArray line = values2.at(i);
        for (int j = 15; j > 0; j--) {
            line.setBit(j, line.testBit(j - 1));
        }
        line.setBit(0, false);
        line &= mask5x7;
        values2.replace(i, line);
    }
    updateMatrixView();
}

void CharacterGenerator::on_moveUpBtn_clicked() {
    values2.removeFirst();
    values2.append(QBitArray(16));
    updateMatrixView();
}

void CharacterGenerator::on_moveDownBtn_clicked() {
    values2.prepend(QBitArray(16));
    values2.removeLast();
    if (ui->comboBox->currentText() == "5x7") {
        values2.replace(14, QBitArray(16));
        values2.replace(15, QBitArray(16));
    }
    updateMatrixView();
}

void CharacterGenerator::on_invertBtn_clicked() {
    for (int i = 0; i < values.size(); i++) {
        values.replace(i, ~values.at(i));
    }
    updateMatrixView();
    if(ui->comboBox->currentText() == "5x7") {
        //add clearing extra data when selected 5x7 matrix type
    }
}

void CharacterGenerator::on_clearBtn_clicked() {
    for (int i = 0; i < values.size(); i++) {
        values.replace(i, 0);
    }
    updateMatrixView();
}

void CharacterGenerator::updateResult() {
    this->ui->result->setText("result");
    this->repaint();
}

void CharacterGenerator::reject() {
    this->deleteLater();
    QDialog::reject();
}

CharacterGenerator::~CharacterGenerator() {
    delete ui;
}
