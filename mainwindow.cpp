#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "workingthread.h"

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

    WorkingThread* workingThread = new WorkingThread(this, inputPath, outputPath);
    connect(workingThread, SIGNAL(error(QString)), this, SLOT(on_error(QString)));
    connect(workingThread, SIGNAL(complete(QString)), this, SLOT(on_error(QString)));

    workingThread->start();
}

void MainWindow::on_pushButtonChooseInput_clicked()
{
    getPathFromUser(ui->lineEditInputPath, OPEN);
}

void MainWindow::on_pushButtonChooseOutput_clicked()
{
    getPathFromUser(ui->lineEditOutputPath, SAVE);
}

void MainWindow::on_error(QString error)
{
    message(error, QMessageBox::Ok);
}

void MainWindow::on_complete(QString outputPath)
{
    message("Data decompressed to " + outputPath, QMessageBox::Ok);
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
