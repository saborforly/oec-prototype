#ifndef OECALG_H
#define OECALG_H

#include "EvtStore/EvtStoreSvc.h"  
#include "EvtNavigator/NavBuffer.h"
#include "InputReviser/InputReviser.h"
#include "Event/OECEvent.h"
#include "EvtResult/EvtResultSvc.h"
#include "TTimeStamp.h"
#include <string>
#include <vector>
#include <list>
#include "TFile.h"
#include "TTree.h"

#include "SniperKernel/AlgBase.h"

using namespace std;

class OECAlg: public AlgBase{
    public:
        OECAlg(const string &name);
        ~OECAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:

        //for toyMC
        bool b_toyMCOpt;
        TFile *m_toyInputFile;
        TTree *m_toyInputTree;
        int i_toyEvts;
        int i_toyEvtsMax;
        float f_E;float f_T;float f_x;float f_y;float f_z;
        bool registerData(JM::OECEvent *);
        TTimeStamp m_currentTimeStamp;
        EvtResultSvc *m_singleResultSvc;//to get single tag
        EvtStoreSvc *m_storeSvc;//to get energy, vertex, etc..

        //JM::OECEvent *m_thisEvent;//this event
        //vector<JM::OECEvent*> v_pEvents;//proceeding events
        //for input
        bool b_loop;
        //InputReviser *m_reviser;
        JM::NavBuffer *m_buf;


        //define cut conditions and tag
        struct CutRange{
            double low;
            double high;
        };
        enum tECut: unsigned{//8 bits
            tEltLow,
            tEltnHLow,
            tEltPDLow,
            tEltnHHigh,
            tEltPDHigh,
            tEgtPDHigh,
            tENum,
            tEAny=0xFF
        };
        enum pECut: unsigned{
            pEltLow,
            pEltBiPoHigh,
            pEltIBDHigh,
            pEltPDLow,
            pEltPDHigh,
            pEgtPDHigh,
            pENum,
            pEAny=0xFF
        };
        enum distCut: unsigned{
            distlt,
            distgt,
            distNum,
            distAny=0xFF
        };
        enum timeCut: unsigned{
            timeltPDcut,
            timegtPDcut,
            timeNum,
            timeAny=0xFF
        };
        enum Tag: unsigned{
            tagIBD,
            tagBiPo,
            tagProtonDecay,
            tagSpallationNeutron,
            tagOther,
            tagNum
        };
        struct TagConditionPair{
            unsigned int selection;
            tECut _tECut;
            pECut _pECut;
            distCut _distCut;
            timeCut _timeCut;
            Tag tag;
        };
        Tag get_tag(double tE,double pE,double dist,double time);
        tECut get_tECut(double);
        pECut get_pECut(double);
        distCut get_distCut(double);
        timeCut get_timeCut(double);
        const CutRange tECutRange[(int)tECut::tENum];
        const CutRange pECutRange[(int)pECut::pENum];
        const CutRange distCutRange[(int)distCut::distNum];
        const CutRange timeCutRange[(int)timeCut::timeNum];
        const string TagName[(int)Tag::tagNum];
        vector<TagConditionPair> tables;
};
#endif
