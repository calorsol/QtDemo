#include "ClientWidget.h"

CClientWidget::CClientWidget(QWidget *parent) : QWidget(parent)
{
    initClientWidget();
    connect(buttonConnect,SIGNAL(clicked()), this, SLOT(on_buttonConnect_clicked()));//һ����ӡ��connect success�������ӳɹ���
    isStart = true;
    connect(tcpSocket, SIGNAL(readyRead()),this,SLOT(clientReadFile()));
    connect(buttonDisplayTree,&QPushButton::clicked,this,&CClientWidget::slotTreeDisplay);
}

//���ۺ���������ʾ���νṹ��
void CClientWidget::slotTreeDisplay()
{
    //��ʾ���ӳɹ�
    CClientTreeView *w2 = new CClientTreeView();
    w2->setWindowTitle("Tree");
    w2->show();
    QString xmlPath3 = qApp->applicationDirPath()+"/client/test2.xml";
    QFile fileTemp(xmlPath3);
    fileTemp.remove();

}


//��С�����ڶ�ClientWidget���г�ʼ��
void CClientWidget::initClientWidget()
{
    tcpSocket = new QTcpSocket(this);
    ClientWidget = new QWidget(this);
    ClientWidget->resize(400, 300);
    buttonConnect = new QPushButton(ClientWidget);
    buttonConnect->setText("connect");
    buttonDisplayTree = new QPushButton(ClientWidget);
    buttonDisplayTree->move(0,60);
    buttonDisplayTree->setText("DisplayTree");
}

//��С������������Ӧ��ַ��socket
void CClientWidget::on_buttonConnect_clicked()
{

    //tcpSocket = new QTcpSocket(this);
    QString ip = "127.0.0.1";
    quint16 port = 8888;
    qDebug()<<"CClientWidget::initClientWidget port"<<port;
    tcpSocket->connectToHost(QHostAddress(ip), port);
    connect(tcpSocket, &QTcpSocket::connected,
            [=]()
    {
        //��ʾ���ӳɹ�
        qDebug()<<"connect success";
    }
    );
}

//��С�����ڵ�Socket��readyRead()ʱ��ִ�ж�ȡsocket���ļ�
void CClientWidget::clientReadFile()
{
    //ȡ�����յ�����
    QByteArray buf = tcpSocket->readAll();//�����buf���ݸ�ServerTreeView
    qDebug()<<"CClientWidget::clientReadFile buf.size()"<<buf.size();

    if(true == isStart)//�ڵ�һ���ļ�ͷ��������ʱ�򣬲�Ҫȥ������Щ���ݣ��ȵڶ��λ�����n�����ȥ����
    {//����ͷ
        isStart = false;
        //����ͷ����Ϣ QString buf = "hello##1024"
        //                    QString str = "hello##1024#mike";
        //                            str.section("##", 0, 0)
        //��ʼ��
        //�ļ���
        fileName = QString(buf).section("##", 0, 0);
        //�ļ���С
        fileSize = QString(buf).section("##", 1, 1).toInt();
        qDebug()<<"CClientWidget::clientReadFile fileSize"<<fileSize;
        recvSize = 0;   //�Ѿ������ļ���С

        //���ļ�
        //�����ļ�����
        QString xmlPath2 = qApp->applicationDirPath()+"/client/test2.xml";
        file.setFileName(xmlPath2);
        //file.setFileName(fileName);//���ɵ�file��Ŀ¼�ڵ�ǰ�ļ�����qApp->applicationDirPath()

        //ֻд��ʽ��ʽ�����ļ�
        bool isOk = file.open(QIODevice::WriteOnly);
        if(false == isOk)
        {
            qDebug() << "CClientWidget::clientReadFile WriteOnly error 49";
            tcpSocket->disconnectFromHost(); //�Ͽ�����
            tcpSocket->close(); //�ر��׽���
            return; //������ļ�ʧ�ܣ��жϺ���
        }
    }
    else //�ļ���Ϣ
    {
        bufAll+=buf;
        //���������ﲻ�ϵĽ�buf���ܵ�bufAll֮��
        qint64 len = file.write(buf);//����������һ��д���ļ��Ĳ�����
        qDebug()<<"CClientWidget::clientReadFile len"<<len;
        if(len >0) //�������ݴ���0
        {
            recvSize += len; //�ۼƽ��մ�С

        }

        if(recvSize == fileSize) //�ļ��������
        {
            qDebug()<<"file done";
            //emit signalBufEmit(bufAll);
            //p_treeView = new CClientTreeView();

            emit signalBufEmit(bufAll);
            //p_treeView->initTreeView(bufAll);
            // p_treeView->receiveByte(bufAll);
            //�ȸ�������(�����ļ���ɵ���Ϣ)
            tcpSocket->write("file done");
            // textEdit->append("file receive over");
            QMessageBox::information(this, "over", "file receive over");
            file.close(); //�ر��ļ�
            //�Ͽ�����
            tcpSocket->disconnectFromHost();
            tcpSocket->close();
        }
    }
}