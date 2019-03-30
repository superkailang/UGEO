#include"Tree.h"
#include"IJKGrid.h"
#include<queue>
#include"Gridplot.h"
namespace UMSM
{
	MyTree::MyTree(){
		//root->NumberChild = 0;
	};
	MyTree::~MyTree(){ };

	void MyTree::Initlization(intArray TopLayers, intArray BottomLayers, NodeDatas nodeDatas, StrucGrid SGridData)
	{
		int stage = 0;
		//int childNum = (int)phasemaps[stage].TopLayers.size();
		//int TopLayer = phasemaps[0].TopLayers[0];
		//int BottomLayer = phasemaps[0].BottomLayers.back();		
		//root->NodeData.TopLayer = TopLayer;
		//root->NodeData.BottomLayer = BottomLayer;		
		//创建树
		//void MyTree::createTree(TreeNode * &node, intArray TopLayers, intArray BottomLayers, NodeDatas nodeDatas, StrucGrid SGridData)		
		int TopIndex = -1, BottomIndex = -2;
		int NumberLayers = (int)BottomLayers.size();
		int NumberChild = (int)nodeDatas.size()*NumberLayers;
		CPGNode nodeData;
		nodeData.KIndex = TopLayers[0]; nodeData.dz = BottomLayers.back() - TopLayers[0]+1;
		root = new TreeNode(NumberChild, nodeData);		
		for (int i = 0; i < NumberLayers; i++)
		{
			TopIndex = TopLayers[i]; BottomIndex = BottomLayers[i];
			for (int j = 0; j < NumberChild; j++)
			{
				CPGNode node1;
				//Merge(TopIndex, BottomIndex, SGridData, nodeDatas[i], SGridData.NX, SGridData.NY, node1);
				root->childNotes[i*NumberChild + j] = new TreeNode(0, node1);
			}
		}
	}
	
	void MyTree::Merge(int TopLayer, int BottomLayer, StrucGrid SGridData, nodeData NoteData, int NX, int NY, CPGNode &node)
	{
		//	// 合并 X,Y;
		double BULKVOLUME = 0, DZMTRXV = 0, PORO = 0, PERMEABILITY = 0, SOIL = 0;
		int dd[4];
		int temp = NoteData.i + NoteData.j*NX;
		int temp2;
		dd[0] = temp;
		dd[1] = temp + NoteData.dx - 1;
		dd[2] = temp + (NoteData.dy - 1)*NX;
		dd[3] = dd[2] + NoteData.dx - 1;
		int dxy = NoteData.dx*NoteData.dy;
		int NG = dxy*(BottomLayer - TopLayer + 1);
		for (int k = TopLayer; k <= BottomLayer; k++)
		{
			for (int m = 0; m < NoteData.dy; m++)
			{
				for (int n = 0; n < NoteData.dx; n++)
				{
					temp2 = temp + n + m*NX + k*NX*NY;
					//BULKVOLUME;DZMTRXV; 
					if (SGridData.Flag[2])
						PORO += SGridData.PORO[temp];
					if (SGridData.Flag[3])
						PERMEABILITY += SGridData.PERMEABILITY[temp];
					if (SGridData.Flag[4])
						SOIL += SGridData.SOIL[temp];
				}
			}
		}
		if (SGridData.Flag[2])
		{
			PORO = PORO / NG;
			node.PORO = PORO;
		}
		if (SGridData.Flag[3])
		{
			PERMEABILITY = PORO / NG;
			node.PERMEABILITY = PERMEABILITY;
		}
		if (SGridData.Flag[4])
		{
			SOIL = SOIL / NG;
			node.SOIL = SOIL;
		}
		//生成粗化的网格
		node.ACTNUM = 1;
		for (int m = 0; m < 4; m++)
		{
			temp = dd[m] + TopLayer*NX*NY;
			node.CPGCells.corner[m] = SGridData.CPGCells[temp].corner[m];
			temp = dd[m] + BottomLayer*NX*NY;
			node.CPGCells.corner[m + 4] = SGridData.CPGCells[temp].corner[m + 4];
		}
	}
	
	////求树的深度
	int MyTree::depth(TreeNode * &node)
	{
		if (flag)
		{
			if (node == NULL)
				return -1;
			if (node->NumberChild > 0)
			{
				int h = depth(node->childNotes[0]);
				return h + 1;
			}
			return 0;
		}	
		return -1;
	}
	
	ZoneData MyTree::GetZoneData(int Depth)
	{
		preOrder(0, Depth, root);
		int NG = (int)Treeque.size();
		ZoneData zoneData(NG);
		int k = 0;
		while (!Treeque.empty())
		{
			TreeNode *tempNote = Treeque.front();
			zoneData.Nodes[k] = tempNote->NodeData;			
			k++;
			Treeque.pop();
		}
		return zoneData;
	}
	vector<ZoneData> MyTree::GetZoneData(int Depth, PhaseMap Layers)
	{		
		preOrder(0, Depth,root);
		int NG = (int)Treeque.size(),k=0,NumLayers=(int)Layers.TopLayers.size();
		int *NumCount = new int[NumLayers];
		vector<ZoneData> zoneDatas;
		TreeNode *tempNote;
		memset(NumCount, 0, sizeof(int)*NumLayers);
		for (int i = 0; i < NumLayers; i++){
			ZoneData zoneData(NG);
			zoneDatas.push_back(zoneData);
		}
		for (int kk = 0; kk < NG;kk++)
		{
			tempNote = Treeque.front();
			for (int mm = 0; mm < NumLayers; mm++){
				int TopLayer = (Layers.TopLayers[mm] - 1), BottomLayer = Layers.BottomLayers[mm] - 1;
				if (tempNote->NodeData.KIndex >= TopLayer && tempNote->NodeData.KIndex <= BottomLayer){
					k = NumCount[mm];
					zoneDatas[mm].Nodes[k] = tempNote->NodeData;					
					NumCount[mm]++;
					break;
				}
			}
			Treeque.pop();
		}
		for (int mm = 0; mm < NumLayers; mm++){
			zoneDatas[mm].NumberCell = NumCount[mm];
		}
		return zoneDatas;
	}
	
	void MyTree::preOrder(int level, int depth, TreeNode *note)
	{
		if (note == NULL)
		{
			return;
		}
		if (level == depth)
		{
			Treeque.push(note);
		}
		else if (level < depth){
			for (int i = 0; i < note->NumberChild; i++)
			{
				preOrder(level + 1, depth, note->childNotes[i]);
			}
		}
	}		
	////计算单位长度，为查找点做准备
	//int cal(int num)
	//{
	//	int result = 1;
	//	if (1 == num)
	//		result = 1;
	//	else
	//	{
	//		for (int i = 1; i < num; i++)
	//			result = 2 * result;
	//	}
	//	return result;
	//}
	//
	////查找点
	//int maxdepth = 0;
	//int times = 0;
	//static double xMin = 0, xMax = 0, yMin = 0, yMax = 0, zMin = 0, zMax = 0;
	//int tmaxdepth = 0;
	//double txm = 1, tym = 1, tzm = 1;
	//
	//template<class T>
	//void find(OctreeNode<T> *& p, double x, double y, double z)
	//{
	//	double xMind = (p->xMax - p->xMin) / 2;
	//	double yMind = (p->yMax - p->yMin) / 2;
	//	double zMind = (p->yMax - p->yMin) / 2;
	//	times++;
	//	if (x > xMax || x<xMin || y>yMax || y<yMin || z>zMax || z < zMin)
	//	{
	//		cout << "该点不在场景中！" << endl;
	//		return;
	//	}
	//	if (x <= p->xMin + txm && x >= p->xMax - txm && y <= p->yMin + tym &&
	//		y >= p->yMax - tym && z <= p->zMin + tzm && z >= p->zMax - tzm)
	//	{
	//		cout << endl << "找到该点！" << "该点位于" << endl;
	//		cout << " xMin: " << p->xMin << " xMax: " << p->xMax;
	//		cout << " yMin: " << p->yMin << " yMax: " << p->yMax;
	//		cout << " zMin: " << p->zMin << " zMax: " << p->zMax;
	//		cout << "节点内！" << endl;
	//		cout << "共经过" << times << "次递归！" << endl;
	//	}
	//	else if (x < (p->xMax - xMind) && y < (p->yMax - yMind) && z < (p->zMax - zMind))
	//	{
	//		cout << "当前经过节点坐标：" << endl;
	//		cout << " xMin: " << p->xMin << " xMax: " << p->xMax;
	//		cout << " yMin: " << p->yMin << " yMax: " << p->yMax;
	//		cout << " zMin: " << p->zMin << " zMax: " << p->zMax;
	//		cout << endl;
	//		find(p->bottom_left_back, x, y, z);
	//	}
	//	else if (x < (p->xMax - xMind) && y<(p->yMax - yMind) && z>(p->zMax - zMind))
	//	{
	//		cout << "当前经过节点坐标：" << endl;
	//		cout << " xMin: " << p->xMin << " xMax: " << p->xMax;
	//		cout << " yMin: " << p->yMin << " yMax: " << p->yMax;
	//		cout << " zMin: " << p->zMin << " zMax: " << p->zMax;
	//		cout << endl;
	//		find(p->top_left_back, x, y, z);
	//	}
	//	else if (x >(p->xMax - xMind) && y < (p->yMax - yMind) && z < (p->zMax - zMind))
	//	{
	//		cout << "当前经过节点坐标：" << endl;
	//		cout << " xMin: " << p->xMin << " xMax: " << p->xMax;
	//		cout << " yMin: " << p->yMin << " yMax: " << p->yMax;
	//		cout << " zMin: " << p->zMin << " zMax: " << p->zMax;
	//		cout << endl;
	//		find(p->bottom_right_back, x, y, z);
	//	}
	//	else if (x >(p->xMax - xMind) && y<(p->yMax - yMind) && z>(p->zMax - zMind))
	//	{
	//		cout << "当前经过节点坐标：" << endl;
	//		cout << " xMin: " << p->xMin << " xMax: " << p->xMax;
	//		cout << " yMin: " << p->yMin << " yMax: " << p->yMax;
	//		cout << " zMin: " << p->zMin << " zMax: " << p->zMax;
	//		cout << endl;
	//		find(p->top_right_back, x, y, z);
	//	}
	//	else if (x<(p->xMax - xMind) && y>(p->yMax - yMind) && z < (p->zMax - zMind))
	//	{
	//		cout << "当前经过节点坐标：" << endl;
	//		cout << " xMin: " << p->xMin << " xMax: " << p->xMax;
	//		cout << " yMin: " << p->yMin << " yMax: " << p->yMax;
	//		cout << " zMin: " << p->zMin << " zMax: " << p->zMax;
	//		cout << endl;
	//		find(p->bottom_left_front, x, y, z);
	//	}
	//	else if (x<(p->xMax - xMind) && y>(p->yMax - yMind) && z >(p->zMax - zMind))
	//	{
	//		cout << "当前经过节点坐标：" << endl;
	//		cout << " xMin: " << p->xMin << " xMax: " << p->xMax;
	//		cout << " yMin: " << p->yMin << " yMax: " << p->yMax;
	//		cout << " zMin: " << p->zMin << " zMax: " << p->zMax;
	//		cout << endl;
	//		find(p->top_left_front, x, y, z);
	//	}
	//	else if (x > (p->xMax - xMind) && y > (p->yMax - yMind) && z < (p->zMax - zMind))
	//	{
	//		cout << "当前经过节点坐标：" << endl;
	//		cout << " xMin: " << p->xMin << " xMax: " << p->xMax;
	//		cout << " yMin: " << p->yMin << " yMax: " << p->yMax;
	//		cout << " zMin: " << p->zMin << " zMax: " << p->zMax;
	//		cout << endl;
	//		find(p->bottom_right_front, x, y, z);
	//	}
	//	else if (x >(p->xMax - xMind) && y > (p->yMax - yMind) && z > (p->zMax - zMind))
	//	{
	//		cout << "当前经过节点坐标：" << endl;
	//		cout << " xMin: " << p->xMin << " xMax: " << p->xMax;
	//		cout << " yMin: " << p->yMin << " yMax: " << p->yMax;
	//		cout << " zMin: " << p->zMin << " zMax: " << p->zMax;
	//		cout << endl;
	//		find(p->top_right_front, x, y, z);
	//	}
	//}
	//
	//
	////main函数
	//int main()
	//{
	//	OctreeNode<double> * rootNode = NULL;
	//	int choiced = 0;
	//	while (true)
	//	{
	//		system("cls");
	//		cout << "请选择操作：\n";
	//		cout << "1.创建八叉树 2.先序遍历八叉树\n";
	//		cout << "3.查看树深度 4.查找节点   \n";
	//		cout << "0.退出\n\n";
	//		cin >> choiced;
	//		if (choiced == 0)
	//		{
	//			return 0;
	//		}
	//		else if (choiced == 1)
	//		{
	//			system("cls");
	//			cout << "请输入最大递归深度：" << endl;
	//			cin >> maxdepth;
	//			cout << "请输入外包盒坐标，顺序如下：xMin,xMax,yMin,yMax,zMin,zMax" << endl;
	//			cin >> xMin >> xMax >> yMin >> yMax >> zMin >> zMax;
	//			if (maxdepth >= 0 || xMax > xMin || yMax > yMin || zMax > zMin || xMin > 0 || yMin > 0 || zMin > 0)
	//			{
	//				tmaxdepth = cal(maxdepth);
	//				txm = (xMax - xMin) / tmaxdepth;
	//				tym = (yMax - yMin) / tmaxdepth;
	//				tzm = (zMax - zMin) / tmaxdepth;
	//				createOctree(rootNode, maxdepth, xMin, xMax, yMin, yMax, zMin, zMax);
	//			}
	//			else
	//			{
	//				cout << "输入错误！";
	//				return 0;
	//			}
	//		}
	//		else if (choiced == 2)
	//		{
	//			system("cls");
	//			cout << "先序遍历八叉树结果：\n";
	//			i = 1;
	//			preOrder(rootNode);
	//			cout << endl;
	//			system("pause");
	//		}
	//		else if (choiced == 3)
	//		{
	//			system("cls");
	//			int dep = depth(rootNode);
	//			cout << "此八叉树的深度为" << dep + 1 << endl;
	//			system("pause");
	//		}
	//		else if (choiced == 4)
	//		{
	//			system("cls");
	//			cout << "请输入您希望查找的点的坐标，顺序如下：x,y,z\n";
	//			double x, y, z;
	//			cin >> x >> y >> z;
	//			times = 0;
	//			cout << endl << "开始搜寻该点……" << endl;
	//			find(rootNode, x, y, z);
	//			system("pause");
	//		}
	//		else
	//		{
	//			system("cls");
	//			cout << "\n\n错误选择！\n";
	//			system("pause");
	//		}
	//	}
};