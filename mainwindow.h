#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonCreateTar_clicked();

    void on_pushButtonChooseInput_clicked();

    void on_pushButtonChooseOutput_clicked();

    void on_error(QString error);
    void on_complete(QString outputPath);

private:
    void getPathFromUser(QLineEdit* widget, int dialogPurpose);
    void message(QString messageText, QMessageBox::StandardButton buttons);

private:
    Ui::MainWindow *ui;

    enum DialogPurpose {
        OPEN,
        SAVE
    };
};

#endif // MAINWINDOW_H
