#ifndef THREADONE_H
#define THREADONE_H

#include <QObject>
#include <QtWidgets>
//#include "MyWidget.h"
class ThreadOne : public QThread
{
    Q_OBJECT
public:
    explicit ThreadOne(QObject *parent = nullptr);

signals:
     void over();
public slots:
protected:
    void run();//���߳�ִ�е����ݽ�ͨ�����¸��麯��ʵ��
};

#endif // THREADONE_H
