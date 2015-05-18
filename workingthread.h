#ifndef WORKINGTHREAD_H
#define WORKINGTHREAD_H

#include <QThread>
#include <QMessageBox>

class WorkingThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkingThread(QObject *parent = 0, QString inputPath = "", QString outputPath = "");
    void run();

    void stop();

private:
    bool stopThread;
    QString inputPath;
    QString outputPath;

signals:
    void valueChanged(int);
    void complete(QString /*outputPath*/);
    void error(QString /*errorText*/);

public slots:

};

#endif // WORKINGTHREAD_H
