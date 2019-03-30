#include"GridProcess.h"

namespace UMSM{

	bool DataProcess::JudgeFacies(int *Facies, int NGrid,intArray &Layers,int Type)
	{
		if (NGrid < 1){
			return false;
		}
		if (IsFacesMerge){
			if (Type == 0)
			{
				FaciesMerge tempFace;
				bool Firstflag = true;
				int Face1, Face2;
				Layers.push_back(0);
				Layers.push_back(Facies[0]);
				for (int i = 0; i < NGrid; i++)
				{
					if (Facies[i] != Facies[0]){
						Face1 = Facies[i]<Facies[0] ? Facies[i] : Facies[0];
						Face2 = Facies[i]>Facies[0] ? Facies[i] : Facies[0];
						if (Firstflag){
							for (int j = 0; j < (int)FMerge.size(); j++){
								if (FMerge[j].i == Face1&& FMerge[j].j == Face2){
									Layers.push_back(Facies[i]);
									tempFace.i = Face1; tempFace.j = Face2;
									Firstflag = false;
									break;
								}
							}
							if (Firstflag){
								return false;
							}
						}
						else
						{
							if (tempFace.i == Face1&& tempFace.j == Face2){
								;
							}
							else{
								return false;
							}
						}
					}
				}
				return true;
			}
			else
			{
				return JudgeAnotherFacies(Facies, NGrid, Layers);
			}
		}
		else
		{
			return true;
		}
	}

	bool DataProcess::JudgeAnotherFacies(int *Facies, int NGrid, intArray &Layers)
	{		
		FaciesMerge tempFace;
		bool sand=false,mud=false;
		for (int i = 0; i < NGrid; i++)
		{
			if ( Facies[i] ==0 ){
				mud = true;
			}
			else if ( Facies[i]>0 ){
				sand = true;
			}			
		}
		if (mud == true && sand == true){
			return false;
		}
		else if ( mud == true){
			Layers.push_back(-1);
			Layers.push_back(0);
		}
		else if (sand == true){
			Layers.push_back(-1);
			Layers.push_back(1);
		}
		else
		{
			Layers.push_back(-1);
			//printf("Unexpected.....");
		}
		return true;
	}

	bool DataProcess::isMerge(intArray Face1, intArray Face2){
		if (Face1.size() > 0 && Face2.size() > 0){
			if (Face1[0] == -1){  // LOD1;
				if (Face1[1] == Face2[1]){
					return true;
				}
				else{
					return false;
				}
			}
			else{
				// LOD2;   4,5,6; 
				if (Face1.size() <= Face2.size())
				{
					return isFaces_Merge(Face1,Face2);
				}
				else{
					return isFaces_Merge(Face2, Face1);
				}
			}
		}
		else{
			return false;
		}		
		return true;		
	}
	bool DataProcess::isFaces_Merge(intArray Face1, intArray Face2){
		bool is_valid = true; 
		for (int j = 1; j < Face1.size(); j++){
			is_valid = true;
			for (int i = 1; i < Face2.size(); i++){
				if (Face2[i] == Face1[j]){
					is_valid = false;
					break;
				}
			}
			if (is_valid){ // 不存在;
				int temp_Face1 = Face1[j] < Face2[0] ? Face1[j] : Face2[0];
				int temp_Face2 = Face1[j] > Face2[0] ? Face1[j] : Face2[0];
				for (int j = 0; j < (int)FMerge.size(); j++){
					if (FMerge[j].i == temp_Face1 && FMerge[j].j == temp_Face2){
						is_valid = false;
						break;
					}
				}
			}
			if (is_valid){
				return false;
			}
		}
		return true;
	}
	bool DataProcess::isLayer_merge(nodeData t1, nodeData t2){
		if (t1.dx == t2.dx && t1.dy == t2.dy){
			return true;
		}
		return false;
	}
	void getLayer(intArray TopLayers, intArray BottomLayers, int TopIndex,int BottomIndex,int &iIndex,int &j){
		bool first = false;
		iIndex = -1;
		for (int i = 0; i < (int)TopLayers.size(); i++){
			if (TopLayers[i] >= TopIndex && BottomLayers[i] <= BottomIndex)
			{
				if (!first){
					iIndex = i;
					first = true;
				}
				j = i;
			}
		}
	}
	void getSingleLayer(intArray & SingleLayerTop, intArray & SingleLayerBottom, vector<PhaseMap> Phasemaps, int TopLayer, int BottomLayer){
		int Size = (int)Phasemaps.size() - 1;
		for (int i = 0; i < (int)Phasemaps[Size].TopLayers.size(); i++)
		{
			if (Phasemaps[Size].TopLayers[i] >= TopLayer && Phasemaps[Size].TopLayers[i] <= BottomLayer){
				SingleLayerTop.push_back(Phasemaps[Size].TopLayers[i]);
				SingleLayerBottom.push_back(Phasemaps[Size].BottomLayers[i]);
			}
		}
	}
	DataProcess::DataProcess(StrucGrid &SGridData, MyTree &Tree)
	{		
		//output_grid(SGridData, SGridData.NX/10, SGridData.NY/10, 2, 0); // 层位数+ 开始层位;
		int dxx = 8, dyy = 8;
		int i = 0;
		vector<NodeDatas> noteDatas; // 保存构造树的缓存信息;
		SetParameter(dxx, dyy);
		Initlization(SGridData);
		MergeGrid(SGridData, noteDatas);
		TreeInitlization(Tree, SGridData.Phasemaps[i].TopLayers, SGridData.Phasemaps[i].BottomLayers, noteDatas, SGridData); // Data 按照层位存储;
		currentType = 0;
		for (int j = 0; j < Tree.root->NumberChild; j++)
		{
			for (int jj = 0; jj < Tree.root->childNotes[j]->NumberChild; jj++){
				TreeMake(1, SGridData, dxx, dyy, Tree.root->childNotes[j]->childNotes[jj]);
			}
		}
		for (int j = 0; j < Tree.root->NumberChild; j++){
			Tree.root->childNotes[j];
			for (int m = 0; m < Tree.root->childNotes[j]->NumberChild; m++){
				Tree.root->childNotes[j]->childNotes[m];
			}
		}		
	}
	DataProcess::~DataProcess()
	{
		delete[]MM;
		keywords.clear();
		FMerge.clear();		
	}
	void DataProcess::TreeMake(int Stage, StrucGrid &SGridData, int dxx, int dyy, TreeNode *note)
	{
		if (Stage < StageNum)
		{
			dxx = dxx / 2; dyy = dyy / 2;
			intArray TopLayers = SGridData.Phasemaps[Stage].TopLayers;
			intArray BottomLayers = SGridData.Phasemaps[Stage].BottomLayers;
			TreeChildInit2(Stage, TopLayers, BottomLayers, note, SGridData, dxx, dyy);
		}
	}
	int find(intArray indexs, int id)
	{
		for (int i = 0; i < (int)indexs.size(); i++)
		{
			if (indexs[i] == id)
				return i;
		}
		return -1;
	}
	void DataProcess::TreeChildInit(int Stage, intArray TopLayers, intArray BottomLayers, TreeNode *Note, StrucGrid &SGridData, int dxx, int dyy)
	{
		int indx1, indx2, dzz,KIndex,tempBottomIndex; // 分组;
		int TopIndex, BottomIndex, NumberChild;
		TopIndex = Note->NodeData.KIndex + 1;
		BottomIndex = Note->NodeData.KIndex + Note->NodeData.dz;
		//获取 顶层Id_1 底层id_2; 小层数;
		getLayer(TopLayers, BottomLayers, TopIndex, BottomIndex, indx1, indx2);
		int NumberLayers = indx2 - indx1 + 1;
		NodeDatas tempNoteDatas;
		bool isFine=true;
		int mode = (BottomLayers[indx2] - BottomIndex);
		for (int k = 0; k < NumberLayers; k++)
		{
			if (k == 0) {  KIndex = Note->NodeData.KIndex; }
			else { 
				KIndex = TopLayers[indx1 + k] - 1; 
			}
			tempBottomIndex = BottomLayers[indx1+k];
			if (k == NumberLayers - 1){
				tempBottomIndex = BottomIndex;
			}
			dzz = tempBottomIndex - KIndex;
			intArray FacieLayers;
			FacieLayers.push_back(0);
			FacieLayers[0] = -1;
			if (Note->NodeData.dx < dxx&&Note->NodeData.dy < dyy)
			{				
				//tempNoteDatas.push_back(nodeData(Note->NodeData.IIndex, Note->NodeData.JIndex, KIndex, Note->NodeData.dx, Note->NodeData.dy, dzz, FacieLayers, isFine,));
			}
			else if (Note->NodeData.dx < dxx)
			{
				int ny = Note->NodeData.dy / dyy;
				int nyy = Note->NodeData.dy % dyy;
				for (int j = 0; j < ny; j++)
				{
					//tempNoteDatas.push_back(nodeData(Note->NodeData.IIndex, Note->NodeData.JIndex + j*dyy, KIndex, Note->NodeData.dx, ny, dzz, FacieLayers));
				}
				if (nyy > 0)
				{
					//tempNoteDatas.push_back(nodeData(Note->NodeData.IIndex, Note->NodeData.JIndex + ny*dyy, KIndex, Note->NodeData.dx, nyy, dzz, FacieLayers));
				}
			}
			else if (Note->NodeData.dy < dyy)
			{
				int nx = Note->NodeData.dx / dxx;
				int nxx = Note->NodeData.dx % dxx;;
				for (int i = 0; i < nx; i++)
				{
					//tempNoteDatas.push_back(nodeData(Note->NodeData.IIndex + i*dxx, Note->NodeData.JIndex, KIndex, dxx, Note->NodeData.dy, dzz, FacieLayers));
				}
				if (nxx>0)
				{
					//tempNoteDatas.push_back(nodeData(Note->NodeData.IIndex + nx*dxx, Note->NodeData.JIndex, KIndex, nxx, Note->NodeData.dy, dzz, FacieLayers));
				}
			}
			else
			{
				int nx = Note->NodeData.dx / dxx;
				int ny = Note->NodeData.dy / dyy;
				int nxx = Note->NodeData.dx % dxx;
				int nyy = Note->NodeData.dy % dyy;
				for (int j = 0; j < ny; j++)
				{
					for (int i = 0; i < nx; i++)
					{
						//tempNoteDatas.push_back(nodeData(Note->NodeData.IIndex + i*dxx, Note->NodeData.JIndex + j*dyy, KIndex, dxx, dyy, dzz, FacieLayers));
					}
					if (nxx>0)
					{
						//tempNoteDatas.push_back(nodeData(Note->NodeData.IIndex + nx*dxx, Note->NodeData.JIndex + j*dyy, KIndex, nxx, dyy, dzz, FacieLayers));
					}
				}
				if (nyy > 0)
				{
					for (int i = 0; i < nx; i++)
					{
						//tempNoteDatas.push_back(nodeData(Note->NodeData.IIndex + i*dxx, Note->NodeData.JIndex + ny*dyy, KIndex, dxx, nyy, dzz, FacieLayers));
					}
					if (nxx>0)
					{
						//tempNoteDatas.push_back(nodeData(Note->NodeData.IIndex + nx*dxx, Note->NodeData.JIndex + ny*dyy, KIndex, nxx, nyy, dzz, FacieLayers));
					}
				}
			}
			FacieLayers.clear();
		}
		NumberChild = (int)tempNoteDatas.size();;
		// 分配空间;
		Note->NumberChild = NumberChild;
		Note->childNotes = new TreeNode*[NumberChild];
		for (int i = 0; i < NumberChild; i++)
		{
			CPGNode node1;
			Merge(SGridData, tempNoteDatas[i], SGridData.NX, SGridData.NY, node1);
 			Note->childNotes[i] = new TreeNode(0, node1);
			TreeMake(Stage + 1, SGridData, dxx, dyy, Note->childNotes[i]);
		}
	}

	void DataProcess::UpdateCellNeighbor(TreeNode *Note, StrucGrid &SGridData){
		int Nc = Note->NumberChild;		
		intArray Cells;
		BGGindex tempIndex;
		for (int i = 0; i < Nc; i++){
			tempIndex = Note->childNotes[i]->NodeData.BGindex;
			// left						
			addCelsMap(tempIndex, dx, 0, Cells);
			addCelsMap(tempIndex, 0, 0, Cells);			
			addCelsMap(tempIndex, -dx, 0, Cells);
			addCelsMap(tempIndex, 0, -dy, Cells);
			addCelsMap(tempIndex, 0, dy, Cells);						
			FindNeighbor(Note, Cells, i);
			Cells.clear();
			// 上下点暂不考虑
			addCelsMap(tempIndex, Note->childNotes[i]->NodeData.dz, Cells);//up;
			SetUpDown(Note, Cells, i, true);
			Cells.clear();
			addCelsMap(tempIndex, -Note->childNotes[i]->NodeData.dz, Cells);//up;
			SetUpDown(Note, Cells, i, false);
			Cells.clear();
		}
		/*
		// 消除间隙点
		intArray tempIdx;
		for (int i = 0; i < Nc; i++){
			if (Note->childNotes[i]->NodeData.isFine){  // 细网格;				
				// left
				if (Note->childNotes[i]->NodeData.neightbor.left.size()>0){
					tempIdx = Note->childNotes[i]->NodeData.neightbor.left;
					for (int j = 0; j< tempIdx.size(); j++){
						if (Note->childNotes[tempIdx[j]]->NodeData.dx > Note->childNotes[i]->NodeData.dx){							
							DeleteFracture(Note->childNotes[tempIdx[j]]->NodeData.CPGCells, Note->childNotes[i]->NodeData.CPGCells, 0);
						}
					}
				}				
				if (Note->childNotes[i]->NodeData.neightbor.right.size()>0){
					tempIdx = Note->childNotes[i]->NodeData.neightbor.right;
					for (int j = 0; j< tempIdx.size(); j++){
						if (Note->childNotes[tempIdx[j]]->NodeData.dx > Note->childNotes[i]->NodeData.dx){
							DeleteFracture(Note->childNotes[tempIdx[j]]->NodeData.CPGCells, Note->childNotes[i]->NodeData.CPGCells, 1);
						}
					}
				}
				if (Note->childNotes[i]->NodeData.neightbor.up.size()>0){
					tempIdx = Note->childNotes[i]->NodeData.neightbor.up;
					for (int j = 0; j< tempIdx.size(); j++){
						if (Note->childNotes[tempIdx[j]]->NodeData.dx > Note->childNotes[i]->NodeData.dx){
							DeleteFracture(Note->childNotes[tempIdx[j]]->NodeData.CPGCells, Note->childNotes[i]->NodeData.CPGCells, 2);
						}
					}
				}
				if (Note->childNotes[i]->NodeData.neightbor.down.size()>0){
					tempIdx = Note->childNotes[i]->NodeData.neightbor.down;
					for (int j = 0; j< tempIdx.size(); j++){
						if (Note->childNotes[tempIdx[j]]->NodeData.dx > Note->childNotes[i]->NodeData.dx){
							DeleteFracture(Note->childNotes[tempIdx[j]]->NodeData.CPGCells, Note->childNotes[i]->NodeData.CPGCells, 3);
						}
					}
				}
			}
		}
		*/
	}
	void DeleteFracture(CPCell & Cell1, CPCell & Cell2, int type){
		point t1;
		for (int i = 0; i < 8; i = i + 4){
			switch (type) 
			{
			case 0: //left
				if (intersection(Cell1.corner[i + 1], Cell1.corner[i + 2], Cell2.corner[i + 2], Cell1.corner[i + 3], t1)){
					Intercept(Cell1.corner[i + 1], Cell1.corner[i + 2], t1);
					Cell1.corner[i + 3] = t1;
				}
				if (intersection(Cell1.corner[i + 1], Cell1.corner[i + 2], Cell2.corner[i + 0], Cell1.corner[i + 1], t1))
				{
					Intercept(Cell1.corner[i + 1], Cell1.corner[i + 2], t1);
					Cell1.corner[i + 0] = t1;
				}
				break;
			case 1: //right
				if (intersection(Cell1.corner[i + 0], Cell1.corner[i + 3], Cell2.corner[i + 3], Cell1.corner[i + 2], t1)){
					Intercept(Cell1.corner[i + 0], Cell1.corner[i + 3], t1);
					Cell1.corner[i + 2] = t1;
				}
				if (intersection(Cell1.corner[i + 0], Cell1.corner[i + 3], Cell2.corner[i + 0], Cell1.corner[i + 1], t1)){
					Intercept(Cell1.corner[i + 0], Cell1.corner[i + 3], t1);
					Cell1.corner[i + 1] = t1;
				}
				break;
			case 2: //up
				if (intersection(Cell1.corner[i + 0], Cell1.corner[i + 1], Cell2.corner[i + 1], Cell1.corner[i + 2], t1)){
					Intercept(Cell1.corner[i + 0], Cell1.corner[i + 1], t1);
					Cell1.corner[i + 2] = t1;
				}
				if (intersection(Cell1.corner[i + 0], Cell1.corner[i + 1], Cell2.corner[i + 0], Cell1.corner[i + 3], t1)){
					Intercept(Cell1.corner[i + 0], Cell1.corner[i + 1], t1);
					Cell1.corner[i + 3] = t1;
				}
				break;
			case 3:
				if (intersection(Cell1.corner[i + 2], Cell1.corner[i + 3], Cell2.corner[i + 1], Cell1.corner[i + 2], t1)){
					Intercept(Cell1.corner[i + 2], Cell1.corner[i + 3], t1);
					Cell1.corner[i + 1] = t1;
				}
				if (intersection(Cell1.corner[i + 2], Cell1.corner[i + 3], Cell2.corner[i + 0], Cell1.corner[i + 3], t1)){
					Intercept(Cell1.corner[i + 0], Cell1.corner[i + 1], t1);
					Cell1.corner[i + 0] = t1;
				}
				break;
			default:
				break;
			}		
		}
	}
	void DataProcess::addCelsMap(BGGindex tempIndex, int dx, int dy, intArray &Cells){
		//tempIndex.i = tempIndex.i - dx;
		//tempIndex.j = tempIndex.j - dy;
		//std::map<BGGindex, intArray>::iterator iter;
		//iter = CellMap.find({ tempIndex });
		//if (iter != CellMap.end()){
		//	Cells.insert(Cells.end(), iter->second.begin(), iter->second.end());
		//}
	}
	void DataProcess::addCelsMap(BGGindex tempIndex, int dz, intArray &Cells){
		//tempIndex.k = tempIndex.k - dz;
		//std::map<BGGindex, intArray>::iterator iter;
		//iter = CellMap.find({ tempIndex });
		//if (iter != CellMap.end()){
		//	Cells.insert(Cells.end(), iter->second.begin(), iter->second.end());
		//}
	}
	void SetUpDown(TreeNode *Note, intArray Cells, int jIdx,bool type){
		// 上面还有且满足上面的所有i,j,k; 完整;
		if (Cells.size() == 0){
			;
		}
		else{
			int TempImin, TempImax, TempJmin, TempJmax;
			int Imin = Note->childNotes[jIdx]->NodeData.IIndex;
			int Jmin = Note->childNotes[jIdx]->NodeData.JIndex;
			int Imax = Note->childNotes[jIdx]->NodeData.IIndex + Note->childNotes[jIdx]->NodeData.dx;
			int Jmax = Note->childNotes[jIdx]->NodeData.JIndex + Note->childNotes[jIdx]->NodeData.dy;
			for (int i = 0; i < (int)Cells.size(); i++){
				if (Cells[i] != jIdx){
					TempImin = Note->childNotes[Cells[i]]->NodeData.IIndex;
					TempImax = Note->childNotes[Cells[i]]->NodeData.IIndex + Note->childNotes[Cells[i]]->NodeData.dx;
					TempJmin = Note->childNotes[Cells[i]]->NodeData.JIndex;
					TempJmax = Note->childNotes[Cells[i]]->NodeData.JIndex + Note->childNotes[Cells[i]]->NodeData.dy;
					if (TempImax<Imin || TempJmax<Jmin || TempJmin >Jmax || TempImin > Imax){
						;
					}
					else{
						if (type){
							// 疑问？
							Note->childNotes[jIdx]->NodeData.neightbor.up.push_back(jIdx);
						}
						else{
							Note->childNotes[jIdx]->NodeData.neightbor.down.push_back(jIdx);
						}
					}
				}
			}
		}
	}
	void DataProcess::FindNeighbor(TreeNode *Note, intArray Cells, int jIdx){
		int Imin = Note->childNotes[jIdx]->NodeData.IIndex;
		int Jmin = Note->childNotes[jIdx]->NodeData.JIndex;
		//int Kmin = Note->childNotes[jIdx]->NodeData.KIndex;
		int Imax = Note->childNotes[jIdx]->NodeData.IIndex + Note->childNotes[jIdx]->NodeData.dx;
		int Jmax = Note->childNotes[jIdx]->NodeData.JIndex + Note->childNotes[jIdx]->NodeData.dy;
		//int Kmax = Note->childNotes[jIdx]->NodeData.KIndex + Note->childNotes[jIdx]->NodeData.dz;
		int TempImin, TempImax, TempJmin, TempJmax;
		NeightBor tempNeightbor;
		for (int i = 0; i < (int)Cells.size(); i++){
			if (Cells[i] != jIdx){
				TempImin = Note->childNotes[Cells[i]]->NodeData.IIndex;
				TempImax = Note->childNotes[Cells[i]]->NodeData.IIndex + Note->childNotes[Cells[i]]->NodeData.dx;
				TempJmin = Note->childNotes[Cells[i]]->NodeData.JIndex;
				TempJmax = Note->childNotes[Cells[i]]->NodeData.JIndex + Note->childNotes[Cells[i]]->NodeData.dy;
				// left
				if ((TempImax) == Imin){
					if (TempJmin< Jmax || TempJmax > Jmin){
						continue;
					}
					else
					{
						tempNeightbor.left.push_back(i);
						continue;
					}
				}
				// right;
				if (Imax == TempImin){
					if (TempJmin< Jmax || TempJmax > Jmin){
						continue;
					}
					else{
						tempNeightbor.right.push_back(i);
						continue;
					}
				}
				// up
				if ((Jmax) == TempJmin){
					if (TempImax < Imin || TempImin>Imax){
						continue;
					}
					else{
						tempNeightbor.up.push_back(i);
						continue;
					}
				}
				// down 
				if ((Jmin) == TempJmax){
					if (TempImax < Imin || TempImin>Imax){
						continue;
					}
					else{
						tempNeightbor.down.push_back(i);
						continue;
					}
				}
			}
		}
		Note->childNotes[jIdx]->NodeData.neightbor = tempNeightbor;
	}
	void DataProcess::TreeChildInit2(int Stage, intArray TopLayers, intArray BottomLayers, TreeNode *Note, StrucGrid &SGridData, int dxx, int dyy)
	{
		NeightBor tempNeighbor=Note->NodeData.neightbor;
		// 水平分割,垂直merge;
		int indx1, indx2, dzz, KIndex, tempBottomIndex; // 分组;
		int TopIndex, BottomIndex, NumberChild;
		TopIndex = Note->NodeData.KIndex + 1;
		BottomIndex = Note->NodeData.KIndex + Note->NodeData.dz;
		//获取 顶层Id_1 底层id_2; 小层数;
		// getLayer(TopLayers, BottomLayers, TopIndex, BottomIndex, indx1, indx2);
		getLayer_Layer(TopLayers, BottomLayers, TopIndex, BottomIndex, indx1, indx2);
		int NumberLayers = indx2 - indx1 + 1; 
		NodeDatas tempNoteDatas;
		map<BGGindex, int> NoteMap;
		bool isFine = false;
		// int mode = (BottomLayers[indx2] - BottomIndex);
		int Nxy = Note->NodeData.dx*Note->NodeData.dy;
		int *NegighBor = new int[Nxy], *Facies = new int[Nxy];

		// 构造树子节点;
		Note->childNotes = new TreeNode*[NumberLayers];
		Note->NumberChild = NumberLayers;
		/*NumberChild = (int)tempNoteDatas.size();;
		// 分配空间;
		Note->NumberChild = NumberChild;
		for (int i = 0; i < NumberChild; i++)
		{
			CPGNode node1;
			Merge(SGridData, tempNoteDatas[i], SGridData.NX, SGridData.NY, node1);
			Note->childNotes[i] = new TreeNode(0, node1);
			Note->childNotes[i]->NodeData.neightbor = tempNeighbor;
			TreeMake(Stage + 1, SGridData, dxx, dyy, Note->childNotes[i]);
		}
		*/
		for (int k = 0; k < NumberLayers; k++)
		{
			if (k == 0) { KIndex = Note->NodeData.KIndex; }
			else {
				KIndex = TopLayers[indx1 + k] - 1;
			}
			tempBottomIndex = BottomLayers[indx1 + k];
			if (k == NumberLayers - 1){
				tempBottomIndex = BottomIndex;
			}
			dzz = tempBottomIndex - KIndex;
			int iIndex, jIndex;
			for (int _kk = 0; _kk < dzz; _kk++){
				int Kidx = KIndex + _kk;
				for (int j = 0; j < Note->NodeData.dy / dyy; j++)  // 不整除;
				{
					for (int i = 0; i < Note->NodeData.dx / dxx; i++) // 不整除;
					{
						BGGindex Index(Note->NodeData.IIndex + i*dxx, Note->NodeData.JIndex + j*dyy, Kidx);
						GetNeighbor(Note->NodeData.IIndex + i*dxx, Note->NodeData.JIndex + j*dyy, Kidx, dxx, dyy, Kidx, SGridData, NegighBor, Facies, tempNoteDatas, NoteMap, isFine, Index);
					}
					if (Note->NodeData.dx % dxx > 0){
						iIndex = Note->NodeData.IIndex + Note->NodeData.dx / dxx*dxx;
						BGGindex Index(iIndex, Note->NodeData.JIndex + j*dyy, Kidx);
						GetNeighbor(iIndex, Note->NodeData.JIndex + j*dyy, Kidx, Note->NodeData.dx % dxx, dyy, Kidx, SGridData, NegighBor, Facies, tempNoteDatas, NoteMap, isFine, Index);
					}
				}
				if (dyy > 0)
				{
					jIndex = Note->NodeData.JIndex + Note->NodeData.dy / dyy*dyy;
					for (int i = 0; i < Note->NodeData.dx / dxx; i++) // 不整除;
					{
						BGGindex Index(Note->NodeData.IIndex + i*dxx, jIndex, Kidx);
						GetNeighbor(Note->NodeData.IIndex + i*dxx, jIndex, Kidx, dxx, Note->NodeData.dy%dyy, Kidx, SGridData, NegighBor, Facies, tempNoteDatas, NoteMap, isFine, Index);
					}
					if (Note->NodeData.dx % dxx > 0)
					{
						iIndex = Note->NodeData.IIndex + Note->NodeData.dx / dxx*dxx;
						jIndex = Note->NodeData.JIndex + Note->NodeData.dy / dyy*dyy;
						BGGindex Index(iIndex, jIndex, Kidx);
						GetNeighbor(iIndex, jIndex, Kidx, Note->NodeData.dx%dxx, Note->NodeData.dy%dyy, Kidx, SGridData, NegighBor, Facies, tempNoteDatas, NoteMap, isFine, Index);
					}
				}
			}
			// 构造树;
			NumberChild = (int)tempNoteDatas.size();
			if (NumberChild == 0){ // 不可细分;
				BGGindex Index(Note->NodeData.IIndex, Note->NodeData.JIndex, KIndex);
				get_cell_facies(Note->NodeData.IIndex, Note->NodeData.JIndex, KIndex, Note->NodeData.dx, Note->NodeData.dy, dzz, SGridData, NegighBor, Facies, tempNoteDatas, isFine, Index);
				NumberChild = (int)tempNoteDatas.size();
			};
			NodeDatas noteDatas;
			if (dzz > 1){
				DownMergeGrid2(SGridData, noteDatas, tempNoteDatas, TopLayers, BottomLayers, NoteMap, true); // 判断垂向能否合并;
			}
			else{
				for (int xx = 0; xx < tempNoteDatas.size(); xx++){
					tempNoteDatas[xx].dz = 1;
					noteDatas.push_back(tempNoteDatas[xx]);
				}
				//DownMergeGrid2(SGridData, noteDatas, tempNoteDatas, TopLayers, BottomLayers, NoteMap, false); // 判断垂向能否合并;
			}
			tempNoteDatas = noteDatas;
			NumberChild = (int)tempNoteDatas.size();
			CPGNode node1;
			node1.KIndex = indx1 + k; //保存树节点信息;
			Note->childNotes[k] = new TreeNode(NumberChild, node1);
			for (int ii = 0; ii < NumberChild; ii++)
			{
				CPGNode node2;
				Merge(SGridData, tempNoteDatas[ii], SGridData.NX, SGridData.NY, node2);
				Note->childNotes[k]->childNotes[ii] = new TreeNode(0, node2);
				Note->childNotes[k]->childNotes[ii]->NodeData.neightbor = tempNeighbor;
				TreeMake(Stage + 1, SGridData, dxx, dyy, Note->childNotes[k]->childNotes[ii]);
			}
			tempNoteDatas.clear();
			NoteMap.clear();
		}
		// 分配空间;
		/*Note->NumberChild = NumberChild;
		Note->childNotes = new TreeNode*[NumberChild];
		*/
		
	}
	void DataProcess::GetNeighbor(int i, int j, int Layer, int dxx, int dyy, int dzz, StrucGrid &SGridData, int *NegighBor, int *Facies, NodeDatas &noteDatas, map<BGGindex, int> &NoteMap, bool isFine, BGGindex Index)
	{
		int ATNUM,k=0;
		ATNUM = 0;
		int NGrid = dxx*dyy;
		intArray FaceLayers;
		for(int m = 0; m < dyy; m++)
		{
			for (int n = 0; n < dxx; n++)
			{
				int Cell = GetCecllId(i + n, j + m, Layer, NX, NY, NZ);		
				if (SGridData.ACTNUM[Cell])
				{					
					NegighBor[k] = Cell;
					if (IsFacesMerge){
						Facies[ATNUM] = SGridData.FACIES[Cell];
					}
					ATNUM++;
				}
				else
				{
					NegighBor[k] = -1;
				}
				k++;
			}
		}		
		FaceLayers.clear();
		if (ATNUM == NGrid && JudgeFacies(Facies, ATNUM, FaceLayers, currentType)) // 合并; 情形讨论; 
		{
			bool firstflag = true;
			JudgeFault(i,j,NegighBor, SGridData, firstflag, dxx, dyy);
			// 判断断层
			if (firstflag)
			{
				NoteMap.insert( make_pair(BGGindex(i, j, Layer), (int)noteDatas.size()));
				noteDatas.push_back(nodeData(i, j, Layer, dxx, dyy, dzz, FaceLayers, isFine, Index));
			}
			else
			{
				Merger(dxx, dyy, dzz, i, j, Layer, SGridData, noteDatas, NoteMap,true,Index);
			}
		}
		else
		{
			if (ATNUM > 0)
			{
				Merger(dxx, dyy, dzz, i, j, Layer, SGridData, noteDatas, NoteMap, true, Index);
			}
			//else{
			//	cout << "da" << endl;
			//}
			//if (ATNUM >= NGrid / 2) // 边界合并; 情形讨论;			
			//	 noteDatas.push_back(nodeData(i, j, dxx, dyy));			
		}
	}
	void DataProcess::MergeGrid(StrucGrid &SGridData, vector<NodeDatas> &noteDatas)
	{
		// 初始化		
		int k = 0, currIdx = 0;
		int *NegighBor = new int[dxy];
		int *Facies = new int[dxy];
		int dxx, dyy;
		dxx = NX % dx;
		dyy = NY % dy;
		bool isFine = false;
		int TopLayer, BottomLayer, Layer;
		intArray SingleLayerTop, SingleLayerBottom;
		NodeDatas TempnoteDatas;
		NodeDatas TempnoteDatas2;
		map<BGGindex, int> NoteMap;
		NodeDatas noteData;
		FracPillars PostPillars;
		for (int k = 0; k < (int)SGridData.Phasemaps[0].TopLayers.size(); k++)
		{
			TopLayer = SGridData.Phasemaps[0].TopLayers[k];
			BottomLayer = SGridData.Phasemaps[0].BottomLayers[k];
			//获取 顶层Id_1 底层id_2; 小层数;
			getSingleLayer(SingleLayerTop, SingleLayerBottom, SGridData.Phasemaps, TopLayer, BottomLayer);
			//for (int kk = 0; kk < (int)SingleLayerTop.size(); kk++){
			int NumberLayer = (BottomLayer - TopLayer + 1);
				currIdx = 0;
				//for (int kk = 0; kk < NumberLayer; kk++)
				//{
				//int layer_dz = 1;
				//Layer = TopLayer - 1 + kk;
				int layer_dz = NumberLayer;// SingleLayerBottom[k] - SingleLayerTop[k] + 1;
				Layer = TopLayer-1; //SingleLayerTop[kk] - 1; // K
				int kk = NumberLayer;
				for (int j = 0; j < NY / dy; j++)  // 不整除;
				{
					for (int i = 0; i < NX / dx; i++) // 不整除;
					{
						BGGindex Index(i*dx, j*dy, Layer);
						GetNeighbor(i*dx, j*dy, Layer, dx, dy, kk, SGridData, NegighBor, Facies, TempnoteDatas, NoteMap, isFine, Index);
					}
					if (dxx > 0){
						int iIndex = NX / dx*dx, k = 0;
						BGGindex Index(iIndex, j*dy, Layer);
						GetNeighbor(iIndex, j*dy, Layer, dxx, dy, kk, SGridData, NegighBor, Facies, TempnoteDatas, NoteMap, isFine, Index);
					}
				}
				if (dyy > 0)
				{
					int jIndex = NY - NY%dy;
					for (int i = 0; i < NX / dx; i++) // 不整除;
					{
						BGGindex Index(i*dx, jIndex, Layer);
						GetNeighbor(i*dx, jIndex, Layer, dx, dyy, kk, SGridData, NegighBor, Facies, TempnoteDatas, NoteMap, isFine, Index);
					}
					if (dxx > 0)
					{
						int iIndex = NX / dx*dx;
						BGGindex Index(iIndex, jIndex, Layer);
						GetNeighbor(iIndex, jIndex, Layer, dxx, dyy, kk, SGridData, NegighBor, Facies, TempnoteDatas, NoteMap, isFine, Index);
					}
				}
				//update;
				TempnoteDatas2.insert(TempnoteDatas2.end(), TempnoteDatas.begin() + currIdx, TempnoteDatas.end());
				//updateCoord(SGridData, NX, NY, NZ, dx, dy, 0, TempnoteDatas2, PostPillars); // 水平间隙处理;  
				currIdx = (int)TempnoteDatas.size();
				TempnoteDatas2.clear();
			//}
			DownMergeGrid2(SGridData, noteData, TempnoteDatas, SingleLayerTop, SingleLayerBottom, NoteMap, false); // 判断垂向能否合并;
			noteDatas.push_back(noteData);
			noteData.clear();
			NoteMap.clear();
			TempnoteDatas.clear();
			SingleLayerTop.clear();
			SingleLayerBottom.clear();
		}
		// Set_XY_Coords(PostPillars, SGridData.NX, SGridData.NY, SGridData, isFaultPillar);
		// 缺少垂向间隙判断处理;
		// 上下底面;
		// get_Zcoorn_Pillar(SGridData, SGridData.NX, SGridData.NY, SGridData.NZ, dx, dy, 0, noteDatas, PostPillars);
	}
	void DataProcess::DownMergeGrid2(StrucGrid &SGridData, NodeDatas &noteDatas, NodeDatas &TempNote, intArray SingleTopLayer, intArray SingleBottomLayer, map<BGGindex, int> &NoteMap, bool isDivided)
	{
		int Layers = SingleBottomLayer.back() - SingleTopLayer[0]; // 总层位;
		int TopLayer = SingleTopLayer[0] - 1;
		int Size = (int)TempNote.size();
		std::map<BGGindex, int>::iterator iter;
		if (isDivided && IsFacesMerge){
			for (int i = 0; i < Size; i++)
			{
				if (!TempNote[i].visted){
					if (TempNote[i].dz == Layers){  // 全部包含结束;
						TempNote[i].dz = 1; //SingleBottomLayer[Layers] - SingleTopLayer[Layers] + 1;
						noteDatas.push_back(TempNote[i]);
					}
					else{
						intArray TempFacies = TempNote[i].FaceLayers;
						for (int k = (TempNote[i].dz + 1); k <= Layers; k++) 
						{
							iter = NoteMap.find({ BGGindex( TempNote[i].i, TempNote[i].j, TopLayer + k) });
							if (iter == NoteMap.end()){ //下面不存在
								TempNote[i].dz = k - TempNote[i].dz;
								noteDatas.push_back(TempNote[i]);
								break;
							}
							else{
								// 判断能否合并 Facies属性是否相同;
								int Index = iter->second;
								intArray TempFace2 = TempNote[Index].FaceLayers;//SGridData.FACIES[GetCecllId(TempNote[Index].i, TempNote[Index].j, TempNote[Index].k, NX, NY, NZ)];
								if (isMerge(TempFacies, TempFace2) && isLayer_merge(TempNote[Index],TempNote[i])){
									TempNote[Index].visted = true;
									if (k == Layers){
										TempNote[i].dz = k - TempNote[i].dz +1 ;// SingleBottomLayer[k] - SingleTopLayer[TempNote[i].dz] + 1;
										noteDatas.push_back(TempNote[i]);
									}
								}
								else{
									// 不能合并
									TempNote[i].dz = k - TempNote[i].dz;// SingleBottomLayer[k - 1] - SingleTopLayer[TempNote[i].dz] + 1;
									noteDatas.push_back(TempNote[i]);
									break;
								}
							}
						}
					}
				}
			}
		}
		else{
			for (int i = 0; i < Size; i++)
			{
				//TempNote[i].dz = 1;
				//noteDatas.push_back(TempNote[i]);
				//if (TempNote[i].k == TempNote[0].k){
				TempNote[i].dz = SingleBottomLayer.back() - SingleTopLayer[0] + 1;
				noteDatas.push_back(TempNote[i]);
				//}
			}
		}
	}

	void DataProcess::DownMergeGrid(StrucGrid &SGridData, NodeDatas &noteDatas, NodeDatas &TempNote, intArray SingleTopLayer, intArray SingleBottomLayer,map<BGGindex, int> &NoteMap,bool isDivided)
	{	
		int Layers = (int)SingleTopLayer.size()-1;
		int Size = (int)TempNote.size();
		std::map<BGGindex, int>::iterator iter;
		if (isDivided && IsFacesMerge){
			for (int i = 0; i < Size; i++)
			{
				if (!TempNote[i].visted){
					if (TempNote[i].dz == Layers){
						TempNote[i].dz = SingleBottomLayer[Layers] - SingleTopLayer[Layers] + 1;
						noteDatas.push_back(TempNote[i]);
					}
					else{
						//int TempFacies = SGridData.FACIES[GetCecllId(TempNote[i].i, TempNote[i].j, TempNote[i].k, NX, NY, NZ)]; //当前note Faceis;
						intArray TempFacies = TempNote[i].FaceLayers;
						for (int k = TempNote[i].dz + 1; k <= Layers; k++)
						{
							iter = NoteMap.find({ BGGindex(TempNote[i].i, TempNote[i].j, SingleTopLayer[k] - 1) });
							if (iter == NoteMap.end()){ //下面不存在
								TempNote[i].dz = SingleBottomLayer[k - 1] - SingleTopLayer[TempNote[i].dz] + 1;
								noteDatas.push_back(TempNote[i]);
								break;
							}
							else{
								// 判断能否合并 Facies属性是否相同;
								int Index = iter->second;
								intArray TempFace2 = TempNote[Index].FaceLayers;//SGridData.FACIES[GetCecllId(TempNote[Index].i, TempNote[Index].j, TempNote[Index].k, NX, NY, NZ)];
								if (isMerge(TempFacies, TempFace2)){
									TempNote[Index].visted = true;
									if (k == Layers){
										TempNote[i].dz = SingleBottomLayer[k] - SingleTopLayer[TempNote[i].dz] + 1;
										noteDatas.push_back(TempNote[i]);
									}
								}
								else{
									TempNote[i].dz = SingleBottomLayer[k - 1] - SingleTopLayer[TempNote[i].dz] + 1;
									noteDatas.push_back(TempNote[i]);
									break;
								}
							}
						}
					}
				}
			}
		}
		else{
			for (int i = 0; i < Size; i++)
			{
				if (TempNote[i].k == TempNote[0].k){
					TempNote[i].dz = SingleBottomLayer[Layers] - SingleTopLayer[0] + 1;
					noteDatas.push_back(TempNote[i]);
				}
			}
		}
	}
	
	void DataProcess::Initlization(StrucGrid &SGridData)
	{
		// 排序树索引
		if (SGridData.Phasemaps.size() > 0)
		{
			for (int i = 0; i < (int)SGridData.Phasemaps.size(); i++)
			{
				sort(SGridData.Phasemaps[i].TopLayers.begin(), SGridData.Phasemaps[i].TopLayers.end());
				sort(SGridData.Phasemaps[i].BottomLayers.begin(), SGridData.Phasemaps[i].BottomLayers.end());
			}
			sort(SGridData.Phasemaps.begin(), SGridData.Phasemaps.end());
		}
		// 计算Cell八个点的数据
		NX = SGridData.NX; NY = SGridData.NY; NZ = SGridData.NZ;

		int nPillar = (SGridData.NX + 1)*(SGridData.NY + 1);
		isFaultPillar = new bool[nPillar];
		memset(isFaultPillar, false, sizeof(bool)*nPillar);
		getFaultPillar(NX, NY, SGridData.Zcorn, isFaultPillar);
		//UpdateXYCorrds(SGridData, dx/8, dy/8, isFaultPillar);
		SGridData.GenCellCorner();
		//UpdateZCorrds(SGridData, dx /4, dy / 4, isFaultPillar);
		

		NX = SGridData.NX; NY = SGridData.NY; NZ = SGridData.NZ;
		NXY = NX*NY;
		NXYZ = NX*NY*NZ;
		SGridData.NGrid = NXYZ;
		StageNum = (int)SGridData.Phasemaps.size();		
		currentType = 1;
		FMerge = SGridData.FMerge;
		// Keywords;
		string list = "BULKVOLUME DZMTRXV PORO PERMEABILITY SOIL FACIES";		
		keywords = split(list, " ");				
		SGridData.Flag = new bool[int(list.size())];
		memset(SGridData.Flag, false, sizeof(bool)* int(list.size()));
		int j;
		for (int i = 0; i < (int)SGridData.keywords.size(); i++)
		{			
			for (j = 0; j < keywords.size(); j++)
			{
				if (SGridData.keywords[i] == keywords[j])
				{
					SGridData.Flag[j] = true;
					break;
				}
			}
		}		
		if (!SGridData.Flag[2])
		{						
			SGridData.PORO = new double[NXYZ];
		}
		if (!SGridData.Flag[3])
		{
			SGridData.PERMEABILITY = new double[NXYZ];
		}
		if (!SGridData.Flag[4])
		{
			SGridData.SOIL = new double[NXYZ];
		}
		IsFacesMerge = false;
		if (!SGridData.Flag[5])
		{
			SGridData.FACIES = new int[NXYZ];			
		}
		else{
			IsFacesMerge = true;
		}
	}
	void DataProcess::SetParameter(int dxx, int dyy)
	{
		dx = dxx; dy = dyy;
		dxy = dx*dy;
		MM = new int[2 * dx*dy];
		for (int j = 0; j < dy; j++)
		{
			for (int i = 0; i < dx; i++)
			{
				MM[2 * (i + j*dx)] = i;
				MM[2 * (i + j*dx) + 1] = j;
			}
		}
	}
	void DataProcess::JudgeFault(int idx, int jdx, int *NegighBor, StrucGrid &SGridData, bool &firstflag, int dxx, int dyy)
	{
		//// 内部线; 
		int d2, d1, tempPillar;
		int c1 = NegighBor[0];
		for (d2 = 0; d2 <= dyy; d2++)
		{
			for (d1 = 0; d1 <= dxx; d1++)
			{
				tempPillar = (d2 + jdx) * (NX + 1) + d1 + idx;
				if (isFaultPillar[tempPillar]){
					firstflag = false;
					return;
				}
			}
		}
	}
	int DataProcess::GetCecllId(int i, int j, int k, int NX, int NY, int NZ)
	{
		return  k*NX*NY + NX*j + i;
	}
	void DataProcess::get_cell_facies(int i, int j, int Layer, int dxx, int dyy, int dzz, StrucGrid &SGridData, int *NegighBor, int *Facies, NodeDatas &noteDatas, bool isFine, BGGindex Index){
		int ATNUM, k = 0;
		ATNUM = 0;
		int NGrid = dxx*dyy;
		intArray FaceLayers;
		for (int m = 0; m < dyy; m++)
		{
			for (int n = 0; n < dxx; n++)
			{
				int Cell = GetCecllId(i + n, j + m, Layer, NX, NY, NZ);
				if (SGridData.ACTNUM[Cell])
				{
					NegighBor[k] = Cell;
					if (IsFacesMerge){
						Facies[ATNUM] = SGridData.FACIES[Cell];
					}
					ATNUM++;
				}
				else
				{
					NegighBor[k] = -1;
				}
				k++;
			}
		}
		FaceLayers.clear();
		JudgeFacies(Facies, ATNUM, FaceLayers, currentType); // 合并; 情形讨论; 
		noteDatas.push_back(nodeData(i, j, Layer, dxx, dyy, dzz, FaceLayers, isFine, Index));
	}

	void DataProcess::resize(StrucGrid &SGridData, StrucGrid &CSGridData, int NCellID)
	{
		if (NCellID > (NGrid - 1))
		{
			NGrid = 2 * NGrid;
			CPCell *tempCell = CSGridData.CPGCells;
			int *tempAtum = CSGridData.ACTNUM;
			double *tempSOIL = CSGridData.SOIL;
			double *tempPeam = CSGridData.PERMEABILITY;
			double *tempPoro = CSGridData.PORO;
			CSGridData.CPGCells = (CPCell*)malloc(NGrid*sizeof(CPCell));  //new CPCell[NGrid];
			CSGridData.ACTNUM = (int*)malloc(NGrid*sizeof(int));  //new int[CSGridData.NGrid];
			CSGridData.SOIL = (double*)malloc(NGrid*sizeof(double));  //new int[CSGridData.NGrid];
			CSGridData.PERMEABILITY = (double*)malloc(NGrid*sizeof(double));  //new int[CSGridData.NGrid];
			CSGridData.PORO = (double*)malloc(NGrid*sizeof(double));  //new int[CSGridData.NGrid];
			memcpy(CSGridData.CPGCells, tempCell, NCellID*sizeof(CPCell));
			memcpy(CSGridData.ACTNUM, tempAtum, NCellID*sizeof(int));
			memcpy(CSGridData.SOIL, tempSOIL, NCellID*sizeof(double));
			memcpy(CSGridData.PERMEABILITY, tempPeam, NCellID*sizeof(double));
			memcpy(CSGridData.PORO, tempPoro, NCellID*sizeof(double));
			free(tempCell);
			free(tempAtum);
			free(tempSOIL);
			free(tempPeam);
			free(tempPoro);
		}
	};

	void DataProcess::Merge2Grid(int TopLayer, int BottomLayer, StrucGrid &SGridData, StrucGrid &CSGridData, int mm, int *NegighBor, int &NCellID)
	{
		int Left = mm;
		int Right = (mm + 1) % 4;
		CSGridData.ACTNUM[NCellID] = 1;
		int TopCell, BottomCell;
		for (int m = 0; m < 2; m++)
		{
			if (m == 0)
			{
				TopCell = TopLayer*NX*NY + NegighBor[Left];
				BottomCell = TopLayer*NX*NY + NegighBor[Right];
			}
			else
			{
				TopCell = BottomLayer*NX*NY + NegighBor[Left];
				BottomCell = BottomLayer*NX*NY + NegighBor[Right];
			}
			int z2 = 4 * m;
			CSGridData.CPGCells[NCellID].corner[(Left - 1 + 4) % 4 + z2] = SGridData.CPGCells[TopCell].corner[(Left - 1 + 4) % 4 + z2];
			CSGridData.CPGCells[NCellID].corner[Left + z2] = SGridData.CPGCells[TopCell].corner[Left + z2];
			CSGridData.CPGCells[NCellID].corner[Right + z2] = SGridData.CPGCells[BottomCell].corner[Right + z2];
			CSGridData.CPGCells[NCellID].corner[(Right + 1) % 4 + z2] = SGridData.CPGCells[BottomCell].corner[(Right + 1) % 4 + z2];
		}
		NCellID++;
	};

	void DataProcess::Merger(int dxx, int dyy, int dzz, int iIndex, int JIndex, int KIndex, StrucGrid &SGridData, NodeDatas &noteDatas, map<BGGindex, int> &NoteMap, bool isFine, BGGindex Index)
	{
		int Ni = 1, Nj = 1;
		int Dxy;
		Dxy = dxx*dyy;
		int *NegighBor = new int[Dxy];
		int *Facies = new int[Dxy];
		bool flag = false;
		intArray FaceLayers;
		if (dxx > 1 && dyy > 1)
		{
			int dxx2 = dxx % 2;
			int dyy2 = dyy % 2;
			dxx = dxx / 2; dyy = dyy / 2;
			Ni = 2 * Ni; Nj = 2 * Nj;
			Dxy = dxx*dyy;
			for (int j = 0; j < (Nj); j++)
			{
				for (int i = 0; i < (Ni); i++)
				{
					int iDx = iIndex + dxx * i;
					int jDx = JIndex + dyy * j;
					GetNeighbor(iDx, jDx, KIndex, dxx, dyy, dzz, SGridData, NegighBor, Facies, noteDatas, NoteMap, isFine, Index);
				}
				if (dxx2 != 0){
					int iDx = iIndex + dxx * 2;
					int jDx = JIndex + dyy * j;
					GetNeighbor(iDx, jDx, KIndex, dxx2, dyy, dzz, SGridData, NegighBor, Facies, noteDatas, NoteMap, isFine, Index);
				}
			}
			if (dyy2 != 0){
				int jDx = JIndex + dyy * 2;
				for (int i = 0; i < (Ni); i++) // 不整除;
				{
					int iDx = iIndex + dxx * i;
					GetNeighbor(iDx, jDx, KIndex, dxx, dyy2, dzz, SGridData, NegighBor, Facies, noteDatas, NoteMap, isFine, Index);
				}
				if (dxx2 != 0)
				{
					int iDx = iIndex + dxx * 2;
					GetNeighbor(iDx, jDx, KIndex, dxx2, dyy2, dzz, SGridData, NegighBor, Facies, noteDatas, NoteMap, isFine, Index);
				}
			}
		}
		else if (dxx > 1){
			int dxx2 = dxx % 2;
			dxx = dxx / 2;
			Ni = 2 * Ni;
			Dxy = dxx;
			for (int i = 0; i < (Ni); i++)
			{
				int iDx = iIndex + dxx * i;
				int jDx = JIndex;
				GetNeighbor(iDx, jDx, KIndex, dxx, dyy, dzz, SGridData, NegighBor, Facies, noteDatas, NoteMap, isFine, Index);
			}
			if (dxx2 != 0){
				int iDx = iIndex + dxx * 2;
				int jDx = JIndex;
				GetNeighbor(iDx, jDx, KIndex, dxx2, dyy, dzz, SGridData, NegighBor, Facies, noteDatas, NoteMap, isFine, Index);
			}
		}
		else if (dyy > 1){
			int dyy2 = dyy % 2;
			dyy = dyy / 2;
			Nj = 2 * Nj;
			Dxy = dxx*dyy;
			for (int j = 0; j < (Nj); j++)
			{
				int iDx = iIndex;
				int jDx = JIndex + dyy * j;
				GetNeighbor(iDx, jDx, KIndex, dxx, dyy, dzz, SGridData, NegighBor, Facies, noteDatas, NoteMap, isFine, Index);
			};
			if (dyy2 != 0){
				int iDx = iIndex;
				int jDx = JIndex + dyy * 2;
				GetNeighbor(iDx, jDx, KIndex, dxx, dyy2, dzz, SGridData, NegighBor, Facies, noteDatas, NoteMap, isFine, Index);
			}
		}
		else{
			int ATNUM = 0;
			int Cell, k = 0;
			for (int j = 0; j < dyy; j++)
			{
				for (int i = 0; i < dxx; i++)
				{
					Cell = iIndex + i + (JIndex + j)*NX + KIndex*NX*NY;
					if (SGridData.ACTNUM[Cell] == 1)
					{
						Facies[ATNUM] = SGridData.FACIES[Cell];
						ATNUM++;
					}
				}
			}
			FaceLayers.clear();
			if (ATNUM > 0 && ATNUM == Dxy && JudgeAnotherFacies(Facies, ATNUM, FaceLayers))
			{
				NoteMap.insert(make_pair(BGGindex(iIndex, JIndex, KIndex), (int)noteDatas.size()));
				noteDatas.push_back(nodeData(iIndex, JIndex, KIndex, dxx, dyy, dzz, FaceLayers, isFine, Index));
				//FaceLayers.clear();
			}
			else
			{
				printf("Grid Process Error: ");
			}
		}
	}

	void DataProcess::TreeInitlization(MyTree &Tree, intArray TopLayers, intArray BottomLayers, vector<NodeDatas> nodeDatas, StrucGrid &SGridData)
	{
		int stage = 0;
		//创建树 void MyTree::createTree(TreeNode * &node, intArray TopLayers, intArray BottomLayers, NodeDatas nodeDatas, StrucGrid SGridData);
		int NumberChild = (int)nodeDatas.size();
		CPGNode nodeData;
		// nodeData.KIndex = TopLayers[0]-1; nodeData.dz = BottomLayers.back() - TopLayers[0]+1;
		nodeData.KIndex = 0; nodeData.dz = (int)TopLayers.size();
		Tree.root = new TreeNode(NumberChild, nodeData);
		Tree.flag = true;
		for (int i = 0; i < NumberChild; i++){
			CPGNode node1;
			node1.KIndex = i;
			Tree.root->childNotes[i] = new TreeNode((int)nodeDatas[i].size(), node1);
			for (int j = 0; j < nodeDatas[i].size(); j++){
				CPGNode node2;
				Merge(SGridData, nodeDatas[i][j], SGridData.NX, SGridData.NY, node2);
				Tree.root->childNotes[i]->childNotes[j] = new TreeNode(0, node2);
			}
		}
	}
	void DataProcess::Merge(StrucGrid &SGridData, nodeData NoteData, int NX, int NY, CPGNode &node)
	{
		//	// 合并 X,Y;
		double BULKVOLUME = 0, DZMTRXV = 0, PORO = 0, PERMEABILITY = 0, SOIL = 0;
		int *Facies,dd[4];		
		int temp = NoteData.i + NoteData.j*NX + NoteData.k*NX*NY;
		int temp2, BottomLayer = (NoteData.dz-1);
		dd[0] = temp;
		dd[1] = temp + NoteData.dx - 1;
		dd[2] = dd[1] + (NoteData.dy - 1)*NX;
		dd[3] = temp + (NoteData.dy - 1)*NX;
		int dxy = NoteData.dx*NoteData.dy;
		int NG = dxy*(NoteData.dz);
		int c2 = 0;
		Facies = new int[NG];
		int *Facies_count = new int[7];
		memset(Facies_count, 0, sizeof(int)* 7);
		for (int k = 0; k < NoteData.dz; k++)
		{
			for (int m = 0; m < NoteData.dy; m++)
			{
				for (int n = 0; n < NoteData.dx; n++)
				{
					temp2 = temp + n + m*NX + k*NX*NY;
					if (SGridData.ACTNUM[temp2] == 1){
						//BULKVOLUME;DZMTRXV;
						if (SGridData.Flag[2])
							PORO += SGridData.PORO[temp2];
						if (SGridData.Flag[3])
							PERMEABILITY += SGridData.PERMEABILITY[temp2];
						if (SGridData.Flag[4])
							SOIL += SGridData.SOIL[temp2];
						if (SGridData.Flag[5])
							Facies[c2] = SGridData.FACIES[temp2];
						c2++;
						Facies_count[SGridData.FACIES[temp2]]++;
					}
				}
			}
		}
		if (SGridData.Flag[2])
		{
			PORO = PORO / c2;
			node.PORO = PORO;
		}
		if (SGridData.Flag[3])
		{
			PERMEABILITY = PERMEABILITY / c2;
			node.PERMEABILITY = PERMEABILITY;
		}
		if (SGridData.Flag[4])
		{
			SOIL = SOIL / c2;
			node.SOIL = SOIL;
		}
		int max_value = 0;
		int tempid;
		for (int i = 0; i < 7; i++){
			if (Facies_count[i]>max_value){
				max_value = Facies_count[i];
				tempid = i;
			}
		}
		if (SGridData.Flag[5])
		{
			if ( NoteData.FaceLayers.size() > 0){ 
				if (NoteData.FaceLayers[0] == 0){  // L0D2
					if (NoteData.FaceLayers.size() == 2){
 						node.FACIES = SetFacies(FMerge, NoteData.FaceLayers[1], NoteData.FaceLayers[1]);
					}
					else{
						node.FACIES = SetFacies(FMerge, NoteData.FaceLayers[1], NoteData.FaceLayers[2]);
					}
					if (node.FACIES > 4){
						node.FACIES = get_max_facies(SGridData, NoteData, NX, NY);
					}
				}
				else{
					// L0D1
					node.FACIES = NoteData.FaceLayers[1];//MergeFace( Facies, c2);
					tempid = tempid > 0 ? 1 : 0;
					if (node.FACIES != tempid){
						node.FACIES = tempid;//cout << "属性合并出问题" << endl;
					}
				}
			}
			else
			{
				node.FACIES = MergeFace(Facies, c2);
			}
		}
		//生成粗化的网格
		node.ACTNUM = 1;
		for (int m = 0; m < 4; m++)
		{
			temp = dd[m];
			node.CPGCells.corner[m] = SGridData.CPGCells[temp].corner[m];
			temp = dd[m] + BottomLayer*NX*NY;
			node.CPGCells.corner[m+4] = SGridData.CPGCells[temp].corner[m + 4];
		}
		node.KIndex = NoteData.k; node.dz = NoteData.dz;
		node.dx = NoteData.dx; node.dy = NoteData.dy;
		node.IIndex = NoteData.i; node.JIndex = NoteData.j;
		node.BGindex = NoteData.Index; node.isFine = NoteData.isFine;		
		TreeNodeId++;
	}
	int DataProcess::MergeFace(int *Facies, int NumGrid){
		int Face1, Face2;
		if (currentType == 0){
			for (int i = 0; i < NumGrid; i++)
			{
				if (Facies[i] != Facies[0]){
					Face1 = Facies[i]<Facies[0] ? Facies[i] : Facies[0];
					Face2 = Facies[i]>Facies[0] ? Facies[i] : Facies[0];
					for (int j = 0; j < (int)FMerge.size(); j++){
						if (FMerge[j].i == Face1 && FMerge[j].j == Face2){
							return FMerge[j].k;
						}
					}
					//printf("NG merger Error");
					//return Facies[0]; // error;
				}
			}
		}
		else{
			int t1 = 0, t2 = 0;
			for (int i = 0; i < NumGrid; i++)
			{
				if (Facies[i] == 0){
					t1++;
				}
				else if (Facies[i]<5){
					t2++;
				}
			}
			if (t1 == t2 && t1 == 0){
				return 1;
			}
			else if (t1>t2){
				return 0;
			}
			else if (t1 < t2){
				return 1;
			}
		}
		return Facies[0];
	}

	int get_max_facies(StrucGrid &SGridData, nodeData NoteData, int NX, int NY){
		int k_layer = NoteData.k - 1;
		if (k_layer < 0){
			k_layer = NoteData.k + NoteData.dz;
		}
		int temp = k_layer*NX*NY+ NoteData.i+ NoteData.j *NX;
		int temp2;
		int *facie_count = new int[4];
		memset(facie_count,0,sizeof(int)*4);
		for (int m = 0; m < NoteData.dy; m++)
		{
			for (int n = 0; n < NoteData.dx; n++)
			{
				temp2 = temp + n + m*NX;
				if (SGridData.ACTNUM[temp2] == 1){
					facie_count[SGridData.FACIES[temp2]]++;
				}
			}
		}
		int facie_idx=0;
		int max_count = 0;
		for (int i = 0; i<4; i++){
			if (facie_count[i]>max_count){
				facie_idx = i;
				max_count = facie_count[i];
			}
		}
		return facie_idx;
	}

	int SetFacies(vector<FaciesMerge> FMerge, int Face1, int Face2){
		if (Face1 > Face2){
			int TempFace = Face1;
			Face1 = Face2;
			Face2 = TempFace;
		}
		for (int dd = 0; dd < (int)FMerge.size(); dd++){
			if( FMerge[dd].i == Face1 && FMerge[dd].j == Face2){
				return FMerge[dd].k;
			}
		}
		if (Face1 != Face2){
			return Face2;
		}
		return Face1;
	}

	void testFunction(int i1, int j1, int dx, int dy, int NX, int NY){
		for (int d1 = 0; d1 < dx; d1++){
			for (int d2 = 0; d2 < dy; d2++){
				int tempidx = NX*(d2 + j1) + NX*NY + i1 + d1;
				if (tempidx == 187784){
					printf("0");
				}
			}
		}
	}


	
	// 获取垂向间隙;
	void DataProcess::get_Zcoorn_Pillar(StrucGrid &SGridData, int NX, int NY, int NZ, int dx, int dy, int dz, vector<NodeDatas> noteDatas, FracPillars &PostPillars){
		int dxx = NX % dx;
		int dyy = NY % dy;
		int NPillar = (NY + 1)*(NX + 1);
		map<Pillarindex3, CellIdxProps> PillarMap;
		std::map<Pillarindex3, CellIdxProps>::iterator iter;
		for (int i = 0; i < noteDatas.size(); i++){
			for (int j = 0; j < noteDatas[i].size(); j++){
				for (int xx = 0; xx < 2; xx++){ // 上下底面;
					int Kidx = noteDatas[i][j].k + xx * noteDatas[i][j].dz;
					int ddx = noteDatas[i][j].dx;
					int ddy = noteDatas[i][j].dy;
					int idx = noteDatas[i][j].i;
					int jdx = noteDatas[i][j].j;
					// 遍历 dx,dy 所有的索引;
					for (int dd = 0; dd <= ddx; dd++){
						int temp_i = idx + dd;
						for (int mm = 0; mm <= ddy; mm++){
							int temp_j = jdx + mm;
							bool isEndPt = isInCellSide(dd, mm, ddx, ddy);
							AddPillarMap3d(PillarMap, temp_i, temp_j, Kidx, i,j, iter, isEndPt);
						}
					}
				}
			}
		}
		iter = PillarMap.begin();
		NodeDatas tempNotes;
		while (iter != PillarMap.end()){
			if ( iter->second.size() >= 2 ){ // 从中挑出
				for (int c_ = 0; c_ < iter->second.size(); c_++){
					tempNotes.push_back(noteDatas[iter->second[c_].i][iter->second[c_].j]);
				};
				Update_Zcorns( iter->first ,iter->second,NX,NY,NZ, tempNotes,SGridData);
				tempNotes.clear();
			};
			iter++;
		};
		PillarMap.clear();
	};

	// 更新柱线;
	void DataProcess::updateCoord(StrucGrid &SGridData, int NX, int NY, int NZ, int dx, int dy, int dz, NodeDatas TempnoteDatas, FracPillars &PostPillars){
		int dxx = NX % dx;
		int dyy = NY % dy;
		int NPillar = (NY + 1)*(NX + 1);
		map<Pillarindex, CellProps> PillarMap;
		std::map<Pillarindex, CellProps>::iterator iter;
		for (int i = 0; i < TempnoteDatas.size(); i++){
			//如果为细分网格；
			int ddx = TempnoteDatas[i].dx;
			int ddy = TempnoteDatas[i].dy;
			// if (ddx < dx && ddy < dy){  // 细分网格;
			int idx = TempnoteDatas[i].i;
			int jdx = TempnoteDatas[i].j;
			// 遍历 dx,dy 所有的索引;
			for (int dd = 0; dd <= ddx; dd++){
				int temp_i = ddx *dd;
				for (int mm = 0; mm <= ddy; mm++){
					int temp_j = ddy *mm;
					bool isEndPt = isCellEndPt(dd, mm, ddx, ddy);
					AddPillarMap(PillarMap, idx + dd, jdx + mm, i, iter, isEndPt);
				}
			}
		}
		iter = PillarMap.begin(); 
		NodeDatas tempNotes;
		while (iter != PillarMap.end()){
			if (iter->second.size() == 3){ // 从中挑出 ArraySize==3; 
				// UpdateCoords(iter->first.i, iter->first.j, 1, 1, NX, NY, SGridData, isFaultPillar); 
				for (int c_ = 0; c_ < iter->second.size(); c_++){
					tempNotes.push_back(TempnoteDatas[iter->second[c_].cellIdx]);
				};
				UpdateXY_Coords( iter->first, iter->second, tempNotes,PostPillars);
				// NX, NY, SGridData, isFaultPillar
				tempNotes.clear();
			};
			iter++;
		};
		PillarMap.clear();
	};
	
	// 更新 Coords X,Y Direction; (只更新 Top，未考虑断层;)
	void DataProcess::UpdateCoords(int i, int j, int dxx, int dyy, int Nx, int Ny, StrucGrid &SGridData, bool *isFaultPillar){
		int ii, jj;
		int statIdx;
		point startPt, endPt;
		int	dd[4][2];
		point *Pts = new point[4];
		point crossPt;
		int  L1;
		int FaultIdx;
		ii = i - i%dx;
		jj = j - j%dy;
		ii = (ii == Nx ? ii - 1 : ii);
		jj = (jj == Ny ? jj - 1 : jj);
		int temp = (ii + jj * (Nx + 1)) * 6;
		// 断层判断; 返回;
		for (int mm = ii; mm <= ii + dx; mm++){
			for (int nn = jj; nn <= jj + dy; nn++){
				if (mm <= Nx && nn <= Ny){
					int PillarIdx = mm + nn *(Nx + 1);
					if (isFaultPillar[PillarIdx]){
						return;
					}
				}
			}
		}
		for (int mm = ii; mm < ii + dx; mm++){
			for (int nn = jj; nn < jj + dy; nn++){
				if (mm < Nx && nn < Ny){
					int CellIdx = mm + nn *Nx;
					if (SGridData.ACTNUM[CellIdx] != 1){
						return;
					}
				}
			}
		}
		for (int xx = 0; xx < 2; xx++){
			for (int mm = 0; mm < 4; mm++){
				if (mm % 2 == 0){       //X方向上柱线;
					jj = mm / 2 * dyy;
					for (int ii = 0; ii <= dxx; ii++){
						if (setPillarIdxs(isFaultPillar, ii + i, jj + j, dx, dy, Nx, Ny, dd)){
							for (int Np = 0; Np < 4; Np++){
								statIdx = (dd[Np][0] + dd[Np][1] * (Nx + 1)) * 6 + 3 * xx;
								Pts[Np].x = SGridData.Coord[statIdx];
								Pts[Np].y = SGridData.Coord[statIdx + 1];
								Pts[Np].z = SGridData.Coord[statIdx + 2];
								//LinesArray.push_back(Pts[Np]);
							}
							FaultIdx = i + ii + (j + jj) * (Nx + 1);
							L1 = (FaultIdx)* 6 + 3 * xx;
							point v1(SGridData.Coord[L1], SGridData.Coord[L1 + 1], SGridData.Coord[L1 + 2]);
							//PillarPtArray.push_back(v1);
							if (!isFaultPillar[FaultIdx]){
								setPillarPts(Pts, crossPt, v1, SGridData.Coord, L1);
							}
						}
					}
				}
				else{
					if (mm == 1){
						ii = dxx;
					}
					else{
						ii = 0;
					}
					// ofstream outPutText("测试")
					for (int jj = 0; jj <= dyy; jj++){
						FaultIdx = (i + ii + (j + jj) * (Nx + 1));
						L1 = FaultIdx * 6 + 3 * xx;
						point v1(SGridData.Coord[L1], SGridData.Coord[L1 + 1], SGridData.Coord[L1 + 2]);
						if (setPillarIdxs(isFaultPillar, ii + i, jj + j, dx, dy, Nx, Ny, dd)){
							for (int Np = 0; Np < 4; Np++){
								statIdx = (dd[Np][0] + dd[Np][1] * (Nx + 1)) * 6 + 3 * xx;
								Pts[Np].x = SGridData.Coord[statIdx];  
								Pts[Np].y = SGridData.Coord[statIdx + 1];
								Pts[Np].z = SGridData.Coord[statIdx + 2];
								// LinesArray.push_back(Pts[Np]);
							}
							if (!isFaultPillar[FaultIdx]){
								setPillarPts(Pts, crossPt, v1, SGridData.Coord, L1);
							}
						}
					}
				}
			}
		}
	}

	// 更新 Coords X,Y Direction; (2019-1-12)
	void DataProcess::UpdateXY_Coords(Pillarindex pillar, CellProps cells, NodeDatas nodeDatas, FracPillars &PostPillars){
		int EndCellIdx = -1;
		int dt_x = 1E5, dt_y = 1E5;
		// 获取EndPt;
		for (int i = 0; i < nodeDatas.size(); i++){
			if (!cells[i].isEndPt){
				EndCellIdx = i;
			}
			else{
				dt_x = dt_x < nodeDatas[i].dx ? dt_x : nodeDatas[i].dx;
				dt_y = dt_y < nodeDatas[i].dy ? dt_y : nodeDatas[i].dy;
			}
		}
		if (EndCellIdx >= 0){
			// 获取两条柱线;
			int index;
			int temp[4][2] = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };
			int left_temp[4][2] = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };
			if (nodeDatas[EndCellIdx].i == pillar.i){
				index = 3; 
			};
			if (nodeDatas[EndCellIdx].j == pillar.j){
				index = 0;
			};
			if (nodeDatas[EndCellIdx].i + nodeDatas[EndCellIdx].dx == pillar.i){
				index = 1;
			};
			if (nodeDatas[EndCellIdx].j + nodeDatas[EndCellIdx].dy == pillar.j){
				index = 2;
			};
			// 两条柱线;
			vector<Pillarindex> pillarIdxs;
			for (int i = 0; i < 2; i++){
				int p2 = (index + i) % 4;
				pillarIdxs.push_back(Pillarindex(nodeDatas[EndCellIdx].i + nodeDatas[EndCellIdx].dx * temp[p2][0],
					nodeDatas[EndCellIdx].j + nodeDatas[EndCellIdx].dy * temp[p2][1]));
			};
			pillarIdxs.push_back(Pillarindex(pillar.i + dt_x * left_temp[index][0],
				pillar.j + dt_y * left_temp[index][1]));
			pillarIdxs.push_back(pillar);
			PostPillars.push_back(FracPillar(pillar, pillarIdxs, nodeDatas ));
		}
	}

	// 更新 Zcorns Direction; (2019-1-29);
	void DataProcess::Update_Zcorns(Pillarindex3 pillars, CellIdxProps cells, int NX, int NY, int NZ, NodeDatas nodeDatas, StrucGrid &SGridData) {
		int EndCellIdx = -1;
		int dt_x = 0, dt_y = 0,max_range_idx=-1;
		bool isEndFlag = false;
		// 获取EndPt;
		for (int i = 0; i < nodeDatas.size(); i++){
			if (!cells[i].isEndPt){
				if (dt_x < nodeDatas[i].dx && dt_y < nodeDatas[i].dy){
					dt_x = nodeDatas[i].dx;
					dt_y = nodeDatas[i].dy;
					max_range_idx = i;
				}
			}
			else{
				isEndFlag = true;
				EndCellIdx = i;
			}
		}
		vector<Pillarindex3> cellpillas;
		point crossPts;
		point *Pts = new point[6];
		int tempArray[4][2] = { { 0, 0 }, { 1, 0 }, { 1, 1 }, {0,1} };
		if ( max_range_idx >= 0 && isEndFlag){  // 表示有端点且不一致;
			if (nodeDatas[EndCellIdx].dx < nodeDatas[max_range_idx].dx && nodeDatas[EndCellIdx].dy < nodeDatas[max_range_idx].dy){
				if (nodeDatas[EndCellIdx].k != nodeDatas[max_range_idx].k){
					//获取两条柱线;
					cellpillas.push_back(Pillarindex3(pillars.i, pillars.j, nodeDatas[max_range_idx].k));
					cellpillas.push_back(Pillarindex3(pillars.i, pillars.j, nodeDatas[EndCellIdx].k));
					for (int dd = 0; dd < 4; dd++){
						int tempI = nodeDatas[max_range_idx].i + nodeDatas[max_range_idx].dx*tempArray[dd][0];
						int tempJ = nodeDatas[max_range_idx].j + nodeDatas[max_range_idx].dy*tempArray[dd][1];
						cellpillas.push_back(Pillarindex3(tempI, tempJ, nodeDatas[max_range_idx].k));
					}
					//ofstream testPlanePts("test_Plane .txt");
					bool is_flag = true;
					for (int dd = 0; dd < 6; dd++){
						int tempCellIdx = cellpillas[dd].i + cellpillas[dd].j *NX + cellpillas[dd].k *NX*NY;
						if (cellpillas[dd].i >= SGridData.NX || cellpillas[dd].j >= SGridData.NY || cellpillas[dd].k >= SGridData.NZ){
							is_flag = false;
							break;
						}
						Pts[dd] = SGridData.CPGCells[tempCellIdx].corner[0];
						//testPlanePts << Pts[dd].x << " " << Pts[dd].y << " " << Pts[dd].z << endl;
					}
					if (is_flag){
						get_plane_crossPts(Pts[0], Pts[1], Pts[2], Pts[3], Pts[4], crossPts);
						// testPlanePts << crossPts.x << " " << crossPts.y << " " << crossPts.z << endl;
						// testPlanePts.close();
						int cellId = pillars.i + pillars.j*NX + nodeDatas[max_range_idx].k *NX *NY;
						SGridData.CPGCells[cellId].corner[0] = crossPts;
						SGridData.CPGCells[cellId - 1].corner[1] = crossPts;
						SGridData.CPGCells[cellId - NX].corner[3] = crossPts;
						SGridData.CPGCells[cellId - NX - 1].corner[2] = crossPts;
						cellId = pillars.i + pillars.j*NX + nodeDatas[EndCellIdx].k *NX *NY;
						SGridData.CPGCells[cellId].corner[0] = crossPts;
						SGridData.CPGCells[cellId - 1].corner[1] = crossPts;
						SGridData.CPGCells[cellId - NX].corner[3] = crossPts;
						SGridData.CPGCells[cellId - NX - 1].corner[2] = crossPts;
					}
				}
			}
			// pillarIdxs.push_back(Pillarindex(pillar.i + dt_x * left_temp[index][0],
			// 	pillar.j + dt_y * left_temp[index][1]));
			// pillarIdxs.push_back(pillar);
			// PostPillars.push_back(FracPillar(pillar, pillarIdxs, nodeDatas));
		}
	};

	void DataProcess::Set_XY_Coords(FracPillars PostPillar, int Nx, int Ny, StrucGrid &SGridData, bool *isFaultPillar){
		point *Pts = new point[4];
		point crossPt;
		Pillarindex pillar;
		vector<Pillarindex> pillarIdxs;
		PointArray Lines;
		int Pillar_Idx, L1, cellIdx1, cellIdx2, CellIdx, delta_spacing;
		int right_temp[4][2] = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };
		for (int ii = 0; ii < PostPillar.size(); ii++){
			pillar = PostPillar[ii].pillar;
			pillarIdxs = PostPillar[ii].pillars;
			Pillar_Idx = (pillar.i + pillar.j * (Nx + 1));
			for (int xx = 0; xx < 2; xx++){
				bool isValid = true;
				L1 = Pillar_Idx * 6 + 3 * xx;
				point v1(SGridData.Coord[L1], SGridData.Coord[L1 + 1], SGridData.Coord[L1 + 2]);// v1 为原始点;
				int statIdx;
				for (int Np = 0; Np < 4; Np++){
					statIdx = (pillarIdxs[Np].i + pillarIdxs[Np].j * (Nx + 1)) * 6 + 3 * xx;
					if (pillarIdxs[Np].i >= SGridData.NX || pillarIdxs[Np].j >= SGridData.NY){
						isValid = false;
						break;
					}
					Pts[Np].x = SGridData.Coord[statIdx];
					Pts[Np].y = SGridData.Coord[statIdx + 1];
					Pts[Np].z = SGridData.Coord[statIdx + 2];
				}
				if (isValid){
					PostPillar[ii].isValid = setPillarPts(Pts, crossPt, v1, SGridData.Coord, L1);
				}
				else{
					PostPillar[ii].isValid = isValid;
				}
				Lines.push_back(crossPt);
			}
			// 更新Z坐标;
			if (PostPillar[ii].isValid){
				Set_zcoords(pillar.i, pillar.j, SGridData.NX, SGridData.NY, SGridData.NZ, SGridData.Coord, SGridData.Zcorn, SGridData.CPGCells);
			}
		}
		//SGridData.GenCellCorner();
		for (int ii = 0; ii < PostPillar.size(); ii++){
			if (PostPillar[ii].isValid){
				pillar = PostPillar[ii].pillar;
				pillarIdxs = PostPillar[ii].pillars;
				// ofstream Pts_txt("pillar.txt");
				// for (int xx = 0; xx < 2; xx++){
				// 	Pts_txt << Lines[2 * ii + xx].x << " " << Lines[2 * ii + xx].y << " " << Lines[2 * ii + xx].z << endl;
				// }
				// Pts_txt.close();
				for (int layer = 0; layer <= SGridData.NZ; layer++){
					if (layer < SGridData.NZ){
						cellIdx1 = layer*Nx*Ny + pillarIdxs[0].j * Nx + pillarIdxs[0].i;
						cellIdx2 = layer*Nx*Ny + pillarIdxs[1].j * Nx + pillarIdxs[1].i;
						CellIdx = layer*Nx*Ny + pillar.i + pillar.j* Nx;
						delta_spacing = 0;
					}
					else{
						delta_spacing = 4;
					}
					// cellIdx1 = (pillarIdxs[0].j * (Nx + 1) + pillarIdxs[0].i) * 6;
					// cellIdx2 = (pillarIdxs[1].j * (Nx + 1) + pillarIdxs[1].i) * 6;
					// point Pt3(SGridData.Coord[cellIdx1], SGridData.Coord[cellIdx1 + 1], SGridData.Coord[cellIdx1 + 2]);
					// point Pt4(SGridData.Coord[cellIdx2], SGridData.Coord[cellIdx2 + 1], SGridData.Coord[cellIdx2 + 2]);
					L1 = (pillar.i + pillar.j*(Nx + 1)) * 6;
					int ZLoc = layer * 2 * NX * 2 * NY * 2 + pillar.j * 4 * NX + 2 * pillar.i;
					// ofstream corner_Txt("Test_corner.txt");
					// corner_Txt << SGridData.CPGCells[cellIdx1].corner[delta_spacing].x << " " << SGridData.CPGCells[cellIdx1].corner[delta_spacing].y << " " << SGridData.CPGCells[cellIdx1].corner[delta_spacing].z << endl;
					// corner_Txt << SGridData.CPGCells[cellIdx2].corner[delta_spacing].x << " " << SGridData.CPGCells[cellIdx2].corner[delta_spacing].y << " " << SGridData.CPGCells[cellIdx2].corner[delta_spacing].z << endl;
					// corner_Txt.close();
					// Intersect_3d_lines(Lines[2*ii], Lines[2*ii+1], Pt3, Pt4, crossPt);
					if (SGridData.ACTNUM[cellIdx1] && SGridData.ACTNUM[cellIdx2] && SGridData.ACTNUM[CellIdx]){
						if (Intersect_3d_lines(SGridData.CPGCells[cellIdx1].corner[delta_spacing], SGridData.CPGCells[cellIdx2].corner[delta_spacing], Lines[2 * ii], Lines[2 * ii + 1], crossPt)){
							//	//	// 更新Z-coords;
							// 8 个点坐标;
							// point p1 = SGridData.CPGCells[CellIdx].corner[0 + delta_spacing];
							if (SGridData.ACTNUM[CellIdx]){
								SGridData.CPGCells[CellIdx].corner[0 + delta_spacing] = crossPt;
							}
							if (SGridData.ACTNUM[CellIdx - 1]){
								SGridData.CPGCells[CellIdx - 1].corner[1 + delta_spacing] = crossPt;
							}
							if (SGridData.ACTNUM[CellIdx - Nx]){
								SGridData.CPGCells[CellIdx - Nx].corner[3 + delta_spacing] = crossPt;
							}
							if (SGridData.ACTNUM[CellIdx - Nx - 1]){
								SGridData.CPGCells[CellIdx - Nx - 1].corner[2 + delta_spacing] = crossPt;
							}
						}
						//SGridData.Zcorn[ZLoc - 1] = crossPt.z;
						//SGridData.Zcorn[ZLoc - 2*Nx] = crossPt.z;
						//SGridData.Zcorn[ZLoc - 2 * Nx -1 ] = crossPt.z;
						/*
						Zcorn[ZLoc + 1], c.corner[1]);
						c.is_act &= Intercept(Line3[0], Line3[1], Zcorn[ZLoc + 2 * NX], c.corner[3]);
						c.is_act &= Intercept(Line4[0], Line4[1], Zcorn[ZLoc + 2 * NX + 1], c.corner[2]);
						c.is_act &= Intercept(Line1[0], Line1[1], Zcorn[ZLoc + 4 * NX*NY], c.corner[4]);
						c.is_act &= Intercept(Line2[0], Line2[1], Zcorn[ZLoc + 4 * NX*NY + 1], c.corner[5]);
						c.is_act &= Intercept(Line3[0], Line3[1], Zcorn[ZLoc + 4 * NX*NY + 2 * NX], c.corner[7]);
						c.is_act &= Intercept(Line4[0], Line4[1], Zcorn[ZLoc + 4 * NX*NY + 2 * NX + 1], c.corner[6]);
						*/
					}
				}
			}
		}
		Lines.clear();
		delete[] Pts;
	}

	void DataProcess::Set_zcoords(int i, int j, int NX, int NY, int NLayer, double* Coord, double* Zcorn, CPCell* CPGCells){
		int L1, L2, L3, L4, ZLoc;
		point Line1[2], Line2[2], Line3[2], Line4[2];
		
		L1 = (i + j*(NX + 1)) * 6;
		Line1[0].x = Coord[L1];   Line1[0].y = Coord[L1 + 1]; Line1[0].z = Coord[L1 + 2];
		Line1[1].x = Coord[L1 + 3]; Line1[1].y = Coord[L1 + 4]; Line1[1].z = Coord[L1 + 5];

		L2 = (i + 1 + j*(NX + 1)) * 6;
		Line2[0].x = Coord[L2];   Line2[0].y = Coord[L2 + 1]; Line2[0].z = Coord[L2 + 2];
		Line2[1].x = Coord[L2 + 3]; Line2[1].y = Coord[L2 + 4]; Line2[1].z = Coord[L2 + 5];

		L3 = (i + (j + 1)*(NX + 1)) * 6;
		Line3[0].x = Coord[L3];   Line3[0].y = Coord[L3 + 1]; Line3[0].z = Coord[L3 + 2];
		Line3[1].x = Coord[L3 + 3]; Line3[1].y = Coord[L3 + 4]; Line3[1].z = Coord[L3 + 5];

		L4 = (i + 1 + (j + 1)*(NX + 1)) * 6;
		Line4[0].x = Coord[L4];   Line4[0].y = Coord[L4 + 1]; Line4[0].z = Coord[L4 + 2];
		Line4[1].x = Coord[L4 + 3]; Line4[1].y = Coord[L4 + 4]; Line4[1].z = Coord[L4 + 5];
		for (int k = 0; k < NLayer; k++) {
			CPCell &c = CPGCells[i + j*NX + k*NX*NY];
			ZLoc = k * 2 * NX * 2 * NY * 2 + j * 4 * NX + 2 * i;

			c.is_act &= Intercept(Line1[0], Line1[1], Zcorn[ZLoc], c.corner[0]);
			c.is_act &= Intercept(Line2[0], Line2[1], Zcorn[ZLoc + 1], c.corner[1]);
			c.is_act &= Intercept(Line3[0], Line3[1], Zcorn[ZLoc + 2 * NX], c.corner[3]);
			c.is_act &= Intercept(Line4[0], Line4[1], Zcorn[ZLoc + 2 * NX + 1], c.corner[2]);
			c.is_act &= Intercept(Line1[0], Line1[1], Zcorn[ZLoc + 4 * NX*NY], c.corner[4]);
			c.is_act &= Intercept(Line2[0], Line2[1], Zcorn[ZLoc + 4 * NX*NY + 1], c.corner[5]);
			c.is_act &= Intercept(Line3[0], Line3[1], Zcorn[ZLoc + 4 * NX*NY + 2 * NX], c.corner[7]);
			c.is_act &= Intercept(Line4[0], Line4[1], Zcorn[ZLoc + 4 * NX*NY + 2 * NX + 1], c.corner[6]);
		}
	}


	void UpdateZ_Coords(Pillarindex pillar, CellProps cells, NodeDatas nodeDatas, int Nx, int Ny, StrucGrid &SGridData, bool *isFaultPillar){
		int EndCellIdx = -1;
		point crossPt;
		point *Pts = new point[4];
		int dt_x = 0, dt_y = 0;
		// 获取EndPt;
		for (int i = 0; i < nodeDatas.size(); i++){
			if (!cells[i].isEndPt){
				EndCellIdx = i;
			}
			else{
				dt_x = dt_x >nodeDatas[i].dx ? dt_x : nodeDatas[i].dx;
				dt_y = dt_y > nodeDatas[i].dy ? dt_y : nodeDatas[i].dy;
			}
		}
		if (EndCellIdx >= 0){
			// 获取两条柱线;
			int index;
			int temp[4][2] = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };
			int left_temp[4][2] = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };
			if (nodeDatas[EndCellIdx].i == pillar.i){
				index = 3;
			}
			if (nodeDatas[EndCellIdx].j == pillar.j){
				index = 0;
			}
			if (nodeDatas[EndCellIdx].i + nodeDatas[EndCellIdx].dx == pillar.i){
				index = 1;
			}
			if (nodeDatas[EndCellIdx].j + nodeDatas[EndCellIdx].dy == pillar.j){
				index = 2;
			};
			// 两条柱线;
			vector<Pillarindex> pillarIdxs;
			for (int i = 0; i < 2; i++){
				int p2 = (index + i) % 4;
				pillarIdxs.push_back(Pillarindex(nodeDatas[EndCellIdx].i + nodeDatas[EndCellIdx].dx * temp[p2][0],
					nodeDatas[EndCellIdx].j + nodeDatas[EndCellIdx].dy * temp[p2][1]));
			}
			pillarIdxs.push_back(Pillarindex(pillar.i + dt_x * left_temp[index][0],
				pillar.j + dt_y * left_temp[index][1]));
			pillarIdxs.push_back(pillar);
			for (int xx = 0; xx < 2; xx++){
				int Pillar_Idx = (pillar.i + pillar.j * (Nx + 1));
				int L1 = Pillar_Idx * 6 + 3 * xx;
				point v1(SGridData.Coord[L1], SGridData.Coord[L1 + 1], SGridData.Coord[L1 + 2]);
				// 生成Pts;
				int statIdx;
				//ofstream Pts_txt("pillar.txt");
				for (int Np = 0; Np < 4; Np++){
					statIdx = (pillarIdxs[Np].i + pillarIdxs[Np].j * (Nx + 1)) * 6 + 3 * xx;
					Pts[Np].x = SGridData.Coord[statIdx];
					Pts[Np].y = SGridData.Coord[statIdx + 1];
					Pts[Np].z = SGridData.Coord[statIdx + 2];
					//Pts_txt << Pts[Np].x << " " << Pts[Np].y << endl;
				}
				//Pts_txt.close();
				// 求交点更新位置;
				// v1 为原始点;
				setPillarPts(Pts, crossPt, v1, SGridData.Coord, L1);
			}
		}
		delete[] Pts;
	}

	void DataProcess::getFaultPillar(int Nx, int Ny, double *Zcorn, bool *isFaultPillar){
		int k = 0;
		int ZLoc, tempLoc, pillarIdx;
		int dd[3];
		double Zvalue;
		dd[0] = -1;
		dd[1] = -2 * Nx;
		dd[2] = -2 * Nx - 1;
		// 不包括边界点  ????
		for (int i = 1; i < Nx; i++){
			for (int j = 1; j < Ny; j++){
				pillarIdx = i + j* (Nx + 1);
				ZLoc = k * 2 * Nx * 2 * Ny * 2 + j * 4 * Nx + 2 * i; // 0;
				Zvalue = Zcorn[ZLoc];
				for (int ii = 0; ii < 3; ii++){
					tempLoc = ZLoc + dd[ii];
					if (abs(Zcorn[tempLoc] - Zvalue)> smallFloat){
						isFaultPillar[pillarIdx] = true;
						break;
					}
				}
			}
		}
	}

	
	void AddPillarMap(map<Pillarindex, CellProps> &PillarMap, int i, int j, int cellIdx, std::map<Pillarindex, CellProps>::iterator iter, bool isEndPt){
		Pillarindex pillarIdx(i, j);
		iter = PillarMap.find({ pillarIdx });
		CellProp temp(cellIdx, isEndPt);
		if (iter == PillarMap.end()){
			CellProps Cellstemp;
			Cellstemp.push_back(temp);
			PillarMap.insert(make_pair(pillarIdx, Cellstemp));
		}
		else{
			iter->second.push_back(temp);
		}
	}
	void AddPillarMap3d(map<Pillarindex3, CellIdxProps> &PillarMap, int i, int j, int k, int cellI, int CellJ, std::map<Pillarindex3, CellIdxProps>::iterator iter, bool isEndPt){
		Pillarindex3 pillarIdx(i, j,k);
		iter = PillarMap.find({ pillarIdx });
		CellProp2 temp(cellI, CellJ, isEndPt);
		if (iter == PillarMap.end()){
			CellIdxProps Cellstemp;
			Cellstemp.push_back(temp);
			PillarMap.insert(make_pair(pillarIdx, Cellstemp));
		}
		else{
			iter->second.push_back(temp);
		}
	}
	bool isCellEndPt(int dd, int mm, int ddx, int ddy){
		if (dd != 0 && dd != ddx){
			return false;
		}
		if (mm != 0 && mm != ddy){
			return false;
		}
		return true;
	}
	bool isInCellSide(int dd, int mm, int ddx, int ddy){
		if (dd == 0 || dd == ddx){
			return true;
		}
		if (mm == 0 || mm == ddy){
			return true;
		}
		return false;
	}
	bool setPillarIdxs(bool *isFaultPillar, int i, int j, int dx, int dy, int Nx, int Ny, int(*dd)[2]){
		/*
			
		*/
		int ii, jj;
		int i_max, j_max;
		ii = i - i%dx;
		jj = j - j%dy;
		ii = (ii == Nx ? ii - 1 : ii);
		jj = (jj == Ny ? jj - 1 : jj);
		i_max = (ii + dx) > Nx ? Nx : (ii + dx);
		j_max = (jj + dy) > Ny ? Ny : (jj + dy);
		dd[0][0] = i; dd[0][1] = jj;
		dd[1][0] = i; dd[1][1] = j_max;
		dd[2][0] = ii; dd[2][1] = j;
		dd[3][0] = i_max; dd[3][1] = j;
		return true;
	}
	bool setPillarPts(point *Pts, point &crossPt, point v1, double *Coord, int L1){
		if (intersection(Pts[0], Pts[1], Pts[2], Pts[3], crossPt)){
			point tempPt = crossPt - v1;
			tempPt.z = 0;
			double distance = VxNorm(tempPt);
			for (int dd = 0; dd < 4; dd++){
				tempPt = Pts[dd] - crossPt;
				if (VxNorm(tempPt) < 1E-16){
					return false;
				}
			}
			if (distance > InfSmall){
				Coord[L1] = crossPt.x;
				Coord[L1 + 1] = crossPt.y;
				Coord[L1 + 2] = crossPt.z;
			}
			return true;
		}
		else{
			return false;
		}
	}
	
	// 更新Zcoords;
	void update(){

		// Zcorn[index]; // 四个方向全部更新;
	}


	void output_grid(StrucGrid &SGrid, int nx, int ny, int nz, int k_start){
		// 输出Coords;
		cout << "输出网格: " << nx << " "<< ny << " "<<  nz << " ，"<<  endl;
		ofstream CoordsTxt("coord.txt");
		int Nx, Ny, Nz, PillarIdx;
		Nx = SGrid.NX;  Ny = SGrid.NY;  Nz = SGrid.NZ;
		int nPillar = (nx + 1)*(ny + 1);
		CoordsTxt << "COORD                                  -- Generated : Petrel" << endl;
		for (int j = 0; j <= ny; j++){
			for (int i = 0; i <= nx; i++){
				PillarIdx = (i + j* (Nx + 1));
				for (int mm = 0; mm < 6; mm++){
					CoordsTxt << setprecision(9) << SGrid.Coord[6 * PillarIdx + mm] << " ";
				}
			}
		}
		CoordsTxt << endl;
		CoordsTxt.close();
		ofstream ZcornsTxt("Zcorns.txt");
		ZcornsTxt << "ZCORN                                  -- Generated : Petrel" << endl;
		int nGrid = nx * ny * nz;
		for (int k = 0; k < 2 * nz; k++) {
			for (int j = 0; j < 2 * ny; j++) {
				for (int i = 0; i < 2 * nx; i++) {
					int ZLoc =(k + k_start) * Nx * 2 * Ny * 2 + j * 2 * Nx + i;
					ZcornsTxt << setprecision(9) << SGrid.Zcorn[ZLoc] << " ";
				}
			}
		}
		ZcornsTxt << endl;
		ZcornsTxt.close();
		ofstream ACTNUMTxt("ACTNUM.txt");
		ofstream PoroTxt("Poro.txt");
		ofstream PermTxt("Perm.txt");
		ofstream SoilTxt("Soil.txt");
		ofstream FaciesTxt("Facies.txt");
		ACTNUMTxt << "ACTNUM" << endl;
		PoroTxt << "PORO                                   -- Generated : Petrel" << endl;
		PermTxt << "PERMEABILITY                           -- Generated : Petrel" << endl;
		SoilTxt << "SOIL                                   -- Generated : Petrel" << endl;
		FaciesTxt << "FACIES" << endl;
		for (int k = 0; k < nz; k++) {
			for (int j = 0; j < ny; j++){
				for (int i = 0; i < nx; i++){
					int idx = i + j*Nx + (k + k_start) *Nx*Ny;
					ACTNUMTxt << setprecision(9) << SGrid.ACTNUM[idx] << " ";
					PoroTxt << setprecision(9) << SGrid.PORO[idx] << " ";
					PermTxt << setprecision(9) << SGrid.PERMEABILITY[idx] << " ";
					SoilTxt << setprecision(9) << SGrid.SOIL[idx] << " ";
					FaciesTxt << setprecision(9) << SGrid.FACIES[idx] << " ";
				}
			}
		}
		ACTNUMTxt << endl;  PoroTxt << endl; PermTxt << endl;
		SoilTxt << endl; FaciesTxt << endl;
		PoroTxt.close();
		PermTxt.close();
		SoilTxt.close();
		FaciesTxt.close();
		ACTNUMTxt.close();
	}

	void getLayer_Layer(intArray TopLayers, intArray BottomLayers, int TopIndex, int BottomIndex, int &iIndex, int &j){
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
			if (BottomLayers[i] >= BottomIndex){
				j = i;
				break;
			}
		}
	}

};