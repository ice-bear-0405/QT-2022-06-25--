#include "widget.h"
#include "ui_widget.h"
#include "QSerialPort"                                              //使用串口功能
#include "QSerialPortInfo"                                          //串口输出信息
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
    this->resize(1048,962);
    this->setWindowIcon(QIcon(":/img/花卉pink.png"));

/*******************************************************所有的数码管样式代码****************************************************************/
// LCD样式——lcdNumber_temperature                ——温度显示
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

// LCD样式——lcdNumber_humidity                   ——湿度显示
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

// LCD样式——lcdNumber_CO2                         ——CO2
        // 设置能显示的位数
        ui->lcdNumber_CO2->setDigitCount(3);
        // 设置显示的模式为十进制
        ui->lcdNumber_CO2->setMode(QLCDNumber::Dec);
        // 设置显示外观
        ui->lcdNumber_CO2->setSegmentStyle(QLCDNumber::Flat);
        // 设置样式
        ui->lcdNumber_CO2->setStyleSheet("border: 1px solid green; color: green; background: ;");
        // 设置小数点的位置
        ui->lcdNumber_CO2->setSmallDecimalPoint(false);

// LCD样式——lcdNumber_soilHumidity        ——土壤湿度
        // 设置能显示的位数
        ui->lcdNumber_soilHumidity->setDigitCount(3);
        // 设置显示的模式为十进制
        ui->lcdNumber_soilHumidity->setMode(QLCDNumber::Dec);
        // 设置显示外观
        ui->lcdNumber_soilHumidity->setSegmentStyle(QLCDNumber::Flat);
        // 设置样式
        ui->lcdNumber_soilHumidity->setStyleSheet("border: 1px solid green; color: green; background: ;");
        // 设置小数点的位置
        ui->lcdNumber_soilHumidity->setSmallDecimalPoint(false);

/********************************************************串口读写部分*****************************************************************/
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

void Widget::on_openBt_clicked()                                            //打开串口
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
             QMessageBox::information(this,"提示","串口打开成功");
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
    ui->lcdNumber_CO2->display(0);
    ui->lcdNumber_humidity->display(0);
    ui->lcdNumber_soilHumidity->display(0);
    ui->lcdNumber_temperature->display(0);
}

void Widget::on_SerialPort_readyRead()//串口的读取
{
    QByteArray recvData = serialPort->readAll();

        temp1[0] = (recvData[0]-'0');
        temp1[1] = (recvData[2]-'0')*100 + (recvData[3]-'0')*10 + (recvData[4]-'0');
        temp1[2] = (recvData[6]-'0')*10 + (recvData[7]-'0');
        temp1[3] = (recvData[9]-'0')*10 + (recvData[10]-'0');
        temp1[4] = (recvData[12]-'0')*100 + (recvData[13]-'0')*10 + recvData[14]-'0';
        temp1[5] = (recvData[16]-'0')*10 + (recvData[17]-'0');
        temp1[6] = (recvData[19]-'0')*10 + (recvData[20]-'0');

        temp2[0] = recvData[0]-'0';
        temp2[1] = (recvData[2]-'0')*100 + (recvData[3]-'0')*10 + (recvData[4]-'0');
        temp2[2] = (recvData[6]-'0')*10 + (recvData[7]-'0');
        temp2[3] = (recvData[9]-'0')*10 + (recvData[10]-'0');
        temp2[4] = (recvData[12]-'0')*100 + (recvData[13]-'0')*10 + recvData[14]-'0';
        temp2[5] = (recvData[16]-'0')*10 + (recvData[17]-'0');
        temp2[6] = (recvData[19]-'0')*10 + (recvData[20]-'0');

   /*  测试
    *
    *
    *
    *
    *           0      1     2      3     4   5      6
    * 串口协议：端口  二氧化碳 温度  湿度 土壤湿度 水泵 风扇
    *                 123   45   67     45    1    1
    */

if(recvData.size() >= 10)//判断收取的字符长度，不够长度直接clean掉
{
    if((temp1[0]) == 1)
{
        if(temp1[3] == 0 | temp1[2] == 0)
        {
            temp1[3] = 45;
            temp1[2] = 22;
        }
    ui->lcdNumber_humidity->display(temp1[3]);
    ui->lcdNumber_soilHumidity->display(temp1[4]);
    ui->lcdNumber_temperature->display(temp1[2]);

    if(temp1[5] == 0)
    {
        ui->pushButton_pump->setStyleSheet("QPushButton{border-image:url(:/img/水泵-开.png);border:none;color:rgb(255, 255, 255);}");
    }
    else
    {
        ui->pushButton_pump->setStyleSheet("QPushButton{border-image:url(:/img/水泵-关.png);border:none;color:rgb(255, 255, 255);}");
    }
}

    else if((temp2[0]) == 2)
    {

        ui->lcdNumber_CO2->display(temp2[1]);
        if(temp2[6] == 1)
        {
            ui->pushButton_fan->setStyleSheet("QPushButton{border-image:url(:/img/红风扇.png);border:none;color:rgb(255, 255, 255);}");
        }
        else
        {
            ui->pushButton_fan->setStyleSheet("QPushButton{border-image:url(:/img/蓝风扇.png);border:none;color:rgb(255, 255, 255);}");
        }



    }



}
else recvData.clear();
qDebug()<<"num ="<<recvData<<endl;
}

Widget::~Widget()
{
    delete ui;
}








