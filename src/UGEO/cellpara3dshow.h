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
*  @file     cellpara3dshow.h												 *
*  @brief    core class for 3d Grid Plot									 *
*  Details.                                                                  *
*                                                                            *
*  @author   kailanghuang                                                    *
*  @email    kailanghuang@pku.edu.cn                                         *
*  @version  2.0.0.1		                                                 *
*  @date     2018/01, 2019/1												 *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         :  core class for 3d Grid Plot							 *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

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
	MyWidget* parentpointer;					//Framework class pointer
private:   // Menu Action
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
	void creatMenuBar(); //create menu toolbar	
	void CreatTopToolBar(); //create Top toolbar
	void CreatRightToolBar(); // create right toolbar	
	void AddToolBarConnection(); // Add toolbar response events
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
	//Status Value
	QColor currentbackcolor;			//Background Color	
	int visofaxes = 0;					//Whether to display axes
	int iscreateaxes = 0;				//Whether to create axes
	bool isaddUncongText = false;		//if to crate Unocng text
	bool isaddscalarbartick = false;	
	bool bkcolofbk = false;				//if Background color is Black
	bool hasvalue = false;				//cellvalue is set 
	int istransparent = 0;				//transparent value 0--opacity ;1--20%;2--10%
	int colserindex = 0;				//color index
	QString currentkeyName;				//current key name
	QColor celllinecolor;				//color of grid line
	int linewidth = 1;					//line width

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
private:   //top toolbars Actions
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

private:   //right toolbars Actions
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

	// View;
	void setFrontView();
	void setBackView() ;
	void SetTopView()  ;
	void SetBottomView();
	void SetLeftView();
	void SetRightView();
	void set_switch(); // swict view
	
	void Windowtabclose(int index);
	void closeMywidget();    //open case dir
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
	bool isSwitch = true; //  False: unable to switch;
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