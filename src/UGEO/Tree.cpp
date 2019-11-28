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
*  @file     Tree.cpp														 *
*  @brief    Tree Struct For mutil_scale Grid								 *
*  Details.                                                                  *
*                                                                            *
*  @author   kailanghuang                                                    *
*  @email    kailanghuang@pku.edu.cn                                         *
*  @version  2.0.0.1		                                                 *
*  @date     2018/01, 2019/1												 *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         :  Tree Struct For mutil_scale Grid 						 *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

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
		//construct Tree 
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
		//	// Merge X,Y;
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
		//Generate a Coarse  grid
		node.ACTNUM = 1;
		for (int m = 0; m < 4; m++)
		{
			temp = dd[m] + TopLayer*NX*NY;
			node.CPGCells.corner[m] = SGridData.CPGCells[temp].corner[m];
			temp = dd[m] + BottomLayer*NX*NY;
			node.CPGCells.corner[m + 4] = SGridData.CPGCells[temp].corner[m + 4];
		}
	}
	
	////get Tree Depth
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
};