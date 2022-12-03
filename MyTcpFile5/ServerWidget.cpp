#include "ServerWidget.h"

CServerWidget::CServerWidget(QWidget *parent)
    : QWidget(parent)
{
    createUi();
    //p_saveProject->saveProject();
    //�����׽���
    tcpServer = new QTcpServer(this);

    //����
    tcpServer->listen(QHostAddress::Any, 8888);
    setWindowTitle("Server Port��8888");
    //������ť�����ܰ�
    buttonFile->setEnabled(false);
    buttonSend->setEnabled(false);

    //����ͻ��˳ɹ��ͷ���������
    //tcpServer���Զ����� newConnection()
    connect(tcpServer, SIGNAL(newConnection()),this,SLOT(serverNewConnection()));
    connect(&timer, &QTimer::timeout,
            [=]()
    {
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
void CServerWidget::serverNewConnection()
{
    qDebug()<<"has a new Connnection";

    //ȡ�����������ӵ��׽���
    tcpSocket = tcpServer->nextPendingConnection();
    //��ȡ�Է���ip�Ͷ˿�
    QString ip = tcpSocket->peerAddress().toString();
    quint16 port = tcpSocket->peerPort();

    QString str = QString("[%1:%2] success connect").arg(ip).arg(port);
    textEdit->setText(str); //��ʾ���༭��

    //�ɹ����Ӻ󣬲��ܰ�ѡ���ļ�
    buttonFile->setEnabled(true);
    connect(buttonFile,SIGNAL(pressed()), this, SLOT(on_buttonFile_clicked()));
    connect(buttonSend,SIGNAL(pressed()), this, SLOT(on_buttonSend_clicked()));
    connect(tcpSocket, &QTcpSocket::readyRead,
            [=]()
    {
        //ȡ�ͻ��˵���Ϣ
        QByteArray buf = tcpSocket->readAll();
        if(QString(buf) == "file done")
        {//�ļ��������
            textEdit->append("File send over");
            file.close();

            //�Ͽ��ͻ��˶˿�
            tcpSocket->disconnectFromHost();
            tcpSocket->close();
        }
    }
    );

}
//ѡ���ļ��İ�ť
void CServerWidget::on_buttonFile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "open", "../");
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

        //��ʾ���ļ���·��
        textEdit->append(filePath);
        buttonFile->setEnabled(false);
        buttonSend->setEnabled(true);
    }
    else
    {
        qDebug() << "Chose file path is wrong! 118";
    }

}
//�����ļ���ť
void CServerWidget::on_buttonSend_clicked()
{
    buttonSend->setEnabled(false);
    qDebug()<<"buttonSend";
    //�ȷ����ļ�ͷ��Ϣ  �ļ���##�ļ���С
    QString head = QString("%1##%2").arg(fileName).arg(fileSize);
    //����ͷ����Ϣ
    qint64 len = tcpSocket->write( head.toUtf8() );
    if(len > 0)//ͷ����Ϣ���ͳɹ�
    {
        //�����������ļ���Ϣ
        //��ֹTCP��
        //��Ҫͨ����ʱ����ʱ 20 ms
        timer.start(20);
    }
    else
    {
        qDebug() << "The head message send fail 142";
        file.close();
        buttonFile->setEnabled(true);
        buttonSend->setEnabled(false);
    }
}

void CServerWidget::sendData()
{
    textEdit->append("is sending");
    qDebug()<<"exec server sendData";
    qint64 len = 0;
    //QByteArray array = file.readAll();
    //tcpSocket->write(array);
    //tcpSocket->readAll();
    do
    {
        //ÿ�η������ݵĴ�С
        char buf[4*1024] = {0};
        len = 0;
        //���ļ��ж�����
        len = file.read(buf, sizeof(buf));
        //�������ݣ������٣�������

        len = tcpSocket->write(buf, len);
        //���͵�������Ҫ�ۻ�
        sendSize += len;

    }while(len > 0);
    //     //�Ƿ����ļ����
    //     if(sendSize == fileSize)
    //     {
    //         ui->textEdit->append("�ļ��������");
    //         file.close();

    //         //�ѿͻ��˶˿�
    //         tcpSocket->disconnectFromHost();
    //         tcpSocket->close();
    //     }
}


void CServerWidget::createUi()
{
    ServerWidget = new QWidget(this);
    ServerWidget->resize(574, 422);
    buttonFile = new QPushButton();
    buttonSend = new QPushButton();
    ServerWidget->resize(574, 422);
    gridLayout = new QGridLayout(ServerWidget);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    horizontalSpacer = new QSpacerItem(130, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer, 0, 0, 1, 2);
    label = new QLabel(ServerWidget);
    label->setObjectName(QString::fromUtf8("label"));
    QFont font;
    font.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
    font.setPointSize(24);
    label->setFont(font);
    label->setText("Server");
    label->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(label, 0, 2, 1, 1);
    horizontalSpacer_2 = new QSpacerItem(138, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_2, 0, 3, 1, 2);
    textEdit = new QTextEdit(ServerWidget);
    textEdit->setObjectName(QString::fromUtf8("textEdit"));
    textEdit->setReadOnly(true);
    gridLayout->addWidget(textEdit, 1, 0, 1, 5);
    buttonFile = new QPushButton(ServerWidget);
    buttonFile->setText("Chose File");
    buttonFile->setObjectName(QString::fromUtf8("buttonFile"));
    gridLayout->addWidget(buttonFile, 2, 0, 1, 1);
    horizontalSpacer_3 = new QSpacerItem(217, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_3, 2, 1, 1, 3);
    buttonSend = new QPushButton(ServerWidget);
    buttonSend->setText("Send File");
    buttonSend->setObjectName(QString::fromUtf8("buttonSend"));
    gridLayout->addWidget(buttonSend, 2, 4, 1, 1);
    ServerWidget->setLayout(gridLayout);
    ServerWidget->setWindowTitle(QApplication::translate("ServerWidget", "ServerWidget", nullptr));
    label->setText(QApplication::translate("ServerWidget", "\346\234\215\345\212\241\345\231\250", nullptr));
    buttonFile->setText(QApplication::translate("ServerWidget", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
    buttonSend->setText(QApplication::translate("ServerWidget", "\345\217\221\351\200\201\346\226\207\344\273\266", nullptr));
}
