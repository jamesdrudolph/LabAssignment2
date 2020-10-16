#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->lblSliderValue->setText(QString::number(value));
}

void MainWindow::on_btnConnect_clicked()
{
    ui->lblSliderValue->setEnabled(true);
    ui->btnSend->setEnabled(true);
    ui->sliderValue->setEnabled(true);
}
