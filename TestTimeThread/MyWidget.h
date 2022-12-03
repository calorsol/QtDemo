#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include "ThreadOne.h"
#include <QtWidgets>
namespace Ui {
    class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = nullptr);
    ~MyWidget();
protected:
    void dealclicked();
    void dealtimeout();
private:
    Ui::MyWidget *ui;
    QTimer *timer;
    ThreadOne *p_thread;

private slots:
    void dealover();//�������̷߳��صĽ����ź�
};

#endif // MYWIDGET_H
