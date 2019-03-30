/**
* @file    Constants.h
* @author  Li, Xiang
* @date    2016/07
* @brief   Int and float constants. Do not modify this file!
* @version
* Copyright (c) 2016, Ennosoft Co., Ltd.
*/

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