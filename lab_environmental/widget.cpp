#include "widget.h"
#include "ui_widget.h"
#include "QSerialPort"                                              //使用串口功能
#include "QSerialPortInfo"                                          //串口输出信息
#include <QByteArray>
#include <QIODevice>
#include <QMessageBox>
#include "QString"
#include "QDebug"
#include <cstdio>

double temp1[10];
double temp2[10];
char temp3[10];

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(1500,1100);
    this->setWindowIcon(QIcon(":/img/花卉pink.png"));

/*******************************************************所有的数码管样式代码****************************************************************/
// LCD样式——lcdNumber_temperature                ——温度显示
            // 设置能显示的位数
            ui->lcdNumber_temperature->setDigitCount(4);
            // 设置显示的模式为十进制
            ui->lcdNumber_temperature->setMode(QLCDNumber::Dec);
            // 设置显示外观
            ui->lcdNumber_temperature->setSegmentStyle(QLCDNumber::Flat);
            // 设置样式
            ui->lcdNumber_temperature->setStyleSheet("border: 3px solid green; color: bule; background: ;");
            // 设置小数点的位置
            ui->lcdNumber_temperature->setSmallDecimalPoint(false);
// LCD样式——lcdNumber_humidity                ——湿度显示
            // 设置能显示的位数
            ui->lcdNumber_humidity->setDigitCount(4);
            // 设置显示的模式为十进制
            ui->lcdNumber_humidity->setMode(QLCDNumber::Dec);
            // 设置显示外观
            ui->lcdNumber_humidity->setSegmentStyle(QLCDNumber::Flat);
            // 设置样式
            ui->lcdNumber_humidity->setStyleSheet("border: 3px solid green; color: bule; background: ;");
            // 设置小数点的位置
            ui->lcdNumber_humidity->setSmallDecimalPoint(false);
// LCD样式——lcdNumber_smoke                 ——烟雾显示
            // 设置能显示的位数
            ui->lcdNumber_smoke->setDigitCount(4);
            // 设置显示的模式为十进制
            ui->lcdNumber_smoke->setMode(QLCDNumber::Dec);
            // 设置显示外观
            ui->lcdNumber_smoke->setSegmentStyle(QLCDNumber::Flat);
            // 设置样式
            ui->lcdNumber_smoke->setStyleSheet("border: 3px solid green; color: bule; background: ;");
            // 设置小数点的位置
            ui->lcdNumber_smoke->setSmallDecimalPoint(false);
// LCD样式——lcdNumber_light                 ——光照强度显示
            // 设置能显示的位数
            ui->lcdNumber_light->setDigitCount(4);
            // 设置显示的模式为十进制
            ui->lcdNumber_light->setMode(QLCDNumber::Dec);
            // 设置显示外观
            ui->lcdNumber_light->setSegmentStyle(QLCDNumber::Flat);
            // 设置样式
            ui->lcdNumber_light->setStyleSheet("border: 3px solid green; color: bule; background: ;");
            // 设置小数点的位置
            ui->lcdNumber_light->setSmallDecimalPoint(false);

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
    //ui->lcdNumber_CO2->display(0);

    ui->lcdNumber_temperature->display(0);
    ui->lcdNumber_humidity->display(0);
    ui->lcdNumber_light->display(0);
    ui->lcdNumber_smoke->display(0);
    ui->plainTextEdit->clear();
    ui->pushButton_fan->setStyleSheet("QPushButton{border-image:url(:/icon/绿色风扇.png);border:none;color:rgb(255, 255, 255);}");
    ui->pushButton_beep->setStyleSheet("QPushButton{border-image:url(:/icon/绿色声音.png);border:none;color:rgb(255, 255, 255);}");

}

//================================================串口读取问题=========================================================

void Widget::on_SerialPort_readyRead()//串口的读取
{
    QByteArray recvData = serialPort->readAll();


    //端口号1
    temp1[0] = (recvData[0]-'0');
    //光照强度
    temp1[1] = (recvData[2]-'0')*1000 + (recvData[3]-'0')*100 + (recvData[4]-'0')*10 + (recvData[5]-'0');

    //端口号2
    temp2[0] = recvData[0]-'0';
    //温度
    temp2[1] = (recvData[2]-'0')*10 + (recvData[3]-'0');
    //湿度
    temp2[2] = (recvData[5]-'0')*10 + (recvData[6]-'0');
    //CO2
    temp2[3] = (recvData[8]-'0')*100 + (recvData[9]-'0')*10 + (recvData[10]-'0');
    //风扇状态
    temp2[4] = recvData[12]-'0';



//端口3
//    (recvData[2]<'0') ? temp3[1] = (recvData[2]-'0'):temp3[1] = char(recvData[2]);
//    (recvData[3]<'0') ? temp3[2] = (recvData[3]-'0'):temp3[2] = char(recvData[3]);
//    (recvData[4]<'0') ? temp3[3] = (recvData[4]-'0'):temp3[3] = char(recvData[4]);
//    (recvData[5]<'0') ? temp3[4] = (recvData[5]-'0'):temp3[4] = char(recvData[5]);
//    (recvData[6]<'0') ? temp3[5] = (recvData[6]-'0'):temp3[5] = char(recvData[6]);
//    (recvData[7]<'0') ? temp3[6] = (recvData[7]-'0'):temp3[6] = char(recvData[7]);
//    (recvData[8]<'0') ? temp3[7] = (recvData[8]-'0'):temp3[7] = char(recvData[8]);
//    (recvData[9]<'0') ? temp3[8] = (recvData[9]-'0'):temp3[8] = char(recvData[9]);

    temp3[0] = (recvData[0]-'0');
//    temp3[1] = (recvData[2]-'0');
//    temp3[2] = (recvData[3]-'0');
//    temp3[3] = (recvData[4]-'0');
//    temp3[4] = (recvData[5]-'0');
//    temp3[5] = (recvData[6]-'0');
//    temp3[6] = (recvData[7]-'0');
//    temp3[7] = (recvData[8]-'0');
//    temp3[8] = (recvData[9]-'0');

    /*==============================================================================
    *测试端口1
    *                    0        1
    * 串口协议：          端口    光照强度
    *                    1       0029
    *==============================================================================
    *
    * 测试端口2
    *                   0      1      2      3         4
    * 串口协议：         端口   温度    湿度    CO2     风扇状态
    *                   2     29      22    008       1（关）
    *==============================================================================
    *
    * 测试端口3
    *                   0        1
    * 串口协议：         端口     RFID
    *                   3     0f438112
    *============================================================================*/

if(recvData.size() >= 3)   //判断收取的字符长度，不够长度直接clean掉
{

    if((temp1[0]) == 1)//    端口1
    {
//        显示光照强度
        ui->lcdNumber_light->display(temp1[1]);
//        ui->textEdit->setText("serialBuffer");
    }

    else if((temp2[0]) == 2)//    端口2
    {
//        显示温度
        ui->lcdNumber_temperature->display(temp2[1]);
//        显示湿度
        ui->lcdNumber_humidity->display(temp2[2]);
//        显示CO2
        ui->lcdNumber_smoke->display(temp2[3]);
        if(temp2[4] == 0)
                {
                    ui->pushButton_fan->setStyleSheet("QPushButton{border-image:url(:/icon/绿色风扇.png);border:none;color:rgb(255, 255, 255);}");
                    ui->pushButton_beep->setStyleSheet("QPushButton{border-image:url(:/icon/绿色声音.png);border:none;color:rgb(255, 255, 255);}");
        }
                else
                {
                    ui->pushButton_fan->setStyleSheet("QPushButton{border-image:url(:/icon/红色风扇.png);border:none;color:rgb(255, 255, 255);}");
                    ui->pushButton_beep->setStyleSheet("QPushButton{border-image:url(:/icon/红色声音.png);border:none;color:rgb(255, 255, 255);}");
        }
    }
    if((temp3[0]) == 3)//    端口3
    {
//        显示RFID检测到的NFC卡ID号：
        //sprintf_s(buffer,"%f",temp3);
//        ui->textEdit->sprintf("%s",strValue);
//            QString buf;
//            buf = QString(serialPort->readAll());
//        connect(serialPort,&QIODevice::readyRead,this,&Widget::serialPortReadyRead);


        QByteArray ab = recvData.mid(2,8);
        ui->plainTextEdit->appendPlainText(ab);//好用的
        if(ab == "0F438814")
        {
            ui->plainTextEdit->appendPlainText("姓名：万恒志");
        }


//            QString buf;
//            buf = QString(serialPort->readAll());
//        QByteArray ab(buf);
//        QString str = ab;
//        ui->plainTextEdit->appendPlainText(str);

    }
//    QString serialBuffer  = QString(serialPort->readAll());
//    serialBuffer=serialBuffer.append(serialBufferTmp);


}


else recvData.clear();
qDebug()<<"num ="<<recvData<<endl;
//qDebug()<<"num ="<<recvData[10]<<endl;

}

//void Widget::serialPortReadyRead()
//{
//    QString buf;
//    buf = QString(serialPort->readAll());
//    ui->plainTextEdit->appendPlainText(buf);
//}

Widget::~Widget()
{
    delete ui;
}






