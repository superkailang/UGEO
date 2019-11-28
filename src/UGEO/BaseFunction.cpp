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
*  @file     BaseFunction.cpp												 *
*  @brief    Base function for computer Geometry                             *
*  Details.                                                                  *
*                                                                            *
*  @author   kailanghuang                                                    *
*  @email    kailanghuang@pku.edu.cn                                         *
*  @version  2.0.0.1		                                                 *
*  @date     2018/01, 2019/1												 *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         :  Base function for computer Geometry                     *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2018/01/24 | 1.0.0.1   | kailanghuang      | Create file                  *
*  2018/04/10 | 2.0.0.1   | kailanghuang      | add In_planes function       *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/

#include"BaseFunction.h"
/**
* @UMSM Namespace
*/
namespace UMSM {


	bool In_planes(point pt1, point pt2, point pt3, point pt4){
		/*
			Coplanar 3D Points (p1,p2,p3,p4) 
			* @return
			*    true  : p1,p2,p3,p4 are in the same plane 
			*    false : p1,p2,p3,p4 are not in the same plane 
		*/
		double a1 = pt2.x - pt1.x;
		double b1 = pt2.y - pt1.y;
		double c1 = pt2.z - pt1.z;
		double a2 = pt3.x - pt1.x;
		double b2 = pt3.y - pt1.y;
		double c2 = pt3.z - pt1.z;
		double a3 = pt4.x - pt1.x;
		double b3 = pt4.y - pt1.y;
		double c3 = pt4.z - pt1.z;
		//  if the mixture product of three vectors (AB,AC,AD) == 0 
		double value = (a1*b2*c3 + a2*b3*c1 + a3*b1*c2 - a3*b2*c1 - a1*b3*c2 - a2*b1*c3);
		return abs(value)<1E-4;
	}

	bool Intersect_3d_lines(point pt1, point pt2, point pt3, point pt4, point &crossPt){
		/*
		judge if two lines (p1->p2,p3->p4) in the same plane
		* @return
		*    true  : if line p1->p2 intersect with line p3->p4  
		*    false : else 
		*    crossPt:  if intersect, return cross point of two lines
		*/
		double a1 = pt2.x - pt1.x;
		double b1 = pt2.y - pt1.y;
		double c1 = pt2.z - pt1.z;
		double a2 = pt3.x - pt1.x;
		double b2 = pt3.y - pt1.y;
		double c2 = pt3.z - pt1.z;
		double a3 = pt4.x - pt1.x;
		double b3 = pt4.y - pt1.y;
		double c3 = pt4.z - pt1.z;
		////  if the mixture product of three vectors (AB,AC,AD) == 0 
		double value = (a1*b2*c3 + a2*b3*c1 + a3*b1*c2 - a3*b2*c1 - a1*b3*c2 - a2*b1*c3);
		if (abs(value) > 200){
			printf("value: %.2f, There is no intersection between two lines !\n",value);
			return false;
		}
		else{
			// By calculation, the coordinates of the intersection point are obtained
			crossPt = pt1;
			double ratio = a1*(pt4.y - pt3.y) - b1*(pt4.x - pt3.x);
			double y_ratio = a1*(pt4.z - pt3.z) - c1 * (pt4.x - pt3.x);
			double z_ratio = b1*(pt4.z - pt3.z) - c1 * (pt4.y - pt3.y);
			if (abs(ratio) > InfSmall){
				double r = a2* (pt4.y - pt3.y) - b2* (pt4.x - pt3.x) / ratio;
				double x_value = r * a1 + pt1.x;
				double y_value = r * b1 + pt1.y;
				double z_value = r * c1 + pt1.z;
				crossPt.x = x_value;
				crossPt.y = y_value;
				crossPt.z = z_value;
			}
			if (abs(y_ratio) > InfSmall){
				double r = (a2* (pt4.z - pt3.z) - c2* (pt4.x - pt3.x)) / y_ratio;
				double x_value = r * a1 + pt1.x;
				double y_value = r * b1 + pt1.y;
				double z_value = r * c1 + pt1.z;
				crossPt.x = x_value;
				crossPt.y = y_value;
				crossPt.z = z_value;
			}
			else if (abs(z_ratio) > InfSmall){
				double r = (b2* (pt4.z - pt3.z) - c2* (pt4.y - pt3.y)) / z_ratio;
				double x_value = r * a1 + pt1.x;
				double y_value = r * b1 + pt1.y;
				double z_value = r * c1 + pt1.z;
				crossPt.x = x_value;
				crossPt.y = y_value;
				crossPt.z = z_value;
			}
			return false;
		}
	}

	bool get_plane_crossPts(point v2, point v1, point u1, point u2, point u3, point &crossPt){
		double dx = v2.x - v1.x;
		double dy = v2.y - v1.y;
		double dz = v2.z - v1.z;

		double a2 = u2.x - u1.x;
		double b2 = u2.y - u1.y;
		double c2 = u2.z - u1.z;
		double a3 = u3.x - u1.x;
		double b3 = u3.y - u1.y;
		double c3 = u3.z - u1.z;
		//  if the mixture product of three vectors (AB,AC,AD) == 0 
		double a0 = b2*c3 - b3*c2;
		double b0 = a3*c2 - a2*c3;
		double c0 = a2*b3 - a3*b2;

		double dd = (a0 * dx + b0 * dy + c0*dz);
		double ratio = 0;
		bool isZero = !GPGzero(dd);
		if (isZero){
			ratio = -( a0*(v1.x - u1.x) + b0*(v1.y - u1.y) + c0*(v1.z - u1.z)) / dd;
			double x_value = ratio * dx + v1.x;
			double y_value = ratio * dy + v1.y;
			double z_value = ratio * dz + v1.z;
			crossPt.x = x_value;
			crossPt.y = y_value;
			crossPt.z = z_value;
		}
		else{
			double a1 = v1.x - u1.x;
			double b1 = v1.y - u1.y;
			double a2 = u2.x - u1.x;
			double b2 = u2.y - u1.y;
			double c2 = u2.z - u1.z;
			double a3 = u3.x - u1.x;
			double b3 = u3.y - u1.y;
			double c3 = u3.z - u1.z;
			//  if the mixture product of three vectors (AB,AC,AD) == 0 
			double c1 = -(a1*b2*c3 + a3*b1*c2 - a1*b3*c2 - a2*b1*c3)/(a2*b3 - a3*b2);
			crossPt = v1;
			crossPt.z = u1.z + c1;
		}
		if (!In_planes(u1, u2, u3, crossPt)){
		 	cout << "non-coplanar"<< endl;
		}
		return true;
	}

	const char * QString2Char(QString std){
		std::string str = std.toStdString();
		const char* ch = str.c_str();
		return ch;
	}

	void GetCornerBounds(point t1, point t2, double *Bounds){
		/*
			get Bounds from two points (p1,p2)
			* @return
			*    Bounds: (x1,y1)->(x2,y2)
		*/
		if (t1.x> t2.x){
			Bounds[0] = t2.x;
			Bounds[1] = t1.x;
		}
		else{
			Bounds[1] = t2.x;
			Bounds[0] = t1.x;
		}
		if (t1.y> t2.y){
			Bounds[2] = t2.y;
			Bounds[3] = t1.y;
		}
		else{
			Bounds[3] = t2.y;
			Bounds[2] = t1.y;
		}
		if (t1.z> t2.z){
			Bounds[4] = t2.z;
			Bounds[5] = t1.z;
		}
		else{
			Bounds[5] = t2.z;
			Bounds[4] = t1.z;
		}
	}

	vector<string> split(const string &s, const string &seperator){
		vector<string> result;
		typedef string::size_type string_size;
		string_size i = 0;

		while (i != s.size()){
			// Find the first letter in the string that does not equal the seperator
			int flag = 0;
			while (i != s.size() && flag == 0){
				flag = 1;
				for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x]){
					++i;
					flag = 0;
					break;
				}
			}

			// Find another separator and pull out the string between the two
			flag = 0;
			string_size j = i;
			while (j != s.size() && flag == 0){
				for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x]){
					flag = 1;
					break;
				}
				if (flag == 0)
					++j;
			}
			if (i != j){
				result.push_back(s.substr(i, j - i));
				i = j;
			}
		}
		return result;
	}

	bool Intercept(const point L1, const point L2, const double Z, point& e) {
		double dz = L2.z - L1.z;
		double dx = L2.x - L1.x;
		double dy = L2.y - L1.y;
		if (fabs(dz) < InfSmall)
		{
			if ((fabs(dx) > InfSmall) || (fabs(dy) > InfSmall))
			{
				e.x = 0.5 * (L2.x + L1.x);
				e.y = 0.5 * (L2.y + L1.y);
				e.z = Z;
				return false;
			}
			else {
				e.x = L1.x;
				e.y = L1.y;
				e.z = Z;
				return true;
			}
		}
		else {
			double portion = (Z - L1.z) / dz;
			e.x = portion * dx + L1.x;
			e.y = portion * dy + L1.y;
			e.z = Z;
			return true;
		}
	};

	//  get Z depth from Line L1->L2
	bool Intercept(const point L1, const point L2, point &Z) {
		double v1 = sqrt((L2.x - L1.x)*(L2.x - L1.x) + (L2.y - L1.y)*(L2.y - L1.y));
		double v2 = sqrt((Z.x - L1.x)*(Z.x - L1.x) + (Z.y - L1.y)*(Z.y - L1.y));				
		double portion = v2 / v1;
		double dz = L1.z; 
		Z.z = portion * dz + L1.z;
		return true;		
	};

	// Judge that the two lines are parallel
	int parallel(const point& u1, const point& u2, const point& v1, const point& v2){
		return GPGzero(u1.x - u2.x)*(v1.y - v2.y) - (v1.x - v2.x)*(u1.y - u2.y);
	}

	bool intersection(const point& u1, const point& u2, const point& v1, const point& v2,point &ret){
		/*
			* Calculate the intersection point of two line segments (u1,u2), (v1,v2)
			* @return
			*		  true:  two line segments intersect
			*		  false: else
			*	      cross point:  ret is the cross point of two lines
		*/
		ret = u1;
		double value = ((u1.x - u2.x)*(v1.y - v2.y) - (u1.y - u2.y)*(v1.x - v2.x));
		bool isFlag = GPGzero(value);
		if ( !isFlag ){
			double t = ((u1.x - v1.x)*(v1.y - v2.y) - (u1.y - v1.y)*(v1.x - v2.x))
				/ ( (u1.x - u2.x)*(v1.y - v2.y) - (u1.y - u2.y)*(v1.x - v2.x) );
			ret.x += (u2.x - u1.x)*t;
			ret.y += (u2.y - u1.y)*t;
			ret.z += (u2.z - u1.z)*t;
			return true;
		}
		else{
			return false;
		}
	}

	point operator+(const point& v1, const point& v2) {
		point v3;
		v3.x = v1.x + v2.x;
		v3.y = v1.y + v2.y;
		v3.z = v1.z + v2.z;
		return v3;
	};

	point operator-(const point& v1, const point& v2) {
		point v3;
		v3.x = v1.x - v2.x;
		v3.y = v1.y - v2.y;
		v3.z = v1.z - v2.z;
		return v3;
	};

	point operator*(point & v1, const double d)
	{
		point v3;
		v3.x = v1.x * d;
		v3.y = v1.y * d;
		v3.z = v1.z * d;
		return v3;
	}

	point operator/(point & v1, const double d)
	{
		point v3;
		v3.x = v1.x / d;
		v3.y = v1.y / d;
		v3.z = v1.z / d;
		return v3;
	}

	//cross product of vectors (right hand)
	point operator^(const point& v1, const point& v2) {
		point v3;
		v3.x = v1.y*v2.z - v1.z*v2.y;
		v3.y = v1.z*v2.x - v1.x*v2.z;
		v3.z = v1.x*v2.y - v1.y*v2.x;
		return v3;
	};

	// Left hand
	point OutProd(const point& v1, const point& v2) {
		point v3;
		v3.x = v1.z * v2.y - v1.y * v2.z;
		v3.y = v1.x * v2.z - v2.x * v1.z;
		v3.z = v1.x * v2.y - v2.x * v1.y;
		return v3;
	};

	// inner product
	double operator*(point& v1, point& v2)
	{
		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
	}

	// L2-norm
	double VxNorm(const point& v) {
		return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	};
	point TriProj(const point& v1, const point& v2, const point& v3){
		point A;
		A.x = 0.5 * ((v3.z - v2.z)*(v1.y - v2.y) - (v3.y - v2.y)*(v1.z - v2.z));
		A.y = 0.5 * ((v3.x - v2.x)*(v1.z - v2.z) - (v3.z - v2.z)*(v1.x - v2.x));
		A.z = 0.5 * ((v3.y - v2.y)*(v1.x - v2.x) - (v3.x - v2.x)*(v1.y - v2.y));
		return A;
	};

	point PolygProj(const int N, const point* vs){
		point A(0.0, 0.0, 0.0);
		for (int i = 0; i < N - 2; ++i) {
			A = A + TriProj(vs[0], vs[i + 1], vs[i + 2]);
		}
		return A;
	};

	double PolygArea(const int N, const point* vs, point& c)
	{
		point A = PolygProj(N, vs, c);
		return VxNorm(A);
	};

	point PolygProj(const int N, const point* vs, point& c)
	{
		point A(0.0, 0.0, 0.0);

		if (N > 2) {
			double S = 0.0;
			c.x = c.y = c.z = 0.0;
			for (int i = 0; i < N - 2; ++i) {
				point a = TriProj(vs[0], vs[i + 1], vs[i + 2]);
				A = A + a;

				double s = VxNorm(a);
				S = S + s;
				c = c + (vs[0] + vs[i + 1] + vs[i + 2]) * (s * 1 / 3);

			}
			c = c / S;
		}
		else if (N == 2) {
			c = (vs[0] + vs[1]) * 0.5;
		}
		else {
			c = vs[0];
		}

		return A;
	};

	double FCenter(const point &s1, const point &s2, const point &s3, const point &s4, point& fc)
	{
		point A1 = TriProj(s1, s2, s3);
		point A2 = TriProj(s3, s4, s1);
		point SUM1 = s1 + s2 + s3;
		point SUM2 = s3 + s4 + s1;
		double a1 = VxNorm(A1) * 0.5, a2 = VxNorm(A2) * 0.5;
		double a = a1 + a2;
		if (a < InfSmall) {
			a1 = a2 = 1.0 / 6.0;
		}
		else {
			a1 = a1 / (3.0 * a);
			a2 = a2 / (3.0 * a);
		}

		fc.x = a1 * SUM1.x + a2 * SUM2.x;
		fc.y = a1 * SUM1.y + a2 * SUM2.y;
		fc.z = a1 * SUM1.z + a2 * SUM2.z;

		return a;
	};

	double TetraVol(const point& P, const point& a, const point& b, const point& c) {
		double ax = a.x - P.x, ay = a.y - P.y, az = a.z - P.z;
		double bx = b.x - P.x, by = b.y - P.y, bz = b.z - P.z;
		double cx = c.x - P.x, cy = c.y - P.y, cz = c.z - P.z;
		double Volume = fabs((ay*bz - az*by)*cx + (az*bx - ax*bz)*cy + (ax*by - ay*bx)*cz) / (double)6.0;
		return Volume;
	};

	point ArithmCenter(const point &a, const point &b, const point &c, const point &d) {
		point e;
		e.x = 0.25*(a.x + b.x + c.x + d.x);
		e.y = 0.25*(a.y + b.y + c.y + d.y);
		e.z = 0.25*(a.z + b.z + c.z + d.z);
		return e;
	};

	double HexVol(const point* corners, point& Centroid) {

		point c1 = ArithmCenter(corners[0], corners[2], corners[1], corners[4]);
		double v1 = TetraVol(corners[0], corners[2], corners[1], corners[4]);

		point c2 = ArithmCenter(corners[3], corners[2], corners[1], corners[7]);
		double v2 = TetraVol(corners[3], corners[2], corners[1], corners[7]);

		point c3 = ArithmCenter(corners[5], corners[4], corners[1], corners[7]);
		double v3 = TetraVol(corners[5], corners[4], corners[1], corners[7]);

		point c4 = ArithmCenter(corners[6], corners[2], corners[4], corners[7]);
		double v4 = TetraVol(corners[6], corners[2], corners[4], corners[7]);

		point c5 = ArithmCenter(corners[1], corners[2], corners[4], corners[7]);
		double v5 = TetraVol(corners[1], corners[2], corners[4], corners[7]);

		double v = v1 + v2 + v3 + v4 + v5;

		if (v > InfSmall) {
			Centroid.x = (v1 * c1.x + v2 * c2.x + v3 * c3.x + v4 * c4.x + v5 * c5.x) / v;
			Centroid.y = (v1 * c1.y + v2 * c2.y + v3 * c3.y + v4 * c4.y + v5 * c5.y) / v;
			Centroid.z = (v1 * c1.z + v2 * c2.z + v3 * c3.z + v4 * c4.z + v5 * c5.z) / v;
		}
		else {
			FCenter(corners[0], corners[2], corners[3], corners[1], Centroid);
		}

		return v;
	}

	point CPCell::AreaVec(const int dir) {
		point square[4];
		if (dir == XDir) {
			square[0] = corner[1]; square[3] = corner[3];
			square[1] = corner[5]; square[2] = corner[7];
		}
		else if (dir == -XDir) {
			square[1] = corner[2]; square[0] = corner[0];
			square[2] = corner[6]; square[3] = corner[4];
		}
		else if (dir == YDir) {
			square[1] = corner[3]; square[0] = corner[2];
			square[2] = corner[7]; square[3] = corner[6];
		}
		else if (dir == -YDir) {
			square[0] = corner[0]; square[3] = corner[1];
			square[1] = corner[4]; square[2] = corner[5];
		}
		else if (dir == ZDir) {
			square[0] = corner[4]; square[3] = corner[5];
			square[1] = corner[6]; square[2] = corner[7];
		}
		else {
			square[3] = corner[2]; square[2] = corner[3];
			square[0] = corner[0]; square[1] = corner[1];
		}
		return PolygProj(4, square);
	};

	double CPCell::Gen_GeomCenter() {
		// Gen face centroid
		FCenter(corner[1], corner[5], corner[7], corner[3], facec[0]);
		FCenter(corner[0], corner[4], corner[6], corner[2], facec[1]);
		FCenter(corner[2], corner[3], corner[7], corner[6], facec[2]);
		FCenter(corner[0], corner[1], corner[5], corner[4], facec[3]);
		FCenter(corner[4], corner[6], corner[7], corner[5], facec[4]);
		FCenter(corner[0], corner[2], corner[3], corner[1], facec[5]);

		double Vol;
		Vol = HexVol(corner, blockc);

		if (!is_act) Vol = 0.0;

		return Vol;
	}

	void ThrowErr(const char *format, ...) {
		printf("\n Simulator suspended due to error\n >>Press ENTER to exit.");
		getchar();
		exit(EXIT_FAILURE);
	};
	bool is_zero(const double x, const double tol) {
		return (x<tol && x>-tol);
	};

	void Center(point *v, point &center)
	{
		center.x = center.y = center.z = 0;
		int Num = 8;
		for (int i = 0; i < Num; i++)
		{
			center.x += v[i].x / Num;
			center.y += v[i].y / Num;
			center.z += v[i].z / Num;
		}
	};

};