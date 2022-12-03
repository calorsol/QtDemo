#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QTcpServer> //�����׽���
#include <QTcpSocket> //ͨ���׽���
#include <QFile>
#include <QTimer>
#include <QtCore>
#include "ClientTreeView.h"
class CClientWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CClientWidget(QWidget *parent = nullptr);

signals:
    void signalBufEmit(QByteArray byteArray);

public slots:
    void on_buttonConnect_clicked();
    void clientReadFile();
    void slotTreeDisplay();

private:
    QFile file; //�ļ�����
    QString fileName; //�ļ�����
    qint64 fileSize; //�ļ���С
    qint64 recvSize; //�Ѿ������ļ��Ĵ�С
protected:
    void initClientWidget();

public:
    QWidget *ClientWidget;
    //QPushButton *buttonConnect;
    bool isStart;   //��־λ���Ƿ�Ϊͷ����Ϣ
    QByteArray bufAll;
    QTcpSocket *tcpSocket;
    CClientTreeView *p_treeView;
    QPushButton *buttonConnect;
    QPushButton *buttonDisplayTree;

};

#endif // CLIENTWIDGET_H
