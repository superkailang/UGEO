## Source Code Analysis
#####

All souce codes require a C compiler

UGEO project is well organized, include three main aspects

* File IO
* Grid Process
* Grid Visilazation

### Architecture

The following diagram shows the structure of the implementation of the code

![UGEO](https://vueproject-1253784566.cos.ap-chengdu.myqcloud.com/github/%E5%BE%AE%E4%BF%A1%E6%88%AA%E5%9B%BE_20191129205306.png)


### [Base Structure](https://github.com/superkailang/UGEO/blob/master/src/UGEO/Gridstruct.h)
In gridStruct file, grid structure type and basic 3d point structure are defined
```c++
	struct CPCell
	{
		CPCell() :is_act(true) {};
		point corner[8]; // corner points
		point facec[6];
		point blockc;
		bool is_act;
		point AreaVec(const int dir);
		double Gen_GeomCenter(); // return volume
	};
```

corner point cell(CPCell) is defined include points and property
It also contains constant definitions and other basic structure.

### [File IO](https://github.com/superkailang/UGEO/blob/master/src/UGEO/fileReader.h)
File IO operation
```
bool FileReader::Data_File_Dialog(int &FileIndex, QString &fileName)
{
	QStringList fileNameList;
	QFileDialog* fd = new QFileDialog();  // Creating Dialogs
	fd->setNameFilter(filters);
	fd->setFileMode(QFileDialog::ExistingFiles);
	fd->setViewMode(QFileDialog::List);  // Set View mode, list mode or detail mode
	if (fd->exec() == QDialog::Accepted)   // if read on success
	{
		QString sldfieter = fd->selectedNameFilter();  //Determine the format of the returned data;
		fileNameList = fd->selectedFiles();
		fileName = fileNameList[0];
		for (int i = 0; i < ListFilters.size(); i++)
		{
			if (sldfieter == ListFilters.at(i)) //Satisfy the data format
			{
				FileIndex = i;
				return  true; // return file Index;
			}
		}
	}
	return false;
}
```
In Data_File_Dialog function,The program shows the openfile dialog box and read (*.dat *.txt *.GRDECL) files

During InputData function, extract text from file and reads the file bytes and parses the file.

support file type
* *.dat
* *.txt
* *.GRDECL


### [Grid Process](https://github.com/superkailang/UGEO/blob/master/src/UGEO/GridProcess.h)

```c++
	DataProcess::DataProcess(StrucGrid &SGridData, MyTree &Tree)
	{
		int dxx = 8, dyy = 8; //steps
		int i = 0;
		vector<NodeDatas> noteDatas; // Save the cache information for the construct tree;
		SetParameter(dxx, dyy);
		Initlization(SGridData);
		MergeGrid(SGridData, noteDatas);
		TreeInitlization(Tree, SGridData.Phasemaps[i].TopLayers, SGridData.Phasemaps[i].BottomLayers, noteDatas, SGridData); // Data Store by layer;
		currentType = 0;
		for (int j = 0; j < Tree.root->NumberChild; j++)
		{
			for (int jj = 0; jj < Tree.root->childNotes[j]->NumberChild; jj++){
				TreeMake(1, SGridData, dxx, dyy, Tree.root->childNotes[j]->childNotes[jj]); // construct Tree structure
			}
		}
	}
```

During Grid Process function, Corner cells as input are merged in different scale.

#### Tree Structure
```
struct TreeNode
	{
		CPGNode NodeData; // node Data;
		int NumberChild;
		TreeNode **childNotes;  // children
		bool * Cellarray; // visible Face;
		TreeNode(){	}; //node class
		TreeNode(const  int _nodenum, const CPGNode _nodedata) // constructor function
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
```
Tree structure is used to store Grid data in different scales,
we access the tree structure using preorder traversal methods defined in the preOrder function

### Grid Visiulazation

