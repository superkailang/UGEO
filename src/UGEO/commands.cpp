#include <QtWidgets>
#include "commands.h"

DeleteTabCommand::DeleteTabCommand(QTabWidget *TableItem, const int OldIndex, QUndoCommand *parent)
	:QUndoCommand(parent)
{		
	this->Item = TableItem;	
	this->qvtkWidget = (QVTKWidget*)TableItem->widget(Index);
	this->Index = OldIndex;
}
void DeleteTabCommand::undo()
{	
	//this->Item->insertTab(Index,this->qvtkWidget, "3D Window");	
	this->Item->addTab(this->qvtkWidget, "3D Window");
	this->Item->update();	
}
void DeleteTabCommand::redo()
{
	this->Item->removeTab(Index);
}