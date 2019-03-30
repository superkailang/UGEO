#ifndef MYWIDGET_H
#define MYWIDGET_H

#include "QtGui"
#include <QWidget>
#include "qlabel.h"
#include "qboxlayout.h"
#include "qpushbutton.h"

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = 0);
	~MyWidget();
public:
	void setCentralWidget(int WidgetIndex, bool istrag, bool domodel, bool islayout);
	QWidget* CentralFrame;
	bool iscloseenable = true;
	bool isclose = true;
private:
	int vtheight = 20.0;  //顶部标题高度
    QLabel* LeftFrame;
    QLabel* RightFrame;
    QLabel* ButtonFrame;
    QLabel* TopFrame;	

	QPushButton* titleiconbT;
	QLabel* titlelB;

	QPushButton* minbT;
	QPushButton* maxbT;
	QPushButton* closebT;

	int mincloseval = 0;
	int titlelen = 50;

	QSize Centresize;
	
	void setSize();

	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void showEvent(QShowEvent *e) Q_DECL_OVERRIDE;
	void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
	int setCursorStyle(const QPoint& curPoint);    // 当前位置设置鼠标样式

	QPoint m_MousePressPos;				    // 鼠标点击位置  
	QPoint m_MouseMovePos;				    // 鼠标移动位置  
	bool   m_bMousePressed = false;		   // 鼠标是否按下  
	int    m_nMouseResize ;				   // 鼠标设置大小  
	bool   m_bMouseResizePressed = false;    // 设置大小的按下  
	QPoint m_ResizePressPos;			   // 设置大小鼠标按下的点

	void resetgrid(QWidget* frm, double fx, double fy, int index);

	void calchildWidget(QWidget* w);

	int widgetindex;

	QVector<QWidget*> childwidget;
	QVector<QRect> childGeo;

	bool resizeInit=false;
	bool resizenable = false;

	void resizeEvent(QResizeEvent* e);
	int curwidth;
	int curheight;

	bool modal=false;
	bool layout = false;

	bool ismaxsize=false;
	int sizemode = 2; //1-- min 2--mid 3-max

	int hwidth = 1.0;     //左右边框宽度	
	int vbheight = 1.0;   //底部边框高度

	QSize MidSize;
	QPoint frmpos;

	bool record_mid_size=true;

	bool isresizedmove = false;
	bool ismousemove=false;

protected :
	void paintEvent(QPaintEvent * event);
	void mouseDoubleClickEvent(QMouseEvent* e);
	bool eventFilter(QObject *object, QEvent *event);//事件过滤器
private slots:
	void Widgetresize(double fx, double fy);
	void ScreenChange();
	void WidgetMinimun();
public slots:
	void Widgetclose();
	void setTitle(const QString Icon,QString str);
	void setTitle(QString str);
	void showmaxbT();
};
#endif // MYWIDGET_H
