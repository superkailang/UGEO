#include "cellpara3dshow.h"
#include "QtGui"
#include"commands.h"
#include"fileReader.h"

#include"qscrollbar.h"
#include"qpalette.h"
#include"qcolordialog.h"


#include"GridProcess.h"
#include"Gridplot.h"
#include"SerachDialog.h"
#include"PropertySet.h"



static void MouseWheelEvent(vtkObject *caller, unsigned long eid, void *clientdata, void *calldata) //面拾取函数
{
	cellpara3dshow *parent = (cellpara3dshow*)clientdata;
	double distance;
	if (!parent) 
		return;		
	int *ptmp, Id;
	int stage = parent->currentStage;
	switch (eid)
	{
	case vtkCommand::RightButtonPressEvent:
		ptmp = parent->qvtkWidgetView[0].qvtkwidget->GetInteractor()->GetEventPosition();
		parent->m_pvtkCellPicker->Pick(
			(double)ptmp[0],
			(double)ptmp[1],
			0.0,
			parent->qvtkWidgetView[stage].ren);
		Id = parent->m_pvtkCellPicker->GetCellId();
		parent->GetCellId(Id);
		break;
	case vtkCommand::MouseWheelBackwardEvent:
		//cout << "Back" << endl;
		distance = parent->qvtkWidgetView[0].ren->GetActiveCamera()->GetDistance();
		//cout << distance << endl;
		parent->SetDistance(distance);
		break;
	case vtkCommand::MouseWheelForwardEvent:
		//cout << "Forward" << endl;
		distance = parent->qvtkWidgetView[0].ren->GetActiveCamera()->GetDistance();
		parent->SetDistance(distance);
		//cout << distance << endl;
		break;
	default:
		break;
	}
}

cellpara3dshow::cellpara3dshow(QWidget *parent)
: QMainWindow(parent)
{
	ui.setupUi(this); 
	this->ui.tabWidget->setTabsClosable(true);
	
	connect(this->ui.tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(Windowtabclose(int)));
	connect(this->ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(WinTabChange(int)));

	this->undoStack = new QUndoStack(this);
	currentbackcolor.setRgbF(1.0, 1.0, 1.0, 1.0); //背景色
	celllinecolor.setRgbF(128 / 256.0, 128 / 256.0, 128 / 256.0, 1.0); //网格线颜色
	//model3data.col.append(QColor(0, 0, 255, 255));
	//model3data.col.append(QColor(5, 213, 255, 255));
	//model3data.col.append(QColor(5, 255, 160, 255));
	//model3data.col.append(QColor(36, 255, 3, 255));
	//model3data.col.append(QColor(255, 255, 3, 255));
	//model3data.col.append(QColor(255, 160, 7, 255));
	//model3data.col.append(QColor(255, 0, 0, 255));  //默认色卡体系
	this->setWindowTitle("GEO VIEW");
	CreatTopToolBar(); //创立顶部的工具栏
	CreatRightToolBar(); //创立右部的工具栏 
	AddToolBarConnection(); //添加工具栏响应事件
	creatMenuBar(); //建立菜单栏
	SetStatusLabel();			
	righttoolbar->setMouseTracking(true);
	AddTabWidget();
	ProperyIdx = 0;
	PropertyLODs.push_back(0); PropertyLODs.push_back(0); PropertyLODs.push_back(0);
	gridColor.push_back(QColor(0, 0, 255, 255));
	gridColor.push_back(QColor(0, 0, 255, 255));
	gridColor.push_back(QColor(5, 213, 255, 255));
	gridColor.push_back(QColor(5, 255, 160, 255));
	gridColor.push_back(QColor(36, 255, 3, 255));
	gridColor.push_back(QColor(255, 255, 3, 255));
	gridColor.push_back(QColor(255, 160, 7, 255));
	gridColor.push_back(QColor(255, 0, 0, 255));

	hueLut = vtkSmartPointer<vtkLookupTable>::New();
	scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
	AxesActor = vtkSmartPointer<vtkCubeAxesActor>::New();
	InitLutLookup();
}
cellpara3dshow::~cellpara3dshow()
{
	/*delete[]parentpointer;
	delete[]OpenFile;
	delete[]NewFile;
	delete[]SaveFile;
	delete[]SaveFileAs;
	delete[]ImportAction;
	delete[]ExportAction;
	delete[]ExitAction;
	delete[]undoStack;
	parentpointer = NULL;
	OpenFile = NULL;
	NewFile = NULL;
	SaveFile = NULL;
	SaveFileAs = NULL;
	ImportAction = NULL;
	ExportAction = NULL;
	ExitAction = NULL;
	undoStack = NULL;


	delete [] righttoolbar;
	delete [] ResetViewAction;
	delete [] NewWindow;
	delete [] undoAction;
	delete [] redoAction;
	delete [] colfaceaction;
	delete [] lineframeaction;
	delete [] backcoloraction;
	delete [] zaddaction;
	delete [] zminuteaction;
	delete[]  cellrangeaction;
	delete[]  propertyaction;
	righttoolbar = NULL;
	ResetViewAction = NULL;
	NewWindow = NULL;
	undoAction = NULL;
	redoAction = NULL;
	colfaceaction = NULL;
	lineframeaction = NULL;
	backcoloraction = NULL;
	zaddaction = NULL;
	zminuteaction = NULL;
	cellrangeaction = NULL;
	propertyaction = NULL;
	*/
}

// IO Operation;
void cellpara3dshow::NewProject()
{
	;
}
void cellpara3dshow::SaveFileAction()
{
	;
}
void cellpara3dshow::SaveFileAsAction()
{
	;
}
void cellpara3dshow::ReadFile()
{
	bool if_echo = true;
	QString FileName;
	int FileIndex;
	FileReader Fileftp(if_echo);
	StrucGrid Grid;
	Wells Well;
	MyTree Tree;
	if (Fileftp.Data_File_Dialog(FileIndex, FileName))
	{		
		readGrid(FileName, Fileftp);					
	}
}
void cellpara3dshow::ExportFile()
{

}
void cellpara3dshow::ExitFile(){
	exit(0);
}

void cellpara3dshow::readGrid(QString FileName, FileReader Fileftp){	
	Wells Well;	
	QByteArray  cc = FileName.toLocal8Bit();
	const char*tempFilePath = cc.data();
	clock_t t1 = clock();
	Fileftp.InputData(tempFilePath, Well, SGrid);
	clock_t t2 = clock();
	printf("Reading Files %d ms \n",(t2-t1));
	if (SGrid.NGrid > 0){		
		DataProcess Process(SGrid, Tree);
		CellValueRange.clear();
		isFirstGrid = true;
		SetProperty(SGrid.Flag);
		InitDrawGrid();
		FilterCellValueRange = CellValueRange;
		for (int i = 0; i < (int)FilterCellValueRange.size(); i++){
			if (i == FilterCellValueRange.size() - 1){
				FilterCellValueRange[i][2].minValue = 1;
				FilterCellValueRange[i][2].maxValue = SGrid.NZ;
			}
			else{
				FilterCellValueRange[i][2].minValue = 1;
				FilterCellValueRange[i][2].maxValue = SGrid.Phasemaps[i].BottomLayers.size();
			}
		}
		getCenter(CellValueRange[2],gridCenter);
		MaxCellValueRange = FilterCellValueRange;
		if (isFirstGrid && CellValueRange.size() > 0){
			AxesActor->SetCamera(qvtkWidgetView[this->CurrentTabIndex].ren->GetActiveCamera());
			AxesActor->SetXTitle("X");
			AxesActor->SetYTitle("Y");
			setAxisColor(false);
			qvtkWidgetView[this->CurrentTabIndex].ren->AddActor(AxesActor);		
			//ResetView();	
			vtkSmartPointer<vtkTextProperty> titleprop = vtkTextProperty::New();
			titleprop->SetFontSize(10.0);
			titleprop->SetColor(0.0, 0.0, 0.0);
			scalarBar->SetWidth(0.1);
			scalarBar->SetTitleTextProperty(titleprop);
			scalarBar->SetLabelTextProperty(titleprop);
			scalarBar->SetLabelFormat("%.4e");
			scalarBar->SetLookupTable(hueLut);
			qvtkWidgetView[this->CurrentTabIndex].ren->AddActor2D(scalarBar);
			qvtkWidgetView[CurrentTabIndex].qvtkwidget->GetRenderWindow()->Render();
		}
		isFirstGrid = false;
		clock_t t3 = clock();
		printf("Draw Grid %d\n",t3-t2);
		query.isProperty = false;
		query.isSpace = false;
		query.LOD = 0;
		
		// 输出部分网格;
		/*
		  SGrid, nx, ny,nz;
		*/
	}
}

void cellpara3dshow::InitDrawGrid(){
	vector<vtkSmartPointer<vtkActor>> tempLineActor;
	vector<vtkSmartPointer<vtkActor>> tempFaceActor;
	vector<vtkSmartPointer<vtkDoubleArray>> tempVal;
	vector<CellRange> tempCellRange;
	vector<FaceNode> ZoneDatas;	
	for (int currentStage2 = 0; currentStage2 < Lineactor.size(); currentStage2++){
		for (int i = 0; i < Lineactor[currentStage2].size(); i++){
			qvtkWidgetView[this->CurrentTabIndex].ren->RemoveActor(Lineactor[currentStage2][i]);
			qvtkWidgetView[this->CurrentTabIndex].ren->RemoveActor(Faceactor[currentStage2][i]);
		}
	}
	if (Faceactor.size()>0){
		Faceactor.clear();
	}
	if (Lineactor.size()>0){
		Lineactor.clear();
	}
	if (Cellval.size() > 0){
		Cellval.clear();
	}
	if (CellZoneDatas.size() > 0){
		CellZoneDatas.clear();
	}
	/*int nc=0;
	if (searchStage == 0){
		nc = 0;
		for (int c_i=0; c_i < Tree.root->NumberChild; c_i++){
			nc  = nc + Tree.root->childNotes[c_i]->NumberChild;
		}
	}
	else{
		nc = SGrid.NGrid;
	}
	bool ** Cellarray = new bool*[nc]; 
	for (int i = 0; i < nc; i++){
		Cellarray[i] = new bool[7];
		memset( Cellarray[i], false, sizeof(bool) * 7);
	}
	*/
	clock_t t1, t2;
	t1 = clock();
	for( int j = searchStage; j < 3; j++ ){
		switch (j)
		{
		case 2:
			if (searchStage < 2){
				GetOrginalFace(0, 2 * searchStage,Tree.root, SGrid, ZoneDatas);
			}
			else{
				GetGridBoundary(SGrid, ZoneDatas);
			}
			if (isFirstGrid){
				GetCellRange(SGrid, tempCellRange);
			}
			break;
		case 0:
			GetTreeNodeBoundary(0,0,Tree.root, SGrid, ZoneDatas); // 优化获取LOD1 的可视面;
			if (isFirstGrid){
				GetNodeCellRange(0, 0, Tree.root, tempCellRange,isPropFlag); // 迭代获取属性值;
			}
			break;
		case 1:
			if (searchStage == 0){
				GetNodeBoundary(Tree.root, SGrid, ZoneDatas);
			}
			else{
				GetTreeNodeBoundary(0,1,Tree.root, SGrid, ZoneDatas); // 优化获取LOD1 的可视面
			}
			if (isFirstGrid){
				GetNodeCellRange(0, 2, Tree.root, tempCellRange, isPropFlag); // 迭代获取属性值; 
			}
			break;
		default:
			break;
		};
		if (tempCellRange.size() > 0){
			CellValueRange.push_back(tempCellRange);
			tempCellRange.clear();
		};
		if (ZoneDatas.size() > 0){
			CellZoneDatas.push_back(ZoneDatas);
			ZoneDatas.clear();
		};
		t2 = clock();
		printf("Stage %d, %d\n",j,t2-t1);
		t1 = t2;
	};
	for (int j = 0; j < CellZoneDatas.size(); j++){
		vtkSmartPointer<vtkActor> CellLineActor = vtkSmartPointer<vtkActor>::New();
		vtkSmartPointer<vtkActor> ZoneActor = vtkSmartPointer<vtkActor>::New(); 
		//vtkSmartPointer<vtkColorTransferFunction> lut = vtkSmartPointer<vtkColorTransferFunction>::New();
		vtkSmartPointer<vtkDoubleArray> val = vtkSmartPointer<vtkDoubleArray>::New();
		DrawGrid(CellZoneDatas[j], CellValueRange[j][ProperyIdx], ProperyIdx, CellLineActor, ZoneActor, val, hueLut);
		tempLineActor.push_back(CellLineActor);
		tempFaceActor.push_back(ZoneActor);
		tempVal.push_back(val);
		Lineactor.push_back(tempLineActor);
		Faceactor.push_back(tempFaceActor);
		Cellval.push_back(tempVal);		
		tempLineActor.clear();
		tempFaceActor.clear();
		tempVal.clear();
		//val->Delete();
	}
	for (int currentStage = 0; currentStage < Lineactor.size(); currentStage++){
		for (int i = 0; i < Lineactor[currentStage].size(); i++){
			qvtkWidgetView[this->CurrentTabIndex].ren->AddActor(Lineactor[currentStage][i]);
			qvtkWidgetView[this->CurrentTabIndex].ren->AddActor(Faceactor[currentStage][i]);
			if (currentStage != 0){
				Lineactor[currentStage][i]->SetVisibility(FALSE);
				Faceactor[currentStage][i]->SetVisibility(FALSE);
			}
		}
	}
	t2 = clock();
	printf("Draw Grid %d\n",t2 - t1);
	Zscale = 1.0;
	SetScale(1.0, 1.0, Zscale);
}

//Set Up UI 
void cellpara3dshow::SetStatusLabel()
{
	/*
	statusmsgLabel = new QLabel;
	statusmsgLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); //lable
	statusBar()->addWidget(statusmsgLabel, 1); //状态栏	

	zdirmultLabel = new QLabel;
	zdirmultLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); //lable
	statusBar()->addWidget(zdirmultLabel, 1); //状态栏
	zdirmultLabel->setText("Z axis scale: 1.0");

	rotateSumLabel = new QLabel;
	rotateSumLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); //lable
	statusBar()->addWidget(rotateSumLabel, 1); //状态栏	

	distanceLabel = new QLabel;
	distanceLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); //lable
	statusBar()->addWidget(distanceLabel, 1);

	kkLabel = new QLabel;
	kkLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); //lable
	statusBar()->addWidget(kkLabel, 7); //状态栏	

	statusBar()->setStyleSheet(
		"background : #41719c;"
		"border: none;"
		"color : white;"
		);
	statusBar()->setSizeGripEnabled(false);
	*/
}
void cellpara3dshow::SetFont()
{	
	int font_size = 9*1.4;
	SystemFont.setFamily("Arial");
	SystemFont.setPixelSize((font_size));
	SystemFont.setBold(true);
	//SystemFont.setFamily("黑体");
	//SystemFont.setPointSize(8);
	//Modeldockwidget->setFont(SystemFont);
	///ReservoirInfowidget->
	//Inputdockwidget->setStyleSheet("QDockwidget{font:bold}");	
}
void cellpara3dshow::creatMenuBar()  //创立菜单栏
{
	QMenu *Filemenu = menuBar()->addMenu("File");
	OpenFile = Filemenu->addAction(QIcon(":/Icons/Load_dynamic_properties.png"), tr(" Open File Path"));
	Filemenu->addSeparator();
	ExitAction = Filemenu->addAction(QIcon(":/Icons/exit.png"), tr(" Exit"));

	connect(OpenFile, SIGNAL(triggered()), this, SLOT(ReadFile()));
	connect(ExitAction, SIGNAL(triggered()), this, SLOT(ExitFile()));

	QMenu *Viewmenu = menuBar()->addMenu("View");
	QMenu *Plotmenu = menuBar()->addMenu("Plot");
	NewWindow = Plotmenu->addAction(QIcon(":/Icons/Open_Current_folder.png"), tr(" New 3D Plot"));
	connect(NewWindow, SIGNAL(triggered()), this, SLOT(AddTabWidget()));
	
	ResetViewAction = Viewmenu->addAction(QIcon(":/Icons/Open_Current_folder.png"), tr(" ResetView"));
	connect(ResetViewAction, SIGNAL(triggered()), this, SLOT(ResetView()));

	// MenuBar Setting;s
	menuBar()->setMouseTracking(true);
	menuBar()->setStyleSheet("background : #d6dbe9;");
	this->setContextMenuPolicy(Qt::CustomContextMenu);
}
void cellpara3dshow::CreatTopToolBar()    //创建顶部的工具条
{
	//mainToolBar
	colfaceaction = new QAction(QIcon(":/Icons/cellface.png"), tr("&Colorface"), this); colfaceaction->setToolTip(tr("Show/hide surfaces")); ui.mainToolBar->addAction(colfaceaction);
	lineframeaction = new QAction(QIcon(":/Icons/lineframe.png"), tr("&LineFrame"), this); lineframeaction->setToolTip(tr("Show/hide meshes")); ui.mainToolBar->addAction(lineframeaction);
	backcoloraction = new QAction(QIcon(":/Icons/backcolor.png"), tr("&BackColor"), this); backcoloraction->setToolTip(tr("Switch background color")); ui.mainToolBar->addAction(backcoloraction);
	zaddaction = new QAction(QIcon(":/Icons/zadd.png"), tr("&ZDiradd"), this); zaddaction->setToolTip(tr("Scale up z-direction")); ui.mainToolBar->addAction(zaddaction);
	zminuteaction = new QAction(QIcon(":/Icons/zminute.png"), tr("&ZDirminute"), this); zminuteaction->setToolTip(tr("Scale down z-direction")); ui.mainToolBar->addAction(zminuteaction);

	Frontviewaction = new QAction(QIcon(":/Icons/frontview.png"), tr("&ZDirminute"), this); Frontviewaction->setToolTip(tr("Front View ")); ui.mainToolBar->addAction(Frontviewaction);
	Topviewaction = new QAction(QIcon(":/Icons/zminute.png"), tr("&ZDirminute"), this); //Topviewaction->setToolTip(tr("Top View")); ui.mainToolBar->addAction(Topviewaction);
	Sideviewaction = new QAction(QIcon(":/Icons/zminute.png"), tr("&ZDirminute"), this); // Sideviewaction->setToolTip(tr("Side View")); ui.mainToolBar->addAction(Sideviewaction);
	

	Bottomviewaction = new QAction(QIcon(":/Icons/bottomview.png"), tr("&ZDirminute"), this); Bottomviewaction->setToolTip(tr("Side View")); ui.mainToolBar->addAction(Bottomviewaction);
	RightSideviewaction = new QAction(QIcon(":/Icons/rightview.png"), tr("&ZDirminute"), this); RightSideviewaction->setToolTip(tr("Side View")); ui.mainToolBar->addAction(RightSideviewaction);
	Backviewaction = new QAction(QIcon(":/Icons/backview.png"), tr("&ZDirminute"), this); Backviewaction->setToolTip(tr("Side View")); ui.mainToolBar->addAction(Backviewaction);

	Manager_action = new QAction(QIcon(":/Icons/manager.png"), tr("&ZDirminute"), this); Manager_action->setToolTip(tr("Side View")); ui.mainToolBar->addAction(Manager_action);


	QSize iconsize(36, 36);
	ui.mainToolBar->setIconSize(iconsize);
	ui.mainToolBar->setMouseTracking(true);
}
void cellpara3dshow::CreatRightToolBar()    //创建右边的工具条
{
	//rightToolBar
	righttoolbar = new QToolBar(this);
	righttoolbar->setOrientation(Qt::Vertical);
	this->addToolBar(Qt::RightToolBarArea, righttoolbar);
	righttoolbar->setStyleSheet("QToolBar{border-bottom-width:0;}");
	cellrangeaction = new QAction(QIcon(":/Icons/cellrange.png"), tr("&cellrange"), this); cellrangeaction->setToolTip(tr("Visible range..."));	righttoolbar->addAction(cellrangeaction);
	propertyaction = new QAction(QIcon(":/Icons/property.png"), tr("&property"), this); propertyaction->setToolTip(tr("Properties..."));	righttoolbar->addAction(propertyaction);	
	QSize iconsize(36, 36);
	righttoolbar->setIconSize(iconsize);
}
void cellpara3dshow::AddToolBarConnection() //添加工具条上Action的响应事件
{
	//MenuBar
	undoAction = undoStack->createUndoAction(this, tr("&Undo"));
	undoAction->setShortcuts(QKeySequence::Undo);

	redoAction = undoStack->createRedoAction(this, tr("&Redo"));
	redoAction->setShortcuts(QKeySequence::Redo);
	//顶部工具栏按钮响应
	connect(colfaceaction, SIGNAL(triggered()), this, SLOT(FaceShow()));
	connect(lineframeaction, SIGNAL(triggered()), this, SLOT(CellLineShow()));	
	connect(backcoloraction, SIGNAL(triggered()), this, SLOT(SetBackground()));
	connect(zaddaction, SIGNAL(triggered()), this, SLOT(ZscaleAdd()));
	connect(zminuteaction, SIGNAL(triggered()), this, SLOT(ZscaleMinus()));
	// 三视图
	connect(Frontviewaction, SIGNAL(triggered()), this, SLOT(setFrontView()));
	connect(Topviewaction, SIGNAL(triggered()), this, SLOT(SetTopView()));
	connect(Sideviewaction, SIGNAL(triggered()), this, SLOT(SetLeftView()));

	connect(Bottomviewaction, SIGNAL(triggered()), this, SLOT(SetBottomView()));
	connect(RightSideviewaction, SIGNAL(triggered()), this, SLOT(SetRightView()));
	connect(Backviewaction, SIGNAL(triggered()), this, SLOT(setBackView()));

	connect(Manager_action, SIGNAL(triggered()), this, SLOT(set_switch()));

	////右部工具栏按钮响应
	connect(cellrangeaction, SIGNAL(triggered()), this, SLOT(cellrangeset()));		   //网格显示范围设置	
	connect(propertyaction, SIGNAL(triggered()), this, SLOT(Propertyset()));		   //网格显示范围设置	
};
void cellpara3dshow::InitLutLookup(){
	// Make the lookup table with a preset number of colours.
	vtkSmartPointer<vtkColorSeries> colorSeries =
		vtkSmartPointer<vtkColorSeries>::New();
	hueLut->SetNumberOfColors(256);
	hueLut->SetHueRange(0.667, 0.0);
	hueLut->Build();
}

//Tab Operation;
void cellpara3dshow::AddTabWidget()
{	
	QVTKWidgetView tempView;		
	this->ui.tabWidget->addTab(tempView.qvtkwidget, "3D Window");
	CurrentTabIndex = this->ui.tabWidget->currentIndex();
	this->ui.tabWidget->setMouseTracking(true);
	this->ui.tabWidget->setUsesScrollButtons(true);	
	this->ui.tabWidget->setCurrentIndex(0);
	//初始化QVTKWidget;
	InilizationQVtkWidget(tempView.qvtkwidget, tempView.ren);
	qvtkWidgetView.push_back(tempView);
}
void cellpara3dshow::WinTabChange(int Index)
{
	this->CurrentTabIndex = Index;
}
void cellpara3dshow::Windowtabclose(int index)
{
	 this->ui.tabWidget->removeTab(index);
	 this->ui.tabWidget->setCurrentIndex(0);
	 CurrentTabIndex = 0;
	 // 除去所有的Actor 和Status
	//QUndoCommand *deleteCommand = new DeleteTabCommand(this->ui.tabWidget,index);
	// undoStack->push(deleteCommand);
	//this->ui.tabWidget->removeTab();	
	// 删除数据;
}

void cellpara3dshow::InilizationQVtkWidget(QVTKWidget *qvtkWidget, vtkSmartPointer< vtkRenderer > ren)
{
	// 初始化 VTK 配置 	
	qvtkWidget->setCursor(Qt::OpenHandCursor); //设置鼠标形状	
	// 设置ui.qvtkWidget的渲染器	
	// Create the rendering window
	vtkSmartPointer<vtkRenderWindow> renwin = vtkSmartPointer<vtkRenderWindow>::New();
	ren->SetBackground(1, 1, 1);
	qvtkWidget->SetRenderWindow(renwin);
	qvtkWidget->GetRenderWindow()->AddRenderer(ren);
	vtkSmartPointer<vtkLight> headlight = vtkSmartPointer<vtkLight>::New();
	headlight->SetLightType(VTK_LIGHT_TYPE_HEADLIGHT);
	headlight->SetColor(1.0, 1.0, 1.0);
	headlight->SetIntensity(1.0);
	ren->AddLight(headlight);
	qvtkWidget->GetRenderWindow()->Render();
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
		vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
	// 面拾取
	m_pvtkCellPicker = vtkSmartPointer< vtkCellPicker >::New();
	m_pvtkCellPicker->SetTolerance(0.005);
	m_pvtkCellPicker->SetTolerance(0.005);
	qvtkWidget->GetInteractor()->SetPicker(m_pvtkCellPicker);
	m_pvtkCellPicker->GetPath();

	//建立回调函数--鼠标，获取坐标
	vtkSmartPointer <vtkCallbackCommand> Press_interactor;
	Press_interactor = vtkSmartPointer < vtkCallbackCommand >::New();
	Press_interactor->SetCallback(MouseWheelEvent);
	Press_interactor->SetClientData(this);
	qvtkWidget->GetInteractor()->AddObserver(vtkCommand::RightButtonPressEvent, Press_interactor);

	vtkSmartPointer <vtkCallbackCommand> callback_interactor;
	callback_interactor= vtkSmartPointer < vtkCallbackCommand >::New();
	callback_interactor->SetCallback(MouseWheelEvent);
	callback_interactor->SetClientData(this);
	qvtkWidget->GetInteractor()->AddObserver(vtkCommand::MouseWheelBackwardEvent, callback_interactor);

	vtkSmartPointer <vtkCallbackCommand> MouseForward_interactor;
	MouseForward_interactor = vtkSmartPointer < vtkCallbackCommand >::New();
	MouseForward_interactor->SetCallback(MouseWheelEvent);
	MouseForward_interactor->SetClientData(this);
	qvtkWidget->GetInteractor()->AddObserver(vtkCommand::MouseWheelForwardEvent, MouseForward_interactor);
}
	
void cellpara3dshow::ResetView()
{
	if (qvtkWidgetView.size() > 0){
		if (CurrentTabIndex >= 0){
			qvtkWidgetView[CurrentTabIndex].ren->ResetCamera();
			qvtkWidgetView[CurrentTabIndex].ren->DrawOn();
			qvtkWidgetView[CurrentTabIndex].qvtkwidget->GetRenderWindow()->Render();

			double distance = qvtkWidgetView[CurrentTabIndex].ren->GetActiveCamera()->GetDistance();
			InitDistance = distance;
			if (currentStage != 0){
				//触发事件;
				SetDistance(distance);
			}
		}
	}
}
void cellpara3dshow::SetDistance(double distance){	
	if (this->isSwitch){
		double thread_1 = 1.0 / 5;
		double thread_2 = 1.0 / 2;
		int tempStage = -1;
		double minDistance = InitDistance - InitDistance * thread_1;
		double maxDistance = InitDistance - InitDistance * thread_2;
		if (distance < maxDistance){
			tempStage = 2; // 最细的
		}
		else if (distance < minDistance){
			tempStage = 1; // 
		}
		else{
			tempStage = 0;
		}
		if ((tempStage + searchStage) < 3){
			if (tempStage != currentStage){
				UpdateView(tempStage);
			}
			currentStage = tempStage;
		}
	}
}
void cellpara3dshow::UpdateView(int stage){ // Update View
	if (this->isSwitch){
		int Newstage = stage + searchStage;
		int NewcurrentStage = searchStage + currentStage;
		if (Newstage < 3 && NewcurrentStage < 3)
		{
			for (int i = 0; i < Lineactor[stage].size(); i++){
				Lineactor[stage][i]->SetVisibility(true);
				Faceactor[stage][i]->SetVisibility(true);
			}
			for (int i = 0; i < Lineactor[currentStage].size(); i++){
				Faceactor[currentStage][i]->SetVisibility(FALSE);
				Lineactor[currentStage][i]->SetVisibility(FALSE);
			}
		}
		qvtkWidgetView[CurrentTabIndex].qvtkwidget->GetRenderWindow()->Render();
	}
}

// 3D WINDOW CLOSE;
void cellpara3dshow::closeMywidget(){
	parentpointer->Widgetclose();
}

// 右部工具栏
void cellpara3dshow::cellrangeset(){
	if (CellValueRange.size() > 0){		
		SearchDialog *New = new SearchDialog(FilterCellValueRange,MaxCellValueRange,SGrid.Phasemaps,query,isPropFlag);
		QPoint ptmp;
		New->cps = this;
		ptmp = mapToGlobal(righttoolbar->pos());
		New->move(ptmp.x() - New->width(), ptmp.y());
		New->show();
	}
}
void cellpara3dshow::UpdateCellRange(int Layer, double *Bounds, double *Value, int Type,vector<bool> PropFilter,vector<bool> SpaceFlag){	
	bool isFlag = false;
	int valid_cellnum = 0;
	// 判断更新策略;
	judgeUpdate(Layer, Bounds, Value, PropFilter, SpaceFlag, isFlag);
	clock_t t1 = clock();
	printf("查询开始 \n");
	bool isSpaceFlag = print_searchInfo(PropFilter,SpaceFlag);
	if (isFlag){
		// 查询逻辑;
		/*
			空间查询:
			  1) root 查询;;
			  2）查询结果进入下一层查询，直至结束；
			  3）查询范围之外的，下一层：isActive;
			  4）查询范围之内的，下一层：isFullActive;
			  5）查询范围部分属于的，下一层：isPartActive;

			可视面绘制：
				1）当前可视面；需要获取最底层的有效和无效网格;
				2) 获取当前LOD层位的可视面;
				3) 绘制下一层;

			属性查询:
				1) 当前层遍历；
				2）当前层Active;


		*/
		/*if (Layer < 2){
			//ResetActView(SGrid, false);  // ? 解决顶面出现问题；提高查询效率
		}
		else{
			ResetActView(SGrid, true);
		}
		*/
		searchStage = Layer;
		switch (Type)
		{
		case 1: // 属性和空间同时查询;
			// 更新查询缓存数据;
			setSpaceFilterValue(Layer, Bounds,SpaceFlag);
			setPropertyValue(Value, Layer, PropFilter);
			UpdateSpace(Layer, Bounds, SpaceFlag);
			UpdateProperty(Layer, Value, PropFilter, isSpaceFlag); // 
			break;
			// 属性和空间查询取并集方法;
			// 先做空间查询; 传入 isFixed 判断已经筛选;
		case 2: // 
			query.isProperty = false;
			setSpaceFilterValue(Layer, Bounds,SpaceFlag);
			UpdateSpace(Layer, Bounds, SpaceFlag);
			/*
			switch (Layer)
			{
			case 0:
			setType(Tree.root, Layer);
			SpaceFilter(Layer, Tree.root, SGrid, Bounds[0], Bounds[1], Bounds[2], Bounds[3], (int)Bounds[4], (int)Bounds[5], SpaceFlag);
			Filter(SGrid, Tree.root, Layer);
			break;
			case 1:
			setType(Tree.root, Layer);
			SpaceFilter2(Layer, Tree.root, SGrid, Bounds[0], Bounds[1], Bounds[2], Bounds[3], (int)Bounds[4], (int)Bounds[5], SpaceFlag);
			Filter(SGrid, Tree.root, Layer);
			break;
			case 2:
			SpaceFilter3(Layer, Tree.root, SGrid, Bounds[0], Bounds[1], Bounds[2], Bounds[3], (int)Bounds[4], (int)Bounds[5], SpaceFlag);
			break;
			default:
			break;
			}
			*/
			break;
		case 3: // 只有属性查询
			query.isSpace = false;
			setPropertyValue(Value, Layer, PropFilter);
			UpdateProperty(Layer, Value, PropFilter,false);
			break;
		default:
			break;
		};
		// 判断查询是否为空;
		// 更新SGrid Data 数据;
		if (Layer < (int)SGrid.Phasemaps.size()){
			setType(Layer, Tree.root, 0, SGrid, valid_cellnum); // 如果非底层查询;
		}
		else{
			get_valid_cellnum(SGrid, valid_cellnum);
		}
		clock_t t2 = clock();
		printf("查询结束，有效网格数: %d,  总消耗时间 %.2f s\n", valid_cellnum,1.0*(t2 - t1) / 1000);
		/*for (int currentStage2 = 0; currentStage2 < Lineactor.size(); currentStage2++){
			for (int i = 0; i < Lineactor[currentStage2].size(); i++){
				qvtkWidgetView[this->CurrentTabIndex].ren->RemoveActor(Lineactor[currentStage2][i]);
				qvtkWidgetView[this->CurrentTabIndex].ren->RemoveActor(Faceactor[currentStage2][i]);
			};
		};
		*/
		// clock_t t3 = clock();
		printf("可视化开始: ");
		clock_t t3 = clock();
		InitDrawGrid();
		clock_t t4 = clock();
		printf("可视化结束:  总消耗时间 %.2f s\n ", 1.0*(t4 - t3) / 1000);
		// 沉积相更新
		if (ProperyIdx==6){
			set_grid_prop(ProperyIdx);
		}
		//clock_t t4 = clock();
		//printf("Draw Grid %d\n", t4 - t3);
	}
}

// 空间查询
void cellpara3dshow::UpdateSpace(int Layer, double *Bounds, vector<bool> SpaceFlag){
	if (SpaceFlag[0] || SpaceFlag[1] || SpaceFlag[2]){
		updateSpace(0,Layer, Tree.root, SGrid, Bounds[0], Bounds[1], Bounds[2], Bounds[3], (int)Bounds[4] - 1, (int)Bounds[5] - 1, SpaceFlag);
	}
}

// 属性查询;
void cellpara3dshow::UpdateProperty(int Layer, double *Value, vector<bool> PropFilter,int isFixed){
	// isFixed true: 表示取并集;
	int TotalFaceLayer = (int)SGrid.Phasemaps.size(); // LOD 0, LOD 2;
	if (Layer < TotalFaceLayer){
		for (int i = 0; i < PropFilter.size(); i++){
			if (PropFilter[i]){
				// 迭代查询;
				GeoPropUpdate(Layer, Tree.root, SGrid, Value[2 * i], Value[2 * i + 1], i + 1, isFixed);
				isFixed = isFixed + 1;
			}
		}
	}
	else{
		// 直接查询底部节点;节省Tree数据存储
		for (int i = 0; i < PropFilter.size(); i++){
			if (PropFilter[i]){
				GeoFilter(SGrid, Value[2 * i], Value[2 * i + 1], i + 1, isFixed);
				isFixed = isFixed + 1;
			};
		};
	};
	/*switch (Layer)
	{
	case 0:
		for (int i = 0; i < PropFilter.size(); i++){
			if (PropFilter[i]){
				GeoFilter(Tree.root, SGrid, Value[2 * i], Value[2 * i + 1], i + 1);
			}
		}
		break;
	case 1:
		for (int i = 0; i < PropFilter.size(); i++){
			if (PropFilter[i]){
				for (int j = 0; j < Tree.root->NumberChild; j++){
					GeoFilter(Tree.root->childNotes[j], SGrid, Value[2 * i], Value[2 * i + 1], i + 1);
				}
			}
		};
		break;
	case 2:
		for (int i = 0; i < PropFilter.size(); i++){
			if (PropFilter[i]){
				GeoFilter(SGrid, Value[2 * i], Value[2 * i + 1], i + 1);
			}
		}
		break;
	default:
		break;
	}
	*/
}

void cellpara3dshow::judgeUpdate(int Layer, double *Bounds, double *Value, vector<bool> &PropFilter, vector<bool> &SpaceFlag, bool &isFlag){
	if (query.LOD != Layer){
		query.LOD = Layer;
		isFlag = true; // 保存数据筛选条件;
		if (query.LOD == (int)SGrid.Phasemaps.size()){
			ResetActView(SGrid, true); //底层由于树索引问题采取Reset 方式; 
			//return;
		}
	}
	// 判断Previous 是否有筛选; 然后;筛选之后判断是否为边界之外;
	for (int i = 0; i < SpaceFlag.size(); i++){
		if (SpaceFlag[i]){
			USpaceFlag[Layer][i] = true;
		}
		else{
			if (USpaceFlag[Layer][i]){
				SpaceFlag[i] = true;
				if (Bounds[2 * i] < (MaxCellValueRange[Layer][i].minValue + InfSmall) && Bounds[2 * i + 1] > (MaxCellValueRange[Layer][i].maxValue - InfSmall)){
					USpaceFlag[Layer][i] = false;
				};
			};
		}
	};
	for (int i = 0; i < PropFilter.size(); i++){
		if (isPropFlag[i]){
			if (PropFilter[i]){
				UPropFilter[Layer][i] = true;
			}
			else{
				if (UPropFilter[Layer][i]){
					PropFilter[i] = true;
					if (Value[2 * i] < (MaxCellValueRange[Layer][i + 3].minValue + FloatSmall) && Value[2 * i + 1] > (MaxCellValueRange[Layer][i + 3].maxValue - FloatSmall)){
						UPropFilter[Layer][i] = false;
					};
				};
			};
		}
		else{
			PropFilter[i] = false;
		}
	};
	for (int i = 0; i < SpaceFlag.size(); i++){
		if (SpaceFlag[i]){
			isFlag = true;
			break;
		}
	}
	if (!isFlag){
		for (int i = 0; i < PropFilter.size(); i++){
			if (PropFilter[i]){
				isFlag = true;
				break;
			}
		}
	}
}

void cellpara3dshow::SetScale(double xscale, double yscale, double zscale){
	if (qvtkWidgetView.size() > 0){
		qvtkWidgetView[CurrentTabIndex].qvtkwidget->GetRenderWindow()->Render();
		ResetView();
		SetTransform();
	}
}

void cellpara3dshow::SetBackground(){
	isBackground = !isBackground;
	if (isBackground)
	{
		this->qvtkWidgetView[CurrentTabIndex].ren->SetBackground(1, 1, 1);
		setAxisColor(false);
	}
	else
	{
		this->qvtkWidgetView[CurrentTabIndex].ren->SetBackground(0, 0, 0);
		setAxisColor(true);
	}
	qvtkWidgetView[CurrentTabIndex].qvtkwidget->GetRenderWindow()->Render();
}
void cellpara3dshow::FaceShow(){	
	if (Lineactor.size() > 0)
	{
		isFaceshow = !isFaceshow;
		for (int i = 0; i < Lineactor[currentStage].size(); i++){
			if (isFaceshow){
				qvtkWidgetView[this->CurrentTabIndex].ren->AddActor(Faceactor[currentStage][i]);
			}
			else{
				qvtkWidgetView[this->CurrentTabIndex].ren->RemoveActor(Faceactor[currentStage][i]);
			}
		}
		qvtkWidgetView[CurrentTabIndex].qvtkwidget->GetRenderWindow()->Render();
	}
}
void cellpara3dshow::CellLineShow(){
	if (Lineactor.size() > 0){
		isLineshow = !isLineshow;
		for (int i = 0; i < Lineactor[currentStage].size(); i++){
			if (isLineshow){
				qvtkWidgetView[this->CurrentTabIndex].ren->AddActor(Lineactor[currentStage][i]);
			}
			else{
				qvtkWidgetView[this->CurrentTabIndex].ren->RemoveActor(Lineactor[currentStage][i]);
			}
		}
		qvtkWidgetView[CurrentTabIndex].qvtkwidget->GetRenderWindow()->Render();
	}
}
void cellpara3dshow::ZscaleAdd(){
	Zscale = Zscale*rate;
	SetScale(1.0, 1.0, Zscale);
}
void cellpara3dshow::ZscaleMinus(){
	Zscale = Zscale/rate;
	SetScale(1.0, 1.0, Zscale);
}

void cellpara3dshow::Propertyset(){
	if (Property.size() > 0){
		PropertySet *SetTab = new PropertySet(Property, PropertyLODs[currentStage]);
		QPoint ptmp;
		SetTab->cps = this;
		ptmp = mapToGlobal(righttoolbar->pos());
		SetTab->move(ptmp.x() - SetTab->width(), ptmp.y());
		SetTab->show();
	}
}
void cellpara3dshow::SetProperty(bool *flag){
	QString list = "PORO PERMEABILITY SOIL FACIES BULKVOLUME DZMTRXV";
	Property.clear();
	Property.append("x"); Property.append("y"); Property.append("z");
	UPropFilter.clear(); USpaceFlag.clear();
	vector<bool> tempProp; vector<bool> tempSpace;
	QStringList keywords = list.split(" ");
	isPropFlag = new bool[keywords.size()];
	memset(isPropFlag,false,sizeof(bool)*keywords.size());
	for (int j = 0; j < keywords.size(); j++)
	{
		tempProp.push_back(false);
		for (int i = 0; i < (int)SGrid.keywords.size(); i++){
			if (keywords[j].toStdString() == SGrid.keywords[i]){
				Property.append(keywords[j]);
				isPropFlag[j] = true;
			}
		}
	}
	int TotalMapLayer = (int)SGrid.Phasemaps.size() + 1;
	//初始化;
	for (int i = 0; i < TotalMapLayer; i++){
		UPropFilter.push_back(tempProp);
	}
	for (int i = 0; i < 3; i++){
		tempSpace.push_back(false);
	}
	for (int i = 0; i < TotalMapLayer; i++){
		USpaceFlag.push_back(tempSpace);
	}
}

void cellpara3dshow::UpdateGridProperty(int idx, QString PropertyTxt){	
	QString list = "x y z PORO PERMEABILITY SOIL FACIES BULKVOLUME DZMTRXV";
	QStringList keywords = list.split(" ");
	for (int j = 0; j < keywords.size(); j++)
	{
		if (PropertyTxt == keywords[j]){
			idx = j;
			break;
		}
	}
	set_grid_prop(idx);
}
void cellpara3dshow::set_grid_prop(int idx){
	ProperyIdx = idx;
	for (int currentStage = 0; currentStage < Faceactor.size(); currentStage++){
		PropertyLODs[currentStage] = idx;
		for (int i = 0; i < Faceactor[currentStage].size(); i++){
			UpdatePropperty(Cellval[currentStage][i], idx, CellZoneDatas[currentStage]);
			Faceactor[currentStage][i]->GetMapper()->GetInput()->GetPointData()->SetScalars(Cellval[currentStage][i]);
			if (idx != 6){
				Faceactor[currentStage][i]->GetMapper()->SetScalarRange(CellValueRange[2][idx].minValue, CellValueRange[2][idx].maxValue);
			}
			else{
				Faceactor[currentStage][i]->GetMapper()->SetScalarRange(CellValueRange[i][idx].minValue, CellValueRange[i][idx].maxValue);
			}
		}
	}
	// scalarBar->SetTitle(keywords[idx].toLocal8Bit().data());
	// SetNumberOfLabels;
	// 连续值设定;
	if (idx == 6){

		// 视需求再修改;
		scalarBar->SetNumberOfLabels(8);
		QVector<QColor> col;

		col.append(QColor(0, 0, 255, 255));
		col.append(QColor(5, 213, 255, 255));
		col.append(QColor(0, 100, 0, 255));
		col.append(QColor(0, 255, 3, 255));
		col.append(QColor(255, 255, 3, 255));
		col.append(QColor(255, 160, 7, 255));
		col.append(QColor(255, 0, 255, 255));  //默认色卡体系
		col.append(QColor(255, 0, 0, 255));  //默认色卡体系

		vtkSmartPointer<vtkColorTransferFunction> lut = vtkSmartPointer<vtkColorTransferFunction>::New();
		for (int i = 0; i < col.length(); i++)
		{
			QColor color = col[i];
			double dColor[3];
			dColor[0] = static_cast<double> (color.red()) / 255.0;
			dColor[1] = static_cast<double> (color.green()) / 255.0;
			dColor[2] = static_cast<double> (color.blue()) / 255.0;
			lut->AddRGBPoint(i, dColor[0], dColor[1], dColor[2]);
		}

		for (int currentStage = 0; currentStage < Faceactor.size(); currentStage++){
			for (int i = 0; i < Faceactor[currentStage].size(); i++){
				Faceactor[currentStage][i]->GetMapper()->SetLookupTable(lut);
			}
		}
		scalarBar->SetLookupTable(lut);
	}
	else{
		for (int currentStage = 0; currentStage < Faceactor.size(); currentStage++){
			for (int i = 0; i < Faceactor[currentStage].size(); i++){
				Faceactor[currentStage][i]->GetMapper()->SetLookupTable(hueLut);
			}
		}
		scalarBar->SetLookupTable(hueLut);
	}
	qvtkWidgetView[CurrentTabIndex].qvtkwidget->GetRenderWindow()->Render();
}

void cellpara3dshow::GetCellId(int FaceId){
	;
}

void cellpara3dshow::setSpaceFilterValue(int Layer, double *Bounds, vector<bool> SpaceFilter){
	query.isSpace = true;
	for (int j = 0; j < 3; j++){
		if (SpaceFilter[j]){
			FilterCellValueRange[Layer][j].minValue = Bounds[2 * j];
			FilterCellValueRange[Layer][j].maxValue = Bounds[2 * j + 1];
		}
	}
}

void cellpara3dshow::setPropertyValue(double *Value, int Layer, vector<bool> PropFilter){
	query.isProperty = true;
	for (int i = 0; i < PropFilter.size(); i++){
		if (PropFilter[i]){
			FilterCellValueRange[Layer][i + 3].minValue = Value[2 * i];
			FilterCellValueRange[Layer][i + 3].maxValue = Value[2 * i + 1];
		}
	}
}

void cellpara3dshow::getCenter(vector<CellRange> cells, double *center){
	for (int i = 0; i < 3; i++){
		center[i] = (cells[i].minValue + cells[i].maxValue) / 2;
	}
	double xmin, xmax, ymin, ymax, zmin, zmax;
	xmin = cells[0].minValue;
	ymin = cells[1].minValue;
	zmin = cells[2].minValue;
	xmax = cells[0].maxValue;
	ymax = cells[1].maxValue;
	zmax = cells[2].maxValue;
	Lxyz = pow(pow(xmax - center[0], 2) + pow(ymax - center[1], 2), 0.5);
	Lxyz = 3.85*Lxyz;
}

void cellpara3dshow::SetTransform(){
	vtkSmartPointer<vtkTransform> transf = vtkSmartPointer<vtkTransform>::New();
	transf->Identity();
	transf->Translate(gridCenter);
	transf->Scale(1.0, 1.0, Zscale);
	cout << "Setting Scale: " << Zscale<<endl;
	transf->Translate(-gridCenter[0], -gridCenter[1], -gridCenter[2]);

	for (int currentStage = 0; currentStage < Lineactor.size(); currentStage++){
		for (int i = 0; i < Lineactor[currentStage].size(); i++){
			Lineactor[currentStage][i]->SetUserTransform(transf);
			Faceactor[currentStage][i]->SetUserTransform(transf);
		}
	}

	double boundtmp[6];
	for (int dd = 0; dd < 3; dd++){
		if (dd == 2){
			double GAPLength = 0;// abs(CellValueRange[2][dd].maxValue - CellValueRange[2][dd].minValue)*0.2;
			boundtmp[2 * dd] = Zscale*(CellValueRange[2][dd].minValue - GAPLength) + (1 - Zscale)*(gridCenter[2]) - GAPLength;
			boundtmp[2 * dd + 1] = Zscale*(CellValueRange[2][dd].maxValue + GAPLength) + (1 - Zscale)*(gridCenter[2]) + GAPLength;
		}
		else{
			boundtmp[2 * dd] = CellValueRange[2][dd].minValue;
			boundtmp[2 * dd + 1] = CellValueRange[2][dd].maxValue;
		}
	}
	AxesActor->SetCamera(qvtkWidgetView[this->CurrentTabIndex].ren->GetActiveCamera());
	AxesActor->SetBounds(boundtmp);
	AxesActor->SetXTitle("X");
	AxesActor->SetYTitle("Y");
	qvtkWidgetView[this->CurrentTabIndex].ren->AddActor(AxesActor);
}

void cellpara3dshow::setAxisColor(bool isWhite){
	double *bcolor = new double[3];
	for (int i = 0; i < 3; i++){
		bcolor[i] = isWhite ? 1.0 : 0.0;
	}
	AxesActor->GetXAxesGridlinesProperty()->SetColor(bcolor);
	AxesActor->GetYAxesGridlinesProperty()->SetColor(bcolor);
	AxesActor->GetZAxesGridlinesProperty()->SetColor(bcolor);
	AxesActor->GetXAxesLinesProperty()->SetColor(bcolor);
	AxesActor->GetYAxesLinesProperty()->SetColor(bcolor);
	AxesActor->GetZAxesLinesProperty()->SetColor(bcolor);
	AxesActor->GetTitleTextProperty(0)->SetColor(bcolor);
	AxesActor->GetLabelTextProperty(0)->SetColor(bcolor);
	AxesActor->GetTitleTextProperty(1)->SetColor(bcolor);
	AxesActor->GetLabelTextProperty(1)->SetColor(bcolor);
	AxesActor->GetTitleTextProperty(2)->SetColor(bcolor);
	AxesActor->GetLabelTextProperty(2)->SetColor(bcolor);

	vtkSmartPointer<vtkTextProperty> titleprop = vtkTextProperty::New();
	titleprop->SetFontSize(1);
	titleprop->SetColor(bcolor);
	scalarBar->SetLabelTextProperty(titleprop);
}

void cellpara3dshow::setFrontView(){
	upsurfaceshow(4);
}
void cellpara3dshow::setBackView(){
	upsurfaceshow(5);
}
void cellpara3dshow::SetTopView(){
	upsurfaceshow(0);
}
void cellpara3dshow::SetBottomView(){
	upsurfaceshow(1);
}
void cellpara3dshow::SetLeftView(){
	upsurfaceshow(2);
}
void cellpara3dshow::SetRightView(){
	upsurfaceshow(3);
}
void cellpara3dshow::set_switch(){
	if (! isSwitch) { // 之前单一LOD 
		isSwitch = !isSwitch;
		ResetView();
	}
	else{
		isSwitch = !isSwitch;
	}
}

void cellpara3dshow::upsurfaceshow(int type) 
{
	
	qvtkWidgetView[this->CurrentTabIndex].ren->GetActiveCamera()->SetFocalPoint(gridCenter[0], gridCenter[1], gridCenter[2]);
	
	double dx,dy,dz;
	dx = 0; dy = 0; dz = 0;
	double x, y, z;
	x = gridCenter[0]; y = gridCenter[1]; z = gridCenter[2];
	switch (type){
	case 0:
		dy = 1;// 上视图;
		z = gridCenter[2] + Lxyz;
		break;
	case 1:
		z = gridCenter[2] - Lxyz;
		dy = -1; // 下视图;
		break;
	case 2:
		x = gridCenter[0] - Lxyz;
		dz = 1; // 左视图;
		break;
	case 3:
		x = gridCenter[0] +  Lxyz;
		dz = -1; // 右视图;
		break;
	case 4:
		y = gridCenter[1] + Lxyz;
		dx = 1; // 正视图;
		break;
	case 5:
		y = gridCenter[1] -  Lxyz;
		dx = -1; // 后视图;
		break;
	default:
		break;
	}
	qvtkWidgetView[this->CurrentTabIndex].ren->GetActiveCamera()->SetPosition(x,y,z);
	qvtkWidgetView[this->CurrentTabIndex].ren->GetActiveCamera()->SetViewUp(dx, dy, dz); // 上视图;
	qvtkWidgetView[this->CurrentTabIndex].ren->UpdateLightsGeometryToFollowCamera();
	qvtkWidgetView[this->CurrentTabIndex].ren->ResetCameraClippingRange();
	qvtkWidgetView[CurrentTabIndex].qvtkwidget->GetRenderWindow()->Render();
};

void cellpara3dshow::get_valid_cellnum(StrucGrid &SGrid, int &valid_cellnum){
	int nx, ny, nz;
	nx = SGrid.NX; ny = SGrid.NY; nz = SGrid.NZ;
	for (int k = 0; k < nz; k++){
		int idx_k = k*nx*ny;
		for (int j = 0; j < ny; j++){
			int idx_j = j*nx;
			for (int i = 0; i < nx; i++){
				int idx = idx_j + idx_k + i;
				// isFixed 取并集; 
				if (SGrid.ACTNUM[idx] ){
					if (SGrid.CPGCells[idx].is_act){
						valid_cellnum = valid_cellnum + 1;
					}
				}
			}
		}
	}
}

bool cellpara3dshow::print_searchInfo(vector<bool> PropFilter, vector<bool> SpaceFlag){
	QString list = "x y z PORO PERMEABILITY SOIL FACIES BULKVOLUME DZMTRXV";
	QStringList keywords = list.split(" ");
	bool isSpaceFlag = false;
	printf("查询条件:\n"); // 输出层次;
	for (int i = 0; i < SpaceFlag.size(); i++){
		if (SpaceFlag[i]){
			isSpaceFlag = true;
			printf("\t 空间查询: %s \n", keywords[i].toStdString().data() );
		}
	}
	for (int i = 0; i < PropFilter.size(); i++){
		if (PropFilter[i]){
			printf("\t 语义查询: %s \n", keywords[i + 3].toStdString().data());
		}
	}
	return isSpaceFlag;
}

