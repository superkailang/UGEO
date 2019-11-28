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
*  @file     SearchDialog.h													 *
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

#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include<QStringList>
#include"cellpara3dshow.h"
#include"Struct.h"

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QPushButton;
class QLineEdit;
class QLabel;
class QComboBox;
class QCheckBox;
class QRadioButton;
class QHBoxLayout;
class QVBoxLayout;
class QDateEdit;
class QGridLayout;
class QTabWidget;


class TxtTab : public QWidget{
	Q_OBJECT
public:
	explicit TxtTab(QStringList LayerItem, vector<vector<CellRange>> Range, vector<vector<CellRange>> MaxValueRange, vector<PhaseMap> Phase, queryState query, bool *isPropFlag, QWidget *parent = 0);
	~TxtTab();

	bool GetFilterValue(int &Layer, double *Value, vector<bool> &TempChange);
	bool GetSpaceBounds(double *Bounds,vector<bool> &TempChange);
	bool JudgeInput();

private slots:
	void UpdateText();
	void comboBoxValueChanged(int index);
	void OnLineChange(const QString &str);
private:
	bool *isPropFlag;
	int SheetLabelNum;
	int SheetTxtNum;
	int ACheckBoxNum;
	QStringList SheetLabelList;
	QStringList CheckstringList;
	QStringList UnitBoxList;

	QLabel **SheetLabels;
	QLineEdit **SheetLineEdits;
	QCheckBox **CheckBoxs;

	QComboBox *UnitBox;
	QHBoxLayout *TopLeftLayout;
	QGridLayout *gridLayout;

	vector<PhaseMap> Phase;
	vector<vector<CellRange>> Range;
	vector<vector<CellRange>> MaxValueRange;
	int currentIdx = 0;

private:
	vector<vector<bool>> isChange;
	bool isLoad = false;	
	QFont Labelfont;
	void InitLization(QStringList LayerItem);
	void SetTxt();
};

class SearchDialog: public QWidget
{
	Q_OBJECT
public:
	explicit SearchDialog(vector<vector<CellRange>> CellValueRange, vector<vector<CellRange>> MaxValueRange, vector<PhaseMap> Phase, queryState query, bool *isPropFlag,QWidget *parent = 0);
	~SearchDialog();

	cellpara3dshow *cps;

private slots:
	void submit();
	void quit();
private:
	TxtTab *txtTab;
	QVBoxLayout *TopLeftLayout;
	QFont Labelfont;
	int ButtomNum;
	QPushButton **Buttons;
	QDialogButtonBox *buttonBox;
	QStringList ButtonStringList;
	void Initlization();
};

#endif