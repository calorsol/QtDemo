#ifndef SERVERTREEMODEL_H
#define SERVERTREEMODEL_H

#include <QObject>
#include <QtWidgets>
#include <QtCore>
#include <QtXml>
#include <QtGui>
#include <iostream>
class CTreeValueElement;
class CServerTreeModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit CServerTreeModel(QObject *parent = nullptr);
    ~CServerTreeModel();
    bool readFile(const QString &fileName);
//    void parseRootMembers(const QDomElement &element);
//    void parseFolderMembers(const QDomElement &element);
//    void parseFileMembers(const QDomElement &element);
    void parseAllMembers(const QDomElement &element,QStandardItem *ParentItem);
    bool readByte(const QByteArray &byteArray);
    QList<QStandardItem *> childItems;
    QStandardItem *item1;
//    QStandardItem *item1_1;
//    QStandardItem *item1_1_1;
//    QStandardItem *item1_1_1_1;
    QStandardItem *ChildItem;
signals:

public slots:
protected:
    QStringList  m_headerList; //��ͷ�б�
    QString      m_fileName;   //�ļ�����
    QDomDocument m_doc;        //�򿪵��ĵ�
    QMap<QString,QString> m_typeMap;//QMap����һ��ģ���࣬���ṩ���ں�������ֵ䡣
    QVector<QVector<CTreeValueElement>> m_dataVector;////�����,�ڲ���
    QStandardItemModel *goodsModel;
};
class CTreeValueElement
{
public:
    void setValue(QString name,QString value)
    {
        m_element.setAttribute(name,value);
    }
public:
    QString     m_name;
    QString     m_value;
    QDomElement m_element;
};

#endif // SERVERTREEMODEL_H
