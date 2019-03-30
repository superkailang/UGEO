
#ifndef COMMANDS_H
#define COMMANDS_H
#include <QUndoCommand>
#include<QVTKWidget.h>
#include<qtabwidget.h>
#include"qpoint.h"
//class QTabWidget;
// Tab Command;
class DeleteTabCommand : public QUndoCommand
{
public:    
	explicit DeleteTabCommand(QTabWidget *TableItem, const int OldIndex, QUndoCommand *parent = 0);
    void undo() override;
    void redo() override;
private:
	QTabWidget *Item;
	QVTKWidget *qvtkWidget;
	int Index;    
};
#endif
