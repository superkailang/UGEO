#ifndef PROPERTYSET_H
#define PROPERTYSET_H

#include <QDialog>
#include<QStringList>
#include"cellpara3dshow.h"

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QPushButton;
class QLineEdit;
class QLabel;
class QComboBox;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;

class PropertySet : public QWidget{
	Q_OBJECT
public:
	explicit PropertySet(QStringList LayerItem,int idx,QWidget *parent = 0);	
	cellpara3dshow *cps;
private:
	int SheetLabelNum;		
	QStringList SheetLabelList;
	QStringList CheckstringList;
	QStringList UnitBoxList;

	QLabel **SheetLabels;	
	int ButtomNum;
	QPushButton **Buttons;
	QDialogButtonBox *buttonBox;
	QStringList ButtonStringList;

	QComboBox *UnitBox;
	QVBoxLayout *TopLeftLayout;
	QGridLayout *gridLayout;	
private:
	QFont Labelfont;
	void InitLization(QStringList LayerItem, int idx);
private slots:
	void submit();
	void quit();
};
#endif