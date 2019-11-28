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
*  @file     GridProcess.h													 *
*  @brief    Grid Process for Scaling										 *
*  Details.                                                                  *
*                                                                            *
*  @author   kailanghuang                                                    *
*  @email    kailanghuang@pku.edu.cn                                         *
*  @version  2.0.0.1		                                                 *
*  @date     2018/01, 2019/1												 *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         :  Grid Process for Scaling								 *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/
#ifndef GRIDPROCESS__
#define GRIDPROCESS__
#include"Gridstruct.h"
#include"Tree.h"
#include"IJKGrid.h"
#include <fstream>

namespace UMSM{	
	void getSingleLayer(intArray & SingleLayerTop, intArray & SingleLayerBottom, vector<PhaseMap> Phasemaps, int TopLayer, int BottomLayer);
	void getLayer(intArray TopLayers, intArray BottomLayers, int TopIndex, int BottomIndex, int &iIndex, int &j);
	void getLayer_Layer(intArray TopLayers, intArray BottomLayers, int TopIndex, int BottomIndex, int &iIndex, int &j);
	int SetFacies(vector<FaciesMerge> FMerge, int Face1, int Face2);
	void DeleteFracture(CPCell & Cell1, CPCell & Cell2, int type);
	void SetUpDown(TreeNode *Note, intArray Cells, int jIdx, bool type);
	void AddFace(CPGNode note, FaceNode & Face, int *Left);
	void testFunction(int i1, int j1, int dx, int dy, int NX, int NY);
	int get_max_facies(StrucGrid &SGridData, nodeData NoteData, int NX, int NY);

	// output Grid;
	void output_grid(StrucGrid &SGrid, int nx, int ny, int nz, int k_start);

	/*
		* Core Class for Data Processing
	*/
	class DataProcess
	{
	public:
		DataProcess(){};
		DataProcess(StrucGrid &SGridData, MyTree &Tree);
		~DataProcess();
		bool isMerge(intArray Face1, intArray Face2);
		bool isFaces_Merge(intArray Face1, intArray Face2);
		bool isLayer_merge(nodeData t1, nodeData t2);
	private:
		int NX,NY,NZ;
		int NXY;
		int NXYZ;
		int dx, dy;
		int dxy;
		int *MM;
		int NGrid;
		int StageNum;
		vector<string> keywords;
		vector<FaciesMerge> FMerge;
		
		int TreeNodeId=0;
		int currentType;
		bool IsFacesMerge;

		void Initlization(StrucGrid &SGridData);
		void SetParameter(int dxx, int dyy);
		bool JudgeFacies(int *Facies, int NGrid, intArray &Layers, int Type);
		bool JudgeAnotherFacies(int *Facies, int NGrid, intArray &Layers);
		void UpdateCellNeighbor(TreeNode *Note, StrucGrid &SGridData);
		void FindNeighbor(TreeNode *Note, intArray Cells, int jIdx);
		void addCelsMap(BGGindex tempIndex, int dx, int dy, intArray &Cells);
		void addCelsMap(BGGindex tempIndex, int dz, intArray &Cells);

		int MergeFace(int *Facies, int NumGrid);
		void Merge2Grid(int TopLayer, int BottomLayer, StrucGrid &SGridData, StrucGrid &CSGridData, int mm, int *NegighBor, int &NCellID);
		void Merge1Grid(int TopLayer, int BottomLayer, StrucGrid &SGridData, StrucGrid &CSGridData, int CellId, int &NCellID);
		int GetCecllId(int i, int j, int k, int NX, int NY, int NZ);
		void get_cell_facies(int i, int j, int Layer, int dxx, int dyy, int dzz, StrucGrid &SGridData, int *NegighBor, int *Facies, NodeDatas &noteDatas, bool isFine, BGGindex Index);

		void MergeGrid(StrucGrid &SGridData, vector<NodeDatas> &noteDatas);
		void GetNeighbor(int i, int j, int Layer, int dxx, int dyy, int dzz, StrucGrid &SGridData, int *NegighBor, int *Facies, NodeDatas &noteDatas, map<BGGindex, int> &NoteMap, bool isFine, BGGindex Index);
		void Merger(int dxx, int dyy, int dzz, int iIndex, int JIndex, int KIndex, StrucGrid &SGridData, NodeDatas &noteDatas, map<BGGindex, int> &NoteMap, bool isFine, BGGindex Index);
		void DownMergeGrid(StrucGrid &SGridData, NodeDatas &noteDatas, NodeDatas &TempNote, intArray SingleTopLayer, intArray SingleBottomLayer, map<BGGindex, int> &NoteMap, bool isDivided);
		
		
		void DownMergeGrid2(StrucGrid &SGridData, NodeDatas &noteDatas, NodeDatas &TempNote, intArray SingleTopLayer, intArray SingleBottomLayer, map<BGGindex, int> &NoteMap, bool isDivided);
		
		void JudgeFault(int idx, int jdx, int *NegighBor, StrucGrid &SGridData, bool &firstflag, int dxx, int dyy);
		void Merge(StrucGrid &SGridData, nodeData NoteData, int NX, int NY, CPGNode &node);

		void resize(StrucGrid &SGridData, StrucGrid &CSGridData, int NCellID);
		// Tree Operation
		void TreeInitlization(MyTree &Tree, intArray TopLayers, intArray BottomLayers, vector<NodeDatas> nodeDatas, StrucGrid &SGridData);
		void TreeMake(int Stage, StrucGrid &SGridData, int dxx, int dyy, TreeNode *Tree);
		void TreeChildInit(int Stage, intArray TopLayers, intArray BottomLayers, TreeNode *Note, StrucGrid &SGridData, int dxx, int dyy);
		void TreeChildInit2(int Stage, intArray TopLayers, intArray BottomLayers, TreeNode *Note, StrucGrid &SGridData, int dxx, int dyy);

		// Deal with Pillar Fractures
		void get_Zcoorn_Pillar(StrucGrid &SGridData, int NX, int NY, int NZ, int dx, int dy, int dz, vector<NodeDatas> noteDatas, FracPillars &PostPillars);
		void updateCoord(StrucGrid &SGridData, int NX, int NY, int NZ, int dx, int dy, int dz, NodeDatas TempnoteDatas, FracPillars &PostPillars);
		void UpdateXY_Coords(Pillarindex pillar, CellProps cells, NodeDatas nodeDatas, FracPillars &PostPillars);
		void Set_XY_Coords(FracPillars PostPillar, int Nx, int Ny, StrucGrid &SGridData, bool *isFaultPillar);
		void Set_zcoords(int i, int j, int NX, int NY, int NLayer, double* Coord, double* Zcorn, CPCell* CPGCells);

		void UpdateCoords(int i, int j, int dxx, int dyy, int Nx, int Ny, StrucGrid &SGridData, bool *isFaultPillar);
		void Update_Zcorns(Pillarindex3 pillars, CellIdxProps cells, int NX, int NY, int NZ, NodeDatas nodeDatas, StrucGrid &SGridData);
		void getFaultPillar(int Nx, int Ny, double *Zcorn, bool *isFaultPillar);


		bool *isFaultPillar;

	};
	
	bool setPillarPts(point *Pts, point &crossPt, point v1, double *Coord, int L1);
	void AddPillarMap(map<Pillarindex, CellProps> &PillarMap, int i, int j, int cellIdx, std::map<Pillarindex, CellProps>::iterator iter, bool isEndPt);
	void AddPillarMap3d(map<Pillarindex3, CellIdxProps> &PillarMap, int i, int j, int k, int cellI,int CellJ, std::map<Pillarindex3, CellIdxProps>::iterator iter, bool isEndPt);
	bool isCellEndPt(int dd, int mm, int ddx, int ddy);
	bool isInCellSide(int dd, int mm, int ddx, int ddy);
	bool setPillarIdxs(bool *isFaultPillar, int i, int j, int dx, int dy, int Nx, int Ny, int(*dd)[2]);

}
#endif