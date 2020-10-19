#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnConnect_clicked();

    void on_sliderContrastValue_valueChanged(int value);

    void on_sliderBrightnessValue_valueChanged(int value);

private:
    bool connected = false;
    QUdpSocket *udpSocket;

    Ui::MainWindow *ui;
    void connect();
    void disconnect();
    void toggleEnabledControls();
    void toggleWidgetEnabled(QWidget *qWidget);
    void sendValue(int value, QString valueType);
};
#endif // MAINWINDOW_H
