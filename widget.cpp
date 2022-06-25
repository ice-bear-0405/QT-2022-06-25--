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
    this->resize(1048,612);
    this->setWindowTitle("基于ZigBee的智能灯控");
    this->setWindowIcon(QIcon(":/img/2.png"));
    QStringList serialNamePort;

    serialPort = new QSerialPort();                                     //新建一个串口对象
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(serialPortReadyRead_Slot()));
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        serialNamePort<<info.portName();
    }

        ui->serialCb->addItems(serialNamePort);
        serialPort->open(QSerialPort::ReadWrite);                           //打开串口读写模式
        // LCD样式——lcdNumber_smoke                  ——烟雾报警
            // 设置能显示的位数
            ui->lcdNumber_smoke->setDigitCount(3);
            // 设置显示的模式为十进制
            ui->lcdNumber_smoke->setMode(QLCDNumber::Dec);
            // 设置显示外观
            ui->lcdNumber_smoke->setSegmentStyle(QLCDNumber::Flat);
            // 设置样式
            ui->lcdNumber_smoke->setStyleSheet("border: 3px solid green; color: green; background: ;");
            // 设置小数点的位置
            ui->lcdNumber_smoke->setSmallDecimalPoint(false);

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
    ui->lcdNumber_smoke->display(0);
    ui->pushButton_IR1->setStyleSheet("QPushButton{border-image:url(:/img/蓝人像.png);border:mome;color:rgb(255,255,255);}");
    ui->pushButton_IR2->setStyleSheet("QPushButton{border-image:url(:/img/蓝人像.png);border:mome;color:rgb(255,255,255);}");
    ui->textEdit_IR1->setText("现在没有人");
    ui->textEdit_IR2->setText("现在没有人");
}


void Widget::on_SerialPort_readyRead()//串口的读取
{
    /*  测试
     *   113281253
     *        fire = recvData[0]
     * temperature = recvData[1]、recvData[2]
     *    humidity = recvData[3]、recvData[4]
     *         MQ2 = recvData[5]、recvData[6]
     *       MQ135 = recvData[7]、recvData[8]
    */

    QByteArray recvData = serialPort->readAll();

    //数据的处理_修改数据处理
    int sky = recvData[0]-'0';                              //光敏传感器
    int MQ2 = (recvData[1]-'0')*10 + (recvData[2]-'0');         //烟雾报警传感器
    int LED = recvData[3]-'0';                              //LED灯
    int IR1 = recvData[4]-'0';                              //红外1
    int IR2 = recvData[5]-'0';                              //红外2



    //判断串口
    if(recvData.size() >= 4)
    {
        if(sky == 0)
        {
            ui->pushButton_sky->setStyleSheet("QPushButton{border-image: url(:/img/蓝月亮.png);border:none;color:rgb(255, 255, 255);}");
            ui->textEdit_sky->setText("现\n在\n是\n黑\n天\n");
        }
        else if(sky != 1)
        {
            ui->pushButton_sky->setStyleSheet("QPushButton{border-image: url(:/img/红太阳.png);border:none;color:rgb(255, 255, 255);}");
            ui->textEdit_sky->setText("现\n在\n是\n白\n天\n");
        }
        ui->lcdNumber_smoke->display(MQ2);
        if(LED == 0)
        {
            ui->pushButton_light->setStyleSheet("QPushButton{border-image:url(:/img/灯空白.png);border:none;color:rgb(255,255,255);}");
        }
        else if(LED != 0)
        {
            ui->pushButton_light->setStyleSheet("QPushButton{border-image:url(:/img/灯.png);border:none;color:rgb(255,255,255);}");
        }
        if(IR1 == 0)
        {
            ui->pushButton_IR1->setStyleSheet("QPushButton{border-image:url(:/img/蓝人像.png);border:mome;color:rgb(255,255,255);}");
            ui->textEdit_IR1->setText("现在没有人");
        }
        else if(IR1 != 0)
        {
            ui->pushButton_IR1->setStyleSheet("QPushButton{border-image:url(:/img/红人像.png);border:mome;color:rgb(255,255,255);}");
            ui->textEdit_IR1->setText("现在有人");
        }
        if(IR2 == 0)
        {
            ui->pushButton_IR2->setStyleSheet("QPushButton{border-image:url(:/img/蓝人像.png);border:mome;color:rgb(255,255,255);}");
            ui->textEdit_IR2->setText("现在没有人");
        }
        else if(IR2 != 0)
        {
            ui->pushButton_IR2->setStyleSheet("QPushButton{border-image:url(:/img/红人像.png);border:mome;color:rgb(255,255,255);}");
            ui->textEdit_IR2->setText("现在有人");
        }
    }




    else recvData.clear();
    qDebug()<<"num ="<<recvData<<endl;
}

