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
*  @file     fileReader.h													 *
*  @brief    file IO 														 *
*  Details.                                                                  *
*                                                                            *
*  @author   kailanghuang                                                    *
*  @email    kailanghuang@pku.edu.cn                                         *
*  @version  2.0.0.1		                                                 *
*  @date     2018/01, 2019/1												 *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         :  read Eclipse or Petrel Grid File						 *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

#ifndef FILEREADER_H
#define FILEREADER_H
#include"qstringlist.h"
#include <fstream>
#include <string>
#include <iostream>
#include <io.h>
#include"Constants.h"

#include"IJKGrid.h"
#include"struct.h"

using namespace UMSM;

inline bool UGStrCmp(const char* a, const char* b) { return (_stricmp(a, b) == 0); };
inline bool UGIsDigit(const char* a) { return (isdigit(a[0]) || a[0] == '-' || a[0] == '.'); };

class FileReader {
	public:
		FileReader(const bool _if_echo);
		FileReader(FileReader& obj);
		~FileReader();
		bool Data_File_Dialog(int &FileIndex, QString &fileName);
		bool InputData(const char* filename, Wells &wells, StrucGrid& GridData);
	private:

		QStringList ListFilters;  // file filter 
		QString filters;

		char   file_dir[MAX_STRING_LEN];

		bool ReadWell(WellPath &Wells);
		bool InputWellData(const char* filename, string wellname, WellPath &Wells);
		bool ReadWellData(Wells &wells);
		bool readMergeFacies(vector<FaciesMerge> &Faices);

		bool   readBuffer();
		int    readInt();
		int    readDir(); ///
		double readReal();
		bool readName(char* ch);
		bool   read_Idx_or_Name(char* ch);
		int    readValue(double* db);
		int    readValue(int* int32);
		void   readString(char* ch);
		template<typename numtype>
		int    readArray(numtype* a, const int len);

		void   skip_sharp();
		void   skip_slash();
		bool   meet_NaN();//if next char number
		void   echo();
		void   new_echoline(const int _len);
		void   end_echoline();
		bool   FileEnd() { return ifp.peek() == EOF; };
		std::ifstream ifp;
		bool   if_echo;
		int    echoline_len;
		int    echo_count;
		int    judge;
		char   cbuffer[MAX_STRING_LEN];
		char   vbuffer[MAX_STRING_LEN];

		void readCOORD(StrucGrid& SGridData, double& XCoord0, double& YCoord0);
		double *readZcorn(const int length);
	};
	void TypeFileDir(char* FileDir);
	vector<string> getFiles(string cate_dir, vector<string> &filenames);
	void Getfilename(char *filename);
#endif