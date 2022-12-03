#include "ImageSlider.h"

CImageSlider::CImageSlider(QWidget *parent) : QSlider(parent)
{
    m_value=0;
    m_mousePress = false;
    m_isMoving=false;
    m_SliderisEnter = false;
}

CImageSlider::~CImageSlider()
{

}

void CImageSlider::mousePressEvent(QMouseEvent *event){
    //    this.x:�ؼ�ԭ�㵽�����Ե��x����룻
    //    globalPos.x�������λ�õ���Ļ��Ե��x����룻
    //    pos.x�������λ�õ����ؼ���Ե�ľ��룻
    //    this.width:���ؼ��Ŀ��;
    //ע��Ӧ�ȵ��ø��������������¼����������Բ�Ӱ���϶������
    QSlider::mousePressEvent(event);
    m_isMoving = false;
    m_mousePress = true;
    //qDebug()<<"CImageSlider::mousePressEvent"<<maximum()<<minimum();
    //��ȡ����λ�ã����ﲢ����ֱ�Ӵ�ev��ȡֵ����Ϊ������϶��Ļ�����꿪ʼ�����λ��û�������ˣ�
    double pos = event->pos().x() / (double)width();
    double value = pos * (maximum() - minimum()) + minimum();
    // qDebug()<<"CImageSlider::mouseMoveEvent"<<value;
    //value + 0.5 ��������
    if(value>maximum()){
        value=maximum();
    }
    if(value<minimum()){
        value=minimum();
    }
    m_value=value+0.5;
    setValue(m_value);
    //emit sliderMoved( m_value );
    //�򸸴��ڷ����Զ����¼�event type�������Ϳ����ڸ������в�������¼����д���
    QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));
    QCoreApplication::sendEvent(parentWidget(), &evEvent);
}
void CImageSlider::mouseMoveEvent(QMouseEvent *event){
    QSlider::mouseMoveEvent(event);
    double pos = event->pos().x() / (double)width();
    double value = pos * (maximum() - minimum()) + minimum();
    if(value>maximum()){
        value=maximum();
    }
    if(value<minimum()){
        value=minimum();
    }
    //value + 0.5 ��������
    if(m_mousePress){
        m_value=value + 0.5;
        m_isMoving=true;
        //emit sliderMoved(m_value);
    }
    setValue(value + 0.5);
    //�򸸴��ڷ����Զ����¼�event type�������Ϳ����ڸ������в�������¼����д���
    QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));
    QCoreApplication::sendEvent(parentWidget(), &evEvent);
}

void CImageSlider::mouseReleaseEvent(QMouseEvent *event){
    QSlider::mouseReleaseEvent(event);
    //qDebug()<<"mouseReleaseEvent"<<m_value;
    m_mousePress = false;
    m_isMoving=false;
    emit sliderReleasedAt(m_value);//�׳������ź�
}

void CImageSlider::enterEvent(QEvent *event)
{
    m_SliderisEnter = true;
    QWidget::enterEvent(event);
    update();
}
void CImageSlider::leaveEvent(QEvent *event)
{
    m_SliderisEnter = false;
    QWidget::leaveEvent(event);
    //qDebug()<<"CImageSlider::leaveEvent";
    update();
}
int CImageSlider::returnTotalImgNum(int pictureNum)
{
    int sliderPictureNum = pictureNum;
    allPictureNum = pictureNum;
    //qDebug()<<" CImageSlider::returnTotalImgNum"<<sliderPictureNum;
    this->setMinimum(1);
    this->setMaximum(sliderPictureNum);//����slder�����ֵ
    //qDebug()<<"CImageSlider::returnTotalImgNum"<<sliderPictureNum;
    return sliderPictureNum;
}
void CImageSlider::returnCurrentIndex(int index)
{
    currentIndex = index;
    this->setValue(index);
}
void CImageSlider::paintEvent(QPaintEvent *event)
{
    QSlider::paintEvent(event);
    if(m_SliderisEnter==false)
    {
        QImage image(1000,1000,QImage::Format_ARGB32);//����ͼƬ����ͼƬ�ϻ�
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        //painter.begin(&image);
        painter.setPen(QColor(255, 255, 255));
        QFont font = painter.font();
        font.setPixelSize(20);//�ı������С
        font.setFamily("Microsoft YaHei");
        painter.setFont(font);
        int x = 30;
        int y = 15;
        painter.drawText(x,y, QStringLiteral("%1/%2").arg(currentIndex).arg(allPictureNum));
        //painter.end();
    }
}
