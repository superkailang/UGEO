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

#ifndef COMMANDS_H
#define COMMANDS_H
#include <QUndoCommand>
#include<QVTKWidget.h>
#include<qtabwidget.h>
#include"qpoint.h"
//class QTabWidget;
// Tab Command;
class DeleteTabCommand : public QUndoCommand
{
public:    
	explicit DeleteTabCommand(QTabWidget *TableItem, const int OldIndex, QUndoCommand *parent = 0);
    void undo() override;
    void redo() override;
private:
	QTabWidget *Item;
	QVTKWidget *qvtkWidget;
	int Index;    
};
#endif
