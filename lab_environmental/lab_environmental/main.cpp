#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    //标题名称
    w.setWindowTitle("基于ZigBee的实验室环境与安全智能监测");
    w.show();
    return a.exec();
}
