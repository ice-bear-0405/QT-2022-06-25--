#include "widget.h"
#include "ui_widget.h"
#include <QWidget>
#include<QSerialPort>                                               //使用串口功能
#include<QSerialPortInfo>                                           //串口输出信息
#include <QLCDNumber>
#include <QByteArray>
#include <QIODevice>
#include <QMessageBox>
#include <QDebug>

int temp1[4]= {0};//终端1数据
int temp2[2]= {0};//终端2数据

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //this->resize(714,451);
    this->setWindowTitle("基于cc2530的食堂环境监测");
    this->setWindowIcon(QIcon(":/img/食堂图标.png"));

    // LCD样式——lcdNumber_temperature                  ——温度
    // 设置能显示的位数
    ui->lcdNumber_temperature->setDigitCount(3);
    // 设置显示的模式为十进制
    ui->lcdNumber_temperature->setMode(QLCDNumber::Dec);
    // 设置显示外观
    ui->lcdNumber_temperature->setSegmentStyle(QLCDNumber::Flat);
    // 设置样式
    ui->lcdNumber_temperature->setStyleSheet("border: 3px solid green; color: green; background: ;");
    // 设置小数点的位置
    ui->lcdNumber_temperature->setSmallDecimalPoint(false);

    // LCD样式——lcdNumber_humidity                  ——湿度
    // 设置能显示的位数
    ui->lcdNumber_humidity->setDigitCount(3);
    // 设置显示的模式为十进制
    ui->lcdNumber_humidity->setMode(QLCDNumber::Dec);
    // 设置显示外观
    ui->lcdNumber_humidity->setSegmentStyle(QLCDNumber::Flat);
    // 设置样式
    ui->lcdNumber_humidity->setStyleSheet("border: 3px solid green; color: green; background: ;");
    // 设置小数点的位置
    ui->lcdNumber_humidity->setSmallDecimalPoint(false);

    // LCD样式——lcdNumber_MQ2                  ——烟雾报警
    // 设置能显示的位数
    ui->lcdNumber_MQ2->setDigitCount(3);
    // 设置显示的模式为十进制
    ui->lcdNumber_MQ2->setMode(QLCDNumber::Dec);
    // 设置显示外观
    ui->lcdNumber_MQ2->setSegmentStyle(QLCDNumber::Flat);
    // 设置样式
    ui->lcdNumber_MQ2->setStyleSheet("border: 3px solid green; color: green; background: ;");
    // 设置小数点的位置
    ui->lcdNumber_MQ2->setSmallDecimalPoint(false);

    // LCD样式——lcdNumber_MQ135                  ——气体
    // 设置能显示的位数
    ui->lcdNumber_MQ135->setDigitCount(3);
    // 设置显示的模式为十进制
    ui->lcdNumber_MQ135->setMode(QLCDNumber::Dec);
    // 设置显示外观
    ui->lcdNumber_MQ135->setSegmentStyle(QLCDNumber::Flat);
    // 设置样式
    ui->lcdNumber_MQ135->setStyleSheet("border: 3px solid green; color: green; background: ;");
    // 设置小数点的位置
    ui->lcdNumber_MQ135->setSmallDecimalPoint(false);

    QStringList serialNamePort;

        serialPort = new QSerialPort();                                     //新建一个串口对象
        connect(serialPort,SIGNAL(readyRead()),this,SLOT(serialPortReadyRead_Slot()));
        foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
        {
              serialNamePort<<info.portName();
        }

        ui->serialCb->addItems(serialNamePort);
        serialPort->open(QSerialPort::ReadWrite);                           //打开串口读写模式

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
    ui->lcdNumber_MQ135->display(0);
    ui->lcdNumber_MQ2->display(0);
    ui->lcdNumber_humidity->display(0);
    ui->lcdNumber_temperature->display(0);
    ui->pushButton_fire->setStyleSheet("QPushButton{border-image: url(:/img/蓝火焰.png);border:none;color:rgb(255, 255, 255);}");
}

void Widget::on_SerialPort_readyRead()//串口的读取
{
    /*  测试
     * 1/105/24/16/000/0/2/000/00/00/026/0/
     *
    */
    //QByteArray recvData = serialPort->readAll();
    //QByteArray recvTemp = serialPort->readAll();
    //数据的处理_修改数据处理
        QByteArray recvData = serialPort->readAll();
        QByteArray recvTemp = serialPort->readAll();

        int temperature = (recvData[6]-'0')*10 + (recvData[7]-'0');
        int humidity = (recvData[9]-'0')*10 + (recvData[10]-'0');
        int MQ2 = (recvData[2]-'0')*100 + (recvData[3]-'0')*10 + (recvData[4]-'0');
        int fire2 = (recvData[18]-'0');
        int fire1 = (recvData[16]-'0');
        int MQ135 = (recvData[12]-'0')*100 + (recvData[13]-'0')*10 + (recvData[14]-'0');
        int port = (recvData[0]-'0');

        if(port == 1)
        {
        temp1[0]= MQ2;
        temp1[1]= temperature;
        temp1[2] = humidity;
        temp1[3] = fire1;
        }
        else if(port == 2)
        {
        temp2[0] = MQ135;
        temp2[1] = fire2;

        }

        if(fire1==0 && fire2==0)//火焰传感器
        {
            ui->pushButton_fire->setStyleSheet("QPushButton{border-image: url(:/img/蓝火焰.png);border:none;color:rgb(255, 255, 255);}");
        }
        else if(fire1==1 || fire2==1)
        {
            ui->pushButton_fire->setStyleSheet("QPushButton{border-image: url(:/img/红火焰.png);border:none;color:rgb(255, 255, 255);}");
        }
        ui->lcdNumber_MQ2->display(temp1[0]);
        ui->lcdNumber_humidity->display(temp1[2]);
        ui->lcdNumber_temperature->display(temp1[1]);
        ui->lcdNumber_MQ135->display(temp2[0]);


    //else recvData.clear();
  qDebug()<<"num ="<<recvData<<endl;
}


