#include "ImageViewer.h"

CDeepPictureViewer::CDeepPictureViewer(QWidget *parent)
    :QWidget(parent)
{
    m_col = 4;//������ʾ������
    m_currentSelected = nullptr;
    m_disableDelete   = false;

    m_currentIndex = 0;//��ǰѡ�е�ͼƬ������
    m_startIndex   = 0;//��ǰ��ʼ������
    m_endIndex     = 0;//��ǰ����������
    m_totalPageNum = 0;//��ǰ��ҳ��
    m_currentPage  = 0; //��ǰҳ
    m_isShowFileName = true;
    m_isAutoSelected = true;
    createUi();
    setUseControl(true);
    p_controlLabel->setVisible(false);
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QPalette pal;//QPalette�����ÿ��С����״̬����ɫ��
    pal.setColor(QPalette::Background,Qt::black);
    this->setPalette(pal);
    this->setAutoFillBackground(true);
    QString filePath =  qApp->applicationDirPath()+"/xml/test.xml";//��ȡ�̶�xmL�ļ��е����ݽ�����ʾ
    QStringList allPictureList = readFile(filePath);//��ȡxml�ļ��е�����ͼƬ����Ϣ������ʾ
    addImageList(allPictureList);//������ͼƬ�����addImageList,�ɳɹ�ʵ����ʾ
    connect(p_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(slotSliderValue(int)));//��ȡ�����ֵ��������Ӧ�Ĳ���

}

//����ImageView����ѡ���ͼƬ�����е�ͼƬ���бȶԣ��ɵõ���ǰѡ���ͼƬ��index
void CDeepPictureViewer::slotSelectedIndex(QString imageName)
{
    //qDebug()<<"CDeepPictureViewer::slotSelectedIndex pictureList"<<pictureList.size();
    for(int i =0;i<pictureList.size();i++)
    {
        if(imageName==pictureList.at(i))
        {
            m_currentIndex = i;//����ѡ���ͼƬ��index
        }
    }
    update();
}

CDeepPictureViewer::~CDeepPictureViewer()
{

}

//��ȡ�����ֵ�����䵽��ߣ�����ͼƬ����ʾ
void CDeepPictureViewer::slotSliderValue(int value)
{
    emit signalSilderValue(value,pictureList);
    gotoPage(value-1);
    updatePage();
}

//����xml�ļ�����xml�ļ��е���Ϣ��ʾ��ͼƬ(xml�ļ���Ϣ��ͼƬ)
QStringList CDeepPictureViewer::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error:"
                     "Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        std::cerr << "Error: Parse error at line " << errorLine << ", "
                  << "column " << errorColumn << ": "
                  << qPrintable(errorStr) << std::endl;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "root")
    {
        std::cerr << "Error: Not a root file" << std::endl;
    }
    else{
        QFileInfo appInfo(root.attribute("Path"));
        QString value = appInfo.baseName();//�ļ���
    }

    QDomNode child = root.firstChild();
    while(!child.isNull())
    {
        QFileInfo appInfo(child.toElement().attribute("Path"));
        QString filepath = appInfo.filePath();
        pictureList.append(filepath);
        child = child.nextSibling();
    }
    return pictureList;
}

//����ȡ����ɾ�����ܣ�Ӧ����û��Ӧ�õ������
void CDeepPictureViewer::setDisableDelete(bool flag)
{
    m_disableDelete = flag;
}

//���Ʋ�����ˮƽ����
void CDeepPictureViewer::createUi()
{
    p_layout = new QHBoxLayout();
    p_layout->setContentsMargins(0,0,0,0);
    this->setLayout(p_layout);
    createControlWidget();

}

//����Ҳ�и����������ض�Ӧ�ļ����е�����ͼƬ���Խ�����ʾ�����������ڸ�����ʾ�ķ�ʽ�ˣ�ʹ�ö�ȡxml�ļ���Ϣ������ʾ��
void CDeepPictureViewer::loadPath(QString path)
{
    QDir dir = QDir(path);
    QStringList   fileList;
    QFileInfoList list = dir.entryInfoList(QDir::Files|QDir::NoDot|QDir::NoDotDot);
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        fileList.append(fileInfo.filePath());
    }
    addImageList(fileList);//�����fileList�Ǵ����List
}

//���ÿ��ƽ���
void CDeepPictureViewer::createControlWidget()
{
    p_controlWidget = new QWidget(this);//���ƽ���
    p_controlLabel  = new QLabel(this); //���ƽ���label
    p_controlLabel->setText("0/0");
    p_controlLabel->setStyleSheet("color:white;");

    p_chooseButton = new QPushButton();//���ͼƬ��ť
    p_chooseButton->setText(QString::fromLocal8Bit("���ͼƬ"));
    p_chooseButton->setMinimumWidth(120);
    connect(p_chooseButton,SIGNAL(released()),this,SLOT(slotAddPicture()));

    p_prevButton = new QPushButton(); //ǰһҳ��ť
    p_prevButton->setText(QString::fromLocal8Bit("��һҳ"));
    p_prevButton->setMinimumWidth(80);
    connect(p_prevButton,SIGNAL(released()),this,SLOT(slotGotoPrevPage()));

    p_slider = new CImageSlider();
    p_slider->setOrientation(Qt::Horizontal);

    p_slider->setTickInterval(1);
    p_slider->setSingleStep(1);//�����ڼ����ϰ��¡��������ʱ�Ĳ���ֵ:

    p_nextButton = new QPushButton(); //��һҳ��ť
    p_nextButton->setText(QString::fromLocal8Bit("��һҳ"));
    p_nextButton->setMinimumWidth(80);
    connect(p_nextButton,SIGNAL(released()),this,SLOT(slotGotoNextPage()));

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(p_chooseButton);
    layout->addStretch(1);
    layout->addWidget(p_prevButton);
    layout->addStretch(1);
    layout->addWidget(p_slider);
    layout->addStretch(1);
    layout->addWidget(p_controlLabel);
    layout->addStretch(1);
    layout->addWidget(p_nextButton);
    layout->setAlignment(Qt::AlignLeft);
    layout->setContentsMargins(1,1,1,1);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    p_controlWidget->setLayout(layout);
}

//����ҳ��
void CDeepPictureViewer::updatePage()
{
    int total = m_col;//m_col��ÿһҳ������
    int totalImageNum = m_totalImageList.size();//������Ի���ܵ�ͼƬ��ҳ��
    if(totalImageNum>0)
    {
        if(totalImageNum%total==0)
            m_totalPageNum = (totalImageNum/total);
        else
            m_totalPageNum = (totalImageNum/total)+1;
    }
    else
    {
        m_totalPageNum = 0;
        m_currentPage  = 0;
    }

    QString str;
    if(totalImageNum>0)
    {
        str= QString("%1/%2").arg(m_currentPage+1).arg(m_totalPageNum);
        p_slider->returnTotalImgNum(m_totalPageNum);//����Ӧ�����ִ���p_slider���и���
        p_slider->returnCurrentIndex(m_currentPage+1);
    }
    p_controlLabel->setText(str);
}

//ȡ��û�б��ù���item
CImgViewElement* CDeepPictureViewer::nextImgPic()
{
    CImgViewElement *item = nullptr;
    int size = m_totalVector.size();
    for(int i = 0;i<size;i++)
    {
        item = m_totalVector.at(i);//������ͼƬ����ֵ��Item

        if(item->m_fileName.isEmpty())//��һ�����ʱ���item�Ƿ��Ѿ�loadImage�����û�о�ֱ�ӷ������item
            return item;
    }
    item = new CImgViewElement();//����м��ع�item�ˣ����½�һ��CImgViewElement����
    m_totalVector.append(item);//���������������m_totalVector�����档
    item->installEventFilter(this);//���С�ؼ���װ������
    item->setShowText(m_isShowFileName);
    connect(item,SIGNAL(signalDelete()),this,SLOT(slotDelete()));
    return item;
}

//�õ���Ӧname��Item,����δʹ��
CImgViewElement* CDeepPictureViewer::getItemByName(QString name)
{
    foreach (CImgViewElement *item, m_totalVector)
    {
        if(item->m_fullName==name)
        {
            return item;
        }
    }
    return nullptr;
}

//�����Ĳ����ǰ�imageList���������ݼ��ؽ���
void CDeepPictureViewer::addImageList(QStringList imageList)
{
    int  total = m_col;//��������

    if(m_currentImageList.size()==0)//�жϵ�ǰ����ʾ�б��Ƿ����0
    {
        m_totalPageNum = 0;
        m_currentPage  = 0;
    }

    QStringList addedList;
    QString     fileName;

    //����ļ��Ƿ����
    clearAll();//�Ҹ���������addList�Ļ��ƣ�����ÿ��ִ��������������Ǽ����Ҹ�����������Ƭ
    while(imageList.size()>0)
    {
        //QString roiFileName;
        fileName    = imageList.takeFirst();//���������һ��Ԫ�ص�ֵ����ͬʱɾ������

        //����ļ������ھ�����
        QFileInfo info(fileName);

        if(!info.exists())
            continue;//ִ����������
        if(!m_totalImageList.contains(fileName))//���Ŀǰ�ܵ�ͼƬ�б���������ļ������ͽ������totalImage
        {
            addedList.append(fileName);
            m_totalImageList.append(fileName);

        }
    }
    //��ʱm_totalImageList��ӵ�����е�image
    //������ҳ����
    int totalImageNum = m_totalImageList.size();//�����ҳ�����ǽ�Ϊ׼ȷ��ҳ����
    if(totalImageNum>0)
    {
        if(totalImageNum%total==0)
            m_totalPageNum = (totalImageNum/total);
        else
            m_totalPageNum = (totalImageNum/total)+1;
    }
    else
    {
        m_totalPageNum = 0;
        m_currentPage  = 0;
    }

    //�����ǰ����Ŀؼ�����û�����ͼ�����ӣ�������˾Ͳ����������
    int tCurrentNum = 0;
    if(m_totalVector.size()<total)//�����ǰ���е�ͼƬ�������ڿɷ��õ������������µ�ͼƬҪ�ӽ�����ʱ�򣬽��������µĿؼ�
    {
        while(m_totalVector.size()<total)
        {
            if(addedList.size()<=0)//������һ���жϣ����Ƿ���Ҫ�ӵ�Ԫ���Ƿ��ж�����
                break;
            fileName = addedList.takeFirst();//��õ�һ��fileName,Ȼ���Ƴ����fileName
            CImgViewElement *item = nextImgPic();//������һ��װ���ڲ����ݵ�item��Ȼ�����item�ټ�����Ӧ��ͼƬ

            item->loadImage(fileName);//����Ͱ�addedList�е�file�����ؽ���
            //item->isSelected();
            if(m_size.isValid())//m_size�ǵ�ǰÿ���ؼ��Ĵ�С
                item->setFixedSize(m_size.width(),m_size.height());
            p_layout->addWidget(item);//�ڲ�������ӿؼ���ɾ����ʱ��Ҳ�ǻ�ȡ�ؼ�����ɾ��
        }
    }
    //m_totalVector�ǵ�ǰ����������
    //���㵱ǰҳ��ʾ����ʼ����ֹ���
    m_startIndex = m_currentPage*total;
    tCurrentNum  = m_totalImageList.size() - m_startIndex;
    if(tCurrentNum>=total)
        tCurrentNum = total;
    m_endIndex   = m_startIndex + tCurrentNum;
    m_currentImageList = m_totalImageList.mid(m_startIndex,m_endIndex-m_startIndex);
    for(int i=0;i<m_currentImageList.size();i++)
    {
        fileName = m_currentImageList.at(i);
        if(i<m_totalVector.size())
        {
            CImgViewElement *item = m_totalVector.at(i);
            if(item->m_fullName!=fileName)
            {
                item->loadImage(fileName);
                if(m_size.isValid())
                    item->setFixedSize(m_size.width(),m_size.height());
            }
            if(item->isHidden())
            {
                item->show();
            }
        }
    }
    updatePage();
    //qDebug()<<"CDeepPictureViewer addPicture m_totalVector"<<m_totalVector.size();
}

void CDeepPictureViewer::clearAll()
{
    m_currentIndex = 0;//��ǰ�߰��е�ͼƬ������
    m_startIndex   = 0;//��ǰ��ʼ������
    m_endIndex     = 0;//��ǰ����������
    m_totalPageNum = 0;//��ǰ��ҳ��
    m_currentPage  = 0; //��ǰҳ

    foreach (CImgViewElement*item, m_totalVector)
    {
        item->hide();
        item->clearImg();
    }
    m_currentImageList.clear();
    m_totalImageList.clear();
    updatePage();
}

void CDeepPictureViewer::resizeEvent(QResizeEvent * event)
{
    QWidget::resizeEvent(event);
    calculateWidget();
}

//����ÿ���ؼ�Ӧ���еĴ�С
void CDeepPictureViewer::calculateWidget()
{
    int width  = this->size().width();
    int height = this->size().height();
    if(m_isShowPrevNextControl)
    {
        height-=p_controlWidget->height();
        int x;
        x = (width - p_controlWidget->width())/2;
        p_controlWidget->move(x,height);
    }

    int colSpacing = (m_col -1)*10+4;
    int rowSpacing = 40;

    qreal ww,wh;
    ww = qreal(width  - colSpacing)/m_col;
    wh = qreal(height - rowSpacing)/1;

    int w,h;
    w = qFloor(ww);
    h = qFloor(wh);
    m_size.setWidth(w);
    m_size.setHeight(h);

    foreach (CImgViewElement* item, m_totalVector)
    {
        if(item->size()!=m_size)
            item->setFixedSize(w,h);
    }
    p_layout->update();
}

void CDeepPictureViewer::showEvent(QShowEvent *event)
{
    setSeleceted(0);
    calculateWidget();
    QWidget::showEvent(event);
}

bool CDeepPictureViewer::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent*>(event);
        if(mouseEvent->button() == Qt::LeftButton)
        {
            clearAllSelected();
            CImgViewElement *t = dynamic_cast<CImgViewElement *>(watched);
            if(t)
            {
                m_currentSelected = t;
                emit signalCurrentFileName(t->m_fullName);
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void CDeepPictureViewer::clearAllSelected()
{
    QList<CImgViewElement*> list = this->findChildren<CImgViewElement*>();
    foreach (CImgViewElement* item, list)
    {
        if(item->isSelected())
        {
            item->setSelected(false);
        }
    }
}

void CDeepPictureViewer::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        clearAllSelected();
        m_currentSelected = nullptr;
        emit signalNoSelected();
    }

    QWidget::mousePressEvent(event);
}

CImgViewElement* CDeepPictureViewer::getCurrent()
{
    return m_currentSelected;
}

void CDeepPictureViewer::slotGotoNextPage()
{
    if(m_currentPage>=m_totalPageNum-1)
    {
        return;
    }
    m_currentPage++;
    gotoPage(m_currentPage);
}

//�ڵ������ͼƬ��ť��ִ�еĲۺ���
void CDeepPictureViewer::slotAddPicture()
{
    QString picturePath = QFileDialog::getOpenFileName(this, "open", "../");//����õ����Ѿ�����ȷ��·����
    QStringList fileList;
    if(false == picturePath.isEmpty())
    {

        QFileInfo info(picturePath);
        fileList.append(info.filePath());
    }
    else
    {
        qDebug() << "CDeepPictureViewer::slotAddPicture Chose file path is wrong! 118";
    }
    pictureList.append(fileList);
    addImageList(pictureList);//�������ʾ������֮��
    addXmlMes(picturePath);
}

//����ӵ���Ϣ����xml�ļ�֮��
void CDeepPictureViewer::addXmlMes(QString picturePath)
{
    QString filePath =  qApp->applicationDirPath()+"/xml/test.xml";
    QDomDocument doc;
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"CDeepPictureViewer::addXmlMes file open fail";
    }
    else{
        qDebug()<<"CDeepPictureViewer::addXmlMes open success";
    }

    QByteArray array = file.readAll();//�ֽ�������
    if(!doc.setContent(array))
    {
        qDebug()<<"CDeepPictureViewer::addXmlMes fail setContent";
    }
    file.close();
    QDomElement root = doc.documentElement();
    QDomElement childElt = doc.createElement("tmp");//����һ����ʱ���ڵ�
    childElt.setTagName("Picture");
    childElt.setAttribute("Path",picturePath);
    root.appendChild(childElt);//���������root�ĺ���

    bool bSuccess = file.open(QIODevice::WriteOnly);//�����ļ�Ҫ�ٴδ򿪵Ļ���һ��Ҫ��֤��ǰ������ļ��Ѿ�ִ�йرղ�����
    if(bSuccess == false)
    {
        qDebug()<<"CDeepPictureViewer::addXmlMes file could write";
    }
    else{
        qDebug()<<"CDeepPictureViewer::addXmlMes file open fail";
    }
    QTextStream out(&file);
    doc.save(out, 4);

    file.close();
}
void CDeepPictureViewer::slotGotoPrevPage()
{
    if(m_currentPage<=0)
    {
        return;
    }
    m_currentPage--;
    gotoPage(m_currentPage);
}

void CDeepPictureViewer::slotDelete()
{
    if(m_disableDelete)
    {
        qDebug()<<" CDeepPictureViewer::slotDelete";
        return;
    }
}

void CDeepPictureViewer::keyPressEvent(QKeyEvent * event)
{
    if(event->key()==Qt::Key_Delete)//�ǵ�PictureListҲҪ����ɾ��
    {
        event->accept();
        deepDelete();//�����°�ťdelete����ִ��ɾ������
    }
    QWidget::keyPressEvent(event);
}

//��Ϊ��delete����������Ҫִ�еĺ���
void CDeepPictureViewer::deepDelete()
{
    //deleteXmlMes(pictureList.at(m_currentIndex));//��ɾ��xml�ļ��ϵ�ǰѡ�е�ͼƬ��Ϣ
    //pictureList.removeAt(m_currentIndex);//��pictureList���Ƴ���ͼƬ��Ϣ
    //addImageList(pictureList);//�ڰѾ������ĵ������addImageList֮�У����ǵ�һ�ַ��������ַ���ִ��Ч�ʽϵ�
    deepDeleteLayout();
    update();

}

//ִ��ɾ���ĺ���
void CDeepPictureViewer::deepDeleteLayout()
{
    //deleteXmlMes(pictureList.at(m_currentIndex));//��ɾ��xml�ļ��ϵ�ǰѡ�е�ͼƬ��Ϣ�����Գ���ʱ����ע�͵���

    if(m_totalImageList.size()<=m_col&&m_totalVector.size()>0)//�����ǰͼƬ��������һ��С�ڵ����������ͱ����ܹ�ֻ��4�ŵ������ֱ��ɾ���ÿؼ�����
    {
        CImgViewElement *item2 = deleteImgPic();//���Ҫɾ�����Ǹ��ؼ�
        //if(m_totalVector.size()==1)

        p_layout->removeWidget(item2);//���ÿؼ�����ɾ��
        delete item2;//ն�ݳ���
        m_totalImageList.removeAt(m_currentIndex);
        m_totalVector.remove(m_currentIndex);
        pictureList.removeAt(m_currentIndex);
        if(m_totalVector.size()>0)
            setSeleceted(m_currentIndex);
    }
    else if(m_totalImageList.size()>m_col)
    {
        CImgViewElement *item2 = deleteImgPic();//���Ҫɾ�����Ǹ��ؼ�
        p_layout->removeWidget(item2);//���ÿؼ�����ɾ��
        delete item2;//ն�ݳ���
        m_totalVector.remove(m_currentIndex);
        m_totalImageList.removeAt(m_currentIndex);
        pictureList.removeAt(m_currentIndex);
        setSeleceted(m_currentIndex); 
        int m_replaceIndex = (m_currentPage+1)*m_col; 
        QString fileName=pictureList.at(m_replaceIndex-1);;//���fileName����ֵҪע��
        CImgViewElement *item = nextImgPic();
        item->loadImage(fileName);
        if(m_size.isValid())
            item->setFixedSize(m_size.width(),m_size.height());
        p_layout->addWidget(item);
    }
    updatePage();
    this->update();
}


CImgViewElement* CDeepPictureViewer::deleteImgPic()
{
    CImgViewElement *item = nullptr;
    item = m_totalVector.at(m_currentIndex);
    return  item;
}

//ɾ����ӦͼƬ·����xml�ļ���Ϣ
void CDeepPictureViewer::deleteXmlMes(QString picturePath)
{
    QString filePath =  qApp->applicationDirPath()+"/xml/test.xml";
    QDomDocument doc;
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"CDeepPictureViewer::addXmlMes file open fail";
    }
    else{
        qDebug()<<"CDeepPictureViewer::addXmlMes open success";
    }
    QByteArray array = file.readAll();//�ֽ�������
    if(!doc.setContent(array))
    {
        qDebug()<<"CDeepPictureViewer::addXmlMes fail setContent";
        //return;
    }
    file.close();
    QDomElement root = doc.documentElement();
    QDomNodeList picture=doc.elementsByTagName("Picture"); //�ɱ�ǩ����λ
    root.removeChild(picture.at(m_currentIndex));//�Ƴ���index��childλ��
    if(!file.open(QFile::WriteOnly|QFile::Truncate))
    {
        qDebug()<<"CDeepPictureViewer::deleteXmlMes open fail";
        return;
    }
    else
    {
        qDebug()<<"CDeepPictureViewer::deleteXmlMes open success";
    }

    QTextStream out_stream(&file);
    doc.save(out_stream,4); //����4��
    file.close();
}

void CDeepPictureViewer::gotoPage(int pageNum)
{
    m_currentPage = pageNum;
    int total = m_col;
    //���㵱ǰҳ����Ҫ������ʾ�Ŀؼ�
    int tCurrentNum = 0;
    m_startIndex = m_currentPage*total;
    tCurrentNum = m_totalImageList.size() - m_startIndex;
    if(tCurrentNum>=total)
        tCurrentNum = total;
    m_endIndex   = m_startIndex + tCurrentNum;
    m_currentImageList = m_totalImageList.mid(m_startIndex,tCurrentNum);
    int size = m_totalVector.size();
    for(int i = 0;i<size;i++)
    {
        CImgViewElement *t = m_totalVector.at(i);
        t->clearFocus();
        if(i<m_currentImageList.size())
        {
            QString tFileName = m_currentImageList.at(i);
            t->loadImage(tFileName);
            t->setSelected(false);
            t->show();
        }
        else
        {
            t->clearImg();
            t->hide();
        }
    }
    m_currentSelected = nullptr;
    if(!m_isAutoSelected)
        emit signalNoSelected();
    updatePage();
    setSeleceted(0);
    this->update();
}

void CDeepPictureViewer::setUseControl(bool flag)
{
    m_isShowPrevNextControl = flag;
    p_controlLabel->setVisible(flag);
}

void CDeepPictureViewer::setShowImageName(bool flag)
{
    m_isShowFileName = flag;
    foreach (CImgViewElement *t, m_totalVector)
    {
        t->setShowText(flag);
    }
}

void CDeepPictureViewer::setAutoSelectedFirst(bool flag)
{
    m_isAutoSelected = flag;
}

//���õ�ǰindexΪselecte
void CDeepPictureViewer::setSeleceted(int index)
{
    if(!m_isAutoSelected)
        return;
    //qDebug()<<"CDeepPictureViewer::setSeleceted"<<m_currentImageList.size()<<index;
    if(m_currentImageList.size()<0||m_currentImageList.size()<index)
    {
        qDebug()<<"1212121212";
        emit signalNoSelected();
        return;
    }
    CImgViewElement *item = m_totalVector.at(index);
    foreach (CImgViewElement *view, m_totalVector)
    {
        if(item!=view)
        {
            if(view->isSelected())
                view->setSelected(false);
        }
    }
    item->setSelected(true);
    m_currentSelected = item;
    //qDebug()<<"m_currentSelected->m_fullName"<<m_currentSelected->m_fullName;
    emit signalCurrentFileName(m_currentSelected->m_fullName);
    //    this->updatePage();
    //    this->update();
    //    p_layout->update();
}

void CDeepPictureViewer::gotoPrevImage()
{
    if(!m_currentSelected)
        return;
    if(m_totalVector.size()<=0)
        return;
    int index = m_totalVector.indexOf(m_currentSelected);
    qDebug()<<"CDeepPictureViewer::gotoPrevImage"<<index;
    if(index<0)
        return;
    if(index>0)
        setSeleceted(--index);
    else
    {
        if(m_currentPage==0)
            return;
        else
        {
            slotGotoPrevPage();
            setSeleceted(m_totalVector.size()-1);
        }
    }
}

void CDeepPictureViewer::gotoNextImage()
{
    if(!m_currentSelected)
        return;
    if(m_totalVector.size()<=0)
        return;
    int size = m_totalVector.size();
    int index = m_totalVector.indexOf(m_currentSelected);
    //qDebug()<<"CDeepPictureViewer::gotoNextImage"<<index;
    if(index<0)
        return;
    if(index>=0&&index<size-1)
    {
        if(m_endIndex!=m_currentPage*m_col+index+1)
            setSeleceted(++index);
    }
    else
    {
        if(m_currentPage==m_totalPageNum)
            return;
        else if(m_endIndex!=(m_totalPageNum-1)*m_col+index+1)
        {
            slotGotoNextPage();
            setSeleceted(0);
        }
    }
}

/****************************************
 *
 *           �ܶ�ȡROI�Ŀؼ�
 *
 * *************************************/
CSingleImageView::CSingleImageView(QWidget *parent)
    :QWidget(parent)
{
    //������괩͸
    setAttribute(Qt::WA_TransparentForMouseEvents,true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint);

    //���ñ�����ɫ
    QPalette palette = this->palette();
    QColor c(0,0,0,0);
    palette.setColor(QPalette::Base, c);
    this->setAutoFillBackground(true);
    this->setPalette(palette);

}

CSingleImageView::~CSingleImageView()
{

}

void CSingleImageView::slotSlideValue(int value,QStringList pictureList)
{
    loadImage(pictureList.at(value));// �������������Img�Ǳ�ҲҪ���и���ҳ��
    emit signalImgSlider(value,pictureList);
    this->update();
}

void CSingleImageView::clearImage()
{
    m_image = QImage();
    this->update();
}


//���ض�Ӧ�ļ�����ͼƬ�����ŵ�ͼƬ
void CSingleImageView::loadImage(QString fileName)
{
    m_image.load(fileName);
    emit signalSelectedImageName(fileName);
    //qDebug()<<"CSingleImageView1::loadImage"<<fileName;
    this->update();
}

void CSingleImageView::getWidthAndHeight(int &w,int &h)
{
    w = m_image.width();
    h = m_image.height();
}

void CSingleImageView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    if(m_image.isNull())
    {
        painter.drawImage(this->rect(),m_image);
        return;
    }
    int widget_width =this->size().width();
    int widget_height =this->size().height();

    int image_width  = m_image.width();
    int image_height = m_image.height();

    qreal view_ratio =(qreal)widget_width/(qreal)widget_height;
    qreal img_ratio  =(qreal)image_width/(qreal)image_height;

    int rect_x     = 0;
    int rect_y     = 0;
    int rectWidth  = widget_width;
    int rectHeight = widget_height;

    //�ؼ�̫���ˣ��߶�̫С���Ը߶�Ϊ��׼
    if(view_ratio>img_ratio)
    {
        rectWidth = widget_height*img_ratio;
        rect_x    = (widget_width - rectWidth)/2;
    }
    //�ؼ�̫���ˣ��߶�̫���Կ��Ϊ��׼
    else if(view_ratio<img_ratio)
    {
        rectHeight = widget_width/img_ratio;
        rect_y     = (widget_height -rectHeight)/2;
    }

    QRect rect = QRect(rect_x,rect_y,rectWidth,rectHeight);
    painter.drawImage(rect,m_image);
}

/**************************************
 *
 * ����ͼƬ�Ŀؼ�
 *
 * ***************************************/
CImgViewElement::CImgViewElement(QWidget *parent)
    :QWidget(parent)
{
    m_fileName  = "";
    m_fullName  = "";
    m_selected = false;
    m_isEnter  = false;
    m_isShowText = true;
    setAttribute(Qt::WA_TranslucentBackground, true);
    createUi();
    this->setFocusPolicy(Qt::ClickFocus);


}

CImgViewElement::~CImgViewElement()
{

}

//void CImgViewElement::slotImgCur(QString currentPictureName)
//{
//    qDebug()<<"CImgViewElement::slotImgCu"<<currentPictureName;
//}

void CImgViewElement::createUi()
{
    QVBoxLayout *layout = new QVBoxLayout();
    p_singleImg = new CSingleImageView(this);
    //    bool flag = connect(p_singleImg,SIGNAL(signalImgSlider(int,QStringList)),this,SLOT(slotSlider(int,QStringList)));
    //    qDebug()<<"Img"<<flag;
    layout->addWidget(p_singleImg);
    layout->setContentsMargins(0,1,0,0);
    //�����϶���
    layout->setAlignment(Qt::AlignTop|Qt::AlignHCenter);
    this->setLayout(layout);
}

void CImgViewElement::setFontSize(int size)
{
    m_font.setPixelSize(size);
    this->setFont(m_font);
    resetUi();
}

void CImgViewElement::slotSlider(int value,QStringList pictureList)
{
    loadImage(pictureList.at(value));
    //qDebug()<<"CImgViewElement::slotSlider";
    setSelected(true);
    update();
}
void CImgViewElement::loadImage(QString imageName)
{
    QFileInfo info(imageName);
    m_fullName  = imageName;
    m_fileName  = info.fileName();
    //qDebug()<<"CImgViewElement::loadImage"<<imageName;
    p_singleImg->loadImage(imageName);
}

void CImgViewElement::resetUi()
{
    int width      = this->width();
    int height     = this->height();

    int textHeight = fontMetrics().height()*2*1.1;
    int ww,wh;
    ww = width-2;
    if(m_isShowText)
        wh = height - textHeight;
    else
        wh = height-2;
    if(wh<=0)
        return;
    p_singleImg->setFixedSize(ww,wh);
    qreal x,y;
    x = 0;
    y = height - fontMetrics().height()*2;
    m_textRect.setRect(x,y,width,fontMetrics().height()*2);
}

void CImgViewElement::clearImg()
{
    m_fullName  = "";
    m_fileName  = "";
    QImage img;
    p_singleImg->clearImage();
    this->update();
}

void CImgViewElement::showEvent(QShowEvent *event)
{
    resetUi();
    QWidget::showEvent(event);
}

//�����ָ������ЩС��Ƭ�Ļ�ͼ�¼���
void CImgViewElement::paintEvent(QPaintEvent * event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    QStringList nameList;
    QString paintText;
    if(m_title.isEmpty())
        paintText = m_fileName;
    else
        paintText = m_title;
    caculateLines(m_textRect.width(),paintText,nameList);
    int nameListSize = nameList.size();
    //�����ʾ���У���������к��滹�еĻ������á���������
    for(int i = 0; i < nameListSize; i++)
    {
        QString showTitle = nameList.at(i);
        if(i == 1 && nameListSize > 1&&showTitle.size()>3)
        {
            showTitle.chop(3);
            showTitle.append("...");
        }
        if(i >1)
        {
            break;
        }
        if(m_isShowText)
        {
            qreal x,y,w,h;
            x = m_textRect.x();
            y = m_textRect.y();
            w = m_textRect.width();
            h = m_textRect.height();
            y+=i*fontMetrics().height();
            QRect drawRect = QRect(x,y,w,h);
            QPen pen = painter.pen();
            pen.setColor(Qt::white);
            painter.save();
            painter.setPen(pen);
            painter.drawText(drawRect,Qt::AlignTop|Qt::AlignHCenter,showTitle);
            painter.restore();
        }
    }

    if(m_selected)
    {
        //        QPainterPath path;
        //        path.addRoundedRect(this->rect(),0,0);
        //        painter.fillPath(path,QColor(0,255,0,100));

        QColor c;
        c.setNamedColor("#00ff00");
        QPen pen;
        pen.setColor(c);
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawRect(this->rect());
    }
    //��껬��
    else if(m_isEnter)
    {
        QColor c;
        c.setNamedColor("#ffff00");
        c.setAlpha(180);
        QPen pen;
        pen.setColor(c);
        pen.setStyle(Qt::SolidLine);

        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawRect(this->rect());
    }
}

void CImgViewElement::resizeEvent(QResizeEvent * event)
{
    resetUi();
    QWidget::resizeEvent(event);
}

void CImgViewElement::caculateLines(int rectWidth, QString sourceStr, QStringList &destStrList)
{
    int soutceSize = sourceStr.size();
    QString lineStr;
    lineStr.clear();
    for(int i = 0 ; i < soutceSize; i++)
    {
        QString charStr = sourceStr.at(i);
        lineStr.append(charStr);
        int fontsize = fontMetrics().width(lineStr);
        //����һ���ַ������������ȴ�������ȣ������������ַ�
        if(fontsize > rectWidth)
        {
            lineStr.chop(1);
            destStrList<<lineStr;
            lineStr.clear();
            lineStr.append(charStr);
        }

        if( i == sourceStr.size() - 1)
        {
            destStrList<<lineStr;
        }
    }
}

void CImgViewElement::enterEvent(QEvent *event)
{
    m_isEnter = true;
    QWidget::enterEvent(event);
    update();
}

void CImgViewElement::leaveEvent(QEvent *event)
{
    m_isEnter = false;
    QWidget::leaveEvent(event);
    update();
}

void CImgViewElement::keyPressEvent(QKeyEvent * event)
{
    if(event->key()==Qt::Key_Delete)//�ǵ�PictureListҲҪ����ɾ��
    {
        event->accept();
        qDebug()<<"CImgViewElement::keyPressEvent";
        emit signalDelete();
    }

    QWidget::keyPressEvent(event);
}

void CImgViewElement::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        setSelected(true);
    }
    update();
}

void CImgViewElement::setSelected(bool flag)
{
    m_selected = flag;
    update();
}

bool CImgViewElement::isSelected()
{
    return m_selected;
}

void CImgViewElement::setShowText(bool flag)
{
    m_isShowText = flag;
    resetUi();
}
