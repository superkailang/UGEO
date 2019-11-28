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
*  @file     mywidget.cpp													 *
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

#include "mywidget.h"
#include "cellpara3dshow.h"
#include"EnumDef.h"
#include"qdockwidget.h"
#include"qdesktopwidget.h"
#include"qtablewidget.h"
#include"qtabwidget.h"
#include"qtreewidget.h"
#include"qtoolbutton.h"

MyWidget::MyWidget(QWidget *parent) : QWidget(parent)
{
	LeftFrame = new QLabel(this);
	RightFrame = new QLabel(this);
	ButtonFrame = new QLabel(this);
	TopFrame = new QLabel(this);
	titlelB = new QLabel(this);
	titleiconbT = new QPushButton(this);
	minbT = new QPushButton(this);
	closebT = new QPushButton(this);
	maxbT = new QPushButton(this);


	titlelB->setVisible(false);
	//titlelB->adjustSize();
	titleiconbT->setVisible(false);
	maxbT->setVisible(false);


	this->setMouseTracking(true);

	LeftFrame->setMouseTracking(true);
	TopFrame->setMouseTracking(true);
	RightFrame->setMouseTracking(true);
	ButtonFrame->setMouseTracking(true);

	m_nMouseResize = eCursorNormal;

	connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(ScreenChange()));
	connect(minbT, SIGNAL(clicked()), this, SLOT(WidgetMinimun()));
	connect(closebT, SIGNAL(clicked()), this, SLOT(Widgetclose()));

	QWidget::installEventFilter(this);



}
MyWidget::~MyWidget(){
	delete CentralFrame;
	delete LeftFrame;
	delete RightFrame;
	delete ButtonFrame;
	delete TopFrame;
	delete titlelB;
	delete titleiconbT;
	delete minbT;
	delete closebT;
	delete maxbT;
}
bool MyWidget::eventFilter(QObject *object, QEvent *event)  //event Filter
{
	if (object == this){
		if (event->type() == QEvent::WindowDeactivate){
			titlelB->setStyleSheet("color: rgb(98, 100, 107)");
			titleiconbT->setIcon(QIcon(":/Icons/Ennosoft_logo_gray.png"));

		}
		else if (event->type() == QEvent::WindowActivate){
			titlelB->setStyleSheet("color: rgb(0, 0, 0)");
			titleiconbT->setIcon(QIcon(":/Icons/My.png"));
		}
	}

	return QObject::eventFilter(object, event);
}
void MyWidget::setSize(){

	int hwidth = 4.0;     //left and right border margin
	int vtheight = 20.0;  // top height of title
	int vbheight = 4.0;   //bottom border margin

	int BaseSizeX = 1280;
	int BaseSizeY = 720;  //Base resolution

	QDesktopWidget* desktopWidget = QApplication::desktop();
	QRect screenRect = desktopWidget->screenGeometry();   //current screen resolution

	double fx = (double)screenRect.width() / BaseSizeX;
	double fy = (double)screenRect.height() / BaseSizeY;   //ratio of screen resolution

	if (widgetindex == 13){
		fx = 1.0;
		fy = 1.0;
	}

	hwidth = hwidth*fx;
	vtheight = vtheight*fy;
	vbheight = vbheight*fy;
	QSize Basesize;

	Basesize.setWidth(Centresize.width() + 2 * hwidth);
	Basesize.setHeight(Centresize.height() + vtheight + vbheight);

	curwidth = Basesize.width();
	curheight = Basesize.height();
	this->setGeometry(0, 0, curwidth, curheight);  //Center Windows Size;


	TopFrame->setGeometry(0, 0, Basesize.width(), vtheight);
	LeftFrame->setGeometry(0, vtheight, hwidth, Centresize.height() + vbheight - 1);
	RightFrame->setGeometry(Basesize.width() - hwidth, vtheight, hwidth, Centresize.height() + vbheight - 1);
	ButtonFrame->setGeometry(0, Centresize.height() + vtheight, Basesize.width(), vbheight);
	CentralFrame->setGeometry(hwidth, vtheight, Centresize.width(), Centresize.height());


	int btwidth = 25.0;    // Minimum close button width
	int btheight = 15.0;    // Minimum close button height

	int bty = 3.0;
	mincloseval = fx*mincloseval;

	titlelen = 100;

	btwidth = fx*btwidth;
	btheight = fy*btheight;
	bty = fy*bty;

	minbT->setGeometry(Basesize.width() - 2 * mincloseval - 2 * btwidth, bty, btwidth, btheight);
	closebT->setGeometry(Basesize.width() - mincloseval - btwidth, bty, btwidth, btheight);

	maxbT->setGeometry(Basesize.width() - 3 * mincloseval - 3 * btwidth, bty, btwidth, btheight);
	titleiconbT->setGeometry(0, 0, vtheight, vtheight);
	titlelB->setGeometry(mincloseval + btwidth + 3, bty + 2, titlelen, btheight);

	int len = titlelB->width();

	//Set style
	TopFrame->setStyleSheet(
		"background: #3399CC;"
		"border-top: 1px solid  black;"
		"border-left: 1px solid  black;"
		"border-right: 1px solid  black;"
		/*"border-top-radius: 3px;"*/
		);
	LeftFrame->setStyleSheet(
		"background: #3399CC;"
		"border-left: 1px solid  black;"
		/*"border-bottom-left-radius: 3px;"*/
		);
	RightFrame->setStyleSheet(
		"background: #3399CC;"
		"border-right: 1px solid  black;"
		/*"border-bottom-right-radius: 3px;"*/
		);
	ButtonFrame->setStyleSheet(
		"background: #3399CC;"
		"border-bottom: 1px solid  black;"
		/*"border-bottom-radius: 3px;"*/
		);

}
void MyWidget::setCentralWidget(int WidgetIndex, bool istrag, bool domodel, bool islayout) //Set Central Windows
{
	switch (WidgetIndex)
	{
	case  1:   //Widget class 
		//CentralFrame = new Widget(this);
		Centresize.setWidth(565);
		Centresize.setHeight(570);
		break;
	case  2:   //carfinset class
		CentralFrame = new QDockWidget(tr("PEBI Model"), this);
		//Centresize.setWidth(410);
		//Centresize.setHeight(500);
		break;
	case  3:   //cellline class
		/*CentralFrame = new cellline(this);
		Centresize.setWidth(200);
		Centresize.setHeight(40);*/
		break;
	case  4:   //cellpara3dshow class
		CentralFrame = new cellpara3dshow(this);
		Centresize.setWidth(960);
		Centresize.setHeight(540);
		break;
	case  5:   //cellparastatisanaly class
		//CentralFrame = new cellparastatisanaly(this);
		Centresize.setWidth(360);
		Centresize.setHeight(220);
		break;
	case  6:   //cellrange class
		//CentralFrame = new cellrange(this);
		Centresize.setWidth(230);
		Centresize.setHeight(180);
		break;
	case  7:   //colorbarset class
		//CentralFrame = new colorbarset(this);
		Centresize.setWidth(150);
		Centresize.setHeight(270);
		break;
	case  8:   //gridparasetclass
		//CentralFrame = new GridParaSet(this);
		Centresize.setWidth(291);
		Centresize.setHeight(240);
		break;
	case  9:   //mycalender class 
		//CentralFrame = new QCalendarWidget(this);
		Centresize.setWidth(200);
		Centresize.setHeight(160);
		break;
	case  10:   //propeoscoefedit class 
		//CentralFrame = new PropEosCoefEdit(this);
		Centresize.setWidth(550);
		Centresize.setHeight(330);
		break;
	case  11:   //propertyset class 
		//CentralFrame = new propertyset(this);
		Centresize.setWidth(173);
		Centresize.setHeight(279);
		break;
	case  12:   //scalplot class 
		//CentralFrame = new scalplot(this);
		Centresize.setWidth(442);
		Centresize.setHeight(295);
		break;
	case  13:   //timeslider class 
		//CentralFrame = new timeslider(this);
		Centresize.setWidth(670);
		Centresize.setHeight(70);
		break;
	case  14:   //well3dshowset class 
		//CentralFrame = new well3dshowset(this);
		Centresize.setWidth(240);
		Centresize.setHeight(144);
		break;
	case  15:   //QFileDialog class 
		//CentralFrame = new QFileDialog(this);
		Centresize.setWidth(435);
		Centresize.setHeight(270);
		break;	
	default:
		break;
	}
	widgetindex = WidgetIndex;	
	resizenable = istrag;
	modal = domodel;
	layout = islayout;
	if (modal)
		this->setWindowModality(Qt::ApplicationModal);

	CentralFrame->setMouseTracking(true);
	CentralFrame->setStyleSheet("background	: #fafafa;");
	//CentralFrame->setAttribute(Qt::WA_TranslucentBackground);
	CentralFrame->setWindowFlags(Qt::FramelessWindowHint);
	setSize();
	if (WidgetIndex != 13){
		calchildWidget(CentralFrame);
		ScreenChange();
	}	
	resizeInit = true;
	this->setWindowFlags(Qt::FramelessWindowHint);
	QDesktopWidget* desktopWidget = QApplication::desktop();
	QRect screenRect = desktopWidget->screenGeometry();   //current screen resolution
	this->move((screenRect.width() - this->width())*0.5, (screenRect.height() - this->height())*0.5);
	if (WidgetIndex != -1){
		//QString qss;
		//QFile  f(":/Icons/css.qss");
		//f.open(QFile::ReadOnly);
		//qss = QLatin1String(f.readAll());
		//f.close();
		//this->setStyleSheet(qss);
	}
	titleiconbT->setStyleSheet("background : transparent;"
		"border-image:none;"
		"border:none;");

	maxbT->setStyleSheet("background : transparent;"
		"border-image:none;"
		"border:none;");

	minbT->setStyleSheet("background : transparent;"
		"border-image:none;"
		"border:none;");

	closebT->setStyleSheet("background : transparent;"
		"border-image:none;"
		"border:none;");

	minbT->setStyleSheet(
		"QPushButton{border-image: url(:/Icons/frmminbtstyle.png);"
		"background : transparent;"
		"color: rgb(0, 0, 0);"
		"border: none;}"
		"QPushButton:hover{border-image: url(:/Icons/frmminbtstyle-hover.png);"
		"background : transparent;"
		"color: rgb(0, 0, 0);"
		"border: none;}"
		"QPushButton:pressed{border-image: url(:/Icons/frmminbtstyle-hover.png);"
		"background : transparent;"
		"color: rgb(0, 0, 0);"
		"border: none;}"
		);

	closebT->setStyleSheet(
		"QPushButton{border-image: url(:/Icons/frmclosebtstyle.png);"
		"background : transparent;"
		"color: rgb(0, 0, 0);"
		"border: none;}"
		"QPushButton:hover{border-image: url(:/Icons/frmclosebtstyle-hover.png);"
		"background : transparent;"
		"color: rgb(0, 0, 0);"
		"border: none;}"
		"QPushButton:pressed{border-image: url(:/Icons/frmclosebtstyle-hover.png);"
		"background : transparent;"
		"color: rgb(0, 0, 0);"
		"border: none;}"
		);

	/*minbT->setIcon(QIcon(":/Icons/frmminbtstyle.png"));
	minbT->setIconSize(QSize(minbT->width(), minbT->height()));

	closebT->setIcon(QIcon(":/Icons/frmclosebtstyle.png"));
	closebT->setIconSize(QSize(closebT->width(), closebT->height()));*/

}
void MyWidget::mousePressEvent(QMouseEvent *event){

	if (event->button() == Qt::LeftButton &&
		TopFrame->rect().contains(event->globalPos() - this->frameGeometry().topLeft()))
	{
		m_MousePressPos = event->globalPos();
		m_bMousePressed = true;
	}

	if (eCursorNormal != m_nMouseResize)
	{
		m_bMouseResizePressed = true;
		m_ResizePressPos = event->pos();
	}
	event->ignore();
}
void MyWidget::mouseReleaseEvent(QMouseEvent *event){

	if (event->button() == Qt::LeftButton)
	{
		m_bMousePressed = false;
		m_bMouseResizePressed = false;
		m_ResizePressPos.setX(0);
		m_ResizePressPos.setY(0);
		if (resizenable)
			m_nMouseResize = setCursorStyle(event->pos());
		if (ismousemove){
			ismousemove = false;
			this->resize(curwidth*1.01, curheight*1.01);
		}
	}
	event->ignore();
}
void MyWidget::mouseMoveEvent(QMouseEvent *event) {

	if (m_bMousePressed)
	{
		m_MouseMovePos = event->globalPos();
		this->move(this->pos() + m_MouseMovePos - m_MousePressPos);
		m_MousePressPos = m_MouseMovePos;
		frmpos = this->pos();
		return;
	}


	if (resizenable){
		QPoint curPoint = event->pos();

		if (!m_bMouseResizePressed)
		{
			m_nMouseResize = setCursorStyle(curPoint);
		}

		if (m_bMouseResizePressed && !m_ResizePressPos.isNull())
		{
			double xper;
			double yper;

			switch (m_nMouseResize)
			{
			case eCursorHor:
				xper = abs(curPoint.x() - curwidth);
				if (xper >= 16) {
					ismousemove = true;
					this->resize(curPoint.x(), this->height());
					curwidth = curPoint.x();
				}
				break;
			case eCursorVer:
				yper = abs(curPoint.y() - curheight);
				if (yper >= 16) {
					ismousemove = true;
					this->resize(this->width(), curPoint.y());
					curheight = curPoint.y();
				}
				break;
			case eCursorHorVer:
				xper = abs(curPoint.x() - curwidth);
				yper = abs(curPoint.y() - curheight);
				if (xper >= 16 || yper >= 16) {
					ismousemove = true;
					this->resize(curPoint.x(), curPoint.y());
					curwidth = curPoint.x();
					curheight = curPoint.y();
				}
				break;
			default:
				break;
			}
		}
	}

	event->ignore();
}
int MyWidget::setCursorStyle(const QPoint& curPoint){

	int nCurWidth = RightFrame->x();
	int nCurHeight = ButtonFrame->y();
	int nRes = eCursorNormal;


	if ((nCurWidth - curPoint.x() <= 3) && (nCurHeight - curPoint.y() <= 3))
	{
		setCursor(Qt::SizeFDiagCursor);
		nRes = eCursorHorVer;
	}
	else if (nCurWidth - curPoint.x() <= 3)
	{
		setCursor(Qt::SizeHorCursor);
		nRes = eCursorHor;
	}
	else if (nCurHeight - curPoint.y() <= 3)
	{
		setCursor(Qt::SizeVerCursor);
		nRes = eCursorVer;
	}
	else
	{
		if (ismousemove){
			ismousemove = false;
			this->resize(curwidth*1.01, curheight*1.01);
		}
		setCursor(Qt::ArrowCursor);
		nRes = eCursorNormal;
	}

	return nRes;
}
void MyWidget::Widgetresize(double fx, double fy){

	resizeInit = false;
	this->setGeometry(this->x(), this->y(), fx*Centresize.width() + 2 * childGeo[1].width(), fy*Centresize.height() + childGeo[3].height() + childGeo[2].height());
	RightFrame->setGeometry(this->width() - childGeo[1].width(), 0, childGeo[1].width(), this->height());
	ButtonFrame->setGeometry(0, this->height() - childGeo[2].height(), this->width(), childGeo[2].height());
	TopFrame->setGeometry(0, 0, this->width(), childGeo[3].height());
	LeftFrame->setGeometry(0, 0, childGeo[4].width(), this->height());
	CentralFrame->setGeometry(childGeo[4].width(), childGeo[3].height(), fx*Centresize.width(), fy*Centresize.height());
	minbT->setGeometry(this->width() - 2 * mincloseval - childGeo[6].width() - childGeo[7].width(), childGeo[6].y(), childGeo[6].width(), childGeo[6].height());
	closebT->setGeometry(this->width() - mincloseval - childGeo[7].width(), childGeo[7].y(), childGeo[7].width(), childGeo[7].height());
	maxbT->setGeometry(this->width() - 3 * mincloseval - childGeo[6].width() - childGeo[7].width() - childGeo[8].width(), childGeo[8].y(), childGeo[8].width(), childGeo[8].height());
	titleiconbT->setGeometry(mincloseval, childGeo[9].y(), childGeo[9].width(), childGeo[9].height());
	titlelB->setGeometry(mincloseval + childGeo[9].width() + 5, childGeo[10].y(), childGeo[10].width(), childGeo[10].height());

	for (int i = 11; i < childwidget.length(); i++){
		resetgrid(childwidget[i], fx, fy, i);
	}

	if (record_mid_size){
		MidSize.setWidth(this->width());
		MidSize.setHeight(this->height());
	}

	resizeInit = true;

}
void MyWidget::resetgrid(QWidget* frm, double fx, double fy, int index){

	const int font_size = 9;
	double fontfx = fx;
	QString classname = frm->metaObject()->className();
	QString objname = frm->objectName();


	if (classname != "QVTKWidget"){
		int x = childGeo[index].x();
		int y = childGeo[index].y();
		int w = childGeo[index].width();
		int h = childGeo[index].height();
		frm->setGeometry(x*fx, y*fy, w*fx, h*fy);
	}
	else {
		if (!ismousemove){
			cellpara3dshow* window = (cellpara3dshow*)CentralFrame;
			frm->setGeometry(0, 0, window->centralWidget()->width(), window->centralWidget()->height());
			//window->readjustUncongTextactor();
			//window->creatscalartick();
		}
	}

	QFont ft = frm->font();
	ft.setFamily("Arial");
	if (classname == "QLabel"){
		ft.setPixelSize((font_size)* fontfx);
	}
	else if (classname == "QCheckBox"){
		ft.setPixelSize((font_size + 0) * fontfx);
	}
	else if (classname == "QQroupBox"){
		ft.setPixelSize((font_size + 1) * fontfx);
	}
	else if (classname == "QRadioButton"){
		ft.setPixelSize((font_size + 0) * fontfx);
	}
	else if (classname == "QComboBox"){
		ft.setPixelSize((font_size - 1) * fontfx);
	}
	else if (classname == "QLineEdit"){
		ft.setPixelSize((font_size + 0) * fontfx);
	}
	else if (classname == "QListWidget"){
		ft.setPixelSize((font_size + 0) * fontfx);
	}
	else if (classname == "QTreeWidget"){
		ft.setPixelSize((font_size + 0) * fontfx);
		QFont ft2;
		ft2.setFamily("Arial");
		QTreeWidget* Tw = static_cast<QTreeWidget*> (frm);
		ft2.setPixelSize(font_size * fontfx);
		Tw->headerItem()->setFont(0, ft2);

	}
	else if (classname == "QTableWidget"){
		ft.setPixelSize((font_size)* fontfx);
		QFont ft2;
		ft2.setFamily("Arial");
		QTableWidget* Tw = static_cast<QTableWidget*> (frm);
		ft2.setPixelSize((font_size)* fontfx);
		Tw->verticalHeader()->setFont(ft2);
		Tw->horizontalHeader()->setFont(ft2);
	}
	else if (classname == "QPushButton"){

		if (objname == "ModSetupControlSelbT" || objname == "ModSetupGridSelbT" || objname == "ModSetupWellSelbT" || objname == "ModSetupPropSelbT" ||
			objname == "ModSetupAdsorbSelbT" || objname == "ModSetupInitSelbT" || objname == "ModSetupSolverSelbT" || objname == "ModSetupSchedSelbT"
			)
		{
			ft.setPixelSize((font_size + 1) * fontfx);
		}
		else {
			ft.setPixelSize(font_size * fontfx);
			QPushButton* btn = (QPushButton*)frm;
			QSize sz;
			sz.setWidth(0.7*btn->width());
			sz.setHeight(0.7*btn->height());
			btn->setIconSize(sz);
		}

	}
	else if (classname == "QToolButton"){
		ft.setPixelSize(font_size * fontfx);
		QToolButton* btn = (QToolButton*)frm;
		QSize sz;
		sz.setWidth(0.7*btn->width());
		sz.setHeight(0.7*btn->height());
		btn->setIconSize(sz);
	}
	else {
		ft.setPixelSize((font_size)* fontfx);
	}
	frm->setFont(ft);

}
void MyWidget::resizeEvent(QResizeEvent* e){

	if (resizeInit && resizenable){

		int curSizeX = this->width();
		int curSizeY = this->height();

		/*double xper = abs((double)(curSizeX - curwidth) / curwidth);
		double yper = abs((double)(curSizeY - curheight) / curheight);
		double maxper = xper;
		if (yper>maxper) maxper = yper;*/

		double fx = (double)(curSizeX - 2.0*childGeo[1].width()) / Centresize.width();
		double fy = (double)(curSizeY - childGeo[3].height() - childGeo[2].height()) / Centresize.height();   //windows resize ratio

		Widgetresize(fx, fy);

		curwidth = this->width();
		curheight = this->height();

	}
}
void MyWidget::ScreenChange(){

	int BaseSizeX = 1280;
	int BaseSizeY = 720;  //base Resolution

	QDesktopWidget* desktopWidget = QApplication::desktop();
	QRect screenRect = desktopWidget->screenGeometry();   //current screen Resolution

	double fx = (double)screenRect.width() / BaseSizeX;
	double fy = (double)screenRect.height() / BaseSizeY;   //Resolution ratio

	Widgetresize(fx, fy);

	curwidth = this->width();
	curheight = this->height();
}
void MyWidget::calchildWidget(QWidget* w){

	int parent = 1;

	childGeo.clear();
	childwidget.clear();


	childGeo.append(this->geometry()); childwidget.append(this);
	childGeo.append(RightFrame->geometry()); childwidget.append(RightFrame);
	childGeo.append(ButtonFrame->geometry()); childwidget.append(ButtonFrame);
	childGeo.append(TopFrame->geometry()); childwidget.append(TopFrame);
	childGeo.append(LeftFrame->geometry()); childwidget.append(LeftFrame);
	childGeo.append(CentralFrame->geometry()); childwidget.append(CentralFrame);
	childGeo.append(minbT->geometry()); childwidget.append(minbT);
	childGeo.append(closebT->geometry()); childwidget.append(closebT);
	childGeo.append(maxbT->geometry()); childwidget.append(maxbT);
	childGeo.append(titleiconbT->geometry()); childwidget.append(titleiconbT);
	childGeo.append(titlelB->geometry()); childwidget.append(titlelB);


	if (!layout){
		QVector<QWidget*> curwidget;
		QVector<QWidget*> nowwidget;

		const QObjectList list = w->children();
		foreach(QObject* obj, list){
			QWidget* w = (QWidget*)obj;
			curwidget.append(w);
		}

		while (parent>0) {
			parent = 0;
			for (int i = 0; i < curwidget.length(); i++){
				QString classname = curwidget[i]->metaObject()->className();
				QString objname = curwidget[i]->objectName();
				if (classname == "QGroupBox" || classname == "QStackedWidget" || classname == "QWidget" || classname == "QTabWidget" || classname == "QDialog"
					){
					const QObjectList list = curwidget[i]->children();
					foreach(QObject* obj, list){
						QWidget* w = (QWidget*)obj;
						nowwidget.append(w);
					}
					parent++;
				}
				if (classname == "QGroupBox" || classname == "QLineEdit" || classname == "QPushButton"
					|| classname == "QLabel" || classname == "QComboBox" || classname == "QStackedWidget"
					|| classname == "QSpinBox" || classname == "QComboBox" || classname == "QRadioButton"
					|| classname == "QListWidget" || classname == "QSlider" || classname == "QCheckBox"
					|| classname == "QTableWidget" || classname == "QTreeWidget" || classname == "QDateEdit"
					|| classname == "QScrollBar" || classname == "QCustomPlot" || classname == "QVTKWidget"
					|| classname == "QCalendarWidget" || classname == "QToolButton" || classname == "QProgressBar"
					|| classname == "MyTable" || classname == "QTreeView" || classname == "QDoubleSpinBox" || classname == "QTabWidget"
					|| classname == "QTextEdit" || classname == "QDialogButtonBox"
					)
				{
					childGeo.append(curwidget[i]->geometry());
					childwidget.append(curwidget[i]);
				}
			}
			curwidget.clear();
			curwidget = nowwidget;
			nowwidget.clear();
		}
	}



}
void MyWidget::WidgetMinimun(){
	if (!ismaxsize){
		this->showMinimized();
	}
	else {
		if (sizemode == 2){
			frmpos = this->pos();
			sizemode = 3;
			QDesktopWidget* desktopWidget = QApplication::desktop();
			QRect screenRect = desktopWidget->availableGeometry();   //current screen Resolution		
			record_mid_size = false;
			this->resize(screenRect.width(), screenRect.height());
			record_mid_size = true;
			this->move(0, 0);
			/*minbT->setIcon(QIcon(":/Icons/frmmidbtstyle.png"));
			minbT->setIconSize(QSize(minbT->width(), minbT->height()));*/
			minbT->setStyleSheet(
				"QPushButton{border-image: url(:/Icons/frmmidbtstyle.png);"
				"background : transparent;"
				"color: rgb(0, 0, 0);"
				"border: none;}"
				"QPushButton:hover{border-image: url(:/Icons/frmmidbtstyle-hover.png);"
				"background : transparent;"
				"color: rgb(0, 0, 0);"
				"border: none;}"
				"QPushButton:pressed{border-image: url(:/Icons/frmmidbtstyle-hover.png);"
				"background : transparent;"
				"color: rgb(0, 0, 0);"
				"border: none;}"
				);

		}
		else if (sizemode == 3){
			sizemode = 2;
			this->resize(MidSize.width(), MidSize.height());
			QDesktopWidget* desktopWidget = QApplication::desktop();
			QRect screenRect = desktopWidget->screenGeometry();   //current screen Resolution
			this->move(frmpos.x(), frmpos.y());
			/*minbT->setIcon(QIcon(":/Icons/frmmaxbtstyle.png"));
			minbT->setIconSize(QSize(minbT->width(), minbT->height()));*/
			minbT->setStyleSheet(
				"QPushButton{border-image: url(:/Icons/frmmaxbtstyle.png);"
				"background : transparent;"
				"color: rgb(0, 0, 0);"
				"border: none;}"
				"QPushButton:hover{border-image: url(:/Icons/frmmaxbtstyle-hover.png);"
				"background : transparent;"
				"color: rgb(0, 0, 0);"
				"border: none;}"
				"QPushButton:pressed{border-image: url(:/Icons/frmmaxbtstyle-hover.png);"
				"background : transparent;"
				"color: rgb(0, 0, 0);"
				"border: none;}"
				);
		}
	}

}
void MyWidget::Widgetclose(){
	this->close();
}
void MyWidget::setTitle(const QString Icon, QString str){
	titleiconbT->setVisible(true);
	titleiconbT->setIcon(QIcon(Icon));
	titleiconbT->setIconSize(QSize(0.8*titleiconbT->width(), 0.8*titleiconbT->height()));
	titlelB->setVisible(true);
	titlelB->setText(str);
	QFont font;
	font.setFamily("Arial");
	font.setPointSize(10);
	titlelB->setFont(font);


}
void MyWidget::setTitle(QString str){
	titlelB->setVisible(true);
	titlelB->setText(str);
}
void MyWidget::showmaxbT(){

	ismaxsize = true;

	disconnect(minbT, SIGNAL(clicked()), this, SLOT(WidgetMinimun()));

	minbT->setStyleSheet(
		"QPushButton{border-image: url(:/Icons/frmmaxbtstyle.png);"
		"background : transparent;"
		"color: rgb(0, 0, 0);"
		"border: none;}"
		"QPushButton:hover{border-image: url(:/Icons/frmmaxbtstyle-hover.png);"
		"background : transparent;"
		"color: rgb(0, 0, 0);"
		"border: none;}"
		"QPushButton:pressed{border-image: url(:/Icons/frmmaxbtstyle-hover.png);"
		"background : transparent;"
		"color: rgb(0, 0, 0);"
		"border: none;}"
		);

	maxbT->setStyleSheet(
		"QPushButton{border-image: url(:/Icons/frmminbtstyle.png);"
		"background : transparent;"
		"color: rgb(0, 0, 0);"
		"border: none;}"
		"QPushButton:hover{border-image: url(:/Icons/frmminbtstyle-hover.png);"
		"background : transparent;"
		"color: rgb(0, 0, 0);"
		"border: none;}"
		"QPushButton:pressed{border-image: url(:/Icons/frmminbtstyle-hover.png);"
		"background : transparent;"
		"color: rgb(0, 0, 0);"
		"border: none;}"
		);

	/*minbT->setIcon(QIcon(":/Icons/frmmaxbtstyle.png"));
	minbT->setIconSize(QSize(minbT->width(), minbT->height()));

	maxbT->setIcon(QIcon(":/Icons/frmminbtstyle.png"));
	maxbT->setIconSize(QSize(maxbT->width(), maxbT->height()));*/

	maxbT->setVisible(true);
	connect(maxbT, SIGNAL(clicked()), this, SLOT(showMinimized()));
	connect(minbT, SIGNAL(clicked()), this, SLOT(WidgetMinimun()));

}
void MyWidget::paintEvent(QPaintEvent * event){

	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}
void MyWidget::mouseDoubleClickEvent(QMouseEvent* e){

	if (!ismaxsize) return;

	QPoint pos = e->pos();
	QRect toplabelrect = TopFrame->geometry();
	if (toplabelrect.contains(pos)){
		WidgetMinimun();
	}
}
void MyWidget::showEvent(QShowEvent *e){
	this->setAttribute(Qt::WA_Mapped);
	QWidget::showEvent(e);
}
void MyWidget::closeEvent(QCloseEvent *event) {
	if (iscloseenable){
		CentralFrame->close();
		if (isclose){
			event->accept();
		}
		else {
			event->ignore();
		}
	}
	else {
		event->ignore();
	}
}