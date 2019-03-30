#include"fileReader.h"
#include"qfiledialog.h"
#include <iostream>
#include <fstream>

using namespace std;


bool FileReader::Data_File_Dialog(int &FileIndex, QString &fileName)
{
	QStringList fileNameList;
	QFileDialog* fd = new QFileDialog();//创建对话框	
	fd->setNameFilter(filters);
	fd->setFileMode(QFileDialog::ExistingFiles);
	fd->setViewMode(QFileDialog::List);  //设置浏览模式，有 列表（list） 模式和 详细信息（detail）两种方式	
	if (fd->exec() == QDialog::Accepted)   //如果读取成功的执行
	{
		QString sldfieter = fd->selectedNameFilter();  //判断返回的数据的格式;
		fileNameList = fd->selectedFiles();
		fileName = fileNameList[0];
		for (int i = 0; i < ListFilters.size(); i++)
		{
			if (sldfieter == ListFilters.at(i)) //满足数据格式
			{
				FileIndex = i;
				return  true;  //返回文件位置;
			}
		}
	}
	return false;
}

FileReader::FileReader(const bool _if_echo){
	if_echo = _if_echo;
	echoline_len = 0;
	echo_count = 0;
	//ListFilters.append("image(*.jpg *.png *.bmp *.gif *.ico *.eps)");
	ListFilters.append("Input Data(*.dat *.txt *.GRDECL)");
	filters = "Input Data(*.dat *.txt *.GRDECL)";

};
FileReader::FileReader(FileReader& src){
	;
};
FileReader::~FileReader(){
	ifp.clear();
};
bool FileReader::InputData(const char* filename, Wells &wells, StrucGrid& GridData)
{
	ifp.open(filename);
	if (ifp.fail()) {
		return false;
		while (!FileEnd()) {
		}
	}
	// Extract file direction
	strcpy_s(file_dir, filename);
	int file_dir_len = (int)strlen(file_dir);
	while (file_dir_len > 0 && file_dir[file_dir_len - 1] != '\\' && file_dir[file_dir_len - 1] != '/') { //  
		--file_dir_len;
	}
	file_dir[file_dir_len] = '\0';
	int nGrid = -1;
	char kword[MAX_STRING_LEN];
	while (!FileEnd()) {
		skip_sharp();
		echoline_len = 0;
		readString(kword);
		if (UGStrCmp(kword, "INCLUDE")) {
			char fname[MAX_STRING_LEN];
			char filename[MAX_STRING_LEN];
			skip_sharp();
			readName(fname);
			strcpy(filename, file_dir);
			strcat(filename, fname);
			FileReader temp_scanner(*this);
			bool success = temp_scanner.InputData(filename, wells, GridData);
		}
		else if (UGStrCmp(kword, "MAPAXES"))
		{
			double x;
			readValue(&x);	GridData.XCoord0 = x;
			readValue(&x);	GridData.YCoord0 = x;
			//readValue(&x);	GridData.Corner_Z = x;
		}
		else if (UGStrCmp(kword, "WellDATA"))
		{
			if (!ReadWellData(wells))
				return false;
		}
		else if (UGStrCmp(kword, "SPECGRID")) {
			skip_sharp();
			int dim[4];
			int pos = readArray(dim, 4);
			GridData.NX = dim[0]; GridData.NY = dim[1]; GridData.NZ = dim[2];
			GridData.NGrid = GridData.NX*GridData.NY*GridData.NZ;
			nGrid = GridData.NGrid;
		}
		else if (UGStrCmp(kword, "LOD")) {
			skip_sharp();
			int stage;
			stage = readInt();
			skip_sharp();
			PhaseMap tempPhase;
			tempPhase.stage = stage;
			while (isdigit(ifp.peek()))
			{
				int TopLayer = readInt();
				int BottomLayer = readInt(); //stage;
				tempPhase.TopLayers.push_back(TopLayer);
				tempPhase.BottomLayers.push_back(BottomLayer);
				skip_sharp();
				// http://blog.csdn.net/yulinxx/article/details/73522391
			}
			if ((int)tempPhase.TopLayers.size() > 0)
			{
				GridData.Phasemaps.push_back(tempPhase);
				tempPhase.BottomLayers.clear();
				tempPhase.TopLayers.clear();
			}
		}
		else if (UGStrCmp(kword, "Merge")) {
			skip_sharp();
			if (!readMergeFacies(GridData.FMerge)){
				return false;
			}
		}
		else if (UGStrCmp(kword, "COORD")) {
			skip_sharp();
			nGrid = GridData.NX*GridData.NY*GridData.NZ;
			readCOORD(GridData, GridData.XCoord0, GridData.YCoord0);
		}
		else if (UGStrCmp(kword, "ZCORN")) {
			skip_sharp();
			nGrid = GridData.NX*GridData.NY*GridData.NZ;
   			GridData.Zcorn = readZcorn(nGrid * 8);
		}
		else if (UGStrCmp(kword, "ACTNUM"))
		{
			skip_sharp();
			nGrid = GridData.NX*GridData.NY*GridData.NZ;
			GridData.ACTNUM = new int[nGrid];
			readArray(GridData.ACTNUM, nGrid);
		}
		else if (UGStrCmp(kword, "DZMTRXV"))
		{
			skip_sharp();
			nGrid = GridData.NX*GridData.NY*GridData.NZ;
			GridData.DZMTRXV = new double[nGrid];
			readArray(GridData.DZMTRXV, nGrid);
			GridData.keywords.push_back("DZMTRXV");
		}
		else if (UGStrCmp(kword, "PORO"))
		{
			nGrid = GridData.NX*GridData.NY*GridData.NZ;
			skip_sharp();
			GridData.PORO = new double[nGrid];
			readArray(GridData.PORO, nGrid);
			GridData.keywords.push_back("PORO");
		}
		else if (UGStrCmp(kword, "PERMEABILITY"))
		{
			nGrid = GridData.NX*GridData.NY*GridData.NZ;
			skip_sharp();
			GridData.PERMEABILITY = new double[nGrid];
			readArray(GridData.PERMEABILITY, nGrid);
			GridData.keywords.push_back("PERMEABILITY");
		}
		else if (UGStrCmp(kword, "SOIL"))
		{
			nGrid = GridData.NX*GridData.NY*GridData.NZ;
			skip_sharp();
			GridData.SOIL = new double[nGrid];
			readArray(GridData.SOIL, nGrid);
			GridData.keywords.push_back("SOIL");
		}
		else if (UGStrCmp(kword, "FACIES"))
		{
			nGrid = GridData.NX*GridData.NY*GridData.NZ;
			skip_sharp();
			GridData.FACIES = new int[nGrid];
			readArray(GridData.FACIES, nGrid);
			GridData.keywords.push_back("FACIES");
		}
	}
	return true;
};

bool FileReader::ReadWellData(Wells &wells)
{
	skip_sharp();
	int WellNum;
	bool success = true;
	char WellFile[MAX_STRING_LEN];
	char WellName[MAX_STRING_LEN];
	skip_sharp();
	readName(WellName);
	strcpy_s(WellFile, file_dir);
	strcat_s(WellFile, WellName);
	strcat_s(WellFile, "\\*.well");
	////获取该路径下的所有文件
	vector<string> wellnames;
	vector<string> files = getFiles(WellFile, wellnames);
	WellNum = (int)files.size();
	for (int i = 0; i < WellNum; i++)
	{
		FileReader temp_scanner(*this);
		WellPath tempWells;
		success = temp_scanner.InputWellData(files[i].c_str(), wellnames[i], tempWells);
		wells.push_back(tempWells);
	}
	return success;
	//	readString(WellName);
	//	printf("Read WellData: %s \n", WellName);
	//	skip_sharp();
	//	if (!isdigit(ifp.peek()))
	//	{
	//		char fname[MAX_STRING_LEN];
	//		char filename[MAX_STRING_LEN];
	//		readName(fname);
	//		strcpy(filename, file_dir);
	//		strcat(filename, fname);
	//		FileReader temp_scanner(*this);
	//		success = temp_scanner.InputWellData(filename, Wells);
	//	}
	//	else while (isdigit(ifp.peek()))
	//	{
	//		success = ReadWell(Wells);
	//	}
	//}
}
bool FileReader::InputWellData(const char* filename, string wellname, WellPath &Wells)
{
	ifp.open(filename);
	if (ifp.fail()) {
		cout << "Read Error:" << filename << endl;
		return false;
	};
	//char *tempwellname
	wellname.copy(Wells.wellname, wellname.length(), 0);
	Wells.wellname[wellname.length()] = '\0';
	return ReadWell(Wells);
};
bool FileReader::ReadWell(WellPath &Wells)
{
	char KeyName[MAX_STRING_LEN];
	point temp;
	skip_sharp();
	for (int i = 0; i < 10; i++)
	{
		readString(KeyName);
	}
	skip_sharp();
	while (isdigit(ifp.peek()))
	{
		double X;
		readValue(&X); Wells.MD.push_back(X);
		readValue(&X); temp.x = (X);
		readValue(&X); temp.y = (X);
		readValue(&X); temp.z = (X);
		readValue(&X); //TVD
		readValue(&X); //DX
		readValue(&X); //DY
		readValue(&X); //azmiun;
		readValue(&X); //incl
		readValue(&X); //DLS
		Wells.WellTraject.push_back(temp);
		skip_sharp();
	}
	return true;
}
template<typename numtype>
int FileReader::readArray(numtype* a, const int length)
{
	bool _if_echo = if_echo;
	if_echo = false;

	int count = 0, pos = 0;
	// Read the first value.
	count = readValue(a + pos);

	for (int i = pos + 1; i < pos + count; i++) a[i] = a[pos];// assign the first entry
	pos += count;
	// Read the following values.
	while (pos < length) {
		count = readValue(a + pos);
		if (count != 1){
			for (int i = pos + 1; i < pos + count; ++i) a[i] = a[pos];
		}
		pos += count;
	}
	if_echo = _if_echo;
	return pos;
};

void FileReader::readCOORD(StrucGrid& SGridData, double& XCoord0, double& YCoord0)
{
	bool _if_echo = if_echo;
	if_echo = false;

	int nPillar = (SGridData.NX + 1)*(SGridData.NY + 1);
	delete[] SGridData.Coord; SGridData.Coord = new double[nPillar * 6];

	int pos = readArray(SGridData.Coord, nPillar * 6);
	if (pos != nPillar * 6) {
		//ThrowErr("COORD length is not equal to NX*NY*6.");
	}

	//XCoord0 = SGridData.Coord[0];
	//YCoord0 = SGridData.Coord[1];
	//
	//for (int i = 0; i < nPillar; ++i) {
	//	SGridData.Coord[6 * i + 0] -= XCoord0;
	//	SGridData.Coord[6 * i + 1] -= YCoord0;
	//	SGridData.Coord[6 * i + 3] -= XCoord0;
	//	SGridData.Coord[6 * i + 4] -= YCoord0;
	//}

	if_echo = _if_echo;
};
double* FileReader::readZcorn(const int length) {
	bool _if_echo = if_echo;
	if_echo = false;

	double* Zcorn = new double[length];
	int pos = readArray(Zcorn, length);

	if (pos != length) {
		ThrowErr("ZCORN length is not equal to total corner number.");
	}

	if_echo = _if_echo;
	return Zcorn;
};

bool FileReader::readName(char* ch) {
	skip_slash();
	char c = ifp.peek();
	if (c == '\'') {
		int i = 0;
		ifp.get();
		while ((c = ifp.get()) != '\'' && i<MAX_STRING_LEN) { cbuffer[i] = c; ++i; }
		cbuffer[i] = '\0';
		echo();
		strcpy(ch, cbuffer);
		return true;
	}
	else {
		readString(ch);
		return false;
	}
};
int FileReader::readValue(double* db) {
	readBuffer();
	int starLoc = -1;
	int nchar = (int)strlen(cbuffer);
	for (int i = 1; i < nchar-1; i++){    // Check whether there is a "*"
		if (cbuffer[i] == '*') {
			starLoc = i;
			break;
		}
	}
	if (starLoc != -1) {                     // there is a "*"
		strncpy_s(vbuffer, cbuffer + starLoc + 1, nchar - starLoc - 1);
		*db = atof(vbuffer);                  // convert string to double, *db = b
		strncpy_s(vbuffer, cbuffer, starLoc);
		return atoi(vbuffer);                 // convert string to int, return a
	}
	else {                                  // single number
		*db = atof(cbuffer);
		return 1;
	}
};
int FileReader::readValue(int* int32){
	//if (meet_NaN()) return 0;
	readBuffer();
	int starLoc = -1;
	int nchar = (int)strlen(cbuffer);
	for (int i = 1; i < nchar-1; i++){    // Check whether there is a "*"
		if (cbuffer[i] == '*'){
			starLoc = i;
			break;
		}
	}
	if (starLoc != -1) {                     // there is a "*"
		strncpy_s(vbuffer, cbuffer + starLoc + 1, nchar - starLoc - 1);
		*int32 = atoi(vbuffer);              // convert string to int32, *int32 = b
		strncpy_s(vbuffer, cbuffer, starLoc);
		return atoi(vbuffer);                 // convert string to int, return a
	}
	else {                                  // single number
		*int32 = atoi(cbuffer);
		return 1;
	}
};
int FileReader::readInt() {
	readBuffer();
	return atoi(cbuffer);
};
double FileReader::readReal() {
	readBuffer();
	return atof(cbuffer);
};
bool FileReader::read_Idx_or_Name(char* ch) {
	skip_sharp();
	if (ifp.peek() == '\'') {
		int i = 0;
		char c = ifp.get();
		while ((c = ifp.get()) != '\'') { cbuffer[i] = c; ++i; }

		cbuffer[i] = '\0';
		echo();
		strcpy_s(ch, MAX_STRING_LEN, cbuffer);
		return true;
	}
	else if (meet_NaN()) {
		return false;
	}
	else {
		int num = readInt();
		_itoa_s(num, ch, MAX_STRING_LEN, 10);
		return true;
	}
};
void FileReader::readString(char* ch){
	readBuffer();
	strcpy_s(ch, MAX_STRING_LEN, cbuffer);
};
bool FileReader::readBuffer(){
	if (ifp.eof()) {
		if (if_echo) cout << "Reach the end of file." << endl;
		return false;
	}
	skip_slash();
	ifp >> cbuffer;
	//echo();
	return true;
};
void FileReader::echo() {
	if (if_echo) {
		if (echoline_len != 0) {
			//std::cout << cbuffer << " ";
			++echo_count;
			if (echo_count == echoline_len) {
				//std::cout << "\n";
				echo_count = 0;
			}
		}
		else {
			;// std::cout << cbuffer << std::endl;
		}
	}
};
void FileReader::skip_slash(){
	while (!ifp.eof()) {
		char ch;
		ifp >> ch;
		if (ch == '#' || ch == '/') {
			ifp.getline(cbuffer, MAX_STRING_LEN, '\n');
		}
		else {
			ifp.putback(ch);
			return;
		}
	}
};
void FileReader::skip_sharp(){
	while (!ifp.eof()) {
		char ch;
		ifp >> ch;
		if (ch == '#') {
			ifp.getline(cbuffer, MAX_STRING_LEN, '\n');
			if (if_echo)
			{
				//cout << ch << cbuffer << endl;
			}
		}
		else if (ch == '-')
		{
			ifp.getline(cbuffer, MAX_STRING_LEN, '\n');
			if (if_echo)
			{
				//cout << ch << cbuffer << endl;
			}
		}
		else {
			ifp.putback(ch);
			return;
		}
	}
};
bool FileReader::meet_NaN() {
	skip_sharp();
	char ch = ifp.peek();
	if (UGIsDigit(&ch)) {
		return false;
	}
	else {
		if (ch == '/') {
			ifp >> ch;
		}
		return true;
	}
};
void FileReader::new_echoline(const int _len) {
	echoline_len = _len;
	echo_count = 0;
};
void FileReader::end_echoline() {
	if (echo_count != 0) { cout << endl; }
	echo_count = 0;
};
void TypeFileDir(char* FileDir){
	char tmp_dir[MAX_STRING_LEN] = { 0 };
	while (tmp_dir[0] == '\0') {
		cin.getline(tmp_dir, MAX_STRING_LEN);
	}

	int dir_beg = 0, dir_len = (int)strlen(tmp_dir);
	if (tmp_dir[0] == '\"' && tmp_dir[dir_len - 1] == '\"') {
		tmp_dir[dir_len - 1] = '\0';
		dir_beg = 1;
		dir_len -= 2;
	}

	if (_stricmp(tmp_dir + dir_beg + dir_len - 4, ".DAT") == 0) { // 去除".dat"后缀
		tmp_dir[dir_beg + dir_len - 4] = '\0';
		dir_len -= 4;
	}

	strcpy_s(FileDir, MAX_STRING_LEN, tmp_dir + dir_beg);
};

vector<string> getFiles(string cate_dir, vector<string> &filenames)
{
	vector<string> files;//存放文件名
	string p;  //string类很有意思的一个赋值函数:assign()，有很多重载版本		
	p = cate_dir;
	int file_dir_right = (int)p.length();
	while (file_dir_right > 0 && p[file_dir_right - 1] != '\\') {
		--file_dir_right;
	}
	p.erase(file_dir_right, p.length());
	//p[file_dir_right - 1] = '\0';
	__finddata64_t  file;
	__int64 lf;
	//输入文件夹路径  
	if ((lf = _findfirst64(cate_dir.c_str(), &file)) == -1) {
		cout << cate_dir << " not found!!!" << endl;
	}
	else {
		while (_findnext64(lf, &file) == 0) {
			//cout << file.name << endl;
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			string tempstr;
			tempstr.append(p); tempstr.append(file.name);;
			files.push_back(tempstr);
			filenames.push_back(file.name);
		}
	}
	_findclose(lf);

#ifdef linux  
	DIR *dir;
	struct dirent *ptr;
	char base[1000];

	if ((dir = opendir(cate_dir.c_str())) == NULL)
	{
		perror("Open dir error...");
		exit(1);
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)    ///current dir OR parrent dir  
			continue;
		else if (ptr->d_type == 8)    ///file  
			//printf("d_name:%s/%s\n",basePath,ptr->d_name);  
			files.push_back(ptr->d_name);
		else if (ptr->d_type == 10)    ///link file  
			//printf("d_name:%s/%s\n",basePath,ptr->d_name);  
			continue;
		else if (ptr->d_type == 4)    ///dir  
		{
			files.push_back(ptr->d_name);
			/*
			memset(base,'\0',sizeof(base));
			strcpy(base,basePath);
			strcat(base,"/");
			strcat(base,ptr->d_nSame);
			readFileList(base);
			*/
		}
	}
	closedir(dir);
#endif

	//排序，按从小到大排序  
	sort(files.begin(), files.end());
	return files;
}
void Getfilename(char *filename)
{
	//char  file_dir[MAX_STRING_LEN];
	// Extract file direction		
	//strcpy_s(file_dir, filename);
	int file_dir_right = (int)strlen(filename);
	while (file_dir_right > 0 && filename[file_dir_right - 1] != '.') {
		--file_dir_right;
	}
	filename[file_dir_right - 1] = '\0';
}

bool FileReader::readMergeFacies(vector<FaciesMerge> &Faices){
	int v1;
	FaciesMerge Face1;
	while (isdigit(ifp.peek()))
	{
		v1 = readInt(); Face1.i = v1;
		v1 = readInt(); Face1.j = v1;
		v1 = readInt(); Face1.k = v1;
		Faices.push_back(Face1);
		skip_sharp();
	}
	return true;
}