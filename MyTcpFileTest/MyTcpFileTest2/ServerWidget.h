#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QTcpServer> //�����׽���
#include <QTcpSocket> //ͨ���׽���
#include <QFile>
#include <QTimer>

class CServerWidget : public QWidget
{
    Q_OBJECT

public:
    CServerWidget(QWidget *parent = 0);
    ~CServerWidget();
public:
    QPushButton *buttonSend;
    qint64 sendSize; //�Ѿ������ļ��Ĵ�С
    void getServerFile();
public slots:
    void serverNewConnection();
    void on_buttonSend_clicked();
    void slotClientReceiveOver();
protected:
    void initServerWidget();
    void sendData();
private:
    QWidget *ServerWidget;
    //QPushButton *buttonSend;
    QTcpServer *tcpServer; //�����׽���
    QTcpSocket *tcpSocket; //ͨ���׽���
    QFile file; //�ļ�����
    QString fileName; //�ļ�����
    qint64 fileSize; //�ļ���С
    QTimer timer; //��ʱ��
};

#endif // SERVERWIDGET_H
