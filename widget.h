#ifndef WIDGET_H
#define WIDGET_H
#include<QSerialPort>                                               //使用串口功能
#include<QSerialPortInfo>                                           //串口输出信息
#include <QByteArray>
#include <QIODevice>
#include <QMessageBox>
#include "QString"
#include "QDebug"
#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
        QSerialPort *serialPort;
        QByteArray *recvData;
        QByteArray *recvTemp;
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_openBt_clicked();
    void on_closeBt_clicked();
    void on_SerialPort_readyRead();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
