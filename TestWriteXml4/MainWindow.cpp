#include "MainWindow.h"

CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    saveProject();
}

CMainWindow::~CMainWindow()
{
}
void CMainWindow::saveProject()
{
    QString fileName="E:/Test/test.xml";
    QDomDocument doc;
    QString path = "E:/Test";
    saveSelectedToDoc(doc,path);
    QFile file(fileName);

    bool bSuccess = file.open(QIODevice::WriteOnly);
    if(bSuccess == false)
    {
        file.close();
        return;
    }
    QTextStream out(&file);
    doc.save(out, 4);
    file.close();

}
void CMainWindow::saveSelectedToDoc(QDomDocument& doc,QString path)
{
    m_doc.clear();
    QDomProcessingInstruction instruction;
    instruction = m_doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    m_doc.appendChild(instruction);

    QDomElement root = m_doc.createElement("root");
    //QString path     = filePath(m_RootIndex);
    root.setAttribute("Path",path);
    m_doc.appendChild(root);

    saveToElement(root,path);

    doc=m_doc;
}
void CMainWindow::saveToElement(QDomElement &parentElt,QString path)
{
    QDir dir(path);
    if(!dir.exists(path))
        dir.mkdir(path);

    //ȡ�����е��ļ����ļ���������ȥ��.��..���ļ��У�����QTĬ���еģ�
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    //�ļ�������
    dir.setSorting(QDir::DirsFirst);
    //ת����һ��list
    QFileInfoList list = dir.entryInfoList();
    if(list.size()< 1 ) {
        return ;
    }
    for(int i=0;i<list.size();i++)
    {
        //QModelIndex childIndex=this->index(i,0,parentIndex);
        QFileInfo fileInfo = list.at(i);
        bool bisDir = fileInfo.isDir();


        QDomElement childElt=m_doc.createElement("tmp");
        if(bisDir)
        {
            childElt.setTagName("Folder");
            childElt.setAttribute("Path",fileInfo.filePath());
//            if(checkedFlag==Qt::Checked)
//                childElt.setAttribute("CheckFlag","Checked");
//            else
//                childElt.setAttribute("CheckFlag","PartiallyChecked");
            childElt.setAttribute("IsDir","true");
            parentElt.appendChild(childElt);
            qDebug()<<fileInfo.filePath();
            saveToElement(childElt,fileInfo.filePath());
        }
        else
        {
            childElt.setTagName("File");
            childElt.setAttribute("Path",fileInfo.filePath());
            parentElt.appendChild(childElt);
        }

    }
}
void CMainWindow::saveSelectedToFile(QString fileName)
{
    QDomDocument doc;
    QString path = "E:/Test";
    saveSelectedToDoc(doc,path);
    QFile file(fileName);

    bool bSuccess = file.open(QIODevice::WriteOnly);
    if(bSuccess == false)
    {
        file.close();
        return;
    }
    QTextStream out(&file);
    m_doc.save(out, 4);

    file.close();
}
