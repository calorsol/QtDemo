#include "ThreadOne.h"

ThreadOne::ThreadOne(QObject *parent) : QThread(parent)
{

}
void ThreadOne::run()
{
    sleep(10);//ģ��һ��ʱ��5s�ĸ��Ӻ���
    emit over();//���Ӻ��������󷢳��ź�
}
