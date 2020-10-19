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

    //TODO: make this not hardcoded
    udpSocket->connectToHost("192.168.0.102", port);

    //TODO: test if connection is successful
    connectSuccess = true;

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

    //TODO: test if disconnection is successful
    disconnectSuccess = true;

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
