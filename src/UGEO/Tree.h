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
*  @file     Tree.h															 *
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

#ifndef TREE_H
#define TREE_H
#include"Gridstruct.h"
#include"IJKGrid.h"
#include<queue>
namespace UMSM
{
	//define Tree node class 
	struct TreeNode
	{
		// node Data;
		CPGNode NodeData;
		int NumberChild; 
		TreeNode **childNotes;  // children
		// visible Face;
		bool * Cellarray;
		TreeNode(){	}; //node class 
		TreeNode(const  int _nodenum, const CPGNode _nodedata)
		{
			NumberChild = _nodenum;
			NodeData = _nodedata;
			Cellarray = new bool[7];
			memset(Cellarray, false, sizeof(bool)* 7);
			if (NumberChild > 0)
			{
				childNotes = new TreeNode*[_nodenum];
			}
		};
	};
	class MyTree
	{
	public:
		TreeNode *root;		
		void Initlization(intArray TopLayers, intArray BottomLayers, NodeDatas nodeDatas, StrucGrid SGridData);
		int depth(TreeNode * &node);
		bool flag = false;
		ZoneData GetZoneData(int Depth);
		vector<ZoneData> GetZoneData(int Depth, PhaseMap Layers);		
		MyTree();
		~MyTree();
	private:			
		queue<TreeNode *> Treeque;
		void Merge(int TopLayer, int BottomLayer, StrucGrid SGridData, nodeData NoteData, int NX, int NY, CPGNode &node);
		void preOrder(int level, int depth, TreeNode *note);
		void createTree(TreeNode * &node, intArray TopLayers, intArray BottomLayers, NodeDatas nodeDatas, StrucGrid SGridData);
	};
};
#endif;