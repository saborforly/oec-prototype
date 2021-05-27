/*=============================================================================
#
# Author: ZHANG Kun - zhangkun@ihep.ac.cn
# Last modified:	2015-05-11 01:39
# Filename:		RecCdMuonAlg.h
# Description: 
#
=============================================================================*/
#ifndef RecCdMuonAlg_h
#define RecCdMuonAlg_h 1

#include "SniperKernel/AlgBase.h"

#include "EvtNavigator/NavBuffer.h"
#include "RecCdMuonAlg/PmtProp.h"
#include "RecCdMuonAlg/Params.h"
#include <string>
#include "EvtAlgorithms/IEFAlgorithm.h"
#include "Event/RecHeader.h"
#include "EvtStore/EvtStoreSvc.h"

#include "TFile.h"
#include "TGraph.h"
#include "TMath.h"
#include "TSpectrum2.h"
#include "TH2F.h"

class RecGeomSvc; 
class CdGeom; 
class IReconTool; 

class RecCdMuonAlg : public IEFAlgorithm
{
    public:  

        RecCdMuonAlg(const std::string& name);
        virtual ~RecCdMuonAlg();

        bool execute();
        bool initialize();
        bool finalize();
        void reset(){}
    private:

        bool iniBufSvc(); 
        bool iniGeomSvc(); 
        bool iniPmtPos(); 
        bool iniRecTool(); 
  
        bool freshPmtData(); 
    private:

        int m_iEvt; 

        unsigned int m_totPmtNum; 

        double m_sigmaPmt3inch;
        double m_sigmaPmt20inch; 

        bool m_flagUse3inch;
        bool m_flagUse20inch;

        CdGeom*  m_cdGeom; 
        JM::NavBuffer* m_buf; 


        PmtTable m_pmtTable; 
        Params m_params; //set of parameters' key/value

        std::string m_recToolName; 
        IReconTool* m_recTool; 
        EvtStoreSvc *m_storeSvc;
        bool m_flagOpPmtpos;
};

#endif // RecCdMuonAlg_h
