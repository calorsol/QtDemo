#include "ServerTreeView.h"
#include "ServerTreeModel.h"
CServerTreeView::CServerTreeView(QWidget *parent) : QTreeView(parent)
{
    p_saveProject = new CServerSaveProject();
    //�ڸ��Ĳ���ʱ�����齫���ƹ������ļ�·����б�ܸ���һ�·��򣬲��ܸ��õ�ʶ��
    QString fileName="E:/Test/test.xml";//����Ҫд���Ŀ���ļ���
    QString path = "E:/QT_Code/work_content/Day01";//����Ҫ���м���Ŀ���ļ���
    p_saveProject->saveProject(fileName,path);
    initTreeView();
}
CServerTreeView::~CServerTreeView()
{
}
void CServerTreeView::initTreeView()
{
    QString xmlPath = "E:/Test/test.xml";
    p_model = new CServerTreeModel();
   // p_model->readByte(byteArray);
    this->setModel(p_model);//public void setModel(TreeModel newModel):���ݴ��ݵĲ�������TreeModel��ֵ
    //p_model->parseFile(xmlPath);//����xml�ļ�
    p_model->readFile(xmlPath);
    p_model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("�ļ���"));
    //this->setUpdatesEnabled(true);  //�ָ�����ˢ��
}
void CServerTreeView::receiveByte(QByteArray byteArray)
{
    qDebug()<<"receiveByte";
    p_model->readByte(byteArray);
}
