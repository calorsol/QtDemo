#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QObject>
#include <QtWidgets>
#include <QEvent>
#include <QMouseEvent>
class CImageSlider : public QSlider
{
    Q_OBJECT
public:
    explicit CImageSlider(QWidget *parent = nullptr);
    ~CImageSlider();
    QLabel *p_SliderLabel;
signals:
    void sliderReleasedAt(int);//������ͷ�ʱ���׳��������X����λ����Ϣ���ź�
public slots:

protected:
    void mousePressEvent(QMouseEvent *event);  //����
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *ev);
    bool m_isMoving;
    int  m_value;
    bool m_mousePress;
    bool m_SliderisEnter;
    int allPictureNum;
    int currentIndex;

public:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    int returnTotalImgNum(int pictureNum);
    QSlider p_slider;
    void returnCurrentIndex(int index);

};

#endif // IMAGEMODEL_H
