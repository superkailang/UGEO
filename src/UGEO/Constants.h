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
*  @file     Constants.h													 *
*  @brief    Constants														 *
*  Details.                                                                  *
*                                                                            *
*  @author   kailanghuang                                                    *
*  @email    kailanghuang@pku.edu.cn                                         *
*  @version  2.0.0.1		                                                 *
*  @date     2018/01, 2019/1												 *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         :  Int and float constants. Do not modify this file!		 *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

#ifndef Constants_h__
#define Constants_h__

const double InfSmall = 1E-20;
const double InfLarge = 1E+20;
const double smallFloat = 1E-6;
const double PI = 3.1415926;

const int MAX_STRING_LEN = 512;
const double FloatSmall = 1E-6;

// Connection type code
const int XDir = 1;            // 00000001, cross x direction                                 
const int YDir = 2;            // 00000010, cross y direction                                 
const int ZDir = 4;            // 00000100, cross z direction   
const double NearZero = 1E-10;
const double miniValue = 1E-4;

#endif // Constants_h__