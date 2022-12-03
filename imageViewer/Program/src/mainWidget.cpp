#include "mainWidget.h"

CMainWidget::CMainWidget(QWidget *parent)
    :QWidget(parent)
{
    createUi();
    connect(p_imageLabel,SIGNAL(signalSelectedImageName(QString)),p_viewer,SLOT(slotSelectedIndex(QString)));//��ѡ���ͼƬ�����ַ�������ʱ�������ź�֪ͨCDeepPictureViewer�Ǹ���
    connect(p_viewer,SIGNAL(signalSilderValue(int,QStringList)),p_imageLabel,SLOT(slotSlideValue(int,QStringList)));//ͨ����CDeepPictureViewer�Ļ������Ҫ�Ŵ���ʾ��ͼ��

}

CMainWidget::~CMainWidget()
{

}

//������ǵ���CDeepPictureViewer������еļ��ع̶���ַ������ͼƬ�ķ���
void CMainWidget::loadPath(QString path)
{
    p_viewer->loadPath(path);
}

//�������Ui����
void CMainWidget::createUi()
{
    QVBoxLayout *layout = new QVBoxLayout();//���Ǵ�ֱ����
    p_imageLabel = new CSingleImageView();//��һͼƬ��ʾ����
    p_viewer = new CDeepPictureViewer();//���ͼƬ������ʾ����ͼ
    connect(p_viewer,SIGNAL(signalCurrentFileName(QString)),
            this,SLOT(slotItemChanged(QString)));//���connectδʹ���ˣ�û�и����й��ܽ��й���
    p_viewer->setFixedHeight(250);//���ǵ�����ʾ����ͼƬ���ǲ���
    layout->addWidget(p_imageLabel);
    layout->addWidget(p_viewer);
    this->setLayout(layout);
}

//����һ��������ļ������Ϳ�����ʾ����ͼƬ
void CMainWidget::slotItemChanged(QString fileName)
{
    p_imageLabel->loadImage(fileName);
}
