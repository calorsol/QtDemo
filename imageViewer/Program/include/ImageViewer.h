#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QtWidgets>
#include <QtCore>
#include <QtXml>
#include <QtGui>
#include <iostream>
#include <QApplication>
#include <QDomDocument>
#include "ImageSlider.h"

class CImgViewElement;
class CSingleImageView;
class CDeepPictureViewer:public QWidget
{
    Q_OBJECT
public:
    CDeepPictureViewer(QWidget *parent = Q_NULLPTR);
    ~CDeepPictureViewer();
    void setDisableDelete(bool flag);
    void addImageList(QStringList imageList);
    void clearAll();
    void resizeEvent(QResizeEvent * event);
    void calculateWidget();
    bool eventFilter(QObject *watched, QEvent *event);
    void showEvent(QShowEvent *event);
    void clearAllSelected();
    void mousePressEvent(QMouseEvent *event);
    CImgViewElement* getCurrent();
    //CImgViewElement* currentPicture;
    void setUseControl(bool flag);
    void setShowImageName(bool flag);
    void setAutoSelectedFirst(bool flag);
    void setSeleceted(int index);
    void gotoPrevImage();
    void gotoNextImage();
    void loadPath(QString path);
    QStringList pictureList;
    void keyPressEvent(QKeyEvent * event);

signals:
    void signalNoSelected();
    void signalCurrentSelecedInfo(int currentIndex,int totalIndex,CImgViewElement* imgPic);
    void signalCurrentFileName(QString);
    void signalSilderValue(int value,QStringList pictureList);
    void signalDeepDelete();
protected slots:
    void slotDelete();
    void slotGotoNextPage();
    void slotGotoPrevPage();
    void slotAddPicture();
    void slotSliderValue(int value);

    void slotSelectedIndex(QString imageName);
    void deepDelete();
    void deepDeleteLayout();
protected:
    void createUi();
    void createControlWidget();
    void updatePage();
    void gotoPage(int pageNum);
    void addXmlMes(QString picturePath);//����ӵ���Ƭ��·��д��xml�ļ�
    void deleteXmlMes(QString picturePath);//����Ҫɾ�����ļ�·��ɾ��
    //CImgViewElement* layoutNextPic();
    CImgViewElement* deleteImgPic();
    QStringList readFile(const QString &fileName);
    CImgViewElement* nextImgPic();
    CImgViewElement* getItemByName(QString name);
protected:
    QHBoxLayout                   *p_layout;
    CImgViewElement*               m_currentSelected;
    QVector<CImgViewElement*>      m_totalVector;//ÿ����Ƭ�ļ�������
    //CSavePicXml                   *p_saveXml;
    int                            m_col;//������ʾ������
    QSize                          m_size;    //��ǰÿ���ؼ��Ĵ�С
    QStringList                    m_currentImageList;//��ǰ����ʾ�б�
    QStringList                    m_totalImageList;//�ܵ�ͼƬ�б�
    int                            m_currentIndex;//��ǰѡ�е�ͼƬ������
    int                            m_startIndex;  //��ǰ��ʼ������
    int                            m_endIndex;    //��ǰ����������
    int                            m_totalPageNum;//��ǰ��ҳ��
    int                            m_currentPage; //��ǰҳ
protected:
    bool                           m_isAutoSelected;
    bool                           m_isShowFileName;//ÿ��img����Ҫչʾ������
    bool                           m_isShowPrevNextControl;//�Ƿ���ʾ���ư�ť
    QWidget                        *p_controlWidget;//���ƽ���
    QLabel                         *p_controlLabel; //���ƽ���label
    QPushButton                    *p_chooseButton;//ѡ��ĳһ��ͼƬ
    QPushButton                    *p_prevButton; //ǰһҳ��ť
    CImageSlider                   *p_slider;//�����ϵĻ���
    QPushButton                    *p_nextButton; //��һҳ��ť
    bool                            m_disableDelete;
};

class CSingleImageView : public QWidget
{
    Q_OBJECT
public:
    CSingleImageView(QWidget *parent = 0);
    ~CSingleImageView();
    void clearImage();
    void loadImage(QString fileName);
    void getWidthAndHeight(int &w,int &h);
    void paintEvent(QPaintEvent *event);
protected:
    QImage m_image;
signals:
    void signalSelectedImageName(QString selectedImageName);
    void signalImgSlider(int value,QStringList pictureList);
public slots:
    void slotSlideValue(int value,QStringList pictureList);
};

class CImgViewElement : public QWidget
{
    Q_OBJECT
public:
    CImgViewElement(QWidget *parent = 0);
    ~CImgViewElement();
    void clearImg();
    void setFontSize(int size);
    void loadImage(QString imageName);
    void setSelected(bool flag);
    bool isSelected();
    void setShowText(bool flag);

public:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent * event);
    void resizeEvent(QResizeEvent * event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void keyPressEvent(QKeyEvent * event);
    void mousePressEvent(QMouseEvent *event);
signals:
    void signalDelete();
    void signalSelectedImageName(QString pictureName);
protected:
    void createUi();
    void resetUi();
    void caculateLines(int rectWidth, QString sourceStr, QStringList &destStrList);
public:
    QString           m_fileName;
    QString           m_fullName;
    QString           m_title;
protected:
    CSingleImageView *p_singleImg;
    QFont             m_font;
    QRectF            m_textRect;
    bool              m_selected;
    bool              m_isEnter;
    bool              m_isShowText;
protected slots:
    void slotSlider(int value,QStringList pictureList);
    //void slotSlideValue(int value,QStringList pictureList);
   // void slotImgCur(QString currentPictureName);

};

#endif // IMAGEVIEWER_H
