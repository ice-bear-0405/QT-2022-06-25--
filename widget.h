#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QSerialPort>                                               //使用串口功能
#include<QSerialPortInfo>                                           //串口输出信息
#include <QByteArray>
#include <QIODevice>
#include <QMessageBox>
#include "QString"
#include "QDebug"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
private slots:
//    void serialPortReadyRead_Slot();
    void on_openBt_clicked();
    void on_closeBt_clicked();
    void on_SerialPort_readyRead();




public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    int temperature;
    int temp1[10] = {};
    int temp2[10] = {};
    int humidity;
    int pump;
    int soilHumidity;
    int CO2;
    int fan;
private:
    Ui::Widget *ui;
    QSerialPort *serialPort;
    QByteArray *recvData;
    QByteArray *recvTemp;
};

#endif // WIDGET_H
