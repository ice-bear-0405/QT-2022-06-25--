#ifndef OBJECT_H
#define OBJECT_H
#include<QSerialPort>                                               //使用串口功能
#include<QSerialPortInfo>                                           //串口输出信息
#include <QByteArray>
#include <QIODevice>
#include <QMessageBox>
#include "QString"
#include "QDebug"
#include <QWidget>
#include <QTimer>
#include <QWidget>

class Object : public QWidget
{
    Q_OBJECT
public:
    explicit Object(QWidget *parent = nullptr);
    Object()
{
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(onTimeout()));
    m_timer.start(1000);
}
    signals:

public slots:

private:
    QTimer m_timer;
};

#endif // OBJECT_H
