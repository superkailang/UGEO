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
*  @file     Gridstruct.h													 *
*  @brief    Grid Struct													 *
*  Details.                                                                  *
*                                                                            *
*  @author   kailanghuang                                                    *
*  @email    kailanghuang@pku.edu.cn                                         *
*  @version  2.0.0.1		                                                 *
*  @date     2018/01, 2019/1												 *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         :  Grid Struct											 *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

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

	typedef vector<int> intArray; // Define the vector container for int 
	typedef vector<double> realArray; // Define the vector container for double 

	// 3d Point definition
	struct point
	{
		double x, y, z;
		point() {};
		point(const double _x, const double _y, const double _z)
		{
			x = _x; y = _y; z = _z;
		};
	};
	typedef vector<point> PointArray;//Define the vector container points

	struct WellPath  // Define borehole trajectory
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

		point corner[8]; // corner points
		point facec[6];
		point blockc;
		bool is_act;
		point AreaVec(const int dir);
		double Gen_GeomCenter(); // return volume
	};

	struct Layer // define Layer
	{
		int TopLayer;
		int BottomLayer;
	};
	struct PhaseMap // define map for sedimentary facies
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

	struct BGGindex{   // define Grid Index
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
		// (i,j,k) -> original Grid
		int IIndex;
		int JIndex;
		int KIndex;
		int dx, dy, dz;
		BGGindex BGindex;
		NeightBor neightbor;
		bool isFine;
		bool isActive=true;
		bool isChange = false;
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

	struct nodeData  // nodeData for Structed Tree node
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

	// 3D sketching For Grid Face
	struct FaceNode
	{
		point Point[4];
		BGGindex index;		
		int ACTNUM;           // 0 is inactive, 1 is active Grid
		double BULKVOLUME;	  // BULK VOLUME
		double DZMTRXV;		  // 
		double PORO;		  // Porosity 
		double PERMEABILITY;  // Pemearbility 
		double SOIL;		  // Oil Saturation
		int FACIES;			  // sedimentary Facies
	};

	// Pillar index of Grid
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