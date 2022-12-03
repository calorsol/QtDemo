#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QTcpServer> //�����׽���
#include <QTcpSocket> //ͨ���׽���
#include <QFile>
#include <QTimer>
#include "ServerTreeView.h"
class CClientWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CClientWidget(QWidget *parent = nullptr);
    ~CClientWidget();
signals:


private slots:
    void on_buttonConnect_clicked();
    void readFile();
protected:
    void createUi();
public:
    QWidget *ClientWidget;
    QGridLayout *gridLayout_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEditPort;
    QLineEdit *lineEditIP;
    QLabel *label_2;
    QPushButton *buttonConnect;
    QProgressBar *progressBar;
    QTextEdit *textEdit;
    CServerTreeView *p_treeView;

private:
    QTcpSocket *tcpSocket;
    QFile file; //�ļ�����
    QString fileName; //�ļ�����
    qint64 fileSize; //�ļ���С
    qint64 recvSize; //�Ѿ������ļ��Ĵ�С
    bool isStart;   //��־λ���Ƿ�Ϊͷ����Ϣ
};

#endif // CLIENTWIDGET_H
