/** 
 * @file    IJKGrid.cpp
 * @author  Li, Xiang
 * @date    2013/08, 2014/9
 * @brief   Structured grid and LGR definition 
 * @version 
 * Copyright (c) Ennosoft Co., 2016
 */

#include <iostream>
#include "IJKGrid.h"

using namespace std;

namespace UMSM {

void StrucGrid::ImportNXNYNZ(const int _NX, const int _NY, const int _NZ){
    NXNYNZ_info = 1;
    NX = _NX;
    NY = _NY;
    NZ = _NZ;
};

void StrucGrid::GenCellCorner()
{
    int NLayer = NZ / PoroLvl;
    delete[] CPGCells; 
    CPGCells = new CPCell[NX * NY * NLayer];

    //find coordinates of each corner, calculate cell center and cell face centers
    int i, j, k;
    int L1, L2, L3, L4, ZLoc;
    point Line1[2], Line2[2], Line3[2], Line4[2];
    for (j = 0; j < NY; j++) {
        for (i = 0; i < NX; i++) {

            L1 = (i + j*(NX + 1)) * 6;
            Line1[0].x = Coord[L1];   Line1[0].y = Coord[L1 + 1]; Line1[0].z = Coord[L1 + 2];
            Line1[1].x = Coord[L1 + 3]; Line1[1].y = Coord[L1 + 4]; Line1[1].z = Coord[L1 + 5];

            L2 = (i + 1 + j*(NX + 1)) * 6;
            Line2[0].x = Coord[L2];   Line2[0].y = Coord[L2 + 1]; Line2[0].z = Coord[L2 + 2];
            Line2[1].x = Coord[L2 + 3]; Line2[1].y = Coord[L2 + 4]; Line2[1].z = Coord[L2 + 5];

            L3 = (i + (j + 1)*(NX + 1)) * 6;
            Line3[0].x = Coord[L3];   Line3[0].y = Coord[L3 + 1]; Line3[0].z = Coord[L3 + 2];
            Line3[1].x = Coord[L3 + 3]; Line3[1].y = Coord[L3 + 4]; Line3[1].z = Coord[L3 + 5];

            L4 = (i + 1 + (j + 1)*(NX + 1)) * 6;
            Line4[0].x = Coord[L4];   Line4[0].y = Coord[L4 + 1]; Line4[0].z = Coord[L4 + 2];
            Line4[1].x = Coord[L4 + 3]; Line4[1].y = Coord[L4 + 4]; Line4[1].z = Coord[L4 + 5];

            for (k = 0; k < NLayer; k++) {
                CPCell &c = CPGCells[i + j*NX + k*NX*NY];
                ZLoc = k * 2 * NX * 2 * NY * 2 + j * 4 * NX + 2 * i;

                c.is_act &= Intercept(Line1[0], Line1[1], Zcorn[ZLoc], c.corner[0]);
                c.is_act &= Intercept(Line2[0], Line2[1], Zcorn[ZLoc + 1], c.corner[1]);
                c.is_act &= Intercept(Line3[0], Line3[1], Zcorn[ZLoc + 2 * NX], c.corner[3]);
                c.is_act &= Intercept(Line4[0], Line4[1], Zcorn[ZLoc + 2 * NX + 1], c.corner[2]);
                c.is_act &= Intercept(Line1[0], Line1[1], Zcorn[ZLoc + 4 * NX*NY], c.corner[4]);
                c.is_act &= Intercept(Line2[0], Line2[1], Zcorn[ZLoc + 4 * NX*NY + 1], c.corner[5]);
                c.is_act &= Intercept(Line3[0], Line3[1], Zcorn[ZLoc + 4 * NX*NY + 2 * NX], c.corner[7]);
                c.is_act &= Intercept(Line4[0], Line4[1], Zcorn[ZLoc + 4 * NX*NY + 2 * NX + 1], c.corner[6]);
            }
        }
    }
};


void StrucGrid::TOP2DEPTH(const int Top_len, double* Top, double* Bottom) 
{
    int NLayer = NZ/PoroLvl, NLvl0 = NX*NY*NLayer;

    if(DZ_info>0) {

        if (Top_len>=NLvl0) {
            for (int j = 0; j < NY; ++j) {
                for (int i = 0; i < NX; ++i) {
                    Depth[i + j*NX] = Top[i + j*NX] + 0.5*findDZ(i, j, 0);
                    Bottom[i + j*NX] = Top[i + j*NX] + findDZ(i, j, 0);
                }
            }
            for (int k = 1; k < NLayer; ++k) {
                for (int j = 0; j < NY; ++j) {
                    for (int i = 0; i < NX; ++i) {
                        int l = i + j*NX + k*NX*NY, l0 = i + j*NX + (k - 1)*NX*NY;
                        if (Top[l] <= Top[l0]) {
                            Depth[l] = Depth[l0] + 0.5*(findDZ(i, j, k - 1) + findDZ(i, j, k));
                            Top[l] = Top[l0] + findDZ(i, j, k - 1);
                            Bottom[l] = Bottom[l0] + findDZ(i, j, k);
                        }
                        else {
                            Depth[l] = Top[l] + 0.5*findDZ(i, j, k);
                            Bottom[l] = Top[l] + findDZ(i, j, k);
                        }
                    }
                }
            }
        }
        else if(Top_len==NX*NY) {
            for (int j = 0; j < NY; ++j) {
                for (int i = 0; i < NX; ++i) {                
                    Depth[i + j*NX] = Top[i + j*NX] + 0.5*findDZ(i, j, 0);
                    Bottom[i + j*NX] = Top[i + j*NX] + findDZ(i, j, 0);
                }
            }
            for (int k = 1; k < NLayer; ++k) {
                for (int j = 0; j < NY; ++j) {
                    for (int i = 0; i < NX; ++i) { 
                        int l = i + j*NX + k*NX*NY, l0 = i + j*NX + (k - 1)*NX*NY;
                        Depth[l] = Depth[l0] + 0.5*(findDZ(i, j, k - 1) + findDZ(i, j, k));
                        Top[l] = Top[l0] + findDZ(i, j, k - 1);
                        Bottom[l] = Bottom[l0] + findDZ(i, j, k);
                    }
                }
            }
        }
        else {
            ThrowErr("length of TOPS data is neither NX*NY nor NX*NY*NLayer.");
        }
        // Copy depth data to other porosity levels
        for (int lvl=1; lvl<PoroLvl; ++lvl) {
            for(int I0=0; I0<NLvl0; ++I0) {
                Depth[lvl*NLvl0+I0] = Depth[I0];
                Top[lvl*NLvl0+I0] = Top[I0];
                Bottom[lvl*NLvl0+I0] = Bottom[I0];
            }
        }
        
    } else {
        ThrowErr("cannot calculate DEPTH from TOPS when DZ data is not specified.");
    }
};

void StrucGrid::DEPTH2TOP(const int Dep_len, double* Top, double* Bottom) {
    int NLayer = NZ / PoroLvl, NLvl0 = NX*NY*NLayer;

    if (DZ_info > 0) {

        if (Dep_len >= NLvl0) {
            for (int i = 0; i < NLvl0; ++i) {
                Top[i] = Depth[i] - 0.5*findDZ(i);
                Bottom[i] = Depth[i] + 0.5*findDZ(i);
            }
        }
        else if (Dep_len == NX*NY) {
            for (int j = 0; j < NY; ++j) {
                for (int i = 0; i < NX; ++i) {                
                    Top[i + j*NX] = Depth[i + j*NX] - 0.5*findDZ(i, j, 0);
                    Bottom[i + j*NX] = Depth[i + j*NX] + 0.5*findDZ(i, j, 0);
                }
            }
            for (int k = 1; k < NLayer; ++k) {
                for (int j = 0; j < NY; ++j) {
                    for (int i = 0; i < NX; ++i) {                                 
                        Depth[i + j*NX + k*NX*NY] = Depth[i + j*NX + (k - 1)*NX*NY] + 0.5*(findDZ(i, j, k - 1) + findDZ(i, j, k));
                        Top[i + j*NX + k*NX*NY] = Top[i + j*NX + (k - 1)*NX*NY] + findDZ(i, j, k - 1);
                        Bottom[i + j*NX + k*NX*NY] = Bottom[i + j*NX + (k - 1)*NX*NY] + findDZ(i, j, k);
                    }
                }
            }
        }
        else {
            ThrowErr("length of DEPTH data is neither NX*NY nor NX*NY*NLayer.");
        }
        // Copy depth data to other porosity levels
        for (int lvl = 1; lvl < PoroLvl; ++lvl) {
            for (int I0 = 0; I0 < NLvl0; ++I0) {
                Depth[lvl*NLvl0 + I0] = Depth[I0];
                Top[lvl*NLvl0 + I0] = Top[I0];
                Bottom[lvl*NLvl0 + I0] = Bottom[I0];
            }
        }

    }
    else {
        ThrowErr("cannot calculate TOPS from DEPTH when DZ data is not specified.");
    }
};

double StrucGrid::findDX (const int LOC) const {
    if(DXDY_info==1) {
        return DX[(LOC%(NX*NY))%NX];
    } else if(DXDY_info==2) {
        return DX[LOC];
    } else {
        cout<<" Error: DX data not specified."<<endl;
        return 0.0;
    }
};
    
double StrucGrid::findDY (const int LOC) const {
    if(DXDY_info==1) {
        return DY[(LOC%(NX*NY))/NX];
    } else if(DXDY_info==2) {
        return DY[LOC];
    } else {
        cout<<" Error: DY data not specified."<<endl;
        return 0.0;
    }
};

double StrucGrid::findDZ (const int LOC) const {
    if(DZ_info==1) {
        return DZ[LOC/(NX*NY)];
    } else if(DZ_info==2) {
        return DZ[LOC];
    } else {
        cout<<" Error: DZ data not specified."<<endl;
        return 0.0;
    }
};

double StrucGrid::findDX(const int i, const int j, const int k) const{
    if(DXDY_info==1) {
        return DX[i];
    } else if(DXDY_info==2) {
        return DX[i+j*NX+k*NX*NY];
    } else {
        cout<<" Error: DX data not specified."<<endl;
        return 0.0;
    }
}

double StrucGrid::findDY(const int i, const int j, const int k) const {
    if(DXDY_info==1) {
        return DY[j];
    } else if(DXDY_info==2) {
        return DY[i+j*NX+k*NX*NY];
    } else {
        cout<<" Error: DY data not specified."<<endl;
        return 0.0;
    }
}

double StrucGrid::findDZ(const int i, const int j, const int k) const {
    if(DZ_info==1) {
        return DZ[k];
    } else if(DZ_info==2) {
        return DZ[i+j*NX+k*NX*NY];
    } else {
        cout<<" Error: DZ data not specified."<<endl;
        return 0.0;
    }
}

void StrucGrid::CartGrid_CPGCell() {
    int i, j, k, n;
    double x, y, z, hw, hl, hh, V;
    CPGCells = new CPCell[NX*NY*NZ/PoroLvl];
    for(k=0; k<NZ/PoroLvl; ++k) {
        for(j=0; j<NY; ++j) {
            for(i=0; i<NX; ++i) {
                n = i+j*NX+k*NX*NY;
                if(XYCoord_info==1) {
                    x = Xcoord[i];
                    y = Ycoord[j];
                } else {
                    x = Xcoord[n];
                    y = Ycoord[n]; 
                } 
                z = Depth[n];

                if(DXDY_info==1) {
                    hw = 0.5 * DX[i];
                    hl = 0.5 * DY[j];
                } else {
                    hw = 0.5 * DX[n];
                    hl = 0.5 * DY[n];
                }
                if(DZ_info==1) {
                    hh = 0.5 * DZ[k];
                } else {
                    hh = 0.5 * DZ[n];
                }
                
                CPCell& c = CPGCells[n];
                c.blockc.x = x;
                c.blockc.y = y;
                c.blockc.z = z;

                c.facec[0].x = x + hw;
                c.facec[0].y = y;
                c.facec[0].z = z;

                c.facec[1].x = x - hw;
                c.facec[1].y = y;
                c.facec[1].z = z;

                c.facec[2].x = x;
                c.facec[2].y = y + hl;
                c.facec[2].z = z;

                c.facec[3].x = x;
                c.facec[3].y = y - hl;
                c.facec[3].z = z;

                c.facec[4].x = x;
                c.facec[4].y = y;
                c.facec[4].z = z + hh;

                c.facec[5].x = x;
                c.facec[5].y = y;
                c.facec[5].z = z - hh;

                c.corner[0].x = c.corner[2].x = 
                c.corner[4].x = c.corner[6].x = x - hw;
                c.corner[1].x = c.corner[3].x = 
                c.corner[5].x = c.corner[7].x = x + hw;    
                c.corner[0].y = c.corner[1].y = 
                c.corner[4].y = c.corner[5].y = y - hl;
                c.corner[2].y = c.corner[3].y = 
                c.corner[6].y = c.corner[7].y = y + hl;   
                c.corner[0].z = c.corner[1].z = 
                c.corner[2].z = c.corner[3].z = z - hh;     
                c.corner[4].z = c.corner[5].z = 
                c.corner[6].z = c.corner[7].z = z + hh;
               
                V = 8 * hw * hl * hh;
                if(V<NearZero) {
                    c.is_act = false;
                } else {
                    c.is_act = true;
                }          
            }
        }
    }
};

void StrucGrid::CartGrid_Pilar() {
    const int nCoord = (NX + 1)*(NY + 1) * 6;
    const int nz = NZ/PoroLvl;
    delete[] Coord; Coord = new double[nCoord];

    for(int j=0; j<NY; ++j) {
        for(int i=0; i<NX; ++i) {
            int p0 = (i+(NX+1)*j)*6;
            int cu = i+NX*j, cl = i+NX*j+NX*NY*(nz-1);
            Coord[p0  ] = CPGCells[cu].corner[0].x;
            Coord[p0+1] = CPGCells[cu].corner[0].y;
            Coord[p0+2] = CPGCells[cu].corner[0].z;
            Coord[p0+3] = CPGCells[cl].corner[4].x;
            Coord[p0+4] = CPGCells[cl].corner[4].y;
            Coord[p0+5] = CPGCells[cl].corner[4].z;
        }
        int p0 = (NX + (NX + 1)*j) * 6;
        int cu = (NX-1) + NX*j, cl = (NX-1) + NX*j + NX*NY*(nz - 1);
        Coord[p0    ] = CPGCells[cu].corner[1].x;
        Coord[p0 + 1] = CPGCells[cu].corner[1].y;
        Coord[p0 + 2] = CPGCells[cu].corner[1].z;
        Coord[p0 + 3] = CPGCells[cl].corner[5].x;
        Coord[p0 + 4] = CPGCells[cl].corner[5].y;
        Coord[p0 + 5] = CPGCells[cl].corner[5].z;
    }

    for (int i = 0; i < NX; ++i) {
        int p0 = (i + (NX + 1)*NY) * 6;
        int cu = i + NX*(NY-1), cl = i + NX*(NY-1) + NX*NY*(nz - 1);
        Coord[p0]     = CPGCells[cu].corner[2].x;
        Coord[p0 + 1] = CPGCells[cu].corner[2].y;
        Coord[p0 + 2] = CPGCells[cu].corner[2].z;
        Coord[p0 + 3] = CPGCells[cl].corner[6].x;
        Coord[p0 + 4] = CPGCells[cl].corner[6].y;
        Coord[p0 + 5] = CPGCells[cl].corner[6].z;
    }
    int p0 = (NX + (NX + 1)*NY) * 6;
    int cu = (NX - 1) + NX*(NY-1), cl = (NX - 1) + NX*(NY-1) + NX*NY*(nz - 1);
    Coord[p0    ] = CPGCells[cu].corner[3].x;
    Coord[p0 + 1] = CPGCells[cu].corner[3].y;
    Coord[p0 + 2] = CPGCells[cu].corner[3].z;
    Coord[p0 + 3] = CPGCells[cl].corner[7].x;
    Coord[p0 + 4] = CPGCells[cl].corner[7].y;
    Coord[p0 + 5] = CPGCells[cl].corner[7].z;

};

void StrucGrid::Corner_X(const int N0, const int G0, const int NElm, const int* NTG, const double x0, float* x) const 
{
    for(int n=0; n<NElm; ++n) {
        int c = NTG[n + N0] - G0;
        x[8 * n + 0] = (float)(CPGCells[c].corner[4].x + x0);
        x[8 * n + 1] = (float)(CPGCells[c].corner[5].x + x0);
        x[8 * n + 2] = (float)(CPGCells[c].corner[7].x + x0);
        x[8 * n + 3] = (float)(CPGCells[c].corner[6].x + x0);
        x[8 * n + 4] = (float)(CPGCells[c].corner[0].x + x0);
        x[8 * n + 5] = (float)(CPGCells[c].corner[1].x + x0);
        x[8 * n + 6] = (float)(CPGCells[c].corner[3].x + x0);
        x[8 * n + 7] = (float)(CPGCells[c].corner[2].x + x0);
    }
}

void StrucGrid::Corner_Y(const int N0, const int G0, const int NElm, const int* NTG, const double y0, float* y) const
{
    for (int n = 0; n < NElm; ++n) {
        int c = NTG[n + N0] - G0;
        y[8 * n + 0] = (float)(CPGCells[c].corner[4].y + y0);
        y[8 * n + 1] = (float)(CPGCells[c].corner[5].y + y0);
        y[8 * n + 2] = (float)(CPGCells[c].corner[7].y + y0);
        y[8 * n + 3] = (float)(CPGCells[c].corner[6].y + y0);
        y[8 * n + 4] = (float)(CPGCells[c].corner[0].y + y0);
        y[8 * n + 5] = (float)(CPGCells[c].corner[1].y + y0);
        y[8 * n + 6] = (float)(CPGCells[c].corner[3].y + y0);
        y[8 * n + 7] = (float)(CPGCells[c].corner[2].y + y0);
    }
}

void StrucGrid::Corner_Z(const int N0, const int G0, const int NElm, const int* NTG, const double z0, float* z) const
{
    for (int n = 0; n < NElm; ++n) {
        int c = NTG[n + N0] - G0;
        z[8 * n + 0] = -(float)(CPGCells[c].corner[4].z + z0);
        z[8 * n + 1] = -(float)(CPGCells[c].corner[5].z + z0);
        z[8 * n + 2] = -(float)(CPGCells[c].corner[7].z + z0);
        z[8 * n + 3] = -(float)(CPGCells[c].corner[6].z + z0);
        z[8 * n + 4] = -(float)(CPGCells[c].corner[0].z + z0);
        z[8 * n + 5] = -(float)(CPGCells[c].corner[1].z + z0);
        z[8 * n + 6] = -(float)(CPGCells[c].corner[3].z + z0);
        z[8 * n + 7] = -(float)(CPGCells[c].corner[2].z + z0);
    }
}

int StrucGrid::XContact(const int i, const int j, const int k, const int k2, double& Area) const {
    if(DZ_info!=0) {
        int id1 = i+(j+k*NY)*NX, id2 = i+1+(j+k2*NY)*NX;
        double dz1  = findDZ(i,j,k), dz2  = findDZ(i+1,j,k2);
        double dep1 = Depth[id1], dep2 = Depth[id2];
        if(is_zero(dz1-dz2, InfSmall) && is_zero(dep1-dep2, InfSmall)) {
            Area = dz1 * findDY(i,j,k);
            return 2;
        } else {
            double LowerD = min(dep1+0.5*dz1, dep2+0.5*dz2), UpperD = max(dep1-0.5*dz1, dep2-0.5*dz2);
            if(LowerD-UpperD<InfSmall) {
                Area = 0.0;
                return 0;
            } else {
                Area = (LowerD-UpperD) * findDY(i,j,k);
                return 1;
            }
        }
    } else {
        cout<<" Error: DZ data not specified."<<endl;
        Area = 0.0;
        return 0;
    }    
}

int StrucGrid::YContact(const int i, const int j, const int k, const int k2, double& Area) const {
    if(DZ_info!=0) {
        int id1 = i+(j+k*NY)*NX, id2 = i+(j+1+k2*NY)*NX;
        double dz1  = findDZ(i,j,k), dz2  = findDZ(i,j+1,k2);
        double dep1 = Depth[id1], dep2 = Depth[id2];
        if(is_zero(dz1-dz2, InfSmall) && is_zero(dep1-dep2, InfSmall)) {
            Area = dz1 * findDX(i,j,k);
            return 2;
        } else {
            double LowerD = min(dep1+0.5*dz1, dep2+0.5*dz2), UpperD = max(dep1-0.5*dz1, dep2-0.5*dz2);
            if(LowerD-UpperD<InfSmall) {
                Area = 0.0;
                return 0;
            } else {
                Area = (LowerD-UpperD) * findDX(i,j,k);
                return 1;
            }
        }
    } else {
        cout<<" Error: DZ data not specified."<<endl;
        Area = 0.0;
        return 0;
    }    
}

void StrucGrid::Center(const int LOC, double& x, double& y, double& z) const {
    if (XYCoord_info == 1) {
        int I, J, K;
        LOC_IJK(LOC, NX, NY, I, J, K);
        x = Xcoord[I];
        y = Ycoord[J];
        z = Depth[LOC];
    }
    else if (XYCoord_info == 2) {
        x = Xcoord[LOC];
        y = Ycoord[LOC];
        z = Depth[LOC];
    }
};

point StrucGrid::VA(const int L, const int dir) const 
{        
    point va;

    if(CPGCells==nullptr) {
        va.x = va.y = va.z = 0;
        if(dir==XDir) {
            va.x = findDY(L)*findDZ(L);
        } else if (dir == -XDir) {
            va.x = -findDY(L)*findDZ(L);
        } else if (dir == YDir) {
            va.y = findDX(L)*findDZ(L);
        } else if (dir == -YDir) {
            va.y = -findDX(L)*findDZ(L);
        } else if (dir == ZDir) {
            va.z = findDX(L)*findDY(L);
        } else {
            va.z = -findDX(L)*findDY(L);
        }    
    } else {
        va = CPGCells[L].AreaVec(dir);
    }

    return va;
}

point StrucGrid::PN(const int L, const int dir) const
{
    point va;

    if (CPGCells == nullptr) {
        va.x = va.y = va.z = 0;
        if (dir == XDir) {
            va.x = 0.5*findDX(L);
        } else if (dir == -XDir) {
            va.x = -0.5*findDX(L);
        } else if (dir == YDir) {
            va.y = 0.5*findDY(L);
        } else if (dir == -YDir) {
            va.y = -0.5*findDY(L);
        } else if (dir == ZDir) {
            va.z = 0.5*findDZ(L);
        } else {
            va.z = -0.5*findDZ(L);
        }
    }
    else {
        if (dir == XDir) {
            va = CPGCells[L].facec[0] - CPGCells[L].blockc;
        } else if (dir == -XDir) {
            va = CPGCells[L].facec[1] - CPGCells[L].blockc;
        } else if (dir == YDir) {
            va = CPGCells[L].facec[2] - CPGCells[L].blockc;
        } else if (dir == -YDir) {
            va = CPGCells[L].facec[3] - CPGCells[L].blockc;
        } else if (dir == ZDir) {
            va = CPGCells[L].facec[4] - CPGCells[L].blockc;
        } else {
            va = CPGCells[L].facec[5] - CPGCells[L].blockc;
        }
    }

    return va;
};

void StrucGrid::Clear_Coord_Info() {
    DXDY_info = DZ_info = XYCoord_info = 0; 
    DX = DY = DZ = Xcoord = Ycoord = Depth = nullptr;
};

};
