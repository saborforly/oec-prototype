/*=============================================================================
#
# Author: ZHANG Kun - zhangkun@ihep.ac.cn
# Last modified: 2014-12-09 00:44
# Filename: IReconTool.h
# Description: 
=============================================================================*/
#ifndef IRECONTOOL_H
#define IRECONTOOL_H
#include "RecCdMuonAlg/PmtProp.h"
#include "RecCdMuonAlg/Params.h"

namespace JM{
    class RecHeader; 
}
class IReconTool 
{
    public:

        virtual bool reconstruct( JM::RecHeader* ) = 0; 

        virtual bool configure(const Params*, const PmtTable*) = 0; 

        virtual ~IReconTool(){}; 
}; 
#endif
