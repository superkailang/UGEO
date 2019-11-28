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
*  @file     Struct.h														 *
*  @brief    Base  Struct													 *
*  Details.                                                                  *
*                                                                            *
*  @author   kailanghuang                                                    *
*  @email    kailanghuang@pku.edu.cn                                         *
*  @version  2.0.0.1		                                                 *
*  @date     2018/01, 2019/1												 *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         :  Base Struct											 *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

#ifndef STRUCT_H
#define STRUCT_H

#include"QVTKWidget.h"
#include"vtkSmartPointer.h"
#include"vtkRenderer.h"
#include"vtkImageActor.h"
#include<iostream>
#include<vector>

#include"Gridstruct.h"
using namespace std;
/**
* @UMSM Namespace
*/
using namespace UMSM;

	typedef vector<int> intArray;  // Define the vector container for int 
	typedef vector<double> realArray;   // Define the vector container for double 

	struct QVTKWidgetView
	{
		QVTKWidget* qvtkwidget;
		vtkSmartPointer< vtkRenderer> ren;
		QVTKWidgetView()
		{
			qvtkwidget = new QVTKWidget();
			ren = vtkSmartPointer< vtkRenderer>::New();
		};
	};

	typedef vector<point> PointArray; //Define the vector container points

	
	struct ContourGrid
	{
		int Row;
		int Col;
		point *Grids;
	};	
	struct CellRange
	{
		double minValue;
		double maxValue;
	};
	struct queryState{
		int LOD;
		bool isSpace;
		bool isProperty;
	};


#endif