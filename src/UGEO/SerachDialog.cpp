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
*  @file     SearchDialog.cpp												 *
*  @brief    QWidget Dialog For Grid  Search								 *
*  Details.                                                                  *
*                                                                            *
*  @author   kailanghuang                                                    *
*  @email    kailanghuang@pku.edu.cn                                         *
*  @version  2.0.0.1		                                                 *
*  @date     2018/01, 2019/1												 *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         :  QWidget Dialog For Grid Search 						 *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

#include <QtWidgets>
#include"SerachDialog.h"

TxtTab::TxtTab(QStringList LayerItem, vector<vector<CellRange>> Range, vector<vector<CellRange>> MaxValueRange, vector<PhaseMap> Phase, queryState query, bool *isPropFlag, QWidget *parent)
: QWidget(parent)
{
	vector<bool> tempChange;
	int CountSize = 7;
	for (int i = 0; i < Range.size(); i++){
		for (int j = 0; j < CountSize; j++){
			tempChange.push_back(false);
		}
		isChange.push_back(tempChange);
		tempChange.clear();
	}
	this->Phase = Phase;
	this->Range = Range;
	this->MaxValueRange = MaxValueRange;
	InitLization(LayerItem);
	//query;
	UnitBox->setCurrentIndex(query.LOD);
	if (query.isProperty){
		this->CheckBoxs[0]->setChecked(true);
	}
	if (query.isSpace){
		this->CheckBoxs[1]->setChecked(true);
	}
	currentIdx = query.LOD;
	this->isPropFlag = isPropFlag;
	SetTxt();
	isLoad = true;
}
TxtTab::~TxtTab()
{
	Phase.clear();
	Range.clear();;
	MaxValueRange.clear();

	delete[] SheetLabels;
	SheetLabels = NULL;
	delete[] SheetLineEdits;
	SheetLineEdits = NULL;
	delete[] CheckBoxs;
	CheckBoxs = NULL;
	delete[] TopLeftLayout;
	TopLeftLayout = NULL;
	delete[] gridLayout;
	gridLayout = NULL;
	delete[]UnitBox;
	UnitBox = NULL;
	printf("Txt Tab 2");
}
void TxtTab::SetTxt(){
	int idx = 2;
	int i;
	// QString("%0.4f") Range[currentIdx][i + idx].minValue);
	for (i = 1; i < SheetLabelNum - 3; i++)
	{
		SheetLineEdits[2 * i]->setText(QString::number(Range[currentIdx][i + idx].minValue, 'g', 9));
		//SheetLineEdits[2 * i]->setValidator(new QDoubleValidator(MaxValueRange[currentIdx][i + idx].minValue,MaxValueRange[currentIdx][i + idx].maxValue, 6, SheetLineEdits[2 * i]));

		SheetLineEdits[2 * i + 1]->setText(QString::number(Range[currentIdx][i + idx].maxValue, 'g', 9));
		//SheetLineEdits[2 * i + 1]->setValidator(new QDoubleValidator(MaxValueRange[currentIdx][i + idx].minValue,MaxValueRange[currentIdx][i + idx].maxValue, 6, SheetLineEdits[2 * i+1]));
		if (!isPropFlag[i - 1]){
			SheetLineEdits[2 * i]->setEnabled(false);
			SheetLineEdits[2 * i + 1]->setEnabled(false);
		}
	}
	for (int j = 0; j < 3; j++){
		SheetLineEdits[2 * i]->setText(QString::number(Range[currentIdx][j].minValue, 'g', 12));
		SheetLineEdits[2 * i + 1]->setText(QString::number(Range[currentIdx][j].maxValue, 'g', 12));

		if (j < 2){
			//SheetLineEdits[2 * i]->setValidator(new QDoubleValidator(MaxValueRange[currentIdx][j].minValue, MaxValueRange[currentIdx][j].maxValue, 6, SheetLineEdits[2 * i]));
			//SheetLineEdits[2 * i + 1]->setValidator(new QDoubleValidator(MaxValueRange[currentIdx][j].minValue, MaxValueRange[currentIdx][j].maxValue, 6, SheetLineEdits[2 * i + 1]));
		}
		else{
			//SheetLineEdits[2 * i]->setValidator( new QIntValidator( (int)MaxValueRange[currentIdx][j].minValue, (int)MaxValueRange[currentIdx][j].maxValue, 6, SheetLineEdits[2 * i]));
			//SheetLineEdits[2 * i + 1]->setValidator( new QIntValidator( (int) MaxValueRange[currentIdx][j].minValue, (int)MaxValueRange[currentIdx][j].maxValue, 6, SheetLineEdits[2 * i + 1]));
		}		
		i = i + 1;
	}
}
void TxtTab::InitLization(QStringList LayerItem){
	
	Labelfont.setFamily("SimSun"); Labelfont.setBold(true);Labelfont.setPointSize(12);
	QStringList Checklist;
	Checklist.push_back(QString::fromLocal8Bit("LOD")); Checklist.push_back(QString::fromLocal8Bit("Space"));
	SheetLabelList.append(QString::fromLocal8Bit("Layers")); SheetLabelList.append(QString::fromLocal8Bit("Porosity")); SheetLabelList.append(QString::fromLocal8Bit("Permeability"));
	SheetLabelList.append(QString::fromLocal8Bit("Saturation")); SheetLabelList.append(QString::fromLocal8Bit("Facies "));
	SheetLabelList.append("X"); SheetLabelList.append("y"); SheetLabelList.append("k");
	SheetLabelNum = (int)SheetLabelList.size();
	ACheckBoxNum = 2;
	gridLayout = new QGridLayout;
	SheetLabels = new QLabel*[SheetLabelNum];
	SheetLineEdits = new QLineEdit*[2 * SheetLabelNum];
	CheckBoxs = new QCheckBox*[ACheckBoxNum];
	for (int i = 0; i < SheetLabelNum; i++)
	{
		SheetLabels[i] = new QLabel(SheetLabelList[i]);
		SheetLineEdits[2 * i] = new QLineEdit;
		SheetLineEdits[2 * i + 1] = new QLineEdit;
		SheetLabels[i]->setFont(Labelfont);
		SheetLineEdits[2 * i]->setFont(Labelfont);
		SheetLineEdits[2 * i + 1]->setFont(Labelfont);
		connect(SheetLineEdits[2 * i], SIGNAL(textChanged(const QString &)), this, SLOT(OnLineChange(const QString &)));
		connect(SheetLineEdits[2 * i+1], SIGNAL(textChanged(const QString &)), this, SLOT(OnLineChange(const QString &)));
	}
	for (int i = 0; i < ACheckBoxNum; i++)
	{
		CheckBoxs[i] = new QCheckBox();
		CheckBoxs[i]->setText(Checklist[i]);
		CheckBoxs[i]->setFont(Labelfont);
	}
	UnitBox = new QComboBox;
	for (int i = 0; i < (int)LayerItem.size(); i++){
		UnitBox->addItem(LayerItem[i]);
	}
	UnitBox->setMaxVisibleItems(8);
	int Jindex = 1;
	for (int i = 0; i < SheetLabelNum-3; i++)
	{
		gridLayout->addWidget(SheetLabels[i], i + 1, Jindex, 1, 1, Qt::AlignTop);	
		if (i != 0){
			gridLayout->addWidget(SheetLineEdits[2 * i], i + 1, Jindex + 1, 1, 1, Qt::AlignTop);
			gridLayout->addWidget(SheetLineEdits[2 * i + 1], i + 1, Jindex + 2, 1, 1, Qt::AlignTop);
		}
		else{
			gridLayout->addWidget(UnitBox, i + 1, Jindex + 1, 1, 1, Qt::AlignTop);
		}
	}
	int jdx = SheetLabelNum - 2;
	for (int i = SheetLabelNum - 3; i < SheetLabelNum; i++){
		gridLayout->addWidget(SheetLabels[i], i + 2, Jindex, 1, 1, Qt::AlignTop);
		gridLayout->addWidget(SheetLineEdits[2 * i], i + 2, Jindex + 1, 1, 1, Qt::AlignTop);
		gridLayout->addWidget(SheetLineEdits[2 * i + 1], i + 2, Jindex + 2, 1, 1, Qt::AlignTop);
	}
	for (int i = 0; i < ACheckBoxNum; i++){
		int idx = i*jdx;
		gridLayout->addWidget(CheckBoxs[i], idx, 0, 1, 2, Qt::AlignTop);
		QLabel *ATitleLabel = new QLabel("From ");
		QLabel *ATitleLabel2 = new QLabel("To  ");
		ATitleLabel->setFont(Labelfont);
		ATitleLabel2->setFont(Labelfont);
		gridLayout->addWidget(ATitleLabel, idx, 2, 1, 1, Qt::AlignTop);
		gridLayout->addWidget(ATitleLabel2, idx, 3, 1, 1, Qt::AlignTop);
	}
	/*
	for (int i = 1; i < SheetLabelNum-3; i++){
		SheetLineEdits[2 * i]->setValidator(new QDoubleValidator(MaxValueRange[Layer][i+2],
			999.0, 2, SheetLineEdits[2 * i]));
	}*/
	gridLayout->setSpacing(10);
	setLayout(gridLayout);	
	connect(CheckBoxs[0], SIGNAL(clicked()), this, SLOT(UpdateText()));
	connect(UnitBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxValueChanged(int)));
}
bool TxtTab::JudgeInput(){
	int idx = 2;
	int i;
	for (i = 1; i < SheetLabelNum - 3; i++)
	{
		// ¾«¶È error 
		double minvalue = SheetLineEdits[2 * i]->text().trimmed().toDouble();
		double maxvalue = SheetLineEdits[2 * i + 1]->text().trimmed().toDouble();
		if (maxvalue < minvalue){
			QMessageBox::warning(NULL, QString::fromLocal8Bit("Input Error"), SheetLabelList[i] + QString::fromLocal8Bit(" Minimum is greater than maximum"), QMessageBox::Yes, QMessageBox::Yes);
			return false;
		}
		if (minvalue < (MaxValueRange[currentIdx][i + idx].minValue - miniValue)){
			QMessageBox::warning(NULL, QString::fromLocal8Bit("Input Error"), SheetLabelList[i] + QString::fromLocal8Bit(" Minimum value") + QString::number(MaxValueRange[currentIdx][i + idx].minValue), QMessageBox::Yes, QMessageBox::Yes);
			SheetLineEdits[2 * i]->setText(QString::number(MaxValueRange[currentIdx][i + idx].minValue));
			return false;
		}
		if (maxvalue >(MaxValueRange[currentIdx][i + idx].maxValue + miniValue)){
			QMessageBox::warning(NULL, QString::fromLocal8Bit("Input Error"), SheetLabelList[i] + QString::fromLocal8Bit(" Maximum value") + QString::number(MaxValueRange[currentIdx][i + idx].maxValue), QMessageBox::Yes, QMessageBox::Yes);
			SheetLineEdits[2 * i + 1]->setText(QString::number(MaxValueRange[currentIdx][i + idx].maxValue)); 
			return false;
		}
	}
	for (int j = 0; j < 3; j++){

		double minvalue = SheetLineEdits[2 * i]->text().trimmed().toDouble();
		double maxvalue = SheetLineEdits[2 * i + 1]->text().trimmed().toDouble();

		if (maxvalue < minvalue){
			QMessageBox::warning(NULL, QString::fromLocal8Bit("Input Error"), SheetLabelList[i] + QString::fromLocal8Bit(" Minimum is greater than maximum"), QMessageBox::Yes, QMessageBox::Yes);
			return false;
		}
		if (minvalue < (MaxValueRange[currentIdx][j].minValue - miniValue)){
			QMessageBox::warning(NULL, QString::fromLocal8Bit("Input Error"), SheetLabelList[i] + QString::fromLocal8Bit(" Minimum value") + QString::number(MaxValueRange[currentIdx][j].minValue), QMessageBox::Yes, QMessageBox::Yes);

			SheetLineEdits[2 * i]->setText(QString::number(MaxValueRange[currentIdx][j].minValue));

			return false;
		}
		if (maxvalue >(MaxValueRange[currentIdx][j].maxValue + miniValue)){
			QMessageBox::warning(NULL, QString::fromLocal8Bit("Input Error"), SheetLabelList[i] + QString::fromLocal8Bit(" Maximum value") + QString::number(MaxValueRange[currentIdx][j].maxValue), QMessageBox::Yes, QMessageBox::Yes);
			SheetLineEdits[2 * i + 1]->setText(QString::number(MaxValueRange[currentIdx][j].maxValue));
			return false;
		}
		i = i + 1;
	}
	return true;
}
bool TxtTab::GetFilterValue(int &Layer, double *Value, vector<bool> &TempChange){
	Layer = UnitBox->currentIndex();
	if (CheckBoxs[0]->isChecked()){
		for (int i = 1; i < SheetLabelNum-3; i++){
			Value[2 * i - 2] = SheetLineEdits[2 * i]->text().trimmed().toDouble();
			Value[2 * i - 1] = SheetLineEdits[2 * i + 1]->text().trimmed().toDouble();
		};
		for (int i = 3; i < isChange[Layer].size(); i++){
			TempChange.push_back(isChange[Layer][i]);
		}
		return true;
	}	
	return false;
}
bool TxtTab::GetSpaceBounds(double *Bounds,vector<bool> &TempChange){
	if( CheckBoxs[1]->isChecked()){
		int idx = SheetLabelNum - 3;
		for (int i = 0; i < 3; i++){
			int index = idx + i;
			Bounds[2 * i] = SheetLineEdits[2 * index]->text().trimmed().toDouble();
			Bounds[2 * i + 1] = SheetLineEdits[2 * index + 1]->text().trimmed().toDouble();
		};
		for (int i = 0; i < 3; i++){
			TempChange.push_back(isChange[currentIdx][i]);
		};
		return true;
	};
	return false;
}
void TxtTab::UpdateText(){
	if (CheckBoxs[0]->isChecked()){
		int idx=UnitBox->currentIndex();
		int idx2 = SheetLabelNum - 1;
		currentIdx = idx;
		isLoad = false;
		SetTxt();
		isLoad = true;
	}
}
void TxtTab::comboBoxValueChanged(int index){
	if (CheckBoxs[0]->isChecked()){
		int idx = UnitBox->currentIndex();		
		currentIdx = idx;
		isLoad = false;
		SetTxt();		
		isLoad = true;
	}
}
void TxtTab::OnLineChange(const QString &str){
	if (isLoad){
		QLineEdit *LineEdit = dynamic_cast<QLineEdit *>(sender());
		int j = -1;
		bool isFlag = false;
		for (int i = 0; i < SheetLabelNum; i++){
			if (LineEdit == SheetLineEdits[2 * i]){
				j = i;
				isFlag = true;
				break;
			}
			if (LineEdit == SheetLineEdits[2 * i + 1]){
				j = i;
				break;
			}
		}
		if (j>0){
			double value = LineEdit->text().trimmed().toDouble();
			int idx = 2;
			if (j < SheetLabelNum - 3){
				isChange[currentIdx][j + idx] = true;
				if (isFlag){  // max;
					double d1=MaxValueRange[currentIdx][j].maxValue;
				}
				else{
					double d2 = MaxValueRange[currentIdx][j].minValue;
				}
			}
			else{
				j = j - SheetLabelNum + 3;
				isChange[currentIdx][j] = true;
			}
		}
	}
}

//  CellValueRange Current Query Condition // Constained Range -> MaxValueRange
SearchDialog::SearchDialog(vector<vector<CellRange>> CellValueRange, vector<vector<CellRange>> MaxValueRange, vector<PhaseMap> Phase, queryState query, bool *isPropFlag,QWidget *parent)
: QWidget(parent)
{	
	Initlization();
	QTabWidget *tabWidget = new QTabWidget();
	QStringList LayerItem;
	LayerItem.push_back(QString::fromLocal8Bit("LOD1"));
	LayerItem.push_back(QString::fromLocal8Bit("LOD2"));	
	LayerItem.push_back(QString::fromLocal8Bit("LOD3"));
	txtTab = new TxtTab(LayerItem, CellValueRange, MaxValueRange, Phase, query, isPropFlag);
	tabWidget->addTab(txtTab, QString::fromLocal8Bit("Query"));
	tabWidget->setFont(Labelfont);

	TopLeftLayout = new QVBoxLayout;	
	TopLeftLayout->addWidget(tabWidget);	
	TopLeftLayout->addWidget(buttonBox);
	TopLeftLayout->setSpacing(20);
	setLayout(TopLeftLayout);	
	setWindowTitle(QString::fromLocal8Bit("Query"));

}
SearchDialog::~SearchDialog()
{	
	printf(" SearchDialog 4 \n");
	delete []TopLeftLayout;
	TopLeftLayout = NULL;
	delete [] Buttons;
	Buttons = NULL;
	delete[]  buttonBox;
	buttonBox = NULL;
	delete[] txtTab;
	txtTab = NULL;
	printf(" SearchDialog 5 \n");
}
void SearchDialog::Initlization(){
	Labelfont.setFamily("SimSun"); Labelfont.setBold(true); Labelfont.setPointSize(12);
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
	// Button Action;
	//connect(Buttons[1], SIGNAL(clicked()), this, SLOT(reset()));
	connect(Buttons[0], SIGNAL(clicked()), this, SLOT(submit()));
	connect(Buttons[1], SIGNAL(clicked()), this, SLOT(quit()));
}
void SearchDialog::submit()
{
	if (txtTab->JudgeInput()){
		double *Bounds = new double[6];
		bool TempFlag1 = false, Flag2 = false;
		vector<bool> FilterFlag, SpaceFilter;
		int LayerIndex;
		double *Value = new double[8];
		TempFlag1 = txtTab->GetSpaceBounds(Bounds, SpaceFilter);
		Flag2 = txtTab->GetFilterValue(LayerIndex, Value, FilterFlag);
		if (TempFlag1 && Flag2){
			cps->UpdateCellRange(LayerIndex, Bounds, Value, 1, FilterFlag, SpaceFilter); //Spatial and semantic constraints;
		}
		else
		{
			if (TempFlag1){
				cps->UpdateCellRange(LayerIndex, Bounds, Value, 2, FilterFlag, SpaceFilter); //Spatial constraints
			}
			if (Flag2){
				cps->UpdateCellRange(LayerIndex, Bounds, Value, 3, FilterFlag, SpaceFilter); // Semantic constraints;
			}
		}
		close();
	}
}
void SearchDialog::quit()
{	
	close();
}