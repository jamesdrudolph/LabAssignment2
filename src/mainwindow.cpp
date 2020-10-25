#include <iostream>
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
