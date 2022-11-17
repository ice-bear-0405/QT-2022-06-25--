#include "widget.h"
#include <QApplication>
#include<QSerialPort>                                               //使用串口功能
#include<QSerialPortInfo>                                           //串口输出信息
#include <QByteArray>
#include <QIODevice>
#include <QMessageBox>
#include "QString"
#include "QDebug"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    //标题名称
    w.setWindowTitle("基于ZigBee的实验室环境与安全智能监测");
    w.show();
    return a.exec();
}
