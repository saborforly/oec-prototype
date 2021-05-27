/*=============================================================================
#
# Author: ZHANG Kun - zhangkun@ihep.ac.cn
# Last modified: 2015-05-11 00:03
# Filename: PmtProp.h
# Description: 
=============================================================================*/
#ifndef PmtProp_H
#define PmtProp_H
//define PMT properity
#include "TVector3.h"
#include <vector>
enum Pmttype {
    _PMTNULL, 
    _PMTINCH3, 
    _PMTINCH20, 
}; 
struct PmtProp{
    TVector3 pos;
    double q;
    double fht;
    double res; 
    double trise; 
    double amp; 
    double integ; 
    bool used;
    Pmttype type; 
};
typedef std::vector<PmtProp> PmtTable; 
#endif
