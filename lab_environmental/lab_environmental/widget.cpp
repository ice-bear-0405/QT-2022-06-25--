#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //设置窗口的大小，resize：可调
    this->resize(714,451);
    //设置窗口的标题
    this->setWindowTitle("基于cc2530的实验室环境安全检测系统");
}

Widget::~Widget()
{
    delete ui;
}
