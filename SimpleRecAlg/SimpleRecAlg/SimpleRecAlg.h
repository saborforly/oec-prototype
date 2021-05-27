#ifndef SimpleRecAlg_h
#define SimpleRecAlg_h


//#include "InputReviser/InputReviser.h"
#include "EvtStore/EvtStoreSvc.h"  
#include "EvtAlgorithms/IEFAlgorithm.h"
#include "EvtDataTypes/CriteriaItemValue.h"
#include "EvtNavigator/NavBuffer.h"
#include "SniperKernel/AlgBase.h"
#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "RootWriter/RootWriter.h"
#include "JunoTimer/IJunoTimerSvc.h"
#include "JunoTimer/JunoTimer.h"
#include "Hit.h"
#include <set>
class RecGeomSvc;
class CdGeom;

class SimpleRecAlg: public IEFAlgorithm{

    public:
        SimpleRecAlg(const std::string& name);
        virtual ~SimpleRecAlg();

        virtual bool initialize(); 
        bool execute(); 
        bool finalize(); 
        void reset();

        bool ChargeCenterRec();


    private:
        int m_iEvt;
        JM::NavBuffer* m_buf;
        //for OEC output
        CriteriaItemValue* m_energy;
        CriteriaItemValue* m_vertexX;
        CriteriaItemValue* m_vertexY;
        CriteriaItemValue* m_vertexZ;
        CriteriaItemValue* m_time;
        CriteriaItemValue* m_detID;
        EvtStoreSvc *m_storeSvc;
        //InputReviser *m_inputReviser;

        int num_PMT;
        float lightyield;
        int PMT_HIT[90000];
        int totalPE; 
        double min_hit_time;

        IJunoTimerSvc* m_junotimersvc;

        JunoTimerPtr m_timer_event;
        JunoTimerPtr m_timer_allevent;

        std::vector<Hit> Hit_buffer;
        std::vector<double> Readout_PE;
        std::vector<double> Readout_hit_time; 
        std::vector<TVector3> PMT_pos;
        std::vector<int>  PMT_ID;
        
        std::vector<TVector3> ALL_PMT_pos;
        std::vector<TVector3>  m_pmt_pos;
        double ChaCenRec_x;
        double ChaCenRec_y;
        double ChaCenRec_z;

        CdGeom*  m_cdGeom;

        double Total_num_PMT;
        double PMT_R ;
        double Ball_R;

};
#endif
