#include "widget.h"
#include "ui_widget.h"
#include<QSerialPort>                                               //使用串口功能
#include<QSerialPortInfo>                                           //串口输出信息
#include <QByteArray>
#include <QIODevice>
#include <QMessageBox>
#include "QString"
#include "QDebug"
#include <QTimer>
#include <QThread>
#include <QTimer>
#define TIME     2000

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("基于Zigbee的智能门窗");
    this->setWindowIcon(QIcon(":/img/红门.png"));
    QStringList serialNamePort;
            serialPort = new QSerialPort();                                     //新建一个串口对象
            connect(serialPort,SIGNAL(readyRead()),this,SLOT(serialPortReadyRead_Slot()));
            foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
            {
                  serialNamePort<<info.portName();
            }

            ui->serialCb->addItems(serialNamePort);
            serialPort->open(QSerialPort::ReadWrite);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_openBt_clicked()//打开串口
{
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity   checkBits;
//初始化波特率
    if(ui->baundrateCb->currentText() == "9600")
    {
        baudRate = QSerialPort::Baud9600;
    }
    else if(ui->baundrateCb->currentText() == "4800")
    {
        baudRate = QSerialPort::Baud4800;
    }
    else if(ui->baundrateCb->currentText() == "115200")
    {
        baudRate = QSerialPort::Baud115200;
    }
//初始化数据位
    if(ui->dataCb->currentText() == "5")
    {
        dataBits = QSerialPort::Data5;
    }
    else if(ui->dataCb->currentText() == "6")
    {
        dataBits = QSerialPort::Data6;
    }
    else if(ui->dataCb->currentText() == "7")
    {
        dataBits = QSerialPort::Data7;
    }
    else if(ui->dataCb->currentText() == "8")
    {
        dataBits = QSerialPort::Data8;
    }
//初始化停止位
    if(ui->stopCb->currentText() == "1")
    {
        stopBits = QSerialPort::OneStop;
    }
    else if(ui->stopCb->currentText() == "1.5")
    {
        stopBits = QSerialPort::OneAndHalfStop;
    }
    else if(ui->stopCb->currentText() == "2")
    {
        stopBits = QSerialPort::TwoStop;
    }
//初始化校验位
    if(ui->checkCb->currentText() == "none")
    {
        checkBits = QSerialPort::NoParity;
    }
    serialPort->setPortName(ui->serialCb->currentText());
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(dataBits);
    serialPort->setStopBits(stopBits);
    serialPort->setParity(checkBits);
    ui->textEdit_IR->setText(" ");
    if(serialPort->open(QIODevice::ReadWrite) == true)
    {
         QMessageBox::information(this,"提示","打开串口成功");//提示成功
         //防止修改列表cb
         ui->serialCb->setEnabled(false);
         ui->dataCb->setEnabled(false);
         ui->baundrateCb->setEnabled(false);
         ui->checkCb->setEnabled(false);
         ui->stopCb->setEnabled(false);
    }
    else
    {
        QMessageBox::critical(this,"警告","打开串口失败，检查串口是否被占用");//提示失败
    }

    //读串口 显示操作


    connect(serialPort,&QIODevice::readyRead,this,&Widget::on_SerialPort_readyRead);


//    fTimer->start();


}
void Widget::on_closeBt_clicked()//关闭串口
{
    serialPort->close();
    //打开列表
    ui->serialCb->setEnabled(true);
    ui->dataCb->setEnabled(true);
    ui->baundrateCb->setEnabled(true);
    ui->checkCb->setEnabled(true);
    ui->stopCb->setEnabled(true);
    QMessageBox::information(this,"提示","关闭串口成功");
    ui->lcdNumber_humidity->display(0);
    ui->lcdNumber_temperature->display(0);
    ui->lcdNumber_smoke->display(0);
    ui->pushButton_fire->setStyleSheet("QPushButton{border-image:url(:/img/蓝火焰.png);border:none;color:rgb(255, 255, 255);}");
    ui->pushButton_IR->setStyleSheet("QPushButton{border-image:url(:/img/蓝人体红外传感器.png);border:none;color:rgb(255, 255, 255);}");
    ui->textEdit_IR->setText(" ");
}


/*关闭串口定时器
{
    fTimer->stop();
}
*/

void Widget::on_SerialPort_readyRead()//串口的读取
{
    QByteArray recvData = serialPort->readAll();

    //数据的处理_修改数据处理
    int port = recvData[0]-'0';
    int IR = recvData[2]-'0';
    int temperature = (recvData[4]-'0')*10 + (recvData[5]-'0');
    int humidity = (recvData[7]-'0')*10 + (recvData[8]-'0');
    int smoke = (recvData[10]-'0')*100 + (recvData[11]-'0')*10+(recvData[12]);
    int fire = recvData[14]-'0';

 //判断串口+显示LCD数值
    if(recvData.size() >= 12)
        {
                if(fire == 1)                                       //火焰传感器
                {
                    ui->pushButton_fire->setStyleSheet("QPushButton{border-image:url(:/img/蓝火焰.png);border:none;color:rgb(255, 255, 255);}");
                }
                else if(fire == 0)
                {
                    ui->pushButton_fire->setStyleSheet("QPushButton{border-image:url(:/img/红火焰.png);border:none;color:rgb(255, 255, 255);}");
                }

                ui->lcdNumber_humidity->display(humidity);
                ui->lcdNumber_temperature->display(temperature);
                ui->lcdNumber_smoke->display(smoke);
                if(IR == 0)                                         //人体红外传感器
                {
                    ui->pushButton_IR->setStyleSheet("QPushButton{border-image:url(:/img/蓝人体红外传感器.png);border:none;color:rgb(255, 255, 255);}");
                    ui->textEdit_IR->setText("无人");
                }
                else if(IR == 1)
                {
                    ui->textEdit_IR->setText("有人");
                    ui->pushButton_IR->setStyleSheet("QPushButton{border-image:url(:/img/红人体红外传感器.png);border:none;color:rgb(255, 255, 255);}");
                }
                if(port == 1)
                {
                    ui->pushButton_port->setStyleSheet("QPushButton{border-image:url(:/img/蓝色数字1.png);border:none;color:rgb(255, 255, 255);}");
                }
                else if(port == 2)
                {
                    ui->pushButton_port->setStyleSheet("QPushButton{border-image:url(:/img/蓝色数字2.png);border:none;color:rgb(255, 255, 255);}");
                }
        }
        else recvData.clear();
        qDebug()<<"num = "<<recvData<<endl;
}





