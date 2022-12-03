#include "ServerWidget.h"

CServerWidget::CServerWidget(QWidget *parent)
    : QWidget(parent)
{
    initServerWidget();
    //����������������һ���µ����ӣ�ִ�вۺ���serverNewConnection�е����ݡ�
    connect(tcpServer, SIGNAL(newConnection()),this,SLOT(serverNewConnection()));
    //��ȡ�����˵��ļ����ȴ�send��ť������,��ֻ�Ƿ���ͷ����Ϣ���ѣ�������һ��С�Ͷ�ʱ�����ȶ�ʱ��ִ���ִ꣬��������һ��connect����
    connect(buttonSend,SIGNAL(pressed()), this, SLOT(on_buttonSend_clicked()));
    //����server���Դ�tcp�еõ�readyread���źŵĻ���֤������clientӦ���Ѿ������ˡ�
    connect(tcpSocket, SIGNAL(readyRead()),this,SLOT(slotClientReceiveOver()));
    connect(&timer, &QTimer::timeout,
            [=]()
    {
        qDebug()<<"ServerWidget timer";
        //�رն�ʱ��
        timer.stop();
        //�����ļ�
        sendData();
    }
    );
}

CServerWidget::~CServerWidget()
{

}

//��С�����ڳ�ʼ��Server
void CServerWidget::initServerWidget()
{
    ServerWidget = new QWidget(this);
    ServerWidget->resize(574, 422);
    tcpServer = new QTcpServer(this);
    //����
    tcpServer->listen(QHostAddress::Any, 8888);
    setWindowTitle("Server Port��8888");
    buttonSend = new QPushButton(this);
    buttonSend->setText("Send File");
}

//��С���ÿͻ��˽�������Ϣ�󣬻Ὣfile doneд��socket��������˶�ȡ��file doneʱ����ر�socket����
void CServerWidget::slotClientReceiveOver()
{
    QByteArray buf = tcpSocket->readAll();
    if(QString(buf) == "file done")
    {//�ļ��������
        file.close();
        //�Ͽ��ͻ��˶˿�
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
    }

}

//����ĺ����ɻ�ȡ�ͻ��˵�ip��port
void CServerWidget::serverNewConnection()
{
    qDebug()<<"CServerWidget::serverNewConnection";
    //ȡ�����������ӵ��׽���
    tcpSocket = tcpServer->nextPendingConnection();
    //��ȡ�Է���ip�Ͷ˿�
    QString ip = tcpSocket->peerAddress().toString();
    quint16 port = tcpSocket->peerPort();
    QString str = QString("[%1:%2] success connect").arg(ip).arg(port);
    qDebug()<<"CServerWidget::serverNewConnection peer Connect ip and port is"<<ip<<port;
    getServerFile();//��ȡ������ļ�
}

//��С�����ڵ����send��ťʱ������ͷ����Ϣ����������ʱ������ֹ����
void CServerWidget::on_buttonSend_clicked()
{

    //buttonSend->setEnabled(false);
    //qDebug()<<"buttonSend";
    //�ȷ����ļ�ͷ��Ϣ  �ļ���##�ļ���С
    qDebug()<<"CServerWidget::on_buttonSend_clicked"<<fileName<<fileSize;
    QString head = QString("%1##%2").arg(fileName).arg(fileSize);
    //����ͷ����Ϣ
    qint64 len = tcpSocket->write( head.toUtf8() );
    qDebug()<<"CServerWidget::on_buttonSend_clicked"<<len;
    //qint64 len = 10;
    if(len > 0)//ͷ����Ϣ���ͳɹ�
    {
        qDebug()<<"CServerWidget::on_buttonSend_clicked sendhead success";
        //�����������ļ���Ϣ
        //��ֹTCP��
        //��Ҫͨ����ʱ����ʱ 20 ms
        timer.start(20);
    }
    else
    {
        qDebug() << "The head message send fail 142";
        file.close();

        //buttonSend->setEnabled(false);
    }
}

//��С�����ڷ������������ݣ�����̶�ÿ�η������ݵĴ�С������ͳһ����
void CServerWidget::sendData()
{
    qDebug()<<"exec server sendData";
    qint64 len = 0;
    //QByteArray array = file.readAll();
    //tcpSocket->write(array);
    //tcpSocket->readAll();
    sendSize =0;
    do
    {
        //ÿ�η������ݵĴ�С
        char buf[4*1024] = {0};
        len = 0;
        //���ļ��ж�����
        len = file.read(buf, sizeof(buf));
        //�������ݣ������٣�������
        //len = tcpSocket->
        len = tcpSocket->write(buf, len);
        //���͵�������Ҫ�ۻ�
        sendSize += len;
    }while(len > 0);
    qDebug()<<"CServerWidget::sendData"<<sendSize;
}

//�������ڻ�ȡҪ������ļ�
void CServerWidget::getServerFile()
{
    qDebug()<<"CServerWidget::getServerFile() exec";
    QString filePath =  qApp->applicationDirPath()+"/xml/test.xml";
    qDebug()<<"filePath"<<filePath;
    if(false == filePath.isEmpty()) //���ѡ���ļ�·����Ч
    {
        fileName.clear();
        fileSize = 0;

        //��ȡ�ļ���Ϣ
        QFileInfo info(filePath);
        fileName = info.fileName(); //��ȡ�ļ�����
        fileSize = info.size(); //��ȡ�ļ���С
        sendSize = 0; //�����ļ��Ĵ�С

        //ֻ����ʽ���ļ�
        //ָ���ļ�������
        file.setFileName(filePath);
        //���ļ�
        bool isOk = file.open(QIODevice::ReadOnly);
        if(false == isOk)
        {
            qDebug() << "open file fail with readonly 106";
        }
        else {
            qDebug()<<"the path is right";
        }
    }
    else
    {
        qDebug() << "Choose file path is wrong in ServerWidget getFile";
    }
}


