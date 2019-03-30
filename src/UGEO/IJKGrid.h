/** 
 * @file    IJKGrid.h
 * @author  Li, Xiang
 * @date    2013/08, 2014/09
 * @brief   Structured grid definition and boundary condition
 * @version 
 * Copyright (c) Ennosoft Co., 2016
 */

#ifndef IJKGrid_h__
#define IJKGrid_h__
#include"Gridstruct.h"
#include"BaseFunction.h"
namespace UMSM {
	class StrucGrid {
	public:
		StrucGrid() {
			NXNYNZ_info = DXDY_info = DZ_info = XYCoord_info = 0; NX = NY = NZ = 0; PoroLvl = 1; CPGCells = nullptr;
			DX = DY = DZ = Xcoord = Ycoord = Depth = nullptr; Coord = nullptr; Zcorn = nullptr;
		};

		~StrucGrid() { 
			/*if (CPGCells != nullptr){
				delete[] CPGCells;
			}
			if (Coord != nullptr){
				delete[] Coord;
			}
			if (Zcorn != nullptr){
				delete[] Zcorn;
			}
			*/
		};

		void GenCellCorner();
		void Clear_Coord_Info();

		void ImportNXNYNZ(const int _NX, const int _NY, const int _NZ);
		void TOP2DEPTH(const int Top_len, double* Top, double* Bottom);
		void DEPTH2TOP(const int Top_len, double* Top, double* Bottom);
		void CartGrid_CPGCell();
		void CartGrid_Pilar();
		void Corner_X(const int N0, const int G0, const int NElm, const int* NTG, const double x0, float* x) const;
		void Corner_Y(const int N0, const int G0, const int NElm, const int* NTG, const double y0, float* y) const;
		void Corner_Z(const int N0, const int G0, const int NElm, const int* NTG, const double z0, float* z) const;

		bool HasNXNYNZ() const { return (NXNYNZ_info > 0); };
		bool HasDXDYDZ() const { return (NXNYNZ_info > 0 && DXDY_info > 0 && DZ_info > 0); };
		bool HasDZ() const { return DZ_info > 0; };
		bool HasXYZCoord() const { return (XYCoord_info > 0 && Depth != nullptr); };
		void Loc2IJK(const int LOC, int& i, int& j, int& k) const
		{
			if (HasDXDYDZ()) {
				LOC_IJK(LOC, NX, NY, i, j, k);
			}
			else {
				i = LOC; j = 0; k = 0;
			}
		};
		int IJK2Loc(const int i, const int j, const int k) const {
			return i + j * NX + k * NX * NY;
		};

		double findDX(const int LOC) const;
		double findDY(const int LOC) const;
		double findDZ(const int LOC) const;

		double findDX(const int i, const int j, const int k) const;
		double findDY(const int i, const int j, const int k) const;
		double findDZ(const int i, const int j, const int k) const;

		// 0--not contact; 1--partial contact; 2--total contact
		int XContact(const int i, const int j, const int k, const int k2, double& Area) const;
		int YContact(const int i, const int j, const int k, const int k2, double& Area) const;

		void Center(const int LOC, double& x, double& y, double& z) const;
		point VA(const int L, const int dir) const;
		point PN(const int L, const int dir) const;

		int NXNYNZ_info;  // 0--none; 1--exist 
		int DXDY_info;    // 0--none; 1--equal; 2--enum 
		int DZ_info;      // 0--none; 1--equal; 2--enum 
		int XYCoord_info; // 0--none; 1--equal; 2--enum 

		int NX, NY, NZ, PoroLvl=1;
		int NGrid;
		double *DX, *DY, *DZ;
		double *Xcoord, *Ycoord, *Depth;

		vector<PhaseMap> Phasemaps;
		double* Coord;
		double* Zcorn;
		CPCell* CPGCells;
		double XCoord0, YCoord0;
		int *ACTNUM;
		double *BULKVOLUME;
		double *DZMTRXV;
		double *PORO;
		double *PERMEABILITY;
		double *SOIL;
		int *FACIES;
		vector<string> keywords;
		bool *Flag;
		vector<FaciesMerge> FMerge;
	private:
		DISALLOW_COPY_AND_ASSIGN(StrucGrid);
	};
};
#endif // IJKGrid_h__