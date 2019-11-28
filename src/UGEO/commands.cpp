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
*  @file     Commands.h														 *
*  @brief    Commands														 *
*  Details.                                                                  *
*                                                                            *
*  @author   kailanghuang                                                    *
*  @email    kailanghuang@pku.edu.cn                                         *
*  @version  2.0.0.1		                                                 *
*  @date     2018/01, 2019/1												 *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         :  Commands												 *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

#include <QtWidgets>
#include "commands.h"

DeleteTabCommand::DeleteTabCommand(QTabWidget *TableItem, const int OldIndex, QUndoCommand *parent)
	:QUndoCommand(parent)
{		
	this->Item = TableItem;	
	this->qvtkWidget = (QVTKWidget*)TableItem->widget(Index);
	this->Index = OldIndex;
}
void DeleteTabCommand::undo()
{	
	//this->Item->insertTab(Index,this->qvtkWidget, "3D Window");	
	this->Item->addTab(this->qvtkWidget, "3D Window");
	this->Item->update();	
}
void DeleteTabCommand::redo()
{
	this->Item->removeTab(Index);
}