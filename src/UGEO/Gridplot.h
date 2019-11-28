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
*  @file     Girdplot.h														 *
*  @brief    3D Grid Plot Function 											 *
*  Details.                                                                  *
*                                                                            *
*  @author   kailanghuang                                                    *
*  @email    kailanghuang@pku.edu.cn                                         *
*  @version  2.0.0.1		                                                 *
*  @date     2018/01, 2019/1												 *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         :  3d Gird plot Function									 *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

#ifndef GRIDPLOT_h__
#define GRIDPLOT_h__
#include"BaseFunction.h"
#include"Gridstruct.h"
#include"Constants.h"

#include"vtkPointData.h"
#include"vtkActor.h"
#include"vtkPoints.h"
#include"vtkCellArray.h"
#include"vtkPolyData.h"
#include"vtkPolyDataMapper.h"
#include"vtkColorTransferFunction.h"
#include"vtkProperty.h"
#include"vtkDoubleArray.h"
#include"vtkCellData.h"
#include"vtkLookupTable.h"

#include"Tree.h"
#include"Struct.h"
namespace UMSM {

	void AddPoints(vector<FaceNode> FaceNodes, vtkSmartPointer<vtkPoints> points);
	int AddPoints(CPGNode *Cell, int NumberGrid, vtkSmartPointer<vtkPoints> points);

	void GetGridBoundary(StrucGrid &SGrid, vector<FaceNode> &FaceNodes);
	void GetOrginalFace(int depth, int Layer, TreeNode *Note, StrucGrid &SGrid, vector<FaceNode> &FaceNodes);
	// void GetOrginalBoundary2(TreeNode *Note, StrucGrid &SGrid, vector<FaceNode> &FaceNodes, bool **Cellarray);
	void GetOrginalBoundary(TreeNode *Note, StrucGrid &SGrid, vector<FaceNode> &FaceNodes);


	void GetTreeNodeBoundary(int depth, int Layer, TreeNode *Note, StrucGrid &SGrid, vector<FaceNode> &FaceNodes);
	void GetTreeNode(TreeNode *Note, StrucGrid &SGrid, vector<FaceNode> &FaceNodes);
	// void GetTreeNode2(TreeNode *Note, StrucGrid &SGrid, vector<FaceNode> &FaceNodes, bool **Cellarray);

	void GetNodeBoundary(TreeNode *Note, StrucGrid &SGrid, vector<FaceNode> &ZoneDatas);
	void GetBoundary(CPGNode LayerNode,TreeNode *Note, StrucGrid &SGrid, vector<FaceNode> &FaceNodes, bool *Cellarray);
	
	void AddFace(CPGNode note, FaceNode & Face, int *Left);
	void AddFace(CPCell note, FaceNode & Face, int *Left, int ACTNUM, double Poro, double PERMEABILITY, double SOIL, int Facies, double DZMTRXV, double BULKVOLUME);

	void Point2Double(double *P, point P1);
	void DrawGrid(vector<FaceNode> FaceNodes, CellRange cellRange, int idx, vtkSmartPointer<vtkActor> CellLineActor, vtkSmartPointer<vtkActor> ZoneActor, vtkSmartPointer<vtkDoubleArray> val,vtkSmartPointer<vtkLookupTable> hueLut);// vtkSmartPointer<vtkColorTransferFunction> lut);
	void DrawGrid(CPGNode *Cell, int NumberGrid, vtkSmartPointer<vtkActor> CellLineActor, vtkSmartPointer<vtkActor> ZoneActor, vtkSmartPointer<vtkDoubleArray> val, vtkSmartPointer<vtkLookupTable> lut);

	vtkSmartPointer<vtkPoints> AddPoints(vector<FaceNode> FaceNodes);
	void DrawSingleCellLine(vector<FaceNode> FaceNodes, vtkSmartPointer<vtkPoints> points2, vtkSmartPointer<vtkActor> LineActor);
	void DrawSingleCellLine(int NumberGrid, vtkSmartPointer<vtkActor> celllineactor, vtkSmartPointer<vtkPoints> points);

	//void DrawGPGGrid(int cellNum, double *Value, vtkSmartPointer<vtkActor> SingleGridActor, vtkSmartPointer<vtkPoints> points, vtkSmartPointer<vtkDoubleArray> val, vtkSmartPointer<vtkColorTransferFunction> lut);
	void DrawGPGGrid(int NumberGrid, double *CellValue, double zmin, double zmax, vtkSmartPointer<vtkActor> SingleGridActor, vtkSmartPointer<vtkPoints> points, vtkSmartPointer<vtkDoubleArray> val, vtkSmartPointer<vtkLookupTable> hueLut);
	void DrawGPGGrid(vector<FaceNode> FaceNodes, double *CellValue, double zmin, double zmax, vtkSmartPointer<vtkActor> SingleGridActor, vtkSmartPointer<vtkPoints> points, vtkSmartPointer<vtkDoubleArray> val , vtkSmartPointer<vtkLookupTable> hueLut);//vtkSmartPointer<vtkColorTransferFunction> lut);

	double *GetFaceValue(vector<FaceNode> FaceNodes, int Type);
	double *GetFaceValue(CPGNode *Cell, int CellNum, int NumberGrid, int Type);

	void SpaceFilter(StrucGrid &SGrid, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, vector<bool> SpaceFlag);
	void SpaceFilter(int Layer, TreeNode *Nodes, StrucGrid &SGrid, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, vector<bool> SpaceFlag);
	void SpaceFilter2(int Layer, TreeNode *Nodes, StrucGrid &SGrid, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, vector<bool> SpaceFlag);
	void SpaceFilter3(int Layer, TreeNode *Nodes, StrucGrid &SGrid, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, vector<bool> SpaceFlag);

	void setType(int Layer, TreeNode *Nodes, int depth, StrucGrid &SGrid, int &valid_cellnum);
	void Filter(StrucGrid &SGrid, TreeNode *Nodes, int depth);

	bool JudgeSpaceFilter(CPGNode &NodeData, StrucGrid &SGrid, int nx, int ny, int nz, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, vector<bool> SpaceFlag, bool isOriginal);
	bool JudgeSpaceFilter(StrucGrid &SGrid, int idx, int k1, int nx, int ny, int nz, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, vector<bool> SpaceFlag);


	void GeoFilter(TreeNode *Nodes, StrucGrid &SGrid, double valueMin, double valuemax, int type);
	void GeoFilter(StrucGrid &SGrid, double valuemax, double valueMin, int type);
	void ResetActView(StrucGrid &SGrid, bool Value);

	bool JudgeFace(int i, int j, int k, int nx, int ny, int nz, int *ACTNUM, CPCell *cells);
	
	bool JudgeFace(int i, int j, int k, int dx, int dy, int dz, int nx, int ny, int nz, int *ACTNUM, CPCell *cells);

	bool JudgeFace2(int i, int j, int k, int dx, int dy, int dz, int nx, int ny, int nz, int *ACTNUM, CPCell *cells);

	void UpdatePropperty(vtkSmartPointer<vtkDoubleArray> val, int Type, vector<FaceNode> FaceNodes);
	
	void GetCellRange(vector<FaceNode> FaceNodes, vector<CellRange> &cellRange);
	void GetCellRange(StrucGrid &SGrid, vector<CellRange> &cellRange);
	void GetCellRange(TreeNode *Note, vector<CellRange> &cellRange, bool *isPropFlag);
	void GetNodeCellRange(int depth, int Layer, TreeNode *Note, vector<CellRange> &cellRange, bool *isPropFlag);


	void SetGridData(StrucGrid &SGrid, int i1, int j1, int k1, int dx, int dy, int dz, int nx, int ny, int nz);
	void SetFalseGridData(StrucGrid &SGrid, int i1, int j1, int k1, int dx, int dy, int dz, int nx, int ny, int nz);

	void getKrange(int kmin, int kmax, PhaseMap Layers, int &k2, int &k1);


	// Iterative spatial query 
	void updateSpace(int depth, int Layer, TreeNode *Nodes, StrucGrid &SGrid, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, vector<bool> SpaceFlag);
	void UpdateSpaceNodes(int depth, int Layer, int TotalLayer, TreeNode *Nodes, StrucGrid &SGrid, double xmin, double xmax, double ymin, double ymax, int *kmin, int *kmax, vector<bool> SpaceFlag);
	bool SetTreeNode(int depth, int Layer, int TotalLayer, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, BGGindex id_k, TreeNode *Nodes, vector<bool> SpaceFlag);
	void SpaceFilter(int depth, int Layer, int TotalLayer, TreeNode *Nodes, StrucGrid &SGrid, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, int *Layerkmin, int *Layerkmax, vector<bool> SpaceFlag);
	void SetNodeGridSpace(TreeNode *Nodes, StrucGrid &SGrid, int nx, int ny, int nz, double xmin, double xmax, double ymin, double ymax, int kmin, int kmax, vector<bool> SpaceFlag);
	void UpdateSpaceNode(StrucGrid &SGrid, int depth, int Layer, int TotalLayer, TreeNode *Nodes, bool isValue);
	void UpdateGridNode(TreeNode *Nodes, bool isValue);

	// Iterative properties query;
	void SetGeoNodes(int type, double valueMin, double valuemax, TreeNode *Nodes, bool isFixed);//Node Single Node Filter;
	void SetGridProp(int idx, StrucGrid &SGrid, double valueMin, double valuemax, int type); //Grid Single Grid Filter;
	void GeoFilter(StrucGrid &SGrid, double valueMin, double valuemax, int type, bool isFixed); // Grid properties Filter;
	void GeoFilter(int depth, int Layer, TreeNode *Nodes, StrucGrid &SGrid, double valueMin, double valuemax, int type, bool isFixed);
	void GeoPropUpdate(int Layer, TreeNode *Nodes, StrucGrid &SGrid, double valueMin, double valuemax, int type, bool isFixed);// Tree nodes Iteration Filter;


	void getLayer2(intArray TopLayers, intArray BottomLayers, int TopIndex, int BottomIndex, int &iIndex, int &j);
	
};
#endif;