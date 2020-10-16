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

void MainWindow::on_sliderValue_valueChanged(int value)
{
    ui->lblSliderValue->setText(QString::number(value));
}

void MainWindow::on_btnConnect_clicked()
{
    if (!connected) {
        connect();
    } else {
        disconnect();
    }
}

void MainWindow::on_btnSend_clicked()
{
    int intValue = ui->sliderValue->value();
    QString strValue = QString::number(intValue);

    if (intValue <= 9) {
        strValue = "0" + strValue;
    }

    udpSocket->write(strValue.toStdString().c_str());
}

void MainWindow::connect() {
    int port = ui->sbPort->value();
    bool connectSuccess = false;

    //TODO: make this not hardcoded
    udpSocket->connectToHost("192.168.0.104", port);

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
    toggleWidgetEnabled(ui->lblSliderValue);
    toggleWidgetEnabled(ui->btnSend);
    toggleWidgetEnabled(ui->sliderValue);

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
