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

    void on_sliderValue_valueChanged(int value);

    void on_btnSend_clicked();

private:
    bool connected = false;
    QUdpSocket *udpSocket;

    Ui::MainWindow *ui;
    void connect();
    void disconnect();
    void toggleEnabledControls();
    void toggleWidgetEnabled(QWidget *qWidget);
};
#endif // MAINWINDOW_H
