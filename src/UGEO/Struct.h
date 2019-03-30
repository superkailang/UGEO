#ifndef STRUCT_H
#define STRUCT_H

#include"QVTKWidget.h"
#include"vtkSmartPointer.h"
#include"vtkRenderer.h"
#include"vtkImageActor.h"
#include<iostream>
#include<vector>

#include"Gridstruct.h"
using namespace std;
using namespace UMSM;

	typedef vector<int> intArray;//定义int类的vector容器  
	typedef vector<double> realArray;//定义double类的vector容器

	struct QVTKWidgetView
	{
		QVTKWidget* qvtkwidget;
		vtkSmartPointer< vtkRenderer> ren;
		QVTKWidgetView()
		{
			qvtkwidget = new QVTKWidget();
			ren = vtkSmartPointer< vtkRenderer>::New();
		};
	};

	typedef vector<point> PointArray;//定义点类的vector容器

	
	struct ContourGrid
	{
		int Row;
		int Col;
		point *Grids;
	};	
	struct CellRange
	{
		double minValue;
		double maxValue;
	};
	struct queryState{
		int LOD;
		bool isSpace;
		bool isProperty;
	};


#endif