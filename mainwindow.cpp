#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcompressor.h"

#define DEBUG true
#define BACKUP_FILE_V1 1
#define BACKUP_FILE_V2 2
#define BACKUP_FILE_V3 3

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonCreateTar_clicked()
{
    QString inputPath = ui->lineEditInputPath->text();
    QString outputPath = ui->lineEditOutputPath->text();

    QFile inputFile(inputPath);

    // check if input file exists
    if (!inputFile.exists()) {
        message("Input file not found.", QMessageBox::Ok);
        return;
    }

    if (inputFile.open(QIODevice::ReadOnly)) {
        // read magic line
        QByteArray magic = inputFile.readLine();
        // remove \n from line
        magic.replace("\n", "");
        // check file format - file should be equals to "ANDROID BACKUP"
        if (magic != "ANDROID BACKUP") {
            message("Provided file has wrong format.", QMessageBox::Ok);

            inputFile.close();
            return;
        }

        // read version line
        QByteArray versionStr = inputFile.readLine();
        QByteArray compressedStr = inputFile.readLine();
        QByteArray encryptionAlgStr = inputFile.readLine();

        // remove \n
        versionStr.replace("\n", "");
        compressedStr.replace("\n", "");
        encryptionAlgStr.replace("\n", "");

        // check version
        int version = versionStr.toInt();
        if (version < BACKUP_FILE_V1 || version > BACKUP_FILE_V3) {
            message("Don't know how to process version " + version, QMessageBox::Ok);

            inputFile.close();
            return;
        }

        // parse compressed value
        bool compressed = false;
        if (compressedStr.toInt() == 1) {
            compressed = true;
        }

        // check encryption (encrypted files are not supported yet)
        if (encryptionAlgStr.toInt() == 1) {
            message("Encrypted files is not supported.", QMessageBox::Ok);

            inputFile.close();
            return;
        }

        // check if file compressed
        if (compressed) {
            QDataStream inputStream(&inputFile);
            QFile tarFile(outputPath);
            if (!tarFile.open(QIODevice::WriteOnly)) {
                message("Can not create output file.", QMessageBox::Ok);

                inputFile.close();
                return;
            }

            QDataStream outputStream(&tarFile);
            QCompressor::gzipDecompress(inputStream, inputFile.size() - inputFile.pos(), outputStream);

            tarFile.close();

            message("Data decompressed to " + outputPath, QMessageBox::Ok);
        } else {
            message("Not compressed files is not supported yet.", QMessageBox::Ok);
        }

        inputFile.close();
    } else {
        qDebug() << "Unable to open input file.";
    }
}

void MainWindow::on_pushButtonChooseInput_clicked()
{
    getPathFromUser(ui->lineEditInputPath, OPEN);
}

void MainWindow::on_pushButtonChooseOutput_clicked()
{
    getPathFromUser(ui->lineEditOutputPath, SAVE);
}

void MainWindow::message(QString messageText, QMessageBox::StandardButton buttons) {
    QMessageBox msgBox;
    msgBox.setText(messageText);
    msgBox.setStandardButtons(buttons);
    msgBox.exec();
}

void MainWindow::getPathFromUser(QLineEdit* widget, int purpose) {
    if (widget == NULL)
        return;

    QString currentPath = widget->text();
    QString resultFilePath;
    if (purpose == OPEN)
        resultFilePath = QFileDialog::getOpenFileName(this, tr("Choose android backup..."), currentPath, tr("Andoid Backup (*.ab)"));
    else if (purpose == SAVE)
        resultFilePath = QFileDialog::getSaveFileName(this, tr("Save as..."), currentPath, tr("Tar Archive (*.tar)"));

    if (resultFilePath != NULL) {
        widget->setText(resultFilePath);
    }
}
