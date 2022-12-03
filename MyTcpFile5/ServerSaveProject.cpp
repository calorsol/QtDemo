#include "ServerSaveProject.h"

CServerSaveProject::CServerSaveProject(QObject *parent) : QObject(parent)
{


}
CServerSaveProject::~CServerSaveProject()
{

}
void CServerSaveProject::saveProject(QString fileName,QString path)
{

    QDomDocument doc;

    saveSelectedToFile(fileName,path);
}
//��С�����ڽ����õ������Ӻõ�docд��xml�ļ���
void CServerSaveProject::saveSelectedToFile(QString fileName,QString path)
{
    QDomDocument doc;
    //QString path = "E:/QT_Code/work_content";
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

//��С�����ڽ�xml�ļ���һЩͷ��д��xml�ļ���
void CServerSaveProject::saveSelectedToDoc(QDomDocument& doc,QString path)
{
    m_doc.clear();
    //дͷ���Ĳ���
    QDomProcessingInstruction instruction;
    instruction = m_doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    m_doc.appendChild(instruction);

    //��ȡ��ǰ���ڵ㣬����Ϊ�������ݸ�����Ĵ�����
    QDomElement root = m_doc.createElement("root");
    root.setAttribute("Path",path);
    m_doc.appendChild(root);

    saveToElement(root,path);
    doc=m_doc;
}

//��С�����ڵݹ齫Ŀ���ļ��к��ļ���·��д��xml�ļ�
void CServerSaveProject::saveToElement(QDomElement &parentElt,QString path)
{
    QDir dir(path);
    if(!dir.exists(path))
        dir.mkdir(path);

    //ȡ�����е��ļ����ļ���������ȥ��.��..���ļ��У�����QTĬ���еģ�
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    //�ļ�������
    dir.setSorting(QDir::DirsFirst);
    //ת����һ��list
    QFileInfoList list = dir.entryInfoList();//ȡ����ǰ�ļ��е��ļ���Ϣ
    if(list.size()< 1 ) {
        return ;
    }
    for(int i=0;i<list.size();i++)
    {
        QFileInfo fileInfo = list.at(i);
        bool bisDir = fileInfo.isDir();
        QDomElement childElt=m_doc.createElement("tmp");//���ϵ��Ե�ǰ�ڵ�������ӽڵ㣬�����tmpֻ��һ����ʱ������

        if(bisDir)//�����ļ���
        {
            childElt.setTagName("Folder");
            childElt.setAttribute("Path",fileInfo.filePath());
            childElt.setAttribute("IsDir","true");
            parentElt.appendChild(childElt);
            qDebug()<<fileInfo.filePath();
            saveToElement(childElt,fileInfo.filePath());
        }
        else//�����ļ�
        {
            childElt.setTagName("File");
            childElt.setAttribute("IsDir","false");
            childElt.setAttribute("Path",fileInfo.filePath());
            parentElt.appendChild(childElt);
        }
    }
}

