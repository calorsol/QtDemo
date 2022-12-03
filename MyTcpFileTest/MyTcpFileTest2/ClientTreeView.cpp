#include "ClientTreeView.h"

CClientTreeView::CClientTreeView(QDialog *parent) : QTreeView(parent)
{
    qDebug()<<"CClientTreeView";
    p_model = new CClientTreeModel();
    this->setModel(p_model);
    QString xmlPath = qApp->applicationDirPath()+"/client/test2.xml";
    p_model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("�ļ�Ŀ¼"));
    p_model->readFile(xmlPath);//����xml�ļ�
}

CClientTreeView::~CClientTreeView()
{

}


