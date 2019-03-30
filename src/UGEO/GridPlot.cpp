#include"Gridplot.h"

namespace UMSM {


	//获取边界面上的Boundary 点和属性
	void GetBouondary(TreeNode *Note, vector<FaceNode> &FaceNodes){
		NeightBor tempNeighbor;
		FaceNode face;
		int LeftBoundary[4] = { 0, 4, 7, 3 };
		int RightBoundary[4] = { 1, 2, 6, 5 };
		int UpBoundary[4] = { 3, 2, 6, 7 };
		int DownBoundary[4] = { 0, 1, 5, 4 };
		int TopBoundary[4] = { 4, 5, 6, 7 };
		int BottomBoundary[4] = { 0, 1, 2, 3 };
		for (int i = 0; i < Note->NumberChild; i++){
			tempNeighbor = Note->childNotes[i]->NodeData.neightbor;
			if (tempNeighbor.left.size() == 0){
				AddFace(Note->childNotes[i]->NodeData, face, LeftBoundary);
				FaceNodes.push_back(face);
			}
			if (tempNeighbor.right.size() == 0){
				AddFace(Note->childNotes[i]->NodeData, face, RightBoundary);
				FaceNodes.push_back(face);
			}
			if (tempNeighbor.up.size() == 0){
				AddFace(Note->childNotes[i]->NodeData, face, UpBoundary);
				FaceNodes.push_back(face);
			}
			if (tempNeighbor.down.size() == 0){
				AddFace(Note->childNotes[i]->NodeData, face, DownBoundary);
				FaceNodes.push_back(face);
			}
			if (tempNeighbor.top.size() == 0){
				AddFace(Note->childNotes[i]->NodeData, face, TopBoundary);
				FaceNodes.push_back(face);
			}
			if (tempNeighbor.Bottom.size() == 0){
				AddFace(Note->childNotes[i]->NodeData, face, BottomBoundary);
				FaceNodes.push_back(face);
			}
		}
	}


	void AddFace(CPGNode note, FaceNode & Face, int *Left){
		for (int i = 0; i < 4; i++){
			Face.Point[i] = note.CPGCells.corner[Left[i]];
		}
		Face.ACTNUM = note.ACTNUM;
		Face.PORO = note.PORO;
		Face.PERMEABILITY = note.PERMEABILITY;
		Face.SOIL = note.SOIL;
		Face.FACIES = note.FACIES;
		Face.DZMTRXV = note.DZMTRXV;
		Face.BULKVOLUME = note.BULKVOLUME;
	}
	void AddFace(CPCell note, FaceNode & Face, int *Left, int ACTNUM, double Poro, double PERMEABILITY, double SOIL, int Facies, double DZMTRXV, double BULKVOLUME){
		for (int i = 0; i < 4; i++){
			Face.Point[i] = note.corner[Left[i]];
		}
		Face.ACTNUM = ACTNUM;
		Face.PORO = Poro;
		Face.PERMEABILITY = PERMEABILITY;
		Face.SOIL = SOIL;
		Face.FACIES = Facies;
		Face.DZMTRXV = DZMTRXV;
		Face.BULKVOLUME = BULKVOLUME;
	}


	void Point2Double(double *P, point P1){
		P[0] = P1.x;
		P[1] = P1.y;
		P[2] = P1.z;
	}

	void DrawGrid(vector<FaceNode> FaceNodes, CellRange cellRange, int idx, vtkSmartPointer<vtkActor> CellLineActor, vtkSmartPointer<vtkActor> ZoneActor, vtkSmartPointer<vtkDoubleArray> val, vtkSmartPointer<vtkLookupTable> hueLut){
		int CellNum = (int)FaceNodes.size();
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		AddPoints(FaceNodes, points);
		DrawSingleCellLine(FaceNodes, points, CellLineActor);
		double *Value = GetFaceValue(FaceNodes, idx);
		DrawGPGGrid(FaceNodes, Value, cellRange.minValue, cellRange.maxValue, ZoneActor, points, val,hueLut);//lut);
	}
	void DrawGrid(CPGNode *Cell, int NumberGrid, vtkSmartPointer<vtkActor> CellLineActor, vtkSmartPointer<vtkActor> ZoneActor, vtkSmartPointer<vtkDoubleArray> val, vtkSmartPointer<vtkLookupTable> lut){
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		int CellNum = AddPoints(Cell, NumberGrid, points);
		DrawSingleCellLine(CellNum, CellLineActor, points);
		double *Value = GetFaceValue(Cell, CellNum, NumberGrid, 0);
		DrawGPGGrid(CellNum, Value, 2231.4099999999999, 2651.9099999999999, ZoneActor, points, val, lut);
	}

	void AddPoints(vector<FaceNode> FaceNodes, vtkSmartPointer<vtkPoints> points){
		int CellNum = (int)FaceNodes.size();
		int Length = 4;
		double *p3 = new double[3];
		for (int i = 0; i < CellNum; i++)
		{
			for (int j = 0; j < Length; j++)
			{
				Point2Double(p3, FaceNodes[i].Point[j]);
				points->InsertNextPoint(p3);
			}
		}
	}
	int AddPoints(CPGNode *Cell, int NumberGrid, vtkSmartPointer<vtkPoints> points){
		int Length = 8;
		int CellNum = 0;
		double *p3 = new double[3];
		for (int i = 0; i < NumberGrid; i++)
		{
			if (Cell[i].CPGCells.is_act){
				CellNum++;
				for (int j = 0; j < 8; j++){
					Point2Double(p3, Cell[i].CPGCells.corner[j]);
					points->InsertNextPoint(p3);
				}
			}
		}
		return CellNum;
	}

	void DrawSingleCellLine(vector<FaceNode> FaceNodes, vtkSmartPointer<vtkPoints> points, vtkSmartPointer<vtkActor> LineActor)
	{
		// 画线;	
		int CellNum = (int)FaceNodes.size();
		int Length = 4;
		vtkIdType templine[2];
		// 定义line  points cells 
		vtkSmartPointer<vtkCellArray> linepolys = vtkSmartPointer<vtkCellArray>::New();
		vtkSmartPointer<vtkPolyData> linepolydata = vtkSmartPointer<vtkPolyData>::New();
		vtkSmartPointer<vtkPolyDataMapper> linemapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		/////////////// 			
		// 线段的索引;
		int m = 0;
		for (int i = 0; i < CellNum; i++)
		{
			//m = 4 * i;
			for (int j = 0; j < Length; j++)
			{
				templine[0] = m + j;
				templine[1] = m + (j + 1) % 4;
				linepolys->InsertNextCell(2, templine);
			}
			m = m + 4;
		};
		//  设置 LINE
		linepolydata->SetPoints(points); //几何结构
		linepolydata->SetLines(linepolys); //拓扑结构
		linemapper->SetInputData(linepolydata); //导入数据
		LineActor->SetMapper(linemapper);
		LineActor->GetProperty()->SetColor(0.0, 0.0, 0.0);
		LineActor->GetProperty()->SetLineWidth(2);
		LineActor->SetPickable(false);
		//////////////////// 删除数据;
	}
	void DrawSingleCellLine(int NumberGrid, vtkSmartPointer<vtkActor> celllineactor, vtkSmartPointer<vtkPoints> points)
	{
		// Cell 线;
		vtkIdType templine2[2];
		// 定义line  points cells		
		vtkSmartPointer<vtkCellArray> linepolys = vtkSmartPointer<vtkCellArray>::New();
		vtkSmartPointer<vtkPolyData> linepolydata = vtkSmartPointer<vtkPolyData>::New();
		vtkSmartPointer<vtkPolyDataMapper> linemapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		///////////////
		int index = 0;
		// 添加索引
		for (int i = 0; i < NumberGrid; i++)
		{
			index = i * 8;
			for (int j = 0; j < 4; j++){
				templine2[0] = index + j; // 
				templine2[1] = index + (j + 1) % 4;
				linepolys->InsertNextCell(2, templine2);
				templine2[0] = index + 4 + j; // 
				templine2[1] = index + 4 + (j + 1) % 4;
				linepolys->InsertNextCell(2, templine2);
				templine2[0] = index + j; // 
				templine2[1] = index + 4 + j;
				linepolys->InsertNextCell(2, templine2);
			}
		}
		//  设置 LINE
		linepolydata->SetPoints(points); //几何结构
		linepolydata->SetLines(linepolys); //拓扑结构
		linemapper->SetInputData(linepolydata); //导入数据
		celllineactor->SetMapper(linemapper);
		celllineactor->GetProperty()->SetColor(0.0, 0.0, 0.0);
		celllineactor->GetProperty()->SetLineWidth(1);
		celllineactor->SetPickable(false);
		//////////////////// 删除数据;
	}

	double *GetFaceValue(vector<FaceNode> FaceNodes, int Type){
		int CellNum = (int)FaceNodes.size();
		double *Value;
		Value = new double[CellNum * 4];
		int idx = 0;
		for (int i = 0; i < CellNum; i++){
			for (int j = 0; j < 4; j++){
				switch (Type)
				{
				case 0:
					Value[idx++] = FaceNodes[i].Point[j].x;
					break;
				case 1:
					Value[idx++] = FaceNodes[i].Point[j].y;
					break;
				case 2:
					Value[idx++] = FaceNodes[i].Point[j].z;
					break;
				case 3:
					Value[idx++] = FaceNodes[i].PORO;
					break;
				case 4:
					Value[idx++] = FaceNodes[i].PERMEABILITY;
					break;
				case 5:
					Value[idx++] = FaceNodes[i].SOIL;
					break;
				case 6:
					Value[idx++] = FaceNodes[i].FACIES; // 颜色颠倒
					break;
				case 7:
					Value[idx++] = FaceNodes[i].BULKVOLUME;
					break;
				case 8:
					Value[idx++] = FaceNodes[i].DZMTRXV;
					break;
				}
			}
		}
		return Value;
	}
	double *GetFaceValue(CPGNode *Cell, int CellNum, int NumberGrid, int Type){
		int Length = 8;
		double *Value = new double[CellNum * Length];
		CellNum = 0;
		int idx = 0;
		double *p3 = new double[3];
		for (int i = 0; i < NumberGrid; i++)
		{
			idx = i * 8;
			//if (Cell[i].CPGCells.is_act){
			CellNum++;
			for (int j = 0; j < 8; j++){
				Point2Double(p3, Cell[i].CPGCells.corner[j]);
				Value[idx + j] = p3[2];
			}
			//}
		}
		return Value;
	}


	// 画面; Model3DViewData没用
	void DrawGPGGrid(vector<FaceNode> FaceNodes, double *CellValue, double zmin, double zmax, vtkSmartPointer<vtkActor> SingleGridActor, vtkSmartPointer<vtkPoints> points, vtkSmartPointer<vtkDoubleArray> val, vtkSmartPointer<vtkLookupTable> hueLut) //,vtkSmartPointer<vtkColorTransferFunction> lut) // draw GPG网格
	{
		int NumberGrid = (int)FaceNodes.size();
		if (NumberGrid > 0){
			//定义polygon  points cells
			vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
			vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
			vtkSmartPointer<vtkPolyDataMapper> ploygonMapper = vtkSmartPointer<vtkPolyDataMapper>::New(); 
			//////////////// 插入点;			
			for (int i = 0; i < NumberGrid; i++)
			{
				int index = i * 4;
				cells->InsertNextCell(4);
				for (int j = 0; j < 4; j++)
				{
					cells->InsertCellPoint(index + j); // 上下顶面					
					val->InsertNextValue(CellValue[index + j]);
				}
			}
			/// polygon setting
			polydata->SetPoints(points);
			polydata->SetPolys(cells);
			polydata->GetPointData()->SetScalars(val);
			ploygonMapper->SetInputData(polydata);
			// ploygon  Actor
			//lut->SetColorSpaceToHSV();
			//ploygonMapper->SetLookupTable(lut);
			//double zmin, zmax;
			//zmin = zmax = CellValue[0];
			//for (int i = 0; i < NumberGrid; i++){
			//	zmin = zmin < CellValue[i] ? zmin : CellValue[i];
			//	zmax = zmax > CellValue[i] ? zmax : CellValue[i];
			//}			
			ploygonMapper->SetScalarRange(zmin, zmax);
			ploygonMapper->SetLookupTable(hueLut);
			SingleGridActor->SetMapper(ploygonMapper);

			SingleGridActor->GetProperty()->SetAmbient(0.8);
			SingleGridActor->GetProperty()->SetDiffuse(0.2);
			SingleGridActor->GetProperty()->SetSpecular(0.0);
		}
	}
	void DrawGPGGrid(int NumberGrid, double *CellValue, double zmin, double zmax, vtkSmartPointer<vtkActor> SingleGridActor, vtkSmartPointer<vtkPoints> points, vtkSmartPointer<vtkDoubleArray> val, vtkSmartPointer<vtkLookupTable> hueLut)//, vtkSmartPointer<vtkColorTransferFunction> lut) // draw GPG网格
	{
		if (NumberGrid > 0){
			//定义polygon  points cells
			vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
			vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
			vtkSmartPointer<vtkPolyDataMapper> ploygonMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			//////////////// 插入点;			
			int tempIdx, idx = 0;
			int Side[4] = { 0, 1, 1, 0 };
			for (int i = 0; i < NumberGrid; i++)
			{
				int index = i * 8;
				for (int j = 0; j < 8; j++){
					val->InsertNextValue(CellValue[index + j]);
				}
				for (int m = 0; m < 2; m++){
					cells->InsertNextCell(4);
					for (int j = 0; j < 4; j++)
					{
						tempIdx = index + j + 4 * m;
						cells->InsertCellPoint(tempIdx); // 上下顶面						
					}
				}
				for (int j = 0; j < 4; j++)
				{
					cells->InsertNextCell(4);
					cells->InsertCellPoint(index + j);
					cells->InsertCellPoint(index + (j + 1) % 4);
					cells->InsertCellPoint(index + (j + 1) % 4 + 4);
					cells->InsertCellPoint(index + j + 4);
				}
			}
			/// polygon setting
			polydata->SetPoints(points);
			polydata->SetPolys(cells);
			polydata->GetPointData()->SetScalars(val);
			ploygonMapper->SetInputData(polydata);

			// ploygon  Actor
			//lut->SetColorSpaceToHSV();
			//ploygonMapper->SetLookupTable(lut);
			//// model3data 暂时没用
			//for (int i = 0; i < model3data.col.length(); i++)
			//{
			//	QColor color = model3data.col[i];
			//	double dColor[3];
			//	dColor[0] = static_cast<double> (color.red()) / 255.0;
			//	dColor[1] = static_cast<double> (color.green()) / 255.0;
			//	dColor[2] = static_cast<double> (color.blue()) / 255.0;
			//	lut->AddRGBPoint(model3data.colvalue[i], dColor[0], dColor[1], dColor[2]);
			//}
			//double zmin, zmax;
			//zmin = zmax = CellValue[0];
			//for (int i = 0; i < NumberGrid; i++){
			//	zmin = zmin < CellValue[i] ? zmin : CellValue[i];
			//	zmax = zmax > CellValue[i] ? zmax : CellValue[i];
			//}
			//ploygonMapper->SetLookupTable(lut);
			ploygonMapper->SetScalarRange(zmin, zmax);
			ploygonMapper->SetLookupTable(hueLut);
			SingleGridActor->SetMapper(ploygonMapper);

			SingleGridActor->GetProperty()->SetAmbient(0.8);
			SingleGridActor->GetProperty()->SetDiffuse(0.2);
			SingleGridActor->GetProperty()->SetSpecular(0.0);
		}
	}

	void getKrange(int kmin, int kmax, PhaseMap Layers, int &k2, int &k1){
		k1 = 0;
		k2 = 0;
		for (int i = 0; i < Layers.TopLayers.size(); i++){
			if (Layers.TopLayers[i] > kmin){
				break;
			}
		}
		k2 = k1;
		for (int i = k1; i < Layers.TopLayers.size(); i++){
			if (Layers.BottomLayers[i] < kmax){
				break;
			}
			k2 = i;
		}
	}

	void SpaceFilter(StrucGrid &SGrid, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, vector<bool> SpaceFlag){
		if (SpaceFlag[0] || SpaceFlag[1] || SpaceFlag[2]){
			double Bounds[6];
			int nx, ny, nz;
			nx = SGrid.NX; ny = SGrid.NY; nz = SGrid.NZ;
			kmin = kmin - 1; kmax = kmax - 1;
			for (int k = 0; k < nz; k++){
				int idx_k = k*nx*ny;
				if (k >= kmin&&k <= kmax){
					if (SpaceFlag[0] || SpaceFlag[1]){
						for (int j = 0; j < ny; j++){
							int idx_j = j*nx;
							for (int i = 0; i < nx; i++){
								int idx = idx_j + idx_k + i;
								if (SGrid.ACTNUM[idx] == 1){
									GetCornerBounds(SGrid.CPGCells[idx].corner[0], SGrid.CPGCells[idx].corner[7], Bounds);
									SGrid.CPGCells[idx].is_act = true;
									if (SpaceFlag[0]){
										if (Bounds[0] > xmax || Bounds[1]<xmin){
											SGrid.CPGCells[idx].is_act = false;
											continue;
										}
									}
									if (SpaceFlag[1]){
										if (Bounds[2] > ymax || Bounds[3] < ymin){
											SGrid.CPGCells[idx].is_act = false;
											continue;
										}
									}
								}
							}
						}
					}
				}
				else{
					for (int j = 0; j < ny; j++){
						int idx_j = j*nx;
						for (int i = 0; i < nx; i++){
							int idx = idx_j + idx_k + i;
							if (SGrid.ACTNUM[idx] == 1 && SGrid.CPGCells[idx].is_act){
								SGrid.CPGCells[idx].is_act = false;
							}
						}
					}
				}
			}
		}
	}


	void SpaceFilter2(int Layer, TreeNode *Nodes, StrucGrid &SGrid, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, vector<bool> SpaceFlag){
		if (SpaceFlag[0] || SpaceFlag[1] || SpaceFlag[2]){
			int nx, ny, nz;
			xmin = xmin - (1E-4); xmax = xmax + (1E-4);
			ymin = ymin - (1E-4); ymax = ymax + (1E-4);
			nx = SGrid.NX; ny = SGrid.NY; nz = SGrid.NZ;
			kmin = kmin - 1; kmax = kmax - 1;
			kmin = SGrid.Phasemaps[Layer].TopLayers[kmin] - 1;
			kmax = SGrid.Phasemaps[Layer].BottomLayers[kmax] - 1;
			for (int i = 0; i < Nodes->NumberChild; i++){
				Nodes->childNotes[i]->NodeData.isActive = false;
				if (!JudgeSpaceFilter(Nodes->childNotes[i]->NodeData, SGrid, nx, ny, nz, xmin, xmax, ymin, ymax, kmin, kmax, SpaceFlag, false)){
					Nodes->childNotes[i]->NodeData.isActive = true;
					for (int j = 0; j < Nodes->childNotes[i]->NumberChild; j++){
						//Nodes->childNotes[i]->childNotes[j]->NodeData.isActive = false;
						JudgeSpaceFilter(Nodes->childNotes[i]->childNotes[j]->NodeData, SGrid, nx, ny, nz, xmin, xmax, ymin, ymax, kmin, kmax, SpaceFlag, false);
					}
				}
			}
		}
	}

	void SpaceFilter3(int Layer, TreeNode *Nodes, StrucGrid &SGrid, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, vector<bool> SpaceFlag){
		if (SpaceFlag[0] || SpaceFlag[1] || SpaceFlag[2]){
			int nx, ny, nz;
			xmin = xmin - (1E-4); xmax = xmax + (1E-4);
			ymin = ymin - (1E-4); ymax = ymax + (1E-4);
			nx = SGrid.NX; ny = SGrid.NY; nz = SGrid.NZ;
			kmin = kmin - 1; kmax = kmax - 1;
			BGGindex tempIdx;
			CPGNode tempNode;
			for (int i = 0; i < Nodes->NumberChild; i++){
				if (!JudgeSpaceFilter(Nodes->childNotes[i]->NodeData, SGrid, nx, ny, nz, xmin, xmax, ymin, ymax, kmin, kmax, SpaceFlag, true)){
					for (int j = 0; j < Nodes->childNotes[i]->NumberChild; j++){
						if (!JudgeSpaceFilter(Nodes->childNotes[i]->childNotes[j]->NodeData, SGrid, nx, ny, nz, xmin, xmax, ymin, ymax, kmin, kmax, SpaceFlag, true)){
							tempNode = Nodes->childNotes[i]->childNotes[j]->NodeData;
							tempIdx.i = tempNode.IIndex;
							tempIdx.j = tempNode.JIndex;
							tempIdx.k = tempNode.KIndex;
							for (int k1 = 0; k1 < tempNode.dx; k1++){
								for (int k2 = 0; k2 < tempNode.dy; k2++){
									for (int k3 = 0; k3 < tempNode.dz; k3++){
										int idx = tempIdx.i + k1 + (tempIdx.j + k2)*nx + (tempIdx.k + k3)*nx*ny;
										JudgeSpaceFilter(SGrid, idx, (tempIdx.k + k3), nx, ny, nz, xmin, xmax, ymin, ymax, kmin, kmax, SpaceFlag);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	bool JudgeSpaceFilter(CPGNode &NodeData, StrucGrid &SGrid, int nx, int ny, int nz, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, vector<bool> SpaceFlag, bool isOriginal){
		double Bounds[6];
		bool isActive = true;
		BGGindex id_k;
		id_k.i = NodeData.IIndex;
		id_k.j = NodeData.JIndex;
		id_k.k = NodeData.KIndex;
		NodeData.isActive = false;
		if (SpaceFlag[2])
		{
			if ((id_k.k + NodeData.dz - 1) < kmin || id_k.k >kmax){
				if (isOriginal)
				{
					SetFalseGridData(SGrid, id_k.i, id_k.j, id_k.k, NodeData.dx, NodeData.dy, NodeData.dz, nx, ny, nz);
				}
				NodeData.isActive = false;
				return true;
			}
			else{
				if (id_k.k >= kmin && (id_k.k + NodeData.dz - 1) <= kmax){
					;
				}
				else{
					isActive = false;
				}
			}
		}
		if (SpaceFlag[0] || SpaceFlag[1]){
			GetCornerBounds(NodeData.CPGCells.corner[0], NodeData.CPGCells.corner[7], Bounds);
			if (SpaceFlag[0]){
				if (Bounds[0] > xmax || Bounds[1] < xmin){
					NodeData.isActive = false;
					return true;
				}
				else if (Bounds[1] <= xmax && Bounds[0] >= xmin){
					;
				}
				else{
					isActive = false;
				}
			}
			if (SpaceFlag[1]){
				if (Bounds[2] > ymax || Bounds[3] < ymin){
					NodeData.isActive = false;
					return true;
				}
				else if (Bounds[3] <= ymax && Bounds[2] >= ymin){
					;
				}
				else{
					isActive = false;
				}
			}
		};
		if (isActive){
			if (!isOriginal)
			{
				//SetGridData(SGrid, id_k.i, id_k.j, id_k.k, NodeData.dx, NodeData.dy, NodeData.dz, nx, ny, nz);
				NodeData.isActive = true;
			}
			return true;
		}
		return isActive;
	};

	
	void setType(int Layer, TreeNode *Nodes, int depth, StrucGrid &SGrid,int &valid_cellnum){
		if (depth == Layer){
			for (int i = 0; i < Nodes->NumberChild; i++){
				if (Nodes->childNotes[i]->NodeData.isActive){
					for (int j = 0; j < Nodes->childNotes[i]->NumberChild; j++){
						//TreeNode *tempnode =NULL;
						//tempnode = Nodes->childNotes[i]->childNotes[j];
						if ( Nodes->childNotes[i]->childNotes[j]->NodeData.isActive){
							// 计数有效显示网格;
							valid_cellnum = valid_cellnum + 1;
							SetGridData(SGrid, Nodes->childNotes[i]->childNotes[j]->NodeData.IIndex, Nodes->childNotes[i]->childNotes[j]->NodeData.JIndex, Nodes->childNotes[i]->childNotes[j]->NodeData.KIndex, 
								Nodes->childNotes[i]->childNotes[j]->NodeData.dx, Nodes->childNotes[i]->childNotes[j]->NodeData.dy, Nodes->childNotes[i]->childNotes[j]->NodeData.dz, SGrid.NX, SGrid.NY, SGrid.NZ);
						}
						else{
							SetFalseGridData(SGrid, Nodes->childNotes[i]->childNotes[j]->NodeData.IIndex, Nodes->childNotes[i]->childNotes[j]->NodeData.JIndex, Nodes->childNotes[i]->childNotes[j]->NodeData.KIndex, 
								Nodes->childNotes[i]->childNotes[j]->NodeData.dx, Nodes->childNotes[i]->childNotes[j]->NodeData.dy, Nodes->childNotes[i]->childNotes[j]->NodeData.dz, SGrid.NX, SGrid.NY, SGrid.NZ);
						}
					}
				}
				else{
					for (int j = 0; j < Nodes->childNotes[i]->NumberChild; j++){
						//TreeNode *tempnode = NULL;
						//tempnode = Nodes->childNotes[i]->childNotes[j];
						SetFalseGridData(SGrid, Nodes->childNotes[i]->childNotes[j]->NodeData.IIndex, Nodes->childNotes[i]->childNotes[j]->NodeData.JIndex, Nodes->childNotes[i]->childNotes[j]->NodeData.KIndex, 
							Nodes->childNotes[i]->childNotes[j]->NodeData.dx, Nodes->childNotes[i]->childNotes[j]->NodeData.dy, Nodes->childNotes[i]->childNotes[j]->NodeData.dz, SGrid.NX, SGrid.NY, SGrid.NZ);
					}
				}
			}
		}
		else{
			for (int i = 0; i < Nodes->NumberChild; i++){
				if (Nodes->childNotes[i]->NodeData.isActive){
					for (int j = 0; j < Nodes->childNotes[i]->NumberChild; j++){
						if (Nodes->childNotes[i]->childNotes[j]->NodeData.isActive){
							setType(Layer, Nodes->childNotes[i]->childNotes[j], depth + 1, SGrid, valid_cellnum);
						}
						else{
							//TreeNode *tempnode;
							//tempnode = Nodes->childNotes[i]->childNotes[j];
							SetFalseGridData(SGrid, Nodes->childNotes[i]->childNotes[j]->NodeData.IIndex, Nodes->childNotes[i]->childNotes[j]->NodeData.JIndex, Nodes->childNotes[i]->childNotes[j]->NodeData.KIndex, 
								Nodes->childNotes[i]->childNotes[j]->NodeData.dx, Nodes->childNotes[i]->childNotes[j]->NodeData.dy, Nodes->childNotes[i]->childNotes[j]->NodeData.dz, SGrid.NX, SGrid.NY, SGrid.NZ);
						}
					}
				}
				else{
					SetFalseGridData(SGrid, Nodes->childNotes[i]->NodeData.IIndex, Nodes->childNotes[i]->NodeData.JIndex, Nodes->childNotes[i]->NodeData.KIndex,
						Nodes->childNotes[i]->NodeData.dx, Nodes->childNotes[i]->NodeData.dy, Nodes->childNotes[i]->NodeData.dz, SGrid.NX, SGrid.NY, SGrid.NZ);
				}
			}
		}
	}

	void Filter(StrucGrid &SGrid, TreeNode *Nodes, int depth){
		int nx, ny, nz;
		nx = SGrid.NX; ny = SGrid.NY; nz = SGrid.NZ;
		for (int i = 0; i < Nodes->NumberChild; i++){
			if (depth == 0){
				if (Nodes->childNotes[i]->NodeData.isActive){
					BGGindex id_k;
					id_k.i = Nodes->childNotes[i]->NodeData.IIndex;
					id_k.j = Nodes->childNotes[i]->NodeData.JIndex;
					id_k.k = Nodes->childNotes[i]->NodeData.KIndex;
					SetGridData(SGrid, id_k.i, id_k.j, id_k.k, Nodes->childNotes[i]->NodeData.dx, Nodes->childNotes[i]->NodeData.dy, Nodes->childNotes[i]->NodeData.dz, nx, ny, nz);
				}
			}
			else{
				for (int j = 0; j < Nodes->childNotes[i]->NumberChild; j++){
					if (Nodes->childNotes[i]->childNotes[j]->NodeData.isActive){
						BGGindex id_k;
						id_k.i = Nodes->childNotes[i]->childNotes[j]->NodeData.IIndex;
						id_k.j = Nodes->childNotes[i]->childNotes[j]->NodeData.JIndex;
						id_k.k = Nodes->childNotes[i]->childNotes[j]->NodeData.KIndex;
						SetGridData(SGrid, id_k.i, id_k.j, id_k.k, Nodes->childNotes[i]->childNotes[j]->NodeData.dx, Nodes->childNotes[i]->childNotes[j]->NodeData.dy, Nodes->childNotes[i]->childNotes[j]->NodeData.dz, nx, ny, nz);
					}
				}
			}
		}
	}

	void ResetActView(StrucGrid &SGrid, bool Value){
		int nx, ny, nz;
		nx = SGrid.NX; ny = SGrid.NY; nz = SGrid.NZ;
		for (int k = 0; k < nz; k++){
			int idx_k = k*nx*ny;
			for (int j = 0; j < ny; j++){
				int idx_j = j*nx;
				for (int i = 0; i < nx; i++){
					int idx = idx_j + idx_k + i;
					if (SGrid.ACTNUM[idx]){
						SGrid.CPGCells[idx].is_act = Value;
					}
				}
			}
		}
	}


	// 空间查询;
	void updateSpace(int depth, int Layer, TreeNode *Nodes, StrucGrid &SGrid, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, vector<bool> SpaceFlag){
		int *LayerTop = new int[3];
		int *LayerBottom = new int[3];
		int layer_kmin, layer_kmax;
		int temp_kmin, temp_kmax;
		int TotalLayer = (int)SGrid.Phasemaps.size();
		if (Layer < TotalLayer){
			layer_kmin = SGrid.Phasemaps[Layer].TopLayers[kmin] - 1;
			layer_kmax = SGrid.Phasemaps[Layer].BottomLayers[kmax] - 1;
			LayerTop[2] = layer_kmin;
			LayerBottom[2] = layer_kmax;
		}
		else{
			layer_kmin = kmin;
			layer_kmax = kmax;
			LayerTop[2] = kmin; LayerBottom[2] = kmax;
		}
		// layer_kmin
		for (int i = 0; i < 2; i++){
			if (i == Layer){
				LayerTop[i] = kmin;
				LayerBottom[i] = kmax;
			}
			else{
				getLayer2(SGrid.Phasemaps[i].TopLayers, SGrid.Phasemaps[i].BottomLayers, layer_kmin + 1, layer_kmax + 1, temp_kmin, temp_kmax);
				LayerTop[i] = temp_kmin;
				LayerBottom[i] = temp_kmax;
			}
		}
		xmin = xmin - (1E-6); xmax = xmax + (1E-6);
		ymin = ymin - (1E-6); ymax = ymax + (1E-6);
		printf("4-1\n");
		UpdateSpaceNodes(depth, Layer, TotalLayer, Nodes, SGrid, xmin, xmax, ymin, ymax, LayerTop, LayerBottom, SpaceFlag);
		printf("4-2\n");
	}
	// 迭代查询空间信息
	void UpdateSpaceNodes(int depth, int Layer, int TotalLayer, TreeNode *Nodes, StrucGrid &SGrid, double xmin, double xmax, double ymin, double ymax, int *kmin, int *kmax, vector<bool> SpaceFlag){
		/*
			LOD 1 空间查询; LOD1 更新; 终止;
			LOD 2 空间查询; LOD1 更新;-> LOD2 更新;更新截止;
			LOD 3 空间查询; LOD1 更新;-> LOD2 更新-> L0D3 ; 更新截止;
			*/
		vector<bool>  tempFlag;
		if (depth < Layer){
			// 如果层位更新 获取对应的kmin,kmax;
			for (int i = 0; i < Nodes->NumberChild; i++){  // Root
				Nodes->childNotes[i]->NodeData.isActive = true;
				if (SpaceFlag[2]){
					if (Nodes->childNotes[i]->NodeData.KIndex >= kmin[depth] && Nodes->childNotes[i]->NodeData.KIndex <= kmax[depth]){
						SpaceFilter(depth, Layer, TotalLayer, Nodes->childNotes[i], SGrid, xmin, xmax, ymin, ymax, kmin[2], kmax[2], kmin, kmax, SpaceFlag);
					}
					else
					{
						Nodes->childNotes[i]->NodeData.isActive = false;
						if (Layer == TotalLayer){
							//if (depth == TotalLayer - 1){
							//	SetFalseGridData(SGrid, Nodes->childNotes[i]->NodeData.IIndex, Nodes->childNotes[i]->NodeData.JIndex, Nodes->childNotes[i]->NodeData.KIndex,
							//		Nodes->childNotes[i]->NodeData.dx, Nodes->childNotes[i]->NodeData.dy, Nodes->childNotes[i]->NodeData.dz, SGrid.NX, SGrid.NY, SGrid.NZ);
							//}
							//else{
							for (int j = 0; j < Nodes->childNotes[i]->NumberChild; j++){
								TreeNode * tempNode = Nodes->childNotes[i]->childNotes[j];
								SetFalseGridData(SGrid, tempNode->NodeData.IIndex, tempNode->NodeData.JIndex, tempNode->NodeData.KIndex,
									tempNode->NodeData.dx, tempNode->NodeData.dy, tempNode->NodeData.dz, SGrid.NX, SGrid.NY, SGrid.NZ);
							}
						}
					}
				}
				else{
					SpaceFilter(depth, Layer, TotalLayer, Nodes->childNotes[i], SGrid, xmin, xmax, ymin, ymax, kmin[2], kmax[2], kmin, kmax, SpaceFlag);
				}
			}
		}
		else{
			if (depth < TotalLayer){
				tempFlag = SpaceFlag;
				// 如果层位更新 获取对应的kmin,kmax;
				for (int i = 0; i < Nodes->NumberChild; i++){
					if (SpaceFlag[2]){
						tempFlag[2] = false; // false; 
						if (Nodes->childNotes[i]->NodeData.KIndex >= kmin[depth] && Nodes->childNotes[i]->NodeData.KIndex <= kmax[depth]){
							// Space
							Nodes->childNotes[i]->NodeData.isActive = true;
							SpaceFilter(depth, Layer, TotalLayer, Nodes->childNotes[i], SGrid, xmin, xmax, ymin, ymax, kmin[2], kmax[2], kmin, kmax, tempFlag);
						}
						else{
							Nodes->childNotes[i]->NodeData.isActive = false;
						}
					}
					else{
						Nodes->childNotes[i]->NodeData.isActive = true;
						SpaceFilter(depth, Layer, TotalLayer, Nodes->childNotes[i], SGrid, xmin, xmax, ymin, ymax, kmin[2], kmax[2], kmin, kmax, SpaceFlag);
					}
				}
			}
			else{
				SetNodeGridSpace(Nodes, SGrid, SGrid.NX, SGrid.NY, SGrid.NZ, xmin, xmax, ymin, ymax, kmin[2], kmax[2], SpaceFlag);
			}
			//if (Layer == TotalLayer){
				// UpdateSpaceNode(SGrid, depth + 1, Layer, TotalLayer, Nodes->childNotes[i], Nodes->childNotes[i]->NodeData.isActive);
				// SetNodeGridSpace( Nodes->childNotes[i], SGrid, SGrid.NX, SGrid.NY, SGrid.NZ, xmin, xmax, ymin, ymax, kmin, kmax, SpaceFlag);
			//}
		}
	}

	void UpdateGridNode(TreeNode *Nodes, bool isValue){
		// 如果层位更新 获取对应的kmin,kmax;
		for (int i = 0; i < Nodes->NumberChild; i++){
			Nodes->childNotes[i]->NodeData.isActive = isValue;
			for (int j = 0; j < Nodes->childNotes[i]->NumberChild; j++){
				Nodes->childNotes[i]->childNotes[j]->NodeData.isActive = isValue;
			}
		}
	}

	void SpaceFilter(int depth, int Layer, int TotalLayer, TreeNode *Nodes, StrucGrid &SGrid, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, int *Layerkmin, int *Layerkmax, vector<bool> SpaceFlag){
		for (int i = 0; i < Nodes->NumberChild; i++){
			BGGindex id_k;
			id_k.i = Nodes->childNotes[i]->NodeData.IIndex;
			id_k.j = Nodes->childNotes[i]->NodeData.JIndex;
			id_k.k = Nodes->childNotes[i]->NodeData.KIndex;
			if (SetTreeNode(depth, Layer, TotalLayer, xmin, xmax, ymin, ymax, kmin, kmax, id_k, Nodes->childNotes[i], SpaceFlag)){
				if (depth < Layer){
					// 迭代;
					if (depth < TotalLayer){
						//for (int j = 0; j < Nodes->childNotes[i]->NumberChild; j++){
							UpdateSpaceNodes(depth + 1, Layer, TotalLayer, Nodes->childNotes[i], SGrid, xmin, xmax, ymin, ymax, Layerkmin, Layerkmax, SpaceFlag);
						//}
						// SpaceFilter(depth + 1, Layer, TotalLayer, Nodes->childNotes[i], SGrid, xmin, xmax, ymin, ymax, kmin, kmax, SpaceFlag);
						// X.Y 范围之内的 未筛选;  [5,10]范围[1，6]未筛选掉;
					}
					else{
						// SGrid 最底层时; 跳转至筛选;
						SetNodeGridSpace(Nodes->childNotes[i], SGrid, SGrid.NX, SGrid.NY, SGrid.NZ, xmin, xmax, ymin, ymax, kmin, kmax, SpaceFlag);
					}
				}
				//else{
					//if (Layer == TotalLayer){
						//SetNodeGridSpace(Nodes->childNotes[i], SGrid, SGrid.NX, SGrid.NY, SGrid.NZ, xmin, xmax, ymin, ymax, kmin, kmax, SpaceFlag);
					//}
				//}
			}
			else{
				// 全部满足或者不满足;
				if (Layer == TotalLayer){
					TreeNode * tempNode = Nodes->childNotes[i];
					if (tempNode->NodeData.isActive){
						SetGridData(SGrid, tempNode->NodeData.IIndex, tempNode->NodeData.JIndex, tempNode->NodeData.KIndex, tempNode->NodeData.dx, tempNode->NodeData.dy, tempNode->NodeData.dz, SGrid.NX, SGrid.NY, SGrid.NZ);
					}
					else{
						SetFalseGridData(SGrid, tempNode->NodeData.IIndex, tempNode->NodeData.JIndex, tempNode->NodeData.KIndex, tempNode->NodeData.dx, tempNode->NodeData.dy, tempNode->NodeData.dz, SGrid.NX, SGrid.NY, SGrid.NZ);
					};
					// UpdateSpaceNode(SGrid, depth + 1, Layer, TotalLayer, Nodes->childNotes[i], Nodes->childNotes[i]->NodeData.isActive);
				}
				else if (depth < Layer){
					UpdateGridNode(Nodes->childNotes[i], Nodes->childNotes[i]->NodeData.isActive);
				}
			}
		}
	}
	
	bool SetTreeNode(int depth, int Layer, int TotalLayer, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, BGGindex id_k, TreeNode *Nodes, vector<bool> SpaceFlag){
		double Bounds[6];
		bool isPartActive = false;
		if (SpaceFlag[2])
		{
			if ( (id_k.k + Nodes->NodeData.dz - 1) < kmin || id_k.k > kmax){
				Nodes->NodeData.isActive = false;
				return false;
			}
			else{
				if (id_k.k >= kmin && (id_k.k + Nodes->NodeData.dz - 1) <= kmax){
					; //完全满足; 不返回; 继续判断 X,Y;
				}
				else{
					isPartActive = true;
				}
			}
		}
		if (SpaceFlag[0] || SpaceFlag[1]){
			GetCornerBounds(Nodes->NodeData.CPGCells.corner[0], Nodes->NodeData.CPGCells.corner[7], Bounds);
			if (SpaceFlag[0]){
				if (Bounds[0] > xmax || Bounds[1] < xmin){
					Nodes->NodeData.isActive = false;
					return false;
				}
				else{
					if (Bounds[0] >= xmin&& Bounds[1] <= xmax){
						;
					}
					else{
						isPartActive = true;
					}
				}
			}
			if (SpaceFlag[1]){
				if (Bounds[2] > ymax || Bounds[3] < ymin){
					Nodes->NodeData.isActive = false;
					return false;
				}
				else{
					if (Bounds[2] >= ymin&& Bounds[3] <= ymax){
						;
					}
					else{
						isPartActive = true;
					}
				}
			}
		}
		// 空间查询全部满足
		if (isPartActive){
			// 部分满足
			Nodes->NodeData.isActive = true;
			return true;
		}
		else{
			;// 全部满足, 子节点需更新;
			Nodes->NodeData.isActive = true;
			return false;
		}
	}
	
	void UpdateSpaceNode(StrucGrid &SGrid, int depth, int Layer, int TotalLayer, TreeNode *Nodes, bool isValue){
		// 子节点; 
		if (depth <= Layer){
			if (Layer <= TotalLayer){
				for (int i = 0; i < Nodes->NumberChild; i++){
					Nodes->childNotes[i]->NodeData.isActive = isValue;
					if (depth < Layer){
						UpdateSpaceNode(SGrid, depth + 1, Layer, TotalLayer, Nodes->childNotes[i], isValue);
					}
				}
			}
			else{
				if (isValue){
					SetGridData(SGrid, Nodes->NodeData.IIndex, Nodes->NodeData.JIndex, Nodes->NodeData.KIndex, Nodes->NodeData.dx, Nodes->NodeData.dy, Nodes->NodeData.dz, SGrid.NX, SGrid.NY, SGrid.NZ);
				}
				else{
					SetFalseGridData(SGrid, Nodes->NodeData.IIndex, Nodes->NodeData.JIndex, Nodes->NodeData.KIndex, Nodes->NodeData.dx, Nodes->NodeData.dy, Nodes->NodeData.dz, SGrid.NX, SGrid.NY, SGrid.NZ);
				};
			};
		};
	};

	void SetNodeGridSpace(TreeNode *Nodes, StrucGrid &SGrid, int nx, int ny, int nz, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, vector<bool> SpaceFlag){
		BGGindex tempIdx;
		tempIdx.i = Nodes->NodeData.IIndex;
		tempIdx.j = Nodes->NodeData.JIndex;
		tempIdx.k = Nodes->NodeData.KIndex;
		for (int k1 = 0; k1 < Nodes->NodeData.dx; k1++){
			for (int k2 = 0; k2 < Nodes->NodeData.dy; k2++){
				for (int k3 = 0; k3 < Nodes->NodeData.dz; k3++){
					int idx = tempIdx.i + k1 + (tempIdx.j + k2)*nx + (tempIdx.k + k3)*nx*ny;
					JudgeSpaceFilter(SGrid, idx, (tempIdx.k + k3), nx, ny, nz, xmin, xmax, ymin, ymax, kmin, kmax, SpaceFlag);
				}
			}
		}
	}
	
	bool JudgeSpaceFilter(StrucGrid &SGrid, int idx, int k1, int nx, int ny, int nz, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, vector<bool> SpaceFlag){
		if (SpaceFlag[0] || SpaceFlag[1] || SpaceFlag[2]){
			if (SGrid.ACTNUM[idx] == 1){
				SGrid.CPGCells[idx].is_act = true;
				if (SpaceFlag[2]){
					if (k1< kmin || k1>kmax){
						SGrid.CPGCells[idx].is_act = false;
					}
				}
				if (SpaceFlag[0] || SpaceFlag[1]){
					double Bounds[6];
					GetCornerBounds(SGrid.CPGCells[idx].corner[0], SGrid.CPGCells[idx].corner[7], Bounds);
					if (SpaceFlag[0]){
						if (Bounds[0] > xmax || Bounds[1]<xmin){
							SGrid.CPGCells[idx].is_act = false;
							return true;
						}
					}
					if (SpaceFlag[1]){
						if (Bounds[2] > ymax || Bounds[3] < ymin){
							SGrid.CPGCells[idx].is_act = false;
							return true;
						}
					}
				}
			}
		}
		return true;
	}

	// 迭代查询属性信息;
	void GeoPropUpdate(int Layer, TreeNode *Nodes, StrucGrid &SGrid, double valueMin, double valuemax, int type,bool isFixed){
		// 精度问题潜在error;  
		valueMin = (valueMin - miniValue);
		valuemax = (valuemax + miniValue);
		GeoFilter(0, Layer, Nodes, SGrid, valueMin, valuemax, type, isFixed);

	}
	void GeoFilter(StrucGrid &SGrid, double valueMin, double valuemax, int type, bool isFixed){
		int nx, ny, nz;
		nx = SGrid.NX; ny = SGrid.NY; nz = SGrid.NZ;
		for (int k = 0; k < nz; k++){
			int idx_k = k*nx*ny;
			for (int j = 0; j < ny; j++){
				int idx_j = j*nx;
				for (int i = 0; i < nx; i++){
					int idx = idx_j + idx_k + i;
					// isFixed 取并集; 
					if (isFixed){
						if (SGrid.ACTNUM[idx] && SGrid.CPGCells[idx].is_act){
							SetGridProp(idx,SGrid,valueMin,valuemax,type);
						}
					}
					else{
						if (SGrid.ACTNUM[idx]){
							SetGridProp(idx, SGrid, valueMin, valuemax, type);
						}
					}
				}
			}
		}
	}
	void SetGridProp(int idx, StrucGrid &SGrid, double valueMin, double valuemax, int type){
		switch(type)
		{
		case 1:
			if (SGrid.PORO[idx]<valueMin || SGrid.PORO[idx]> valuemax){
				SGrid.CPGCells[idx].is_act = false;
				return;
			}
			break;
		case 2:
			if (SGrid.PERMEABILITY[idx]<valueMin || SGrid.PERMEABILITY[idx]> valuemax){
				SGrid.CPGCells[idx].is_act = false;
				return;
			}
			break;
		case 3:
			if (SGrid.SOIL[idx]<valueMin || SGrid.SOIL[idx]> valuemax){
				SGrid.CPGCells[idx].is_act = false;
				return;
			}
			break;
		case 4:
			if (SGrid.FACIES[idx]<valueMin || SGrid.FACIES[idx]> valuemax){
				SGrid.CPGCells[idx].is_act = false;
				return;
			}
			break;
		default:
			break;
		}
		SGrid.CPGCells[idx].is_act = true;
		return;
	}
	void GeoFilter(int depth, int Layer, TreeNode *Nodes, StrucGrid &SGrid, double valueMin, double valuemax, int type, bool isFixed){
		if (depth == Layer){
			for (int i = 0; i < Nodes->NumberChild; i++){
				if (isFixed && !Nodes->childNotes[i]->NodeData.isActive){
					;
				}
				else{
					for (int j = 0; j < Nodes->childNotes[i]->NumberChild; j++){
						SetGeoNodes(type, valueMin, valuemax, Nodes->childNotes[i]->childNotes[j], isFixed);
					}
				}
			}
		}
		else{
			for (int i = 0; i < Nodes->NumberChild; i++){
				if (isFixed && !Nodes->childNotes[i]->NodeData.isActive){
					;
				}
				else{
					for (int j = 0; j < Nodes->childNotes[i]->NumberChild; j++){
						GeoFilter(depth + 1, Layer, Nodes->childNotes[i]->childNotes[j], SGrid, valueMin, valuemax, type, isFixed);
					}
				}
			}
		}
		/*
		if (Nodes->childNotes[i]->NodeData.isActive){
		switch (type){
		case 1:
		tempValue = Nodes->childNotes[i]->NodeData.PORO;
		break;
		case 2:
		tempValue = Nodes->childNotes[i]->NodeData.PERMEABILITY;
		break;
		case 3:
		tempValue = Nodes->childNotes[i]->NodeData.SOIL;
		break;
		case 4:
		tempValue = Nodes->childNotes[i]->NodeData.FACIES;
		break;
		default:
		tempValue = valueMin - 1;
		break;
		}
		if (tempValue< (valueMin - miniValue) || tempValue >(valuemax + miniValue)){
		Nodes->childNotes[i]->NodeData.isActive = false; // InActive
		}
		else
		{
		Nodes->childNotes[i]->NodeData.isActive = true;
		}
		}
		*/
	}

	void SetGeoNodes(int type, double valueMin, double valuemax, TreeNode *Nodes, bool isFixed){
		if (isFixed && !Nodes->NodeData.isActive){
			return;
		}
		else{
			double tempValue;
			switch (type){
			case 1:
				tempValue = Nodes->NodeData.PORO;
				break;
			case 2:
				tempValue = Nodes->NodeData.PERMEABILITY;
				break;
			case 3:
				tempValue = Nodes->NodeData.SOIL;
				break;
			case 4:
				tempValue = Nodes->NodeData.FACIES;
				break;
			default:
				tempValue = valueMin - 1;
				break;
			}
			if (tempValue<  valueMin || tempValue > valuemax){
				Nodes->NodeData.isActive = false; // InActive
			}
			else{
				Nodes->NodeData.isActive = true;
			}
		}
	}

	// Original Grid 提取表面
	void GetGridBoundary(StrucGrid &SGrid, vector<FaceNode> &FaceNodes){
		bool(*Cellarray)[6] = new bool[SGrid.NGrid][6];
		memset(Cellarray, false, sizeof(bool)*SGrid.NGrid * 6);
		int idx, nx, ny, nz, tempIdx;
		nx = SGrid.NX; ny = SGrid.NY; nz = SGrid.NZ;
		FaceNodes.clear();
		FaceNode face;
		int Boundary[6][4] = { { 0, 1, 2, 3 }, { 4, 5, 6, 7 },
		{ 0, 4, 7, 3 }, { 1, 2, 6, 5 },
		{ 0, 1, 5, 4 }, { 3, 2, 6, 7 }
		};
		int Range[6] = { nx*ny, -nx*ny, 1, -1, nx, -nx };
		for (int j = 0; j < ny; j++){
			int idx_j = j*nx;
			for (int i = 0; i < nx; i++){
				for (int k = 0; k < 2; k++){
					int idx_k = k*(nz - 1)*nx*ny;
					idx = idx_j + idx_k + i;
					Cellarray[idx][k] = true; // True 表示显现;
				}
			}
		}
		for (int j = 0; j < ny; j++){
			int idx_j = j*nx;
			for (int k = 0; k < nz; k++){
				int idx_k = k*nx*ny;
				for (int i = 0; i < 2; i++){
					idx = idx_j + idx_k + i*(nx - 1);
					Cellarray[idx][i + 2] = true; // True 表示显现;
				}
			}
		}
		for (int i = 0; i < nx; i++){
			for (int k = 0; k < nz; k++){
				int idx_k = k*nx*ny;
				for (int j = 0; j < 2; j++){
					int idx_j = j*(ny - 1)*nx;
					idx = idx_j + idx_k + i;
					Cellarray[idx][j + 4] = true; // True 表示显现;
				}
			}
		}
		for (int j = 0; j < ny; j++){
			int idx_j = j*nx;
			for (int i = 0; i < nx; i++){
				for (int k = 0; k < nz; k++){
					int idx_k = k*nx*ny;
					idx = idx_j + idx_k + i;
					if (SGrid.ACTNUM[idx] != 1 || !SGrid.CPGCells[idx].is_act){
						for (int m = 0; m < 6; m++){
							tempIdx = idx + Range[m];
							if (tempIdx >= 0 && tempIdx < SGrid.NGrid){
								if (SGrid.CPGCells[tempIdx].is_act){
									Cellarray[tempIdx][m] = true; // True 表示显现;
								}
							}
						}
					}
				}
			}
		}
		for (int j = 0; j < ny; j++){
			int idx_j = j*nx;
			for (int i = 0; i < nx; i++){
				for (int k = 0; k < nz; k++){
					int idx_k = k*nx*ny;
					idx = idx_j + idx_k + i;
					if (SGrid.ACTNUM[idx] == 1 && SGrid.CPGCells[idx].is_act){
						for (int m = 0; m < 6; m++){
							if (Cellarray[idx][m]){
								AddFace(SGrid.CPGCells[idx], face, Boundary[m], SGrid.ACTNUM[idx], SGrid.PORO[idx], SGrid.PERMEABILITY[idx], SGrid.SOIL[idx], SGrid.FACIES[idx], 0, 0);
								FaceNodes.push_back(face);
							}
						}
					}
				}
			}
		}
		//delete [] Cellarray;
	}

	// 迭代提取可视面;
	void GetOrginalFace(int depth, int Layer, TreeNode *Note, StrucGrid &SGrid, vector<FaceNode> &FaceNodes){
		if (depth == Layer){
			for (int i = 0; i < Note->NumberChild; i++){
				if (Note->childNotes[i]->NodeData.isActive){
					GetOrginalBoundary(Note->childNotes[i], SGrid, FaceNodes);
				}
			}
		}
		else{
			// 迭代;
			for (int i = 0; i < Note->NumberChild; i++){
				if (Note->childNotes[i]->NodeData.isActive){
					GetOrginalFace(depth + 1, Layer, Note->childNotes[i], SGrid, FaceNodes);
				}
			}
		}
	};
	/*
	void GetOrginalBoundary2(TreeNode *Note, StrucGrid &SGrid, vector<FaceNode> &FaceNodes, bool **Cellarray){
		int Nc = Note->NumberChild;
		int Boundary[6][4] = { { 0, 1, 2, 3 }, { 4, 5, 6, 7 },
		{ 0, 4, 7, 3 }, { 1, 2, 6, 5 },
		{ 0, 1, 5, 4 }, { 3, 2, 6, 7 } };
		BGGindex tempIndex;
		FaceNode face;
		int dx, dy, dz, nx, ny, nz, idx=0;
		nx = SGrid.NX; ny = SGrid.NY; nz = SGrid.NZ;
		int tempJ, tempK, tempI;
		for (int d = 0; d < Nc; d++){
			for (int dd = 0; dd < Note->childNotes[d]->NumberChild; dd++){
				if (Cellarray[idx][6]){
					tempIndex.i = Note->childNotes[d]->childNotes[dd]->NodeData.IIndex;
					tempIndex.j = Note->childNotes[d]->childNotes[dd]->NodeData.JIndex;
					tempIndex.k = Note->childNotes[d]->childNotes[dd]->NodeData.KIndex;
					CPGNode note1 = Note->childNotes[d]->childNotes[dd]->NodeData;
					dx = Note->childNotes[d]->childNotes[dd]->NodeData.dx;
					dy = Note->childNotes[d]->childNotes[dd]->NodeData.dy;
					dz = Note->childNotes[d]->childNotes[dd]->NodeData.dz;
					for (int m = 0; m < 6; m++){
						if (Cellarray[idx][m]){
							switch (m)
							{
							case 0:
								tempK = tempIndex.k*nx*ny;
								for (int j = 0; j < dy; j++){
									tempJ = (tempIndex.j + j)*nx;
									for (int i = 0; i < dx; i++){
										int idx = (tempIndex.i + i) + tempJ + tempK;
										if (SGrid.ACTNUM[idx] && SGrid.CPGCells[idx].is_act){
											AddFace(SGrid.CPGCells[idx], face, Boundary[m], SGrid.ACTNUM[idx], SGrid.PORO[idx], SGrid.PERMEABILITY[idx], SGrid.SOIL[idx], SGrid.FACIES[idx], 0, 0);
											FaceNodes.push_back(face);
										}
									}
								}
								break;
							case 1:
								tempK = (tempIndex.k + dz - 1)*nx*ny;
								for (int j = 0; j < dy; j++){
									tempJ = (tempIndex.j + j)*nx;
									for (int i = 0; i < dx; i++){
										int idx = (tempIndex.i + i) + tempJ + tempK;
										if (SGrid.ACTNUM[idx] && SGrid.CPGCells[idx].is_act){
											AddFace(SGrid.CPGCells[idx], face, Boundary[m], SGrid.ACTNUM[idx], SGrid.PORO[idx], SGrid.PERMEABILITY[idx], SGrid.SOIL[idx], SGrid.FACIES[idx], 0, 0);
											FaceNodes.push_back(face);
										}
									}
								}
								break;
							case 2:
								for (int j = 0; j < dy; j++){
									tempJ = (tempIndex.j + j)*nx;
									for (int k = 0; k < dz; k++){
										int idx = (tempIndex.i) + tempJ + (tempIndex.k + k)*nx*ny;
										if (SGrid.ACTNUM[idx] && SGrid.CPGCells[idx].is_act){
											AddFace(SGrid.CPGCells[idx], face, Boundary[m], SGrid.ACTNUM[idx], SGrid.PORO[idx], SGrid.PERMEABILITY[idx], SGrid.SOIL[idx], SGrid.FACIES[idx], 0, 0);
											FaceNodes.push_back(face);
										}
									}
								}
								break;
							case 3:
								tempI = (tempIndex.i + dx - 1);
								for (int j = 0; j < dy; j++){
									tempJ = (tempIndex.j + j)*nx;
									for (int k = 0; k < dz; k++){
										int idx = tempI + tempJ + (tempIndex.k + k)*nx*ny;
										if (SGrid.ACTNUM[idx] && SGrid.CPGCells[idx].is_act){
											AddFace(SGrid.CPGCells[idx], face, Boundary[m], SGrid.ACTNUM[idx], SGrid.PORO[idx], SGrid.PERMEABILITY[idx], SGrid.SOIL[idx], SGrid.FACIES[idx], 0, 0);
											FaceNodes.push_back(face);
										}
									}
								}
								break;
							case 4:
								tempJ = (tempIndex.j)*nx;
								for (int i = 0; i < dx; i++){
									tempI = (tempIndex.i + i);
									for (int k = 0; k < dz; k++){
										int idx = tempI + tempJ + (tempIndex.k + k)*nx*ny;
										if (SGrid.ACTNUM[idx] && SGrid.CPGCells[idx].is_act){
											AddFace(SGrid.CPGCells[idx], face, Boundary[m], SGrid.ACTNUM[idx], SGrid.PORO[idx], SGrid.PERMEABILITY[idx], SGrid.SOIL[idx], SGrid.FACIES[idx], 0, 0);
											FaceNodes.push_back(face);
										}
									}
								}
								break;
							case 5:
								tempJ = (tempIndex.j + dy - 1)*nx;
								for (int i = 0; i < dx; i++){
									tempI = (tempIndex.i + i);
									for (int k = 0; k < dz; k++){
										int idx = tempI + tempJ + (tempIndex.k + k)*nx*ny;
										if (SGrid.ACTNUM[idx] && SGrid.CPGCells[idx].is_act){
											AddFace(SGrid.CPGCells[idx], face, Boundary[m], SGrid.ACTNUM[idx], SGrid.PORO[idx], SGrid.PERMEABILITY[idx], SGrid.SOIL[idx], SGrid.FACIES[idx], 0, 0);
											FaceNodes.push_back(face);
										}
									}
								}
								break;
							default:
								break;
							}
						}
					}
				}
				idx++;
			}
		}
	}
	*/
	void GetOrginalBoundary(TreeNode *Note, StrucGrid &SGrid, vector<FaceNode> &FaceNodes){
		int Nc = Note->NumberChild;
		int Boundary[6][4] = { { 0, 1, 2, 3 }, { 4, 5, 6, 7 },
		{ 0, 4, 7, 3 }, { 1, 2, 6, 5 },
		{ 0, 1, 5, 4 }, { 3, 2, 6, 7 } };
		BGGindex tempIndex;
		FaceNode face;
		int dx, dy, dz, nx, ny, nz;
		nx = SGrid.NX; ny = SGrid.NY; nz = SGrid.NZ;
		int tempJ, tempK, tempI;
		for (int d = 0; d < Nc; d++){
			if ( Note->childNotes[d]->Cellarray[6] ){
				tempIndex.i = Note->childNotes[d]->NodeData.IIndex;
				tempIndex.j = Note->childNotes[d]->NodeData.JIndex;
				tempIndex.k = Note->childNotes[d]->NodeData.KIndex;
				CPGNode note1 = Note->childNotes[d]->NodeData;
				dx = Note->childNotes[d]->NodeData.dx; dy = Note->childNotes[d]->NodeData.dy;
				dz = Note->childNotes[d]->NodeData.dz;
				for (int m = 0; m < 6; m++){
					if (Note->childNotes[d]->Cellarray[m]){
						switch (m)
						{
						case 0:
							tempK = tempIndex.k*nx*ny;
							for (int j = 0; j < dy; j++){
								tempJ = (tempIndex.j + j)*nx;
								for (int i = 0; i < dx; i++){
									int idx = (tempIndex.i + i) + tempJ + tempK;
									if (SGrid.ACTNUM[idx] && SGrid.CPGCells[idx].is_act){
										AddFace(SGrid.CPGCells[idx], face, Boundary[m], SGrid.ACTNUM[idx], SGrid.PORO[idx], SGrid.PERMEABILITY[idx], SGrid.SOIL[idx], SGrid.FACIES[idx], 0, 0);
										FaceNodes.push_back(face);
									}
								}
							}
							break;
						case 1:
							tempK = (tempIndex.k + dz - 1)*nx*ny;
							for (int j = 0; j < dy; j++){
								tempJ = (tempIndex.j + j)*nx;
								for (int i = 0; i < dx; i++){
									int idx = (tempIndex.i + i) + tempJ + tempK;
									if (SGrid.ACTNUM[idx] && SGrid.CPGCells[idx].is_act){
										AddFace(SGrid.CPGCells[idx], face, Boundary[m], SGrid.ACTNUM[idx], SGrid.PORO[idx], SGrid.PERMEABILITY[idx], SGrid.SOIL[idx], SGrid.FACIES[idx], 0, 0);
										FaceNodes.push_back(face);
									}
								}
							}
							break;
						case 2:
							for (int j = 0; j < dy; j++){
								tempJ = (tempIndex.j + j)*nx;
								for (int k = 0; k < dz; k++){
									int idx = (tempIndex.i) + tempJ + (tempIndex.k + k)*nx*ny;
									if (SGrid.ACTNUM[idx] && SGrid.CPGCells[idx].is_act){
										AddFace(SGrid.CPGCells[idx], face, Boundary[m], SGrid.ACTNUM[idx], SGrid.PORO[idx], SGrid.PERMEABILITY[idx], SGrid.SOIL[idx], SGrid.FACIES[idx], 0, 0);
										FaceNodes.push_back(face);
									}
								}
							}
							break;
						case 3:
							tempI = (tempIndex.i + dx - 1);
							for (int j = 0; j < dy; j++){
								tempJ = (tempIndex.j + j)*nx;
								for (int k = 0; k < dz; k++){
									int idx = tempI + tempJ + (tempIndex.k + k)*nx*ny;
									if (SGrid.ACTNUM[idx] && SGrid.CPGCells[idx].is_act){
										AddFace(SGrid.CPGCells[idx], face, Boundary[m], SGrid.ACTNUM[idx], SGrid.PORO[idx], SGrid.PERMEABILITY[idx], SGrid.SOIL[idx], SGrid.FACIES[idx], 0, 0);
										FaceNodes.push_back(face);
									}
								}
							}
							break;
						case 4:
							tempJ = (tempIndex.j)*nx;
							for (int i = 0; i < dx; i++){
								tempI = (tempIndex.i + i);
								for (int k = 0; k < dz; k++){
									int idx = tempI + tempJ + (tempIndex.k + k)*nx*ny;
									if (SGrid.ACTNUM[idx] && SGrid.CPGCells[idx].is_act){
										AddFace(SGrid.CPGCells[idx], face, Boundary[m], SGrid.ACTNUM[idx], SGrid.PORO[idx], SGrid.PERMEABILITY[idx], SGrid.SOIL[idx], SGrid.FACIES[idx], 0, 0);
										FaceNodes.push_back(face);
									}
								}
							}
							break;
						case 5:
							tempJ = (tempIndex.j + dy - 1)*nx;
							for (int i = 0; i < dx; i++){
								tempI = (tempIndex.i + i);
								for (int k = 0; k < dz; k++){
									int idx = tempI + tempJ + (tempIndex.k + k)*nx*ny;
									if (SGrid.ACTNUM[idx] && SGrid.CPGCells[idx].is_act){
										AddFace(SGrid.CPGCells[idx], face, Boundary[m], SGrid.ACTNUM[idx], SGrid.PORO[idx], SGrid.PERMEABILITY[idx], SGrid.SOIL[idx], SGrid.FACIES[idx], 0, 0);
										FaceNodes.push_back(face);
									}
								}
							}
							break;
						default:
							break;
						}
					}

				}
			}
		}
	}


	void GetTreeNodeBoundary(int depth, int Layer, TreeNode *Note, StrucGrid &SGrid, vector<FaceNode> &FaceNodes) { // bool **Cellarray
		if (depth == Layer){
			for (int i = 0; i < Note->NumberChild; i++){
				if (Note->childNotes[i]->NodeData.isActive){
					GetTreeNode(Note->childNotes[i], SGrid, FaceNodes);  // LOD1 Face
				}
			}
		}
		else{
			// 取消迭代
			for (int i = 0; i < Note->NumberChild; i++){ // root
				if (Note->childNotes[i]->NodeData.isActive){  
					for (int j = 0; j < Note->childNotes[i]->NumberChild; j++){  // LOD1 层位
						GetTreeNodeBoundary( depth + 1, Layer,Note->childNotes[i]->childNotes[j], SGrid, FaceNodes); // LOD2 Face
					}
				}
			}
		}
	}
	void GetTreeNode(TreeNode *Note,StrucGrid &SGrid, vector<FaceNode> &FaceNodes){ //bool **Cellarray
		int Nc = Note->NumberChild;
		int LeftBoundary[4] = { 0, 4, 7, 3 }, RightBoundary[4] = { 1, 2, 6, 5 };
		int UpBoundary[4] = { 3, 2, 6, 7 }, DownBoundary[4] = { 0, 1, 5, 4 };
		int TopBoundary[4] = { 4, 5, 6, 7 }, BottomBoundary[4] = { 0, 1, 2, 3 };
		BGGindex tempIndex;
		FaceNode face;
		int dx, dy, dz, nx, ny, nz;
		nx = SGrid.NX; ny = SGrid.NY; nz = SGrid.NZ;
		for (int i = 0; i < Nc; i++){
			if ( Note->childNotes[i]->NodeData.isActive){
				tempIndex.i = Note->childNotes[i]->NodeData.IIndex;
				tempIndex.j = Note->childNotes[i]->NodeData.JIndex;
				tempIndex.k = Note->childNotes[i]->NodeData.KIndex;
				CPGNode note1 = Note->childNotes[i]->NodeData;
				dx = Note->childNotes[i]->NodeData.dx;
				dy = Note->childNotes[i]->NodeData.dy;
				dz = Note->childNotes[i]->NodeData.dz;
				bool FirsrtFlag = false;
				if (JudgeFace2(tempIndex.i, tempIndex.j, tempIndex.k, dx, dy, dz, nx, ny, nz, SGrid.ACTNUM, SGrid.CPGCells)){
					// Top;				
					if (!JudgeFace(tempIndex.i, tempIndex.j, tempIndex.k - 1, dx, dy, 1, nx, ny, nz, SGrid.ACTNUM, SGrid.CPGCells)){
						AddFace(note1.CPGCells, face, TopBoundary, note1.ACTNUM, note1.PORO, note1.PERMEABILITY, note1.SOIL, note1.FACIES, 0, 0);
						FaceNodes.push_back(face);
						Note->childNotes[i]->Cellarray[0] = true;
						FirsrtFlag = true;
					}
					if (!JudgeFace(tempIndex.i, tempIndex.j, tempIndex.k + dz, dx, dy, 1, nx, ny, nz, SGrid.ACTNUM, SGrid.CPGCells)){
						AddFace(note1.CPGCells, face, BottomBoundary, note1.ACTNUM, note1.PORO, note1.PERMEABILITY, note1.SOIL, note1.FACIES, 0, 0);
						FaceNodes.push_back(face);
						Note->childNotes[i]->Cellarray[1] = true;
						FirsrtFlag = true;
					}
					if (!JudgeFace(tempIndex.i - 1, tempIndex.j, tempIndex.k, 1, dy, dz, nx, ny, nz, SGrid.ACTNUM, SGrid.CPGCells)){
						AddFace(note1.CPGCells, face, LeftBoundary, note1.ACTNUM, note1.PORO, note1.PERMEABILITY, note1.SOIL, note1.FACIES, 0, 0);
						FaceNodes.push_back(face);
						Note->childNotes[i]->Cellarray[2] = true;
						FirsrtFlag = true;
					}
					if (!JudgeFace(tempIndex.i + dx, tempIndex.j, tempIndex.k, 1, dy, dz, nx, ny, nz, SGrid.ACTNUM, SGrid.CPGCells)){
						AddFace(note1.CPGCells, face, RightBoundary, note1.ACTNUM, note1.PORO, note1.PERMEABILITY, note1.SOIL, note1.FACIES, 0, 0);
						FaceNodes.push_back(face);
						Note->childNotes[i]->Cellarray[3] = true;
						FirsrtFlag = true;
					}
					if (!JudgeFace(tempIndex.i, tempIndex.j - 1, tempIndex.k, dx, 1, dz, nx, ny, nz, SGrid.ACTNUM, SGrid.CPGCells)){
						AddFace(note1.CPGCells, face, DownBoundary, note1.ACTNUM, note1.PORO, note1.PERMEABILITY, note1.SOIL, note1.FACIES, 0, 0);
						FaceNodes.push_back(face);
						Note->childNotes[i]->Cellarray[4] = true;
						FirsrtFlag = true;
					}
					if (!JudgeFace(tempIndex.i, tempIndex.j + dy, tempIndex.k, dx, 1, dz, nx, ny, nz, SGrid.ACTNUM, SGrid.CPGCells)){
						AddFace(note1.CPGCells, face, UpBoundary, note1.ACTNUM, note1.PORO, note1.PERMEABILITY, note1.SOIL, note1.FACIES, 0, 0);
						FaceNodes.push_back(face);
						Note->childNotes[i]->Cellarray[5] = true;
						FirsrtFlag = true;
					}
					if (FirsrtFlag){
						Note->childNotes[i]->Cellarray[6] = true;
					}
					else{
						Note->childNotes[i]->Cellarray[6] = false;
					}
				}
			}
		}
	}

	/*void GetTreeNode2(TreeNode *Note, StrucGrid &SGrid, vector<FaceNode> &FaceNodes, bool **Cellarray){
		int Nc = Note->NumberChild;
		int LeftBoundary[4] = { 0, 4, 7, 3 }, RightBoundary[4] = { 1, 2, 6, 5 };
		int UpBoundary[4] = { 3, 2, 6, 7 }, DownBoundary[4] = { 0, 1, 5, 4 };
		int TopBoundary[4] = { 4, 5, 6, 7 }, BottomBoundary[4] = { 0, 1, 2, 3 };
		BGGindex tempIndex;
		FaceNode face;
		int dx, dy, dz, nx, ny, nz, idx = 0;
		nx = SGrid.NX; ny = SGrid.NY; nz = SGrid.NZ;
		for (int i = 0; i < Nc; i++){
			//if (Note->childNotes[i]->NodeData.isActive){
			for (int j = 0; j < Note->childNotes[i]->NumberChild; j++){
				if (Note->childNotes[i]->childNotes[j]->NodeData.isActive){
					tempIndex.i = Note->childNotes[i]->childNotes[j]->NodeData.IIndex;
					tempIndex.j = Note->childNotes[i]->childNotes[j]->NodeData.JIndex;
					tempIndex.k = Note->childNotes[i]->childNotes[j]->NodeData.KIndex;
					CPGNode note1 = Note->childNotes[i]->childNotes[j]->NodeData;
					dx = Note->childNotes[i]->childNotes[j]->NodeData.dx;
					dy = Note->childNotes[i]->childNotes[j]->NodeData.dy;
					dz = Note->childNotes[i]->childNotes[j]->NodeData.dz;
					bool FirsrtFlag = false;
					if (JudgeFace2(tempIndex.i, tempIndex.j, tempIndex.k, dx, dy, dz, nx, ny, nz, SGrid.ACTNUM, SGrid.CPGCells)){
						// Top;				
						if (!JudgeFace(tempIndex.i, tempIndex.j, tempIndex.k - 1, dx, dy, 1, nx, ny, nz, SGrid.ACTNUM, SGrid.CPGCells)){
							AddFace(note1.CPGCells, face, TopBoundary, note1.ACTNUM, note1.PORO, note1.PERMEABILITY, note1.SOIL, note1.FACIES, 0, 0);
							FaceNodes.push_back(face);
							Cellarray[idx][0] = true;
							FirsrtFlag = true;
						}
						if (!JudgeFace(tempIndex.i, tempIndex.j, tempIndex.k + dz, dx, dy, 1, nx, ny, nz, SGrid.ACTNUM, SGrid.CPGCells)){
							AddFace(note1.CPGCells, face, BottomBoundary, note1.ACTNUM, note1.PORO, note1.PERMEABILITY, note1.SOIL, note1.FACIES, 0, 0);
							FaceNodes.push_back(face);
							Cellarray[idx][1] = true;
							FirsrtFlag = true;
						}
						if (!JudgeFace(tempIndex.i - 1, tempIndex.j, tempIndex.k, 1, dy, dz, nx, ny, nz, SGrid.ACTNUM, SGrid.CPGCells)){
							AddFace(note1.CPGCells, face, LeftBoundary, note1.ACTNUM, note1.PORO, note1.PERMEABILITY, note1.SOIL, note1.FACIES, 0, 0);
							FaceNodes.push_back(face);
							Cellarray[idx][2] = true;
							FirsrtFlag = true;
						}
						if (!JudgeFace(tempIndex.i + dx, tempIndex.j, tempIndex.k, 1, dy, dz, nx, ny, nz, SGrid.ACTNUM, SGrid.CPGCells)){
							AddFace(note1.CPGCells, face, RightBoundary, note1.ACTNUM, note1.PORO, note1.PERMEABILITY, note1.SOIL, note1.FACIES, 0, 0);
							FaceNodes.push_back(face);
							Cellarray[idx][3] = true;
							FirsrtFlag = true;
						}
						if (!JudgeFace(tempIndex.i, tempIndex.j - 1, tempIndex.k, dx, 1, dz, nx, ny, nz, SGrid.ACTNUM, SGrid.CPGCells)){
							AddFace(note1.CPGCells, face, DownBoundary, note1.ACTNUM, note1.PORO, note1.PERMEABILITY, note1.SOIL, note1.FACIES, 0, 0);
							FaceNodes.push_back(face);
							Cellarray[idx][4] = true;
							FirsrtFlag = true;
						}
						if (!JudgeFace(tempIndex.i, tempIndex.j + dy, tempIndex.k, dx, 1, dz, nx, ny, nz, SGrid.ACTNUM, SGrid.CPGCells)){
							AddFace(note1.CPGCells, face, UpBoundary, note1.ACTNUM, note1.PORO, note1.PERMEABILITY, note1.SOIL, note1.FACIES, 0, 0);
							FaceNodes.push_back(face);
							Cellarray[idx][5] = true;
							FirsrtFlag = true;
						}
						if (FirsrtFlag){
							Cellarray[idx][6] = true;
						}
					}
				}
				idx++;
			}
		}
		if (idx >= SGrid.NGrid){
			printf("errror idx > NGrid \n");
		}
	}
	*/
	void GetNodeBoundary(TreeNode *Note, StrucGrid &SGrid, vector<FaceNode> &ZoneDatas){
		/*
			根据上一层 LOD 0 的CellArray 获取下一层 LOD1 的节点;
		*/
		for (int i = 0; i < Note->NumberChild; i++){   //root 
			if (Note->childNotes[i]->NodeData.isActive){
				for (int j = 0; j < Note->childNotes[i]->NumberChild; j++){  // LOD1 层位; 
					TreeNode * tempNote = Note->childNotes[i]->childNotes[j];
					if (tempNote->NodeData.isActive){
						for (int mm = 0; mm < tempNote->NumberChild; mm++){
							GetBoundary(tempNote->NodeData, tempNote->childNotes[mm], SGrid, ZoneDatas,tempNote->Cellarray); // 优化 LOD 2; 
						}
					}
				}
			}
		};
	}

	void GetBoundary(CPGNode LayerNode, TreeNode *Note, StrucGrid &SGrid, vector<FaceNode> &FaceNodes, bool *Cellarray){ // Cellarray;
		/*
			CellArray 表示Note层位六个方向显示的层位;
		*/
		int Nc = Note->NumberChild;
		int LeftBoundary[4] = { 0, 4, 7, 3 }, RightBoundary[4] = { 1, 2, 6, 5 };
		int UpBoundary[4] = { 3, 2, 6, 7 }, DownBoundary[4] = { 0, 1, 5, 4 };
		int TopBoundary[4] = { 4, 5, 6, 7 }, BottomBoundary[4] = { 0, 1, 2, 3 };
		BGGindex tempIndex;
		FaceNode face;
		int dx, dy, dz, nx, ny, nz;
		nx = SGrid.NX; ny = SGrid.NY; nz = SGrid.NZ;
		dx = LayerNode.dx; dy = LayerNode.dy; dz = LayerNode.dz;
		if ( Cellarray[6]){
			for (int m = 0; m < 6; m++){
				if (Cellarray[m]){
					for (int i = 0; i < Nc; i++){
						tempIndex.i = Note->childNotes[i]->NodeData.IIndex;
						tempIndex.j = Note->childNotes[i]->NodeData.JIndex;
						tempIndex.k = Note->childNotes[i]->NodeData.KIndex;
						CPGNode note1 = Note->childNotes[i]->NodeData;
						switch (m)
						{
						case 0:
							if (tempIndex.k == LayerNode.KIndex){
								AddFace(note1.CPGCells, face, TopBoundary, note1.ACTNUM, note1.PORO, note1.PERMEABILITY, note1.SOIL, note1.FACIES, 0, 0);
								FaceNodes.push_back(face);
							}
							break;
						case 1:
							if (tempIndex.k == (LayerNode.KIndex + dz - Note->childNotes[i]->NodeData.dz)){
								AddFace(note1.CPGCells, face, BottomBoundary, note1.ACTNUM, note1.PORO, note1.PERMEABILITY, note1.SOIL, note1.FACIES, 0, 0);
								FaceNodes.push_back(face);
							}
							break;
						case 2:
							if (tempIndex.i == ( LayerNode.IIndex)){
								AddFace(note1.CPGCells, face, LeftBoundary, note1.ACTNUM, note1.PORO, note1.PERMEABILITY, note1.SOIL, note1.FACIES, 0, 0);
								FaceNodes.push_back(face);
							}
							break;
						case 3:
							if (tempIndex.i == ( LayerNode.IIndex + dx - Note->childNotes[i]->NodeData.dx))
							{
								AddFace(note1.CPGCells, face, RightBoundary, note1.ACTNUM, note1.PORO, note1.PERMEABILITY, note1.SOIL, note1.FACIES, 0, 0);
								FaceNodes.push_back(face);
							}
							break;
						case 4:
							if (tempIndex.j == ( LayerNode.JIndex))
							{
								AddFace(note1.CPGCells, face, DownBoundary, note1.ACTNUM, note1.PORO, note1.PERMEABILITY, note1.SOIL, note1.FACIES, 0, 0);
								FaceNodes.push_back(face);
							}
							break;
						case 5:
							if (tempIndex.j == ( LayerNode.JIndex + dy - Note->childNotes[i]->NodeData.dy))
							{
								AddFace(note1.CPGCells, face, UpBoundary, note1.ACTNUM, note1.PORO, note1.PERMEABILITY, note1.SOIL, note1.FACIES, 0, 0);
								FaceNodes.push_back(face);
							}
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}

	bool JudgeFace(int i, int j, int k, int nx, int ny, int nz, int *ACTNUM, CPCell *cells){
		if (i < 0 || i >= nx){
			return false;
		}
		if (j < 0 || j >= ny){
			return false;
		}
		if (k < 0 || k >= nz){
			return false;
		}
		int idx = i + j*nx + k*ny*nx;
		return ACTNUM[idx] == 1 && cells[idx].is_act;
	}

	bool JudgeFace(int i, int j, int k, int dx, int dy, int dz, int nx, int ny, int nz, int *ACTNUM,CPCell *cells){
		for (int kk = 0; kk < dz; kk++){
			for (int jj = 0; jj < dy; jj++)
			{
				for (int ii = 0; ii < dx; ii++){
					if (!JudgeFace(i + ii, j + jj, k + kk, nx, ny, nz, ACTNUM, cells)){
						return false;
						break;
					}
				}
			}
		}
		return true;
	}

	bool JudgeFace2(int i, int j, int k, int dx, int dy, int dz, int nx, int ny, int nz, int *ACTNUM, CPCell *cells){
		for (int kk = 0; kk < dz; kk++){
			for (int jj = 0; jj < dy; jj++)
			{
				for (int ii = 0; ii < dx; ii++){
					if (JudgeFace(i + ii, j + jj, k + kk, nx, ny, nz, ACTNUM, cells)){
						return true;
						break;
					}
				}
			}
		}
		return false;
	}

	void UpdatePropperty(vtkSmartPointer<vtkDoubleArray> val, int Type, vector<FaceNode> FaceNodes){
		double *Value = GetFaceValue(FaceNodes, Type);
		int CellNum = (int)FaceNodes.size() * 4;
		for (int i = 0; i < CellNum; i++){
			val->SetTuple1(i, Value[i]);
		}
	}

	void GetCellRange(vector<FaceNode> FaceNodes, vector<CellRange> &cellRange){
		bool fistFlag = true;
		cellRange.clear();
		if (cellRange.size()>0){
			fistFlag = false;
		}
		else{
			CellRange TempCellRange;
			TempCellRange.minValue = 0; 
			TempCellRange.maxValue = 0;
			for (int Type = 0; Type < 9; Type++){
				cellRange.push_back(TempCellRange);
			}
		}
		int CellNum = (int)FaceNodes.size();
		double Value;
		for (int i = 0; i < CellNum; i++){
			for (int j = 0; j < 4; j++){
				for (int Type = 0; Type < 9; Type++){
					switch (Type)
					{
					case 0:
						Value = FaceNodes[i].Point[j].x;
						break;
					case 1:
						Value = FaceNodes[i].Point[j].y;
						break;
					case 2:
						Value = FaceNodes[i].Point[j].z;
						break;					
					case 3:
						Value = FaceNodes[i].PORO;
						break;
					case 4:
						Value = FaceNodes[i].PERMEABILITY;
						break;
					case 5:
						Value = FaceNodes[i].SOIL;
						break;
					case 6:
						Value = FaceNodes[i].FACIES; 
						break;
					case 7:
						Value = FaceNodes[i].BULKVOLUME;
						break;
					case 8:
						Value = FaceNodes[i].DZMTRXV;
						break;
					}
					if (fistFlag){
						cellRange[Type].minValue = Value;
						cellRange[Type].maxValue = Value;						
					}
					else{						
						if (cellRange[Type].maxValue < Value){
							cellRange[Type].maxValue = Value;
						}
						if (cellRange[Type].minValue > Value){
							cellRange[Type].minValue = Value;
						}
					}
				}
				fistFlag = false;
			}
		}
	}
	void GetCellRange(StrucGrid &SGrid, vector<CellRange> &cellRange){
		bool fistFlag = true;
		cellRange.clear();
		if (cellRange.size()>0){
			fistFlag = false;
		}
		else{
			CellRange TempCellRange;
			TempCellRange.minValue = 0; 
			TempCellRange.maxValue = 0;
			for (int Type = 0; Type < 9; Type++){
				cellRange.push_back(TempCellRange);
			}
		}		
		int CellNum = (int)SGrid.NGrid;
		double Value;
		int NumberType=9;
		int nPillar = (SGrid.NX + 1)*(SGrid.NY + 1);
		cellRange[0].minValue = SGrid.Coord[0]; cellRange[0].maxValue = SGrid.Coord[0];
		cellRange[1].minValue = SGrid.Coord[1]; cellRange[1].maxValue = SGrid.Coord[1];
		for (int i = 1; i < nPillar*2; i++){ // X,Y cellRange;
			cellRange[0].minValue = SGrid.Coord[3 * i]<cellRange[0].minValue ? SGrid.Coord[3 * i] : cellRange[0].minValue;
			cellRange[0].maxValue = SGrid.Coord[3 * i]>cellRange[0].maxValue ? SGrid.Coord[3 * i] : cellRange[0].maxValue;
			cellRange[1].minValue = SGrid.Coord[3 * i+1]<cellRange[1].minValue ? SGrid.Coord[3 * i+1] : cellRange[1].minValue;
			cellRange[1].maxValue = SGrid.Coord[3 * i+1]>cellRange[1].maxValue ? SGrid.Coord[3 * i+1] : cellRange[1].maxValue;
		}
		//Bottom and Up;
		cellRange[2].minValue = SGrid.Zcorn[0]; cellRange[2].maxValue = SGrid.Zcorn[0];
		int NLcell = CellNum/SGrid.NZ;
		for (int k = 0; k < 2; k++){
			int tempk = 0;
			if (k == 1)
				tempk=CellNum-k*NLcell;
			for (int j = 0; j < NLcell; j++){
				int idx = j + tempk;
				for (int i = 0; i < 8; i++){
					cellRange[2].minValue = SGrid.CPGCells[idx].corner[i].z<cellRange[2].minValue ? SGrid.CPGCells[idx].corner[i].z : cellRange[2].minValue;
					cellRange[2].maxValue = SGrid.CPGCells[idx].corner[i].z>cellRange[2].maxValue ? SGrid.CPGCells[idx].corner[i].z : cellRange[2].maxValue;
				}
			}
		}
		printf("%f,%f\n", cellRange[2].maxValue, cellRange[2].minValue);
		for (int i = 0; i < CellNum; i++){
			for (int Type = 3; Type < NumberType; Type++){
				switch (Type)
				{
				case 3:
					if (!SGrid.Flag[2])
						continue;
					Value = SGrid.PORO[i];
					break;
				case 4:
					if (!SGrid.Flag[3])
						continue;
					Value = SGrid.PERMEABILITY[i];
					break;
				case 5:
					if (!SGrid.Flag[4])
						continue;
					Value = SGrid.SOIL[i];
					break;
				case 6:
					if (!SGrid.Flag[5])
						continue;
					Value = SGrid.FACIES[i];
					break;
				case 7:
					if (!SGrid.Flag[0])
						continue;
					Value = SGrid.BULKVOLUME[i];
					break;
				case 8:
					if (!SGrid.Flag[1])
						continue;
					Value = SGrid.DZMTRXV[i];
					break;
				}
				if (fistFlag){
					cellRange[Type].minValue = Value;
					cellRange[Type].maxValue = Value;
				}
				else{
					if (cellRange[Type].maxValue < Value){
						cellRange[Type].maxValue = Value;
					}
					if (cellRange[Type].minValue > Value){
						cellRange[Type].minValue = Value;
					}
				}
			}
			fistFlag = false;
		}
	}

	void GetNodeCellRange(int depth, int Layer, TreeNode *Note, vector<CellRange> &cellRange,bool *isPropFlag){
		if (depth == Layer){
			for (int i = 0; i < Note->NumberChild; i++){  
				GetCellRange(Note->childNotes[i], cellRange, isPropFlag);// 层位；
			}
		}
		else{
			for (int i = 0; i < Note->NumberChild; i++){
				GetNodeCellRange(depth + 1, Layer, Note->childNotes[i], cellRange, isPropFlag);
			}
		}
	}

	void GetCellRange(TreeNode *Note,vector<CellRange> &cellRange,bool *isPropFlag){
		bool fistFlag = true;	
		bool Flag = true;
		if (cellRange.size()>0){
			fistFlag = false;
		}
		else{
			CellRange TempCellRange;
			TempCellRange.minValue = 0;
			TempCellRange.maxValue = 0;
			for (int Type = 0; Type < 9; Type++){
				cellRange.push_back(TempCellRange);
			}
		}
		int CellNum = (int)Note->NumberChild;
		double Value;
		int NumberType;
		for (int i = 0; i < CellNum; i++){
			for (int j = 0; j < 8; j++){
				if (j == 0){
					NumberType = 9;
				}
				else{
					NumberType = 3;
				}
				for (int Type = 0; Type < NumberType; Type++){
					Flag = true;
					if (Type >= 3){
						Flag = isPropFlag[Type-3];
					}
					if (Flag){
						switch (Type)
						{
						case 0:
							Value = Note->childNotes[i]->NodeData.CPGCells.corner[j].x;
							break;
						case 1:
							Value = Note->childNotes[i]->NodeData.CPGCells.corner[j].y;
							break;
						case 2:
							Value = Note->childNotes[i]->NodeData.CPGCells.corner[j].z;
							break;
						case 3:
							Value = Note->childNotes[i]->NodeData.PORO;
							break;
						case 4:
							Value = Note->childNotes[i]->NodeData.PERMEABILITY;
							break;
						case 5:
							Value = Note->childNotes[i]->NodeData.SOIL;
							break;
						case 6:
							Value = Note->childNotes[i]->NodeData.FACIES;
							break;
						case 7:
							Value = Note->childNotes[i]->NodeData.BULKVOLUME;
							break;
						case 8:
							Value = Note->childNotes[i]->NodeData.DZMTRXV;
							break;
						}
						if (fistFlag){
							cellRange[Type].minValue = Value;
							cellRange[Type].maxValue = Value;
						}
						else{
							if (cellRange[Type].maxValue < Value){
								cellRange[Type].maxValue = Value;
							}
							if (cellRange[Type].minValue > Value){
								cellRange[Type].minValue = Value;
							}
						}
					}
				}
				fistFlag = false;
			}
		}
	}

	void SetGridData(StrucGrid &SGrid, int i1, int j1, int k1, int dx, int dy, int dz, int nx, int ny, int nz){
		for (int k = 0; k < dz; k++){
			int id_k = (k1 + k)*nx*ny;
			for (int j = 0; j < dy; j++){
				int id_j = (j1 + j)*nx;
				for (int i = 0; i < dx; i++){
					int idx = i1 + i + id_j + id_k;
					if (idx >= SGrid.NGrid){
						cout << idx << endl;
					};
					if (SGrid.ACTNUM[idx] == 1){
						SGrid.CPGCells[idx].is_act = true;
					}
				}
			}
		}
	};

	void SetFalseGridData(StrucGrid &SGrid, int i1, int j1, int k1, int dx, int dy, int dz, int nx, int ny, int nz){
		for (int k = 0; k < dz; k++){
			int id_k = (k1 + k)*nx*ny;
			for (int j = 0; j < dy; j++){
				int id_j = (j1 + j)*nx;
				for (int i = 0; i < dx; i++){
					int idx = i1 + i + id_j + id_k;
					if (idx >= SGrid.NGrid){ // 1568784
						cout << idx << endl;
					};
					if (SGrid.ACTNUM[idx] == 1){
						SGrid.CPGCells[idx].is_act = false;
					}
				}
			}
		}
	};

	// 获取
	void getLayer2(intArray TopLayers, intArray BottomLayers, int TopIndex, int BottomIndex, int &iIndex, int &j){
		bool first = false;
		iIndex = -1;
		for (int i = 0; i < (int)TopLayers.size(); i++){
			if (TopLayers[i] <= TopIndex)
			{
				//if (!first){
				iIndex = i;
				//first = true;
				j = i;
			}
			if ( BottomLayers[i] >= BottomIndex){
				j = i;
				break;
			}
		}
	}
};


/*
2019-1-19
1) 裂缝处理
2) 沉积相合并;
3) 其他问题;
*/