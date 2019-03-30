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