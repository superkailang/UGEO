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
*  @file     PropertySet.cpp												 *
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

#include <QtWidgets>
#include"PropertySet.h"

PropertySet::PropertySet(QStringList LayerItem, int idx, QWidget *parent)
: QWidget(parent)
{
	InitLization(LayerItem,idx);
}
void PropertySet::InitLization(QStringList LayerItem, int idx){
	Labelfont.setFamily("SimSun");  // Font Family
	Labelfont.setBold(true);
	Labelfont.setPointSize(12);
	SheetLabelList.append(QString::fromLocal8Bit("Property")); 
	gridLayout = new QGridLayout;
	SheetLabelNum = 1;
	SheetLabels = new QLabel*[SheetLabelNum];
	for (int i = 0; i < SheetLabelNum; i++)
	{
		SheetLabels[i] = new QLabel(SheetLabelList[i]);
		SheetLabels[i]->setFont(Labelfont);		
	}	
	UnitBox = new QComboBox;
	for (int i = 0; i < (int)LayerItem.size(); i++){
		UnitBox->addItem(LayerItem[i]);
		UnitBox->setFont(Labelfont);
	}
	UnitBox->setMaxVisibleItems(8);
	int Jindex = 0;
	for (int i = 0; i < SheetLabelNum; i++)
	{
		gridLayout->addWidget(SheetLabels[i], i + 1, Jindex, 1, 1, Qt::AlignTop);			
		gridLayout->addWidget(UnitBox, i + 1, Jindex + 1, 1, 1, Qt::AlignTop);		
	}				
	ButtonStringList.push_back(QString::fromLocal8Bit("OK"));
	ButtonStringList.push_back(QString::fromLocal8Bit("No"));
	ButtomNum = (int)ButtonStringList.size();
	Buttons = new QPushButton*[ButtomNum];
	buttonBox = new QDialogButtonBox(Qt::Horizontal);
	for (int i = 0; i < ButtomNum; i++)
	{
		Buttons[i] = new QPushButton(ButtonStringList[i]);
		Buttons[i]->setDefault(true);
		Buttons[i]->setFixedWidth(60);
		Buttons[i]->setFont(Labelfont);
		buttonBox->addButton(Buttons[i], QDialogButtonBox::ActionRole);
	}
	this->UnitBox->setCurrentIndex(idx);
	// Button Action;	
	connect(Buttons[0], SIGNAL(clicked()), this, SLOT(submit()));
	connect(Buttons[1], SIGNAL(clicked()), this, SLOT(quit()));	
	TopLeftLayout = new QVBoxLayout();
	TopLeftLayout->setMargin(20);
	TopLeftLayout->setSpacing(40);
	TopLeftLayout->addLayout(gridLayout);
	TopLeftLayout->addWidget(buttonBox);
	setLayout(TopLeftLayout);
	setMinimumHeight(100);
}
void PropertySet::submit(){
	int idx= this->UnitBox->currentIndex();
	QString Property = this->UnitBox->currentText();
	cps->UpdateGridProperty( idx, Property);
	close();
}
void PropertySet::quit(){
	close();
}