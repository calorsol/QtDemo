#include "MyWidget.h"
#include "ui_MyWidget.h"
#include <ThreadOne.h>
MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    p_thread = new ThreadOne(this);
    connect(ui->pushButton,&QPushButton::clicked,this,&MyWidget::dealclicked);//���°�ť��ִ��dealclicked()�ۺ���
    connect(timer,&QTimer::timeout,this,&MyWidget::dealtimeout);//���ݶ�ʱ���������źŸ���LCD��ʾ��������
    connect(p_thread,&ThreadOne::over,this,&MyWidget::dealover);//�����ٵ��߳��ڵĸ��Ӻ���ִ�����,����over�ź�,���յ����źź��ͣ�¼�ʱ��

}

MyWidget::~MyWidget()
{
    delete ui;
}
void MyWidget::dealclicked()
{
    timer->start(200);//������ʱ��,ÿ0.1�뷢��һ���ź�
    p_thread->start();//QThread �Ķ���ͨ��start()���������߳��ļ��е�run()����
}
void MyWidget::dealtimeout()//����LCD��ʾ��������
{
    static int time = 0;
    ui->lcdNumber->display(time);
    time++;
}
void MyWidget::dealover()//���յ��źź�ͣ�¼�ʱ��
{
    timer->stop();
}
