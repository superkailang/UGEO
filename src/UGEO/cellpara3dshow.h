#ifndef CELLPARA3DSHOW_H
#define CELLPARA3DSHOW_H

#include <QMainWindow>
#include "ui_cellpara3dshow.h"
#include"mywidget.h"
#include"Struct.h"
#include"qdockwidget.h"
#include"qtreewidget.h"
#include"qundostack.h"
#include"qtablewidget.h"

//VTK Widget;
#include"vtkRenderWindow.h"
#include"vtkRenderer.h"
#include"vtkSmartPointer.h"
#include"vtkLight.h"
#include"vtkInteractorStyleTrackballCamera.h"
#include"vtkCellPicker.h"
#include"vtkTextProperty.h"
#include"vtkAxisActor2D.h"
#include"vtkProperty2D.h"
#include"vtkCommand.h"
#include"vtkCallbackCommand.h"
#include"vtkObject.h"
#include"vtkCamera.h"
#include"vtkScalarBarActor.h"
#include"vtkLookupTable.h"
#include"vtkTextProperty.h"
#include"vtkCubeAxesActor.h"
#include"vtkTransform.h"
#include"vtkColorSeries.h"

#include"fileReader.h"
#include"Tree.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QAction;
class QUndoStack;
class QDockWidget;
class QTreeWidget;
QT_END_NAMESPACE
class cellpara3dshow : public QMainWindow
{
	Q_OBJECT

public:
	cellpara3dshow(QWidget *parent = 0);
	~cellpara3dshow();
	Ui::cellpara3dshow ui;	
public:
	MyWidget* parentpointer;					//框架类指针
private:   //菜单栏Action
	QAction *OpenFile;	
	QAction *NewFile;
	QAction *SaveFile;
	QAction *SaveFileAs;
	QAction *ImportAction;
	QAction *ExportAction;
	QAction *ExitAction;
private:
	QUndoStack *undoStack;

	void InitLutLookup();
	void SetDockWidgetTree();
	void SetStatusLabel();
	void creatMenuBar(); //建立菜单栏	
	void CreatTopToolBar(); //创立顶部的工具栏
	void CreatRightToolBar(); //创立右部的工具栏	
	void AddToolBarConnection(); //添加工具栏响应事件
	// 
	void SetFont();
	void InilizationQVtkWidget(QVTKWidget *qvtkWidget, vtkSmartPointer<vtkRenderer> ren);
	
	void readGrid(QString FileName, FileReader Fileftp);

public:
	QVector<QVTKWidgetView> qvtkWidgetView;
	int currentStage = 0;
	vtkSmartPointer< vtkCellPicker > m_pvtkCellPicker;
private:	
	int InitDistance;	
private:
	vtkSmartPointer<vtkCubeAxesActor> AxesActor;
	int CurrentTabIndex;
	QFont SystemFont;
	//状态值
	QColor currentbackcolor;			//背景色	
	int visofaxes = 0;					//是否显示坐标轴
	int iscreateaxes = 0;				//是否创建坐标轴	
	bool isaddUncongText = false;		//是否建立Unocng文本
	bool isaddscalarbartick = false;	//是否建立Unocng文本
	bool bkcolofbk = false;				//背景色是否黑色
	bool hasvalue = false;				//cellvalue是否赋值
	int istransparent = 0;				//半透明设置 0--不透明;1--20%;2--10%
	int colserindex = 0;				//色卡色系
	QString currentkeyName;				//当前参数关键字名称
	QColor celllinecolor;				//网格线颜色	
	int linewidth = 1;					//线宽

private:
	QStringList list;
	vector<vector<vtkSmartPointer<vtkActor>>> Lineactor;
	vector<vector<vtkSmartPointer<vtkActor>>> Faceactor;
	vector<vector<vtkSmartPointer<vtkDoubleArray>>> Cellval;
	vector<vector<FaceNode>> CellZoneDatas;
	// Create a lookup table to share between the mapper and the scalarbar
	vtkSmartPointer<vtkLookupTable> hueLut;
	vtkSmartPointer<vtkScalarBarActor> scalarBar;

private:
	QToolBar *righttoolbar;
	QAction *ResetViewAction;
	QAction *NewWindow;
	QAction *undoAction;
	QAction *redoAction;
private:   //顶部工具栏Action
	QAction *colfaceaction;
	QAction *lineframeaction;
	QAction *backcoloraction;
	QAction *zaddaction;
	QAction *zminuteaction;

	QAction *Topviewaction;
	QAction *Bottomviewaction;
	QAction *Sideviewaction;
	QAction *RightSideviewaction;
	QAction *Frontviewaction;
	QAction *Backviewaction;
	QAction *Manager_action;

private:   //右部工具栏Action
	QAction *cellrangeaction;
	QAction *propertyaction;
private slots:
	//IO Opearation;
	void ResetView();
	void NewProject();
	void SaveFileAction();
	void SaveFileAsAction();
	void ReadFile();
	void ExportFile();
	void ExitFile();
	// Tree Widget;
	void WinTabChange(int Index);
	void AddTabWidget();

	// 视图;
	void setFrontView();
	void setBackView() ;
	void SetTopView()  ;
	void SetBottomView();
	void SetLeftView();
	void SetRightView();
	void set_switch(); // 切换
	
	void Windowtabclose(int index);
	void closeMywidget();    //打开case目录
	void cellrangeset();
	void Propertyset();
	// ToolBar
	void FaceShow();
	void CellLineShow();
	void SetBackground();
	void ZscaleAdd();
	void ZscaleMinus();
	void upsurfaceshow(int type);

public:
	// For Dialog Widget Range;
	void UpdateCellRange(int Layer, double *Bounds, double *Value, int Type, vector<bool> PropFilter, vector<bool> SpaceFlag);
	void SetDistance(double distance);
	void UpdateGridProperty(int idx, QString Property);
	void GetCellId(int FaceId);
private:
	bool *isPropFlag;
	bool isSwitch = true; // 自由切换 ; False 不能切换;
	double Lxyz;
	double gridCenter[3];
	vector<vector<bool>> UPropFilter; // Layer Prop AND Space
	vector<vector<bool>> USpaceFlag;
	vector<int> PropertyLODs;
	vector<QColor> gridColor;
	void setSpaceFilterValue(int Layer, double *Bounds, vector<bool> SpaceFilter);
	void InitDrawGrid();
	int searchStage=0;
	vector<vector<CellRange>> CellValueRange;
	bool isSpaceFilter = false;
	bool isPropertyFilter = false;
	vector<vector<CellRange>> FilterCellValueRange;
	vector<vector<CellRange>> MaxCellValueRange;
	queryState query;
	StrucGrid SGrid;
	int ProperyIdx=0;
	MyTree Tree;
	double Zscale=1.0;
	double rate = 1.1;
	QStringList Property;
	bool isFaceshow = true;
	bool isLineshow = true;
	bool isBackground = true;
	bool isFirstGrid = true;
	void UpdateView(int stage);
	void SetProperty(bool *flag);
	void SetScale(double xscale, double yscale, double zscale);
	void UpdateProperty(int Layer, double *Value, vector<bool> PropFilter, int isFixed);
	void UpdateSpace(int Layer, double *Bounds, vector<bool> SpaceFlag);
	void judgeUpdate(int Layer, double *Bounds, double *Value, vector<bool> &PropFilter, vector<bool> &SpaceFlag, bool &isFlag);
	void setPropertyValue(double *Value, int Layer, vector<bool> PropFilter);
	void getCenter(vector<CellRange> cells, double *center);
	void SetTransform();
	void setAxisColor(bool isWhite);
	void get_valid_cellnum(StrucGrid &SGrid, int &valid_cellnum);
	bool print_searchInfo(vector<bool> PropFilter, vector<bool> SpaceFlag);
	void set_grid_prop(int idx);
};

#endif // CELLPARA3DSHOW_H