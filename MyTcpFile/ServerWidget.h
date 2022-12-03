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
    void sendData(); //�����ļ�����
protected:
    void createUi();
//    QPushButton *buttonFile;
//    QPushButton *buttonSend;
private:
    QWidget *ServerWidget;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QTextEdit *textEdit;
    QPushButton *buttonFile;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *buttonSend;

private slots:
    void on_buttonFile_clicked();
    void on_buttonSend_clicked();
private:
    QTcpServer *tcpServer; //�����׽���
    QTcpSocket *tcpSocket; //ͨ���׽���

    QFile file; //�ļ�����
    QString fileName; //�ļ�����
    qint64 fileSize; //�ļ���С
    qint64 sendSize; //�Ѿ������ļ��Ĵ�С

    QTimer timer; //��ʱ��
};

#endif // SERVERWIDGET_H
