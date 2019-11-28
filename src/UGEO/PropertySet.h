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
*  @file     PropertySet.h													 *
*  @brief    QWidget Dialog For Property selection							 *
*  Details.                                                                  *
*                                                                            *
*  @author   kailanghuang                                                    *
*  @email    kailanghuang@pku.edu.cn                                         *
*  @version  2.0.0.1		                                                 *
*  @date     2018/01, 2019/1												 *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         :  QWidget Dialog For Property selection					 *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

#ifndef PROPERTYSET_H
#define PROPERTYSET_H

#include <QDialog>
#include<QStringList>
#include"cellpara3dshow.h"

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QPushButton;
class QLineEdit;
class QLabel;
class QComboBox;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;

class PropertySet : public QWidget{
	Q_OBJECT
public:
	explicit PropertySet(QStringList LayerItem,int idx,QWidget *parent = 0);	
	cellpara3dshow *cps;
private:
	int SheetLabelNum;		
	QStringList SheetLabelList;
	QStringList CheckstringList;
	QStringList UnitBoxList;

	QLabel **SheetLabels;	
	int ButtomNum;
	QPushButton **Buttons;
	QDialogButtonBox *buttonBox;
	QStringList ButtonStringList;

	QComboBox *UnitBox;
	QVBoxLayout *TopLeftLayout;
	QGridLayout *gridLayout;	
private:
	QFont Labelfont;
	void InitLization(QStringList LayerItem, int idx);
private slots:
	void submit();
	void quit();
};
#endif