#ifndef ENUMDEF_H
#define ENUMDEF_H

enum Gridkeyenum{
	grid_DX = 0, grid_DY = 1, grid_DZ = 2,
	grid_DXV = 3, grid_DYV = 4, grid_DZV = 5,
	grid_DEPTH = 6, grid_TOPS = 7,
	grid_COORD = 8, grid_ZCORN = 9,
	grid_PERMX = 10, grid_PERMY = 11, grid_PERMZ = 12,
	grid_PORO = 13, grid_ACTNUM = 14, grid_NTG = 15,
	grid_MULTX = 16, grid_MULTY = 17, grid_MULTZ = 18,
	grid_REGION = 19, grid_SATMAP = 20, grid_PVTMAP = 21,
	grid_ROCKMAP = 22, grid_FIPMAP = 23, grid_EOSMAP = 24,
	grid_LX = 25, grid_LY = 26, grid_LZ = 27,
	grid_SIGMA = 28, grid_LTOSIGMA = 29,
	grid_FAULT = 30, grid_CARFIN = 31, grid_Nkey = 32, grid_EMBF = 32,
	grid_ZDepth = 33, grid_XCoord = 34, grid_YCoord = 35, grid_PCOORD = 36, grid_PIND = 37, grid_PZCORN = 38, grid_GPG=39,
	/*grid_GPG=34,grid_PCOORD=35,grid_PIND=36,grid_PZCORN=37,grid_PFAULTS=38*/
};
enum DynaGridkeyenum{
	grid_POIL = 0, grid_PWAT = 1, grid_PGAS = 2, grid_PAVG = 3,
	grid_SGAS = 4, grid_SOIL = 5, grid_SWAT = 6, Dynagrid_Nkey = 7,
};
enum CARFINGridkeyenum{
	CarFinGrid_PERMX =  0, CarFinGrid_PERMY  =  1, CarFinGrid_PERMZ  =  2,
	CarFinGrid_PORO  =  3, CarFinGrid_ACTNUM =  4, CarFinGrid_NTG    =  5,
	CarFinGrid_MULTX =  6, CarFinGrid_MULTY  =  7, CarFinGrid_MULTZ  =  8,
	CarFinGrid_SATMAP=  9, CarFinGrid_PVTMAP = 10, CarFinGrid_ROCKMAP= 11,
	CarFinGrid_FIPMAP =12, CarFinGrid_EOSMAP = 13, CarFinGrid_NKey   = 14,	
};
enum CARFINGridkeyof3dViewenum{
	CarfView_Fullact = 0, CarfView_act = 1, CarfView_Isshow = 2,
	CarfView_value = 3, CarfView_Dropface = 4, CarfView_NKey = 5,
};
enum EMBFDataof3dViewenum{
	Embf_act = 0, Embf_Isshow = 1, Embf_value = 2,
};
enum GridSecenum{
	gridsec_GEOMETRY = 0,
	gridsec_PROPERTY = 1,
	gridsec_REGION   = 2,
	gridsec_MULTPORO = 3,
	gridsec_FAULT    = 4,
	gridsec_CARFIN   = 5,
	gridsec_EMBF     = 6,
};
enum PropKeyenum{

	Prop_SWGF = 0, Prop_SWOF = 1, Prop_SGOF = 2, Prop_SWGF_IMB = 3, Prop_SWOF_IMB = 4, Prop_SGOF_IMB = 5,
	Prop_SWGF_MIS = 6, Prop_SWOF_MIS = 7, Prop_SGOF_MIS = 8, Prop_SWGF_IMB_MIS = 9, Prop_SWOF_IMB_MIS = 10, Prop_SGOF_IMB_MIS = 11,
	Prop_SOF3 = 12, Prop_SWF3 = 13, Prop_SGF3 = 14, Prop_SOF3_Secondary = 15, Prop_SOF3_Tertiary = 16,
	Prop_SWF3_Increasing = 17, Prop_SWF3_Decreasing = 18, Prop_SGF3_Increasing = 19, Prop_SGF3_Decreasing = 20,
	Prop_STGO = 21, Prop_STOW = 22, Prop_STGW = 23,

	Prop_PVTW = 24,  Prop_PVTO   = 25, Prop_PVTG = 26,  Prop_WATERTAB = 27, Prop_PVDO = 28,    Prop_PVDG = 29,  Prop_DENSITY = 30, 
	Prop_ROCK = 31,  Prop_ROCKTAB = 32, Prop_ROCKTABW = 33, Prop_ROCKPAMA=34,  Prop_OVERBURD=35, 
	Prop_ROCKDEN = 36, Prop_FCHHMBFTA = 37, Prop_KNUDESNA = 38, Prop_KNUDESNB = 39, Prop_NKey = 40, Prop_Line_NKey = 36, Prop_Data_NKey = 4,

	Prop_HYSTOPT=40,Prop_SOF3_PRIMARY = 41, Prop_SWF3_PRIMARY = 42, Prop_SGF3_PRIMARY = 43,
};
enum PropSecenum{
	Propsec_RPC = 0, Propsec_PVT = 1, Propsec_ROCK = 2, Propsec_FLOW = 3,
};
/*enum PropEosCoefnum{
	PropEos_TCRIT = 0, PropEos_PCRIT = 1, PropEos_ZCRIT = 2, PropEos_VCRIT = 3,
	PropEos_NW = 4, PropEos_ACF = 5, PropEos_OMEGAA = 6, PropEos_OMEGAB = 7, PropEos_PARACHOR=8,
	PropEos_BIC = 9, PropEos_PARA = 10, PropEos_NKey=10
};*/
enum EMBFenum{
	EMBF_PORO = 0, EMBF_PERM = 1, EMBF_PERMV = 2, EMBF_THICK = 3, EMBF_Forchheimer = 4,
	EMBF_SATMAP = 5, EMBF_ROCKMAP = 6, EMBF_EOSMAP = 7, EMBF_FIPMAP = 8, 
	EMBF_COALMAP = 9, EMBF_NKey = 10
};
enum Adsorbenum{
	Adsorb_COALMAP = 0, Adsorb_LANGMUIR = 1, Adsorb_LANGMTAB = 2,
};
enum Solutionenum{
	Solution_EQUILPAR = 0, Solution_PBVD = 1, Solution_RSVD = 2,
	Solution_PDVD = 3, Solution_RVVD = 4, Solution_EQUILMAP = 5,
	Solution_PRESI = 6, Solution_SWATI = 7, Solution_SGASI = 8,
	Solution_RSI = 9, Solution_RVI = 10, Solution_Equil_Nkey = 11, Solution_Equil_Line_Nkey = 5, Solution_Equil_Data_Nkey = 6,
	Solution_PWAT = 11, Solution_PGAS = 12, Solution_POIL = 13,
	Solution_SWAT = 14, Solution_SGAS = 15, Solution_SOIL = 16,
	Solution_SWATHMIN = 17, Solution_SWATHMAX = 18, Solution_SGASHMIN = 19,
	Solution_SGASHMAX = 20, Solution_SOILHMIN = 21, Solution_SOILHMAX = 22,
	Solution_RS = 23, Solution_RV = 24, Solution_CGAS = 25,
	Solution_XMF = 26, Solution_YMF = 27, Solution_NonEquil_Nkey = 17,
	Solution_RSTFILE = 28, Solution_RSTSCHED = 29,
	Solution_AQUFETP = 30, Solution_AQUANCON = 31,
	Solution_AQUCHWAT = 32, Solution_AQUCHGAS = 33, 
	Solution_TEMPVD = 34, Solution_ZMFVD = 35, Solution_COMPVD = 36,
	Solution_XMFVP = 37, Solution_YMFVP = 38, Solution_AQUSTREAM = 39, Solution_Nkey = 39,
};
enum tagCursorCtrlStyle{
	eCursorNormal = 0,  //Normal mouse
	eCursorHor = 1,     //horizontal stretching
	eCursorVer = 2,     //vertically stretching
	eCursorHorVer = 3,  //horizontal & vertical stretching
};
enum Stuenum{
	Stu_VPORO = 0, Stu_VKFX = 1, Stu_VKFGX = 2, Stu_VKFY = 3, Stu_VKFGY = 4, Stu_VKFZ = 5, Stu_VKFGZ = 6,
	Stu_MDENS = 7, Stu_SIGMA = 8, Stu_MDESW = 9, Stu_MDESG = 10,

};
enum someconst{
	Max_CoalReg = 10, Max_SolutionReg = 10,
};
#endif 