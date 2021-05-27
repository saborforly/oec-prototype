/*=============================================================================
#
# Author: ZHANG Kun - zhangkun@ihep.ac.cn
# Last modified:	2015-05-11 01:39
# Filename:		RecCdMuonAlg.cc
# Description: 
=============================================================================*/

#include "RecCdMuonAlg.h"

#include "RecCdMuonAlg/IReconTool.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/ToolBase.h"

#include "DataRegistritionSvc/DataRegistritionSvc.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "Event/CalibHeader.h"
#include "Event/RecHeader.h"

#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "Identifier/CdID.h"

#include "EvtDataTypes/CriteriaItemValue.h"
#include "EvtStore/EvtStoreSvc.h"  
#include <fstream>
#include <vector>
#include "TVector3.h"

#include "TFile.h"
#include "TGraph.h"
#include "TMath.h"
#include "TSpectrum2.h"
#include "TH2F.h"
#include "JunoTimer/JunoTimer.h"


DECLARE_ALGORITHM(RecCdMuonAlg); 

    RecCdMuonAlg::RecCdMuonAlg(const std::string& name)
    : IEFAlgorithm(name)
    , m_iEvt(0)
    , m_totPmtNum(0)
    , m_cdGeom(NULL)
    , m_buf(NULL)
      , m_recTool(NULL)
{
    declProp("RecTool", m_recToolName); 
    declProp("Pmt3inchTimeReso",  m_sigmaPmt3inch= 1); 
    declProp("Pmt20inchTimeReso",  m_sigmaPmt20inch= 8); 
    declProp("Use3inchPMT",m_flagUse3inch=true);
    declProp("Use20inchPMT",m_flagUse20inch=true);
    declProp("OutputPmtPos",m_flagOpPmtpos=false);
}
RecCdMuonAlg::~RecCdMuonAlg(){}


    bool 
RecCdMuonAlg::initialize()
{
    m_params.set("Pmt3inchTimeReso", m_sigmaPmt3inch); 
    m_params.set("Pmt20inchTimeReso", m_sigmaPmt20inch); 

    if(not iniBufSvc())return false; 
    if(not iniGeomSvc())return false; 
    if(not iniPmtPos())return false; 
    if(not iniRecTool()) return false; 

    LogInfo  << objName()
        << "   initialized successfully"
        << std::endl; 
    return true;
}

    bool 
RecCdMuonAlg::finalize()
{
    (dynamic_cast<ToolBase*>(m_recTool))->finalize(); 
    LogInfo  << objName()
        << "   finalized successfully" 
        << std::endl; 
    return true;
}

    bool 
RecCdMuonAlg::execute()
{
    LogInfo << "---------------------------------------" << std::endl; 
    LogInfo << "Processing event "<< m_iEvt << std::endl; 
    JunoTimer* m_timer = new JunoTimer("test"); 
    m_timer->start();
    if(not freshPmtData())return false; 

    JM::RecHeader * rh = new JM::RecHeader; 
    m_recTool->reconstruct(rh); 
    //JM::EvtNavigator* nav = m_buf->curEvt(); 
    //nav->addHeader("/Event/Rec", rh); 
    m_timer->stop();
    float used_time=m_timer->elapsed();
    LogInfo << "spend time = "<<used_time<< std::endl;
    LogDebug  << "Done to write RecTrack event" << std::endl; 
    ++m_iEvt; 
    

    return true;
}
bool
RecCdMuonAlg::iniRecTool(){

    m_recTool =  tool<IReconTool>(m_recToolName); 
    if(not m_recTool){
        LogError << "Failed to retrieve reconstruction tool!!  "
             << "Check the tool name [\"" << m_recToolName << "\"]!!"
             << std::endl; 
        return false; 
    }
    if(not m_recTool->configure(&m_params, &m_pmtTable)) return false; 
    return true; 
}

bool 
RecCdMuonAlg::iniBufSvc(){

    //Event navigator
    SniperDataPtr<JM::NavBuffer>  navBuf(getRoot(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" 
            << std::endl;
        return false;
    }
    m_buf = navBuf.data();

    //DataRegistritionSvc
    /*
    SniperPtr<DataRegistritionSvc> drSvc(getRoot(), "DataRegistritionSvc");
    if ( drSvc.invalid() ) {
        LogError << "Failed to get DataRegistritionSvc instance!"
            << std::endl;
        return false;
    }
    drSvc->registerData("JM::RecTrackEvent", "/Event/RecTrack");
    */
    return true; 
}
bool 
RecCdMuonAlg::iniGeomSvc(){

    //Retrieve Geometry service
    SniperPtr<RecGeomSvc> rgSvc(getRoot(), "RecGeomSvc"); 
    if ( rgSvc.invalid()) {
        LogError << "Failed to get RecGeomSvc instance!" 
            << std::endl;
        return false;
    }
    m_cdGeom = rgSvc->getCdGeom(); 
    return true; 
}
bool 
RecCdMuonAlg::iniPmtPos(){

    m_totPmtNum=m_cdGeom->getPmtNum(); 
    m_pmtTable.reserve(m_totPmtNum); 
    m_pmtTable.resize(m_totPmtNum); 
    LogDebug << "Total Pmt num from GeomSvc : " 
        << m_totPmtNum << std::endl; 
    for(unsigned int pid=0;pid<m_totPmtNum;++pid){
        Identifier Id = Identifier(CdID::id(pid, 0)); 
        PmtGeom *pmt = m_cdGeom->getPmt(Id); 
        if(!pmt){
            LogError << "Wrong Pmt Id" << Id << std::endl; 
            return false; 
        }
        TVector3 pmtCenter = pmt->getCenter(); 

        m_pmtTable[pid].pos = pmtCenter;
        if (CdID::is3inch(Id)) {
            m_pmtTable[pid].res = m_sigmaPmt3inch; 
            m_pmtTable[pid].type=_PMTINCH3; 
        }
        else if(CdID::is20inch(Id)) {
            m_pmtTable[pid].res = m_sigmaPmt20inch; 
            m_pmtTable[pid].type = _PMTINCH20; 
        }
        else {
            m_pmtTable[pid].type = _PMTNULL; 
            LogError  <<  "Pmt ["  <<  pid  
                <<  "] is neither 3 inch nor 20 inch!"  
                <<  std::endl; 
            return false; 
        }

    }

    //-----print out pmt positions  
    if(m_flagOpPmtpos){
        std::ofstream of("pmt_info.dat"); 
        for(unsigned int pid=0;pid<m_totPmtNum;++pid){
            of << pid << " : "
                << m_pmtTable[pid].pos.X() << "," 
                << m_pmtTable[pid].pos.Y() << "," 
                << m_pmtTable[pid].pos.Z() 
                << std::endl;
        }
    }

    return true; 

}
bool 
RecCdMuonAlg::freshPmtData(){

    //reset values
    for (unsigned int pid = 0; pid < m_totPmtNum; ++pid)
    {
        m_pmtTable[pid].q=-1; 
        m_pmtTable[pid].fht=999999; 
        m_pmtTable[pid].used=false; 

    }
    
    //read CalibHit data
    JM::EvtNavigator* nav = m_buf->curEvt(); 
    if(not nav){
        LogError << "Can not retrieve the current navigator!!!" 
            << std::endl; 
        return false; 
    }
    JM::CalibHeader* chcol =
        (JM::CalibHeader*) nav->getHeader("/Event/Calib"); 
    if(not chcol){
        LogError << "Can not retrieve \"/Event/Calib\" \
            from current navigator!!!" 
            << std::endl; 
        return false; 
    }
    const std::list<JM::CalibPMTChannel*>& chhlist = 
        chcol->event()->calibPMTCol(); 
    std::list<JM::CalibPMTChannel*>::const_iterator chit = chhlist.begin(); 
    while (chit!=chhlist.end()) {

        JM::CalibPMTChannel  *calib = *chit++; 
        Identifier id = Identifier(calib->pmtId());
        // skip if not CD.
        Identifier::value_type value = id.getValue(); 
        if (not ((value&0xFF000000)>>24 == 0x10)) { 
            //current 0x10 CD, 0x20 WP, 0x30 TT
            continue;
        }
        double nPE = calib->nPE(); 
        float firstHitTime = calib->firstHitTime(); 

        unsigned pid = CdID::module(id); 
        if (pid>m_totPmtNum) {
            LogError << "Data/Geometry Mis-Match: PmtID ("<< pid<< ")>= m_totPmtNum ("<< m_totPmtNum <<")" << std::endl;
            return false;
            //continue;
        }
        m_pmtTable[pid].q = nPE;
        m_pmtTable[pid].fht =firstHitTime;
        if((m_flagUse3inch  &&  CdID::is3inch(id)) ||
                (m_flagUse20inch  &&  CdID::is20inch(id)) 
          ) {
            m_pmtTable[pid].used = true; 
        }


        //LogDebug <<"PMT id"<<pid << "(" << m_pmtTable[pid].pos.x() << "," << m_pmtTable[pid].pos.y() << "," << m_pmtTable[pid].pos.z() << ")"<<" ; nPE ="<<nPE<<" ;  firsthit ="<<firstHitTime<<std::endl;

    }

    LogDebug << "Loading calibrated data done !" << std::endl; 
    return true; 
}
