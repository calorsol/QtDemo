#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "ImageViewer.h"

class CMainWidget:public QWidget
{
    Q_OBJECT
public:
    CMainWidget(QWidget *parent = Q_NULLPTR);
    ~CMainWidget();
    void loadPath(QString path);//���ڼ��ع̶���ַ������ͼƬ��������������Ѷ���������ʹ�õ��ǣ���xml�ļ��н��ж�ȡ
public slots:
    void slotItemChanged(QString);//��Ҫ���ص�ͼƬ�������仯
protected:
    void createUi();
protected:
    CDeepPictureViewer *p_viewer;
    CSingleImageView   *p_imageLabel;

};

#endif // MAINWIDGET_H
