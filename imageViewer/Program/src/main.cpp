#include <QtWidgets>
#include "mainWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CMainWidget w;
    //w.loadPath(qApp->applicationDirPath()+"/Image");//������ֱ�ӽ�Image�������е�ͼƬ�����ؽ���
    w.setFixedSize(800,600);
    w.show();

    int ret = a.exec();
    qDebug()<<"app closed";
    return ret;
}
