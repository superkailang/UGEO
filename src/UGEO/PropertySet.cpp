#include <QtWidgets>
#include"PropertySet.h"

PropertySet::PropertySet(QStringList LayerItem, int idx, QWidget *parent)
: QWidget(parent)
{
	InitLization(LayerItem,idx);
}
void PropertySet::InitLization(QStringList LayerItem, int idx){
	Labelfont.setFamily("宋体");
	Labelfont.setBold(true);
	Labelfont.setPointSize(12);
	SheetLabelList.append(QString::fromLocal8Bit("属性")); 
	gridLayout = new QGridLayout;
	SheetLabelNum = 1;
	SheetLabels = new QLabel*[SheetLabelNum];
	for (int i = 0; i < SheetLabelNum; i++)
	{
		SheetLabels[i] = new QLabel(SheetLabelList[i]);
		SheetLabels[i]->setFont(Labelfont);		
	}	
	UnitBox = new QComboBox;
	for (int i = 0; i < (int)LayerItem.size(); i++){
		UnitBox->addItem(LayerItem[i]);
		UnitBox->setFont(Labelfont);
	}
	UnitBox->setMaxVisibleItems(8);
	int Jindex = 0;
	for (int i = 0; i < SheetLabelNum; i++)
	{
		gridLayout->addWidget(SheetLabels[i], i + 1, Jindex, 1, 1, Qt::AlignTop);			
		gridLayout->addWidget(UnitBox, i + 1, Jindex + 1, 1, 1, Qt::AlignTop);		
	}				
	ButtonStringList.push_back(QString::fromLocal8Bit("确定"));
	ButtonStringList.push_back(QString::fromLocal8Bit("取消"));
	ButtomNum = (int)ButtonStringList.size();
	Buttons = new QPushButton*[ButtomNum];
	buttonBox = new QDialogButtonBox(Qt::Horizontal);
	for (int i = 0; i < ButtomNum; i++)
	{
		Buttons[i] = new QPushButton(ButtonStringList[i]);
		Buttons[i]->setDefault(true);
		Buttons[i]->setFixedWidth(60);
		Buttons[i]->setFont(Labelfont);
		buttonBox->addButton(Buttons[i], QDialogButtonBox::ActionRole);
	}
	this->UnitBox->setCurrentIndex(idx);
	// Button Action;	
	connect(Buttons[0], SIGNAL(clicked()), this, SLOT(submit()));
	connect(Buttons[1], SIGNAL(clicked()), this, SLOT(quit()));	
	TopLeftLayout = new QVBoxLayout();
	TopLeftLayout->setMargin(20);
	TopLeftLayout->setSpacing(40);
	TopLeftLayout->addLayout(gridLayout);
	TopLeftLayout->addWidget(buttonBox);
	setLayout(TopLeftLayout);
	setMinimumHeight(100);
}
void PropertySet::submit(){
	int idx= this->UnitBox->currentIndex();
	QString Property = this->UnitBox->currentText();
	cps->UpdateGridProperty( idx, Property);
	close();
}
void PropertySet::quit(){
	close();
}