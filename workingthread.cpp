#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMessageBox>

#include "workingthread.h"
#include "qcompressor.h"

#define DEBUG true
#define BACKUP_FILE_V1 1
#define BACKUP_FILE_V2 2
#define BACKUP_FILE_V3 3

WorkingThread::WorkingThread(QObject *parent, QString inputPath, QString outputPath) :
    QThread(parent), stopThread(false), inputPath(inputPath), outputPath(outputPath)
{
}

// run() will be called when a thread starts
void WorkingThread::run()
{
    QFile inputFile(inputPath);

    // check if input file exists
    if (!inputFile.exists()) {
        emit error("Input file not found.");
        return;
    }

    if (inputFile.open(QIODevice::ReadOnly)) {
        // read magic line
        QByteArray magic = inputFile.readLine();
        // remove \n from line
        magic.replace("\n", "");
        // check file format - file should be equals to "ANDROID BACKUP"
        if (magic != "ANDROID BACKUP") {
            emit error("Provided file has wrong format.");

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
            emit error("Don't know how to process version " + version);

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
            emit error("Encrypted files is not supported.");

            inputFile.close();
            return;
        }

        // check if file compressed
        if (compressed) {
            QDataStream inputStream(&inputFile);
            QFile tarFile(outputPath);
            if (!tarFile.open(QIODevice::WriteOnly)) {
                emit error("Can not create output file.");

                inputFile.close();
                return;
            }

            QDataStream outputStream(&tarFile);
            QCompressor::gzipDecompress(inputStream, inputFile.size() - inputFile.pos(), outputStream);

            tarFile.close();

            emit complete(outputPath);
        } else {
            emit error("Not compressed files is not supported.");
        }

        inputFile.close();
    } else {
        qDebug() << "Unable to open input file.";
    }


    for (int i = 0; i <= 100; i++)
    {
        if(this->stopThread) break;

        // emit the signal for the count label
        emit valueChanged(i);

        // slowdown the count change, msec
        this->msleep(500);
    }
}

void WorkingThread::stop()
{
    stopThread = true;
}
