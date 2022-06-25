#include "widget.h"
#include "ui_widget.h"
#include<QSerialPort>                                               //使用串口功能
#include<QSerialPortInfo>                                           //串口输出信息
#include <QByteArray>
#include <QIODevice>
#include <QMessageBox>
#include "QString"
#include "QDebug"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("基于ZigBee的智能LED照明系统");
    this->setWindowIcon(QIcon(":/img/灯好看"));
    //this->setWindowOpacity(0.9);
    QStringList serialNamePort;

            serialPort = new QSerialPort();                                     //新建一个串口对象
            connect(serialPort,SIGNAL(readyRead()),this,SLOT(serialPortReadyRead_Slot()));
            foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
            {
                  serialNamePort<<info.portName();
            }

            ui->serialCb->addItems(serialNamePort);
            serialPort->open(QSerialPort::ReadWrite);                           //打开串口读写模式

            //所有的数码管样式代码
            // LCD样式——lcdNumber_sky                  ——光强度
                    // 设置能显示的位数
                    ui->lcdNumber_sky ->setDigitCount(4);
                    // 设置显示的模式为十进制
                    ui->lcdNumber_sky ->setMode(QLCDNumber::Dec);
                    // 设置显示外观
                    ui->lcdNumber_sky ->setSegmentStyle(QLCDNumber::Flat);
                    // 设置样式
                    ui->lcdNumber_sky ->setStyleSheet("border: 3px solid green; color: green; background: ;");
                    // 设置小数点的位置
                    ui->lcdNumber_sky ->setSmallDecimalPoint(false);
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
    connect(serialPort,&QIODevice::readyRead,this,&Widget::on_SerialPort_readyRead);
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

    ui->textEdit_IR->setText(" ");
    //ui->textEdit_sky->setText(" ");
}


void Widget::on_SerialPort_readyRead()//串口的读取
{
/*           recvData[]-'0'                   */

    QByteArray recvData = serialPort->readAll();

    //数据的处理_修改数据处理
    int sky = (recvData[0]-'0')*1000 + (recvData[1]-'0')*100 + (recvData[2]-'0')*10 + (recvData[3]-'0');
    int key = (recvData[5]-'0');
    int IR = (recvData[7]-'0');

    //判断串口+显示LCD数值
    if(recvData.size() >= 6)
    {
        if(IR == 0)//红外传感器
        {
            ui->pushButton_IR->setStyleSheet("QPushButton{border-image:url(:/img/蓝-红外双鉴.png);border:none;color:rgb(255, 255, 255);}");
            ui->textEdit_IR->setText("无人");
            ui->pushButton_IR_2->setStyleSheet("QPushButton{border-image:url(:/img/蓝-红外双鉴.png);border:none;color:rgb(255, 255, 255);}");
            ui->textEdit_IR_2->setText("无人");
        }
        else if(IR == 1)
        {
            ui->pushButton_IR->setStyleSheet("QPushButton{border-image:url(:/img/红-红外双鉴.png);border:none;color:rgb(255, 255, 255);}");
            ui->textEdit_IR->setText("有人");
            ui->pushButton_IR_2->setStyleSheet("QPushButton{border-image:url(:/img/红-红外双鉴.png);border:none;color:rgb(255, 255, 255);}");
            ui->textEdit_IR_2->setText("有人");
        }
        if(key == 0)//开关状态
        {
            ui->pushButton_key->setStyleSheet("QPushButton{border-image:url(:/img/关闭.png);border:none;color:rgb(255, 255, 255);}");
            //ui->textEdit_sky->setText("开关状态关闭");
        }
        else if(key == 1)
        {
            ui->pushButton_key->setStyleSheet("QPushButton{border-image:url(:/img/开启.png);border:none;color:rgb(255, 255, 255);}");
            //ui->textEdit_sky->setText("开关状态打开");
        }
        ui->lcdNumber_sky->display(sky);


        /*if(port == 1)//终端号
        {
            ui->pushButton_fire->setStyleSheet("QPushButton{border-image:url(:/img/数字1.png);border:none;color:rgb(255, 255, 255);}");
        }
        else if(port == 2)
        {
            ui->pushButton_fire->setStyleSheet("QPushButton{border-image:url(:/img/数字2.png);border:none;color:rgb(255, 255, 255);}");
        }
        */

    }
    else recvData.clear();
    qDebug()<<"num ="<<recvData<<endl;
}
