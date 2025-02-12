#include "ClientWidget.h"

CClientWidget::CClientWidget(QWidget *parent) : QWidget(parent)
{
    initClientWidget();
    connect(buttonConnect,SIGNAL(clicked()), this, SLOT(on_buttonConnect_clicked()));//一旦打印出connect success就是连接成功了
    isStart = true;
    connect(tcpSocket, SIGNAL(readyRead()),this,SLOT(clientReadFile()));
    connect(buttonDisplayTree,&QPushButton::clicked,this,&CClientWidget::slotTreeDisplay);
}

//本槽函数用于显示树形结构。
void CClientWidget::slotTreeDisplay()
{
    //提示连接成功
    CClientTreeView *w2 = new CClientTreeView();
    w2->setWindowTitle("Tree");
    w2->show();
    QString xmlPath3 = qApp->applicationDirPath()+"/client/test2.xml";
    QFile fileTemp(xmlPath3);
    fileTemp.remove();

}


//本小类用于对ClientWidget进行初始化
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

//本小类用于连接相应地址的socket
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
        //提示连接成功
        qDebug()<<"connect success";
    }
    );
}

//本小类用于当Socket中readyRead()时，执行读取socket中文件
void CClientWidget::clientReadFile()
{
    //取出接收的内容
    QByteArray buf = tcpSocket->readAll();//把这个buf传递给ServerTreeView
    qDebug()<<"CClientWidget::clientReadFile buf.size()"<<buf.size();

    if(true == isStart)//在第一次文件头发过来的时候，不要去处理这些数据，等第二次或后面的n多次再去处理
    {//接收头
        isStart = false;
        //解析头部信息 QString buf = "hello##1024"
        //                    QString str = "hello##1024#mike";
        //                            str.section("##", 0, 0)
        //初始化
        //文件名
        fileName = QString(buf).section("##", 0, 0);
        //文件大小
        fileSize = QString(buf).section("##", 1, 1).toInt();
        qDebug()<<"CClientWidget::clientReadFile fileSize"<<fileSize;
        recvSize = 0;   //已经接收文件大小

        //打开文件
        //关联文件名字
        QString xmlPath2 = qApp->applicationDirPath()+"/client/test2.xml";
        file.setFileName(xmlPath2);
        //file.setFileName(fileName);//生成的file的目录在当前文件夹下qApp->applicationDirPath()

        //只写方式方式，打开文件
        bool isOk = file.open(QIODevice::WriteOnly);
        if(false == isOk)
        {
            qDebug() << "CClientWidget::clientReadFile WriteOnly error 49";
            tcpSocket->disconnectFromHost(); //断开连接
            tcpSocket->close(); //关闭套接字
            return; //如果打开文件失败，中断函数
        }
    }
    else //文件信息
    {
        bufAll+=buf;
        //可以在这里不断的将buf汇总到bufAll之中
        qint64 len = file.write(buf);//这里做的是一个写入文件的操作。
        qDebug()<<"CClientWidget::clientReadFile len"<<len;
        if(len >0) //接收数据大于0
        {
            recvSize += len; //累计接收大小

        }

        if(recvSize == fileSize) //文件接收完毕
        {
            qDebug()<<"file done";
            //emit signalBufEmit(bufAll);
            //p_treeView = new CClientTreeView();

            emit signalBufEmit(bufAll);
            //p_treeView->initTreeView(bufAll);
            // p_treeView->receiveByte(bufAll);
            //先给服务发送(接收文件完成的信息)
            tcpSocket->write("file done");
            // textEdit->append("file receive over");
            QMessageBox::information(this, "over", "file receive over");
            file.close(); //关闭文件
            //断开连接
            tcpSocket->disconnectFromHost();
            tcpSocket->close();
        }
    }
}
