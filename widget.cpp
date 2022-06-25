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
    this->setWindowTitle("zigbee的厨房有害气体检测和控制");
    this->setWindowIcon(QIcon(":/img/温度计.png"));
    QStringList serialNamePort;

            serialPort = new QSerialPort();                                     //新建一个串口对象
            connect(serialPort,SIGNAL(readyRead()),this,SLOT(serialPortReadyRead_Slot()));
            foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
            {
                  serialNamePort<<info.portName();
            }

            ui->serialCb->addItems(serialNamePort);
            serialPort->open(QSerialPort::ReadWrite);                           //打开串口读写模式


        // LCD样式——lcdNumber_temperature2                  ——温度2
            // 设置能显示的位数
            ui->lcdNumber_temperature2->setDigitCount(3);
            // 设置显示的模式为十进制
            ui->lcdNumber_temperature2->setMode(QLCDNumber::Dec);
            // 设置显示外观
            ui->lcdNumber_temperature2->setSegmentStyle(QLCDNumber::Flat);
            // 设置样式
            ui->lcdNumber_temperature2->setStyleSheet("border: 3px solid green; color: green; background: ;");
            // 设置小数点的位置
            ui->lcdNumber_temperature2->setSmallDecimalPoint(false);

                    // LCD样式——lcdNumber_humidity2                  ——湿度2
                        // 设置能显示的位数
                        ui->lcdNumber_humidity2->setDigitCount(3);
                        // 设置显示的模式为十进制
                        ui->lcdNumber_humidity2->setMode(QLCDNumber::Dec);
                        // 设置显示外观
                        ui->lcdNumber_humidity2->setSegmentStyle(QLCDNumber::Flat);
                        // 设置样式
                        ui->lcdNumber_humidity2->setStyleSheet("border: 3px solid green; color: green; background: ;");
                        // 设置小数点的位置
                        ui->lcdNumber_humidity2->setSmallDecimalPoint(false);

                            // LCD样式——lcdNumber_smoke1                  ——烟雾报警1
                                // 设置能显示的位数
                                ui->lcdNumber_smoke1->setDigitCount(3);
                                // 设置显示的模式为十进制
                                ui->lcdNumber_smoke1->setMode(QLCDNumber::Dec);
                                // 设置显示外观
                                ui->lcdNumber_smoke1->setSegmentStyle(QLCDNumber::Flat);
                                // 设置样式
                                ui->lcdNumber_smoke1->setStyleSheet("border: 3px solid green; color: green; background: ;");
                                // 设置小数点的位置
                                ui->lcdNumber_smoke1->setSmallDecimalPoint(false);
                                // LCD样式——lcdNumber_dust                  ——PM2.5
                                    // 设置能显示的位数
                                    ui->lcdNumber_dust->setDigitCount(3);
                                    // 设置显示的模式为十进制
                                    ui->lcdNumber_dust->setMode(QLCDNumber::Dec);
                                    // 设置显示外观
                                    ui->lcdNumber_dust->setSegmentStyle(QLCDNumber::Flat);
                                    // 设置样式
                                    ui->lcdNumber_dust->setStyleSheet("border: 3px solid green; color: green; background: ;");
                                    // 设置小数点的位置
                                    ui->lcdNumber_dust->setSmallDecimalPoint(false);
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

    ui->lcdNumber_temperature2->display(0);
    ui->lcdNumber_humidity2->display(0);
    ui->lcdNumber_smoke1->display(0);
    ui->lcdNumber_dust->display(0);
}


void Widget::on_SerialPort_readyRead()//串口的读取
{
    QByteArray recvData = serialPort->readAll();

    //数据的处理_修改数据处理
    /*            (recvData[]-'0')*100 + (recvData[]-'0')*10 + (recvData[]-'0');             */

    int port = (recvData[0]='0');
    int smoke1 = (recvData[2]-'0')*100 + (recvData[3]-'0')*10 + (recvData[4]-'0');
    int temperature1 = (recvData[6]-'0')*10 + (recvData[7]-'0');
    int humidity1 = (recvData[9]-'0')*10 + (recvData[10]-'0');
    int dust = (recvData[12]-'0')*100 + (recvData[13]-'0')*10 + (recvData[14]-'0');



    //判断串口+显示LCD数值
    if(recvData.size() >= 12)
    {
        ui->lcdNumber_smoke1->display(smoke1);
        ui->lcdNumber_temperature2->display(temperature1);
        ui->lcdNumber_humidity2->display(humidity1);
        ui->lcdNumber_dust->display(dust);

        if(dust > 26)
        {
            ui->pushButton_dust->setStyleSheet("QPushButton{border-image: url(:/img/红粉尘颗粒.png);border:none;color:rgb(255, 255, 255);}");
        }
        else if(dust <= 26)
        {
            ui->pushButton_dust->setStyleSheet("QPushButton{border-image: url(:/img/黑粉尘颗粒.png);border:none;color:rgb(255, 255, 255);}");
        }
    }
    else recvData.clear();
    qDebug()<<"num ="<<recvData<<endl;
}
