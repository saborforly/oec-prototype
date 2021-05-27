#ifndef RecDummyTool_h
#define RecDummyTool_h
#include "RecCdMuonAlg/IReconTool.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "Event/RecHeader.h"

class RecDummyTool : public IReconTool, public ToolBase{
    public:
        RecDummyTool (const std::string& name);
        virtual ~RecDummyTool (){};

        bool reconstruct(JM::RecHeader*) ;
        bool configure(const Params*,const PmtTable*); 

    private:
        const PmtTable* m_ptab; 

};
#endif

DECLARE_TOOL(RecDummyTool); 


RecDummyTool::RecDummyTool(const std::string& name)
    :ToolBase(name){
    }

bool
RecDummyTool::reconstruct(JM::RecHeader* rh){
    CLHEP::HepLorentzVector start(9, 9, 9, 9); 
    CLHEP::HepLorentzVector end(99, 99, 99, 99); 
    JM::CDTrackRecEvent* evt = new JM::CDTrackRecEvent();
    JM::RecTrack* mtrk = new JM::RecTrack(start, end); 
    mtrk->setPESum(1024); 
    mtrk->setQuality(1); 
    evt->addTrack(mtrk);
    rh->setCDTrackEvent(evt);
    
    LogDebug <<  "Dummy test to  reconstruct the event!"
        << std::endl; 
    return true; 
}

bool
RecDummyTool::configure(const Params* pars, const PmtTable* ptab){
    LogDebug  << "configure the Dummy reconstruct tool!"
        << std::endl; 
    LogDebug  << "Retrieve the key/value [Pmt3inchTimeReso: " 
         << pars->get("Pmt3inchTimeReso") << "] from Params" << std::endl; 
    m_ptab = ptab; 
    return true; 
}

