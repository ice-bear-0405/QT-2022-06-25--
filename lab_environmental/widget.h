#ifndef WIDGET_H
#define WIDGET_H

#include "ui_widget.h"
#include <QWidget>
#include<QSerialPort>                                               //使用串口功能
#include<QSerialPortInfo>                                           //串口输出信息
#include <QByteArray>
#include <QIODevice>
#include <QMessageBox>
#include "QString"
#include "QDebug"
#include <cstdio>

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
//    void serialPortReadyRead();
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QSerialPort *serialPort;
    QByteArray *recvData;
    QByteArray *recvTemp;

    char buffer[20];   //创建一个double转为字符串的缓冲区
private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
