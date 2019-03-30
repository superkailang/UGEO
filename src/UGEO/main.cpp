#include <QApplication>
#include "mywidget.h"
#include"cellpara3dshow.h"
#include"SerachDialog.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);	
	//在最后一个窗口关闭后退出：
	QApplication::setQuitOnLastWindowClosed(true);	
	// Widget ;
	MyWidget *para3dshowWidget = new MyWidget();
	para3dshowWidget->setCentralWidget(4, true, false, false);
	cellpara3dshow* window = (cellpara3dshow*)para3dshowWidget->CentralFrame;
	//para3dshowWidget->setTitle(":/Icons/Ennosoft_logo.png", "UNCONG View");
	para3dshowWidget->showmaxbT();
	window->setAttribute(Qt::WA_DeleteOnClose, true);
	window->parentpointer = para3dshowWidget;		
	window->setWindowIcon(QIcon(":/Icons/My.ico"));
	//window->Init();
	para3dshowWidget->show();
	QSize sz = para3dshowWidget->size();
	//para3dshowWidget->resize(1.02*sz.width(), sz.height());				
	return a.exec();
}