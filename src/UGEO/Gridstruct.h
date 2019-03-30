#ifndef STRUCTS__
#define STRUCTS__
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <math.h>
#include <algorithm>
#include <iomanip>
#include <map>
#include <set>
#include <stack>
#include"Constants.h"

using namespace std;
namespace UMSM{
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&); \
	void operator=(const TypeName&)

	typedef vector<int> intArray;//定义int类的vector容器  
	typedef vector<double> realArray;//定义double类的vector容器

	struct point
	{
		double x, y, z;
		point() {};
		point(const double _x, const double _y, const double _z)
		{
			x = _x; y = _y; z = _z;
		};
	};
	typedef vector<point> PointArray;//定义点类的vector容器

	struct WellPath  //定义井眼轨迹
	{
		char wellname[MAX_STRING_LEN];
		realArray MD;
		realArray Incline;
		realArray Azimuth;
		vector<point> WellTraject; // E Direction  y; // N Direction z; // TVD Direction
	};
	typedef vector<WellPath>Wells;

	struct CPCell
	{
		CPCell() :is_act(true) {};

		point corner[8]; // 核心
		point facec[6];
		point blockc;
		bool is_act;
		point AreaVec(const int dir);
		double Gen_GeomCenter(); // return volume
	};

	struct Layer
	{
		int TopLayer;
		int BottomLayer;
	};
	struct PhaseMap
	{
		intArray TopLayers;
		intArray BottomLayers;
		int stage;
		bool operator <(const  PhaseMap& other) const{
			if (stage != other.stage){ return(stage < other.stage); }
			else{
				return(TopLayers[0] < other.TopLayers[0]);
			}
		}
	};
	struct NeightBor
	{
		intArray left;
		intArray right;
		intArray up;
		intArray down;
		intArray top;
		intArray Bottom;
	};

	struct BGGindex{
		int i;
		int j;
		int k;
		BGGindex() {};
		BGGindex(const int _i, const int _j, const int _Layer)
		{
			i = _i; j = _j; k = _Layer;
		};
		bool operator <(const  BGGindex& other) const{
			if (i != other.i){ return(i< other.i); }
			else if (j != other.j){ return(j < other.j); }
			else{ return(k < other.k); }
		}
	};

	struct CPGNode
	{
		// 对应原始数据
		int IIndex;
		int JIndex;
		int KIndex;
		int dx, dy, dz;
		BGGindex BGindex; // ?
		NeightBor neightbor;
		bool isFine;
		bool isActive=true;
		bool isChange = false;
		// 保存现在数据
		CPCell CPGCells;
		int ACTNUM;
		double BULKVOLUME;
		double DZMTRXV;
		double PORO;
		double PERMEABILITY;
		double SOIL;
		int FACIES;
		CPGNode(){ 
			isChange = false; 
			IIndex = 0; JIndex = 0; KIndex = 0;
			dx = 0; dy = 0; dz = 0;
		};
	};

	struct nodeData
	{
		int i;
		int j;
		int k;
		int dx;
		int dy;
		int dz;
		bool visted;
		bool isFine;
		BGGindex Index;
		intArray FaceLayers;
		nodeData() {};
		nodeData(const int _i, const int _j, const int _k, const int _dx, const int _dy, const int _dz, const intArray _FaceLayers, bool _isFine, BGGindex _Index)
		{
			i = _i; j = _j; k = _k; dx = _dx; dy = _dy; dz = _dz; FaceLayers = _FaceLayers;
			visted = false;
			isFine = _isFine;
			Index = _Index;
		};
	};

	typedef vector<nodeData> NodeDatas;

	struct ZoneData
	{
		int NumberCell;
		CPGNode *Nodes;
		ZoneData(){};
		ZoneData(const int NG){
			NumberCell = NG;
			Nodes = new CPGNode[NG];			
		};
	};
	struct FaciesMerge
	{
		int i;
		int j;
		int k;
		FaciesMerge(){
			i = -1;
			j = -1;
		};
	};

	// 三维绘制
	struct FaceNode
	{
		// 对应原始数据
		point Point[4];
		BGGindex index;
		// 保存现在数据		
		int ACTNUM;
		double BULKVOLUME;
		double DZMTRXV;
		double PORO;
		double PERMEABILITY;
		double SOIL;
		int FACIES;
	};

	// 处理间隙
	struct Pillarindex{
		int i;
		int j;
		Pillarindex() {};
		Pillarindex(const int _i, const int _j){
			i = _i; j = _j;
		};
		bool operator <(const  Pillarindex& other) const{
			if (i != other.i){ return(i < other.i); }
			else { return(j < other.j); }
		}
	};
	struct Pillarindex3{
		int i;
		int j;
		int k;
		Pillarindex3() {};
		Pillarindex3(const int _i, const int _j, const int _k){
			i = _i; j = _j; k = _k;
		};
		bool operator <(const  Pillarindex3& other) const{
			if (i != other.i){ return(i < other.i); }
			else if (j != other.j){ return(j < other.j); }
			else{ return(k < other.k); }
		}
	};
	struct CellProp{
		int cellIdx;
		bool isEndPt;
		CellProp() {};
		CellProp(const int _cellIdx, const bool _isEndPt){
			cellIdx = _cellIdx; isEndPt = _isEndPt;
		};
	};
	struct CellProp2{
		int i;
		int j;
		bool isEndPt;
		CellProp2() {};
		CellProp2(const int _i, const int _j, const bool _isEndPt){
			i = _i; j = _j; isEndPt = _isEndPt;
		};
	};
	typedef vector<CellProp> CellProps;
	typedef vector<CellProp2> CellIdxProps;

	struct FracPillar{
		Pillarindex pillar;
		vector<Pillarindex> pillars;
		NodeDatas nodes;
		bool isValid;
		FracPillar(const Pillarindex _pillar, const vector<Pillarindex>  _cells, const NodeDatas _nodes){
			pillar = _pillar;
			pillars = _cells;
			nodes = _nodes;
			isValid = true;
		};
	};

	typedef vector<FracPillar> FracPillars;
}
#endif