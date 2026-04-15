#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    // 判断按钮当前文本是否为“开灯”
    if(ui->pushButton->text() == "开灯")
    {
        // 打开 LED1
        myled.on(fsmpLeds::LED1);
        // 修改按钮文本为“关灯”
        ui->pushButton->setText("关灯");
    }
    else
    {
        // 关闭 LED1
        myled.off(fsmpLeds::LED1);
        // 修改按钮文本为“开灯”
        ui->pushButton->setText("开灯");
    }
}


void Widget::on_pushButton_2_clicked()
{
    qDebug() <<"123987" << endl;
}

