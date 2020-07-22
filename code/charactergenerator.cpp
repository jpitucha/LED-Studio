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
#include <QBitArray>
#include <math.h>

CharacterGenerator::CharacterGenerator(QWidget *parent) : QDialog(parent), ui(new Ui::CharacterGenerator) {
    ui->setupUi(this);
    for (int i = 0; i < 16; i++) { values.append(0); values2.append(QBitArray(16)); }

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
            tmp += pow(2, i);
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
        chars.resize(0);
        QJsonDocument doc = encodeChars();
        chars.write(doc.toJson());
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
    QStringList keys = userChars.keys();

    QStringList uniqueKeys;
    if (keys.length() == 1) {

    } else if (keys.length() > 1) {
        for (int i = 1; i < keys.length(); i++) {
            if (!uniqueKeys.contains(keys.at(i))) {
                uniqueKeys.append(keys.at(i));
            }
        }
    }

    if (uniqueKeys.length() > 1) {
        return QJsonDocument();
    } else {
        QJsonObject pattern;
        QJsonObject primary;
        QJsonArray a;
        for (int i = 0; i < 16; i++) {
            a.append(userChars.value(keys.at(0)).at(i));
        }

        pattern.insert(keys.at(0).split(" ").at(1), a);
        primary.insert(keys.at(0).split(" ").at(0), pattern);
        QJsonDocument doc;
        doc.setObject(primary);
        return doc;
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
        for (int i = 0; i < values.size(); i++) {
            uint16_t tmp = values.at(i);
            tmp &= 0b1111111111000000;
            values.replace(i, tmp);
        }
        values.replace(14, 0);
        values.replace(15, 0);
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
        for (int y = 15; y >= 0; y--) {
            if ((values.at(i) >> y) & 1) {
                dots.at(index)->setOn();
            } else {
                dots.at(index)->setOff();
            }
            index++;
        }
    }

    //
}

void CharacterGenerator::whenDotClicked(int id) {
    int row = id / 16;
    int col = id % 16;

    values.replace(row, values.at(row) ^ (1 << (15 - col)));

    QBitArray tmp = values2.at(row);
    tmp.toggleBit(col);
    values2.replace(row, tmp);

    updateMatrixView();
}

void CharacterGenerator::on_addChar_clicked() {
    QString name = QInputDialog::getText(this, "New", "Please enter name for new char");
    if (!name.isEmpty()) {
        if (userChars.keys().contains(name)) {
            if (QMessageBox::question(this, "Name exists", "That name already exists. Override?") == QMessageBox::No) {
                return;
            }
        }
        QStringList x;
        double d = 0;
        for(int i = 0; i < 16; i++) {
            d = 0.0;
            for(int j = 0; j < 16; j++) {
                if (values2.at(i).testBit(j)) {
                    d += pow(2, j);
                }
            }
            x.append(QString::number(int(d)));
        }
        userChars.insert(name + " " + ui->comboBox->currentText(), x);
        ui->charsList->addItem(name + " " + ui->comboBox->currentText());
        saveUserCharsToFile();
        qDebug() << "saving user chars to file";
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
    for (int i = 0; i < values.size(); i++) {
        values.replace(i, values.at(i) << 1);
    }
    updateMatrixView();
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
    updateMatrixView();
}

void CharacterGenerator::on_moveUpBtn_clicked() {
    values.removeFirst();
    values.append(0);
    updateMatrixView();
}

void CharacterGenerator::on_moveDownBtn_clicked() {
    values.prepend(0);
    values.removeLast();
    if (ui->comboBox->currentText() == "5x7") {
        values.replace(14, 0);
        values.replace(15, 0);
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
