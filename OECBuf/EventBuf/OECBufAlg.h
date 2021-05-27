#ifndef OECBufAlg_H
#define OECBufAlg_H

#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include "SniperKernel/AlgBase.h"
#include "SniperKernel/SniperLog.h"
#include "InputReviser/InputReviser.h"
#include "EvtStore/EvtStoreSvc.h"  
#include "Event/OECHeader.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"

#include "Event/OECHeader.h"
#include "Event/CalibHeader.h"
#include "TRandom.h"

#include "JunoTimer/JunoTimer.h"
using namespace std;


class OECBufAlg :public AlgBase {

    public:

        OECBufAlg(const std::string& name);
        ~OECBufAlg();
        bool initialize();
        bool execute();
        bool finalize(){return true;}

        //JM::OECEvent* getEvent(){return m_thisEvent;}
        //JM::OECHeader* getHeader(){return m_thisHeader;} 


    private:
        //JM::NavBuffer* m_buf;
        JM::EvtNavigator* m_nav;
        JM::OECHeader* m_thisHeader;
        JM::OECEvent* m_thisEvent;
        EvtStoreSvc *m_storeSvc;
        JM::CalibHeader* m_calHeader;
        IDataMemMgr * m_mMgr; 

        vector<InputReviser*> m_inputReviser;//to fire events
        map<string,float> m_rateMap;
        float f_totRate;
        int i_currentTime_sec;
        int i_currentTime_nanosec;
        TRandom m_myrandom;
        bool getOneEvent();
        JunoTimer* m_timer;
        JunoTimer* m_timer_all;

};


#endif
