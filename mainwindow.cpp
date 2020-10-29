#include <iostream>
#include <fstream>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpSocket = new QUdpSocket(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_transferTestImage_clicked()
{
    QString bmpFilePath;
    if (openFileDialog(bmpFilePath)) {
        transfer_image(bmpFilePath);
    }
}

bool MainWindow::openFileDialog(QString &filePath) {
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Bitmap Images (*.bmp)"));

    if (dialog.exec() && dialog.selectedFiles().length() > 0) {
        filePath = dialog.selectedFiles()[0];
        return true;
    }

    return false;
}

void MainWindow::on_transferOverlayImage_clicked()
{
    QString bmpFilePath;
    if (openFileDialog(bmpFilePath)) {
        transfer_image(bmpFilePath);
    }
}

void MainWindow::transfer_image(QString bmpFilePath)
{
    //std::ifstream is (bmpFilePath.toStdString().c_str(), std::ifstream::binary);
    std::ifstream is (bmpFilePath.toStdString().c_str(), std::ios::in|std::ios::binary|std::ios::ate);
    //get 1024 bytes of the file, send over UDP

    if (is) {
      udpSocket->write("START_IMAGE_SEND");
      int fileSize = is.tellg();
      int remainder = fileSize % 1024;
      qDebug() << "\nFileSize: " << fileSize << " Remainder: " << remainder;
      is.seekg (0, std::ios::beg);
      char * buffer = new char [1024];
      if (fileSize <= 1024)
      {
          buffer = new char [fileSize];
          is.read (buffer, fileSize);
          udpSocket->write(buffer,fileSize);
      }
      else
      {
        for(int x = 0; x < int((fileSize - remainder) / 1024); x++)
        {
            buffer = new char [1024];
            is.read (buffer, 1024);
            udpSocket->write(buffer,1024);
            qDebug() << x;

        }
        if(remainder > 0)
        {
            buffer = new char [remainder];
            is.read (buffer, remainder);
            udpSocket->write(buffer,remainder);
        }
      }
      while(udpSocket->hasPendingDatagrams()){}
      udpSocket->write("END_IMAGE_SEND");
      qDebug() << "Done";
    }
}

void MainWindow::on_btnConnect_clicked()
{
    if (!connected) {
        connect();
    } else {
        disconnect();
    }
}

void MainWindow::on_sliderContrastValue_valueChanged(int value)
{
    ui->lblContrastValue->setText("Contrast: " + QString::number(value));
    sendValue(ui->sliderContrastValue->value(), "C");
}

void MainWindow::on_sliderBrightnessValue_valueChanged(int value)
{
    ui->lblBrightnessValue->setText("Brightness: " + QString::number(value));
    sendValue(ui->sliderBrightnessValue->value(), "B");
}

void MainWindow::sendValue(int value, QString valueType) {
    QString strToSend = QString::number(value);

    if (value <= 9) {
        strToSend = "0" + strToSend;
    }

    strToSend = valueType + strToSend;
    udpSocket->write(strToSend.toStdString().c_str());
}

void MainWindow::connect() {
    int port = ui->sbPort->value();
    bool connectSuccess = false;
    QString ip = ui->txtIP->text(); //"192.168.1.89"

    udpSocket->connectToHost(ip, port);

    connectSuccess = udpSocket->waitForConnected(1000); //waits 1000 ms, returns t/f;
    if (connectSuccess) {
        connected = true;
        ui->btnConnect->setText("Disconnect");
        toggleEnabledControls();
    }
}

void MainWindow::disconnect() {
    bool disconnectSuccess = true;

    udpSocket->write("FF");
    udpSocket->disconnectFromHost();

    disconnectSuccess = (udpSocket->state() == QAbstractSocket::UnconnectedState || udpSocket->waitForDisconnected(1000));

    if (disconnectSuccess) {
        connected = false;
        ui->btnConnect->setText("Connect");
        toggleEnabledControls();
    }
}

void MainWindow::toggleEnabledControls() {
    //toggle enabled of send controls
    toggleWidgetEnabled(ui->lblBrightnessValue);
    toggleWidgetEnabled(ui->lblContrastValue);
    toggleWidgetEnabled(ui->sliderBrightnessValue);
    toggleWidgetEnabled(ui->sliderContrastValue);

    //toggle image send controls
    //toggleWidgetEnabled(ui->);
    //toggle enabled of connect controls
    toggleWidgetEnabled(ui->lblIP);
    toggleWidgetEnabled(ui->lblPort);
    toggleWidgetEnabled(ui->txtIP);
    toggleWidgetEnabled(ui->sbPort);

}

void MainWindow::toggleWidgetEnabled(QWidget *qWidget) {
    //toggle enabled of QWidget
    qWidget->setEnabled(!qWidget->isEnabled());
}
