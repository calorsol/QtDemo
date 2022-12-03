#include "ServerTreeModel.h"

CServerTreeModel::CServerTreeModel(QObject *parent) : QStandardItemModel(parent)
{

}
CServerTreeModel::~CServerTreeModel()
{

}
bool CServerTreeModel::readByte(const QByteArray &byteArray)
{
    QDomDocument doc;
    if(!doc.setContent(byteArray))
    {
        return false;
    }
    QDomElement root = doc.documentElement();
    if (root.tagName() != "root")
    {
        std::cerr << "Error: Not a school file" << std::endl;
        return false;
    }
    else{

        QFileInfo appInfo(root.attribute("Path"));
        QString value = appInfo.baseName();//�ļ���
        item1 = new QStandardItem((value));
        this->appendRow(item1);
        qDebug()<<"root";
    }
    parseAllMembers(root,item1);
    //parseRootMembers(root);
    return true;
}
bool CServerTreeModel::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        return false;
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
        return false;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "root")
    {
        std::cerr << "Error: Not a school file" << std::endl;
        return false;
    }
    else{

        QFileInfo appInfo(root.attribute("Path"));
        QString value = appInfo.baseName();//�ļ���
        item1 = new QStandardItem((value));
        this->appendRow(item1);
        qDebug()<<"root";
    }
    parseAllMembers(root,item1);
    //parseRootMembers(root);
    return true;
}
void CServerTreeModel::parseAllMembers(const QDomElement &element,QStandardItem *ParentItem)
{
    QDomNode child = element.firstChild();
    while(!child.isNull())
    {

        QFileInfo appInfo(child.toElement().attribute("Path"));
        QString value = appInfo.baseName();//�ļ���
        QString suffix = appInfo.suffix();
        QString readvalue = value + "."+suffix;
        ChildItem = new QStandardItem(readvalue);
        childItems.clear();
        childItems.push_back(ChildItem);
        ParentItem->appendRows(childItems);
        childItems.clear();
        if (child.toElement().tagName() == "Folder")
        {

            parseAllMembers(child.toElement(),ChildItem);
        }
        child = child.nextSibling();

    }

}
/*
void CServerTreeModel::parseRootMembers(const QDomElement &element)
{
    QDomNode child = element.firstChild();

    while (!child.isNull())
    {
        QFileInfo appInfo(child.toElement().attribute("Path"));
        QString value = appInfo.baseName();//�ļ���
        item1_1 = new QStandardItem( value);
        childItems.clear();
        childItems.push_back(item1_1);
        item1->appendRows(childItems);
        childItems.clear();
        qDebug()<<"parseRootMembers Folder";
        if (child.toElement().tagName() == "Folder")
        {

            parseFolderMembers(child.toElement());
        }
        child = child.nextSibling();
    }
}
void CServerTreeModel::parseFolderMembers(const QDomElement &element)
{
    QDomNode child = element.firstChild();
    while (!child.isNull())
    {
        QFileInfo appInfo(child.toElement().attribute("Path"));
        QString value = appInfo.baseName();//�ļ���
        item1_1_1 = new QStandardItem(value);
        childItems.clear();
        childItems.push_back(item1_1_1);
        item1_1->appendRows(childItems);
        childItems.clear();
        if (child.toElement().tagName() == "Folder")
        {

            qDebug()<<"parseFolderMembers Folder";
            parseFileMembers(child.toElement());
        }
        child = child.nextSibling();
    }
}
void CServerTreeModel::parseFileMembers(const QDomElement &element)
{
    QDomNode child = element.firstChild();
    while (!child.isNull())
    {
        QFileInfo appInfo(child.toElement().attribute("Path"));
        QString value = appInfo.baseName();//�ļ���
        item1_1_1_1 = new QStandardItem(value);
        childItems.clear();
        childItems.push_back(item1_1_1_1);
        item1_1_1->appendRows(childItems);
        childItems.clear();
        if (child.toElement().tagName() == "Folder")
        {

            qDebug()<<"parseFolderMembers Folder";
            parseFileMembers(child.toElement());
        }
        child = child.nextSibling();
    }
}
/*
/*
void CServerTreeModel::parseFile(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray array = file.readAll();//�ֽ�������
    file.close();
    //setContent�˺������ֽ����������н���XML�ĵ�������������Ϊ�ĵ������ݡ� �����Լ��XML�淶Ҫ����ĵ�����
    if(!m_doc.setContent(array))
    {
        return;
    }
    m_fileName = fileName;
    QDomElement rootElt = m_doc.documentElement();
    //�������Ͷ�Ӧ��ϵ
    qDebug()<<"rootElt.tagName"<<rootElt.tagName();
    qDebug()<<rootElt.attribute("Path");


    QDomNamedNodeMap map = rootElt.attributes();//
    //QDomNamedNodeMap����һ���ڵ㼯�ϣ��ڵ����ͨ������������
    QDomNode node = rootElt.firstChild();//��ȡ��һ���ӽ��

    //    while(!node.isNull())  //����ڵ㲻��
    //    {
    //        if(node.isElement()) //����ڵ���Ԫ��
    //        {
    //            QDomElement e=node.toElement(); //ת��ΪԪ�أ�ע��Ԫ�غͽڵ����������ݽṹ����ʵ���
    //            qDebug()<<e.tagName()<<"-A"<<e.attribute("Path"); //��ӡ��ֵ�ԣ�tagName��nodeName��һ������
    //            QDomNodeList list=e.childNodes();
    //            for(int i=0;i<list.count();i++) //������Ԫ�أ�count��size��������,�����ڱ�ǩ������
    //            {
    //                QDomNode n=list.at(i);
    //                qDebug()<<"n"<<n.nodeValue();
    //                if(node.isElement())
    //                    qDebug()<<n.nodeName()<<":A"<<n.toElement().text();
    //            }
    //        }
    //        node=node.nextSibling(); //��һ���ֵܽڵ�,nextSiblingElement()����һ���ֵ�Ԫ�أ������
    //    }

    //    while(!node.isNull())
    //    {
    //        qDebug()<<"-"<<node.toElement().tagName();
    //        node = node.nextSibling();	//��ȡͬ���Ľ��
    //    }
    int size = map.size();
    qDebug()<<"size"<<size;
    //��������
    this->setColumnCount(size);
    m_typeMap.clear();
    m_headerList.clear();
    for(int i = 0;i<size;i++)
    {
        QDomNode node = map.item(i);
        QString name  = node.nodeName();
        QString value = node.nodeValue();
        qDebug()<<"name"<<name<<value;
        m_typeMap.insert(name,value);
        m_headerList.append(name);
    }
    //���ñ�����
    this->setHorizontalHeaderLabels(m_headerList);
    m_dataVector.clear();
    QDomElement infoElt = rootElt.firstChildElement();
    qDebug()<<"m_headerList.size()"<<m_headerList.size();
    while(!infoElt.isNull())
    {
        QString tagName = infoElt.tagName();
        QString value = infoElt.attribute("Path");
        QString isDir = infoElt.attribute("IsDir");
        qDebug()<<"tagName"<<tagName<<value<<isDir;

        QDomElement infoElt2 = infoElt.firstChildElement();
        while(!infoElt2.isNull())
        {
            QString tagName = infoElt2.tagName();
            QString value = infoElt2.attribute("Path");
            QString isDir = infoElt2.attribute("IsDir");
            qDebug()<<"tagName"<<tagName<<value<<isDir;
            QDomElement infoElt3 = infoElt2.firstChildElement();
            while(!infoElt3.isNull())
            {
                QString tagName = infoElt3.tagName();
                QString value = infoElt3.attribute("Path");
                QString isDir = infoElt3.attribute("IsDir");
                qDebug()<<"tagName"<<tagName<<value<<isDir;
                infoElt3 = infoElt3.nextSiblingElement();
            }

            infoElt2 = infoElt2.nextSiblingElement();
        }
        //        QVector<CTreeValueElement> vector;
        //        for(int i=0;i<m_headerList.size();i++)
        //        {
        //            QString header = m_headerList.at(i);
        //            CTreeValueElement element;
        //            element.m_name  = header;
        //            element.m_value = infoElt.attribute(header,"null");
        //            element.m_element = infoElt;
        //            vector.append(element);
        //        }
        //        m_dataVector.append(vector);
        infoElt = infoElt.nextSiblingElement();

    }
    qDebug()<<"m_dataVector.size()"<<m_dataVector.size();
    this->setRowCount(m_dataVector.size());
}
*/
//QVariant CServerTreeModel::data(const QModelIndex &index, int role) const
//{
//    if(!index.isValid())
//        return QStandardItemModel::data(index,role);////�ڶ����⣺��index��Ч�Ļ���Ϊ�η��ص��������
//    if(role==Qt::DisplayRole)//�������ı���ʽ����. (QString)
//    {
//        int row = index.row();//�õ�model��������
//        int col = index.column();//�õ�model��������
//        if(m_dataVector.size()>row)//��������ɵ�Ԫ�أ��Ƿ�����������
//        {
//            //�����ڣ���ִ�и�ֵ����һ��vector�Ĳ�����
//            ////���������⣬����.at(row)��ָ��ȡ�ڼ��У�Ȼ����ȡ�ڼ�����
//            QVector<CTreeValueElement> vector = m_dataVector.at(row);//v.at(n)��v[n]��һ���ģ�
//            if(vector.size()>col)               ////��m_dataVector����ǿ���һ����ά������
//            {
//                CTreeValueElement element = vector.at(col);
//                return element.m_value;
//            }
//        }
//    }//������ʵ�����������൱���ڻ��Ƶ�һ�����̣���ô����ҪʲôԪ�أ���ֻҪ��ʲôԪ�ؾͿ����ˡ������QColor�����൱��������һ��������
//    //Ŀǰ������Ҫһ����ɫ�����ԣ���ô�㣬���������ˣ���Ҫ����ô�ࡣ
//    else if(role==Qt::BackgroundRole)
//    {
//        QColor K(255,0,0);
//        return K;
//    }
//    else if(role==Qt::TextColorRole)
//    {
//        QColor c(0,255,0);
//        return c;
//    }
//    else if(role ==Qt::DecorationRole ){//������ͼ�����ʽ����Ϊװ�Ρ�
//        QColor c(0,200,0);
//        return c;
//    }
//    return QStandardItemModel::data(index,role);////���ĸ����⣺�����������ɵĹ����ǰ�model�е����ݴ��浽vector�У�
//    //�𰸣���дQStandardItemModel��data����ʵ��ֵ�滻��������ʵ���������ݵĸ��¡�
//}
////���ݲ���������
//bool CServerTreeModel::setData(const QModelIndex & index, const QVariant & value, int role )
//{
//    if(!index.isValid())
//        return QStandardItemModel::setData(index,value,role);//�����������Ч�ģ���ôֱ�ӷ������data .
//    if(role==Qt::EditRole)//�����е������ʺ��ڱ༭���б༭(QString)
//    {                       //�����role��ʵ����һ����ɫ��Ȼ�󣬲����жϵ�ǰ��Ԫ���Ƿ��������ɫ�������ڣ���ִ����Ӧ�Ĳ�����
//        int row = index.row();//���������õ�Ŀǰ�����ĵ�Ԫ�������
//        int col = index.column();
//        if(m_dataVector.size()>row)//���ȡ�õ������Ԫ���������С��m_dataVector�ĸ����Ļ�����ô�Ϳ��Խ�������Ĳ�����
//        {
//            QVector<CTreeValueElement> vector = m_dataVector.at(row);//������ȡ�ø��е�����Ԫ�ء�
//            if(vector.size()>col)//����������Ԫ�صĸ���Ҳ���������������ִ������Ĳ���
//            {
//                CTreeValueElement element = vector.at(col);//ȡ�����ֵ
//                element.m_value = value.toString();
//                element.setValue(element.m_name,element.m_value);//���������Ĳ��������Ѿ��ǽ��µ�ֵ����element��������������᲻��ִ��
//                vector.replace(col,element);//�滻��ֵ
//                m_dataVector.replace(row,vector);
//                //saveXml();//���浽xml�ļ�����ȥ����������һ����Ԫ����޸ģ��Ϳ���ʵ��������һ��������
//                return true;
//            }
//        }
//    }
//    return QStandardItemModel::setData(index,value,role);//���Ҳ����Ҫ����������һ������
//}
////����ʵ�ַ��ر�־����ϣ��ñ�־���ø���Ŀ��ItemIsEnabled��������������ѡ��ItemIsSelectable��
////δ�ҵ����õĵط�����ζ�������ֱ��ִ�е���
////�����ǵģ������ִ��ʱ�����ɻ�����ֱ�Ӿ����ģ���Ϊһ���޷����п��ƣ�
//Qt::ItemFlags  CServerTreeModel::flags(const QModelIndex& index) const
//{
//    if(!index.isValid())
//        return QStandardItemModel::flags(index);
//    int col = index.column();
//    if(m_headerList.size()>col)//�ڵ�Ԫ�񱻵㵽��ʱ����������
//    {
//        QString header = m_headerList.at(col);
//        if(header == "Age")//����ط��ĺ�������̫��Ƶ����ÿ�뺯��ִ�еĴ���̫�࣬ÿ�ζ������Ǳ�ͣ��ͣ��ʱ��̫���ˣ����ʺϹ۲졣������ͱȽ�ǣǿ���бȽϺõĽ��ͣ�����˵��
//        {
//            Qt::ItemFlags defaultFlags = QStandardItemModel::flags(index);
//            quint32 flag = Qt::ItemIsEditable;//It can be edited.
//            flag = ~flag;
//            defaultFlags = defaultFlags&flag;
//            //defaultFlags = defaultFlags|Qt::ItemIsUserCheckable;
//            return defaultFlags;//���ø��в��ɱ༭,���������Ͷ�ĳ����Ԫ���������Ƿ��ܹ����б༭
//        }
//    }
//    return QStandardItemModel::flags(index);//������󣬷���
//}
//�ļ�����
/*
void CServerTreeModel::parseFile(QString fileName)
{
    QFile file(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            QDomDocument dom("WCM");
            if (dom.setContent(&file))
            {
                //ui->treeWidget->clear();
                QDomElement docElem = dom.documentElement();
                listDom(docElem, NULL);
            }
        }
        file.close();


}
*/




//QString CServerTreeModel::getType(int col)
//{
//    if(m_headerList.size()>col)
//    {
//        QString name = m_headerList.at(col);
//        return m_typeMap.value(name);
//    }
//    return "String";
//}

//QString CServerTreeModel::getValue(int row,int col)
//{
//    if(m_dataVector.size()>row)
//    {
//        QVector<CTreeValueElement> vector = m_dataVector.at(row);
//        if(vector.size()>col)
//        {
//            CTreeValueElement element = vector.at(col);
//            return element.m_value;
//        }
//    }
//    return "";
//}
