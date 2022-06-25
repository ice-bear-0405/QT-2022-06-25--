#include "object.h"
#include<QSerialPort>                                               //使用串口功能
#include<QSerialPortInfo>                                           //串口输出信息
#include <QByteArray>
#include <QIODevice>
#include <QMessageBox>
#include "QString"
#include "QDebug"
#include <QWidget>
#include <QTimer>
Object::Object(QWidget *parent) : QWidget(parent)
{

}
