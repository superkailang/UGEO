/*****************************************************************************
*  This program is free software; you can redistribute it and/or modify      *
*  it under the terms of the GNU General Public License version 3 as         *
*  published by the Free Software Foundation.                                *
*                                                                            *
*  You should have received a copy of the GNU General Public License         *
*  along with OST. If not, see <http://www.gnu.org/licenses/>.               *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*  @file     mywidget.h														 *
*  @brief    Class QWidget													 *
*  Details.                                                                  *
*                                                                            *
*  @author   kailanghuang                                                    *
*  @email    kailanghuang@pku.edu.cn                                         *
*  @version  2.0.0.1		                                                 *
*  @date     2018/01, 2019/1												 *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         :  QWidget For UI Layout									 *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

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
	int vtheight = 20.0;  //height of top title
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
	int setCursorStyle(const QPoint& curPoint);    // set Cursor Style

	QPoint m_MousePressPos;				    // Position of Mouse press  
	QPoint m_MouseMovePos;				    // Position of Mouse move  
	bool   m_bMousePressed = false;		   //  True: Mouse Press, False:else   
	int    m_nMouseResize ;				   // Mouse Size  
	bool   m_bMouseResizePressed = false;     
	QPoint m_ResizePressPos;			   

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

	int hwidth = 1.0;     //Left and right border margin	
	int vbheight = 1.0;   //bottom border margin

	QSize MidSize;
	QPoint frmpos;

	bool record_mid_size=true;

	bool isresizedmove = false;
	bool ismousemove=false;

protected :
	void paintEvent(QPaintEvent * event);
	void mouseDoubleClickEvent(QMouseEvent* e);
	bool eventFilter(QObject *object, QEvent *event);//event Filter
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
