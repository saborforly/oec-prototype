#include <boost/python.hpp>
#include "SimpleRecAlg/SimpleRecAlg.h"
#include "SimpleRecAlg/Hit.h"
#include <set>
#include <fstream>

#include "TMath.h"
#include "SniperKernel/AlgFactory.h"
#include "Event/RecHeader.h"
#include "Event/CalibHeader.h"
#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "Identifier/CdID.h"
#include <boost/filesystem.hpp>
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/Incident.h" 
#include "SniperKernel/SniperException.h"
#include "SniperKernel/Task.h"

#include "DataRegistritionSvc/DataRegistritionSvc.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "TVector3.h"
#include <vector>
#include <algorithm>
#include <iostream>
namespace fs = boost::filesystem;

DECLARE_ALGORITHM(SimpleRecAlg); 

SimpleRecAlg::SimpleRecAlg(const std::string& name)
:   IEFAlgorithm(name)
{

    m_iEvt = -1; 
    Total_num_PMT = 17612;
    lightyield=1350;
    PMT_R = 19.5;
    Ball_R = 19.316;
    declProp("lightyield", lightyield=1350);
    declProp("TotalPMT", Total_num_PMT=17612);
    declProp("PMT_R", PMT_R);
    declProp("Ball_R",Ball_R);

    //for OEC output
    m_energy=new CriteriaItemValue;//NULL;
    m_detID=new CriteriaItemValue;//NULL;
}

SimpleRecAlg::~SimpleRecAlg(){
    if (m_energy!=NULL) delete m_energy;
    if (m_detID!=NULL) delete m_detID;
}

bool SimpleRecAlg::initialize()
{
    //for OEC input
    LogInfo  << objName()
        << "   initialized start"
        << std::endl;
    SniperPtr<EvtStoreSvc>  stsvc(getParent(), "EvtStoreSvc");
    if ( stsvc.invalid() ) {
        LogError << "cannot get the EvtStoreSvc." << std::endl;
        return false;
    }
    m_storeSvc = stsvc.data();
    if (!m_storeSvc->put("energy",m_energy)){
        LogError<<"error when put energy!!!!!!"<<endl;
        return false;
    }
    if (!m_storeSvc->put("detectorID",m_detID)){
        LogError<<"error when put detectorID!!!!!!"<<endl;
        return false;
    }
    
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
	LogError << "cannot get the NavBuffer @ /Event" << std::endl;
	return false;
    }
    m_buf = navBuf.data();

    SniperPtr<IJunoTimerSvc> timersvc(getParent(), "JunoTimerSvc");
    if (timersvc.invalid()) {
       LogError << "Failed to get JunoTimerSvc!" << std::endl;
       throw SniperException("Make sure you have load the JunoTimerSvc.");
    } 
    m_junotimersvc = timersvc.data();

    // create timers
    m_timer_event = m_junotimersvc->get("SIMEVT/timerevt");
    m_timer_allevent = m_junotimersvc->get("SIMEVT/timerallevt");
    
    SniperPtr<RecGeomSvc> rgSvc(getParent(),"RecGeomSvc"); 
    if ( rgSvc.invalid()) {
        LogError << "Failed to get RecGeomSvc instance!" << std::endl;
        return false;
    }
    m_cdGeom = rgSvc->getCdGeom(); 
    Total_num_PMT = m_cdGeom->findPmt20inchNum();
    LogInfo << "Total PMT: " << Total_num_PMT << std::endl;
    for(int i = 0; i<Total_num_PMT; i++){
        unsigned int all_pmt_id = (unsigned int)i;
        Identifier all_id = Identifier(CdID::id(all_pmt_id,0));
        PmtGeom *all_pmt = m_cdGeom->getPmt(all_id);
        if ( !all_pmt ) {
            LogError << "Wrong Pmt Id " << i << std::endl;
            return false;
        }
        TVector3 all_pmtCenter = all_pmt->getCenter();
        ALL_PMT_pos.push_back(Ball_R/PMT_R*all_pmtCenter);
    }

/*
    const double Pi = 3.1415926;
    TString filename = "/junofs/users/yanzhang/juno/work/trigger/PMTconfiguration/PMTPos_Acrylic";
    TString inputfile = filename + ".csv";
    std::ifstream sfile;
    sfile.open(inputfile,std::ios::in);
    int line = 0;
    std::vector<int> copyno;
    std::vector<double> theta, phi;
    TVector3 pmt_p ;
    ALL_PMT_pos.clear();
    while (!sfile.eof()) {
        double mag = 19500;
        double tmp_lineno, tmp_copyno;
        double tmp_theta, tmp_phi;
        sfile >>  tmp_copyno >> tmp_theta >> tmp_phi;
        if (!sfile.eof()) {
            copyno.push_back(tmp_copyno);
            theta.push_back(tmp_theta);
            phi.push_back(tmp_phi);


           pmt_p.SetMagThetaPhi(mag,tmp_theta*Pi/180,tmp_phi*Pi/180);
           ALL_PMT_pos.push_back(pmt_p);
          line++;
        }
    }
    sfile.close();
*/



    LogDebug << "This is my own BaryCenter Alg!" << std::endl;

    LogDebug  << objName()
        << "   initialized successfully"
        << std::endl; 
    return true; 
}

bool SimpleRecAlg::execute()
{
    ++m_iEvt;
    LogDebug << "---------------------------------------" << std::endl;
    LogDebug << "Processing event " << m_iEvt << std::endl;
    //m_timer_allevent->start();
 
    JM::EvtNavigator* nav = m_buf->curEvt(); 
    JM::CalibHeader* chcol =(JM::CalibHeader*) nav->getHeader("/Event/Calib"); 
    const std::list<JM::CalibPMTChannel*>& chhlist = chcol->event()->calibPMTCol();
    std::list<JM::CalibPMTChannel*>::const_iterator chit = chhlist.begin();
    double pe_sum = 0;
     
    //m_timer_event->start();
    while (chit!=chhlist.end()) {
        const JM::CalibPMTChannel  *calib = *chit++;

        unsigned int pmtId = calib->pmtId();
        Identifier id = Identifier(pmtId);

        if (not CdID::is20inch(id)) {
            continue;
        }
        pe_sum+=calib->sumCharge();
        
        std::vector<double> time_vec = calib->time();
        for (std::vector<double>::iterator it=time_vec.begin(); it!=time_vec.end(); ++it){
            Hit hit;
            hit.hittime = *it;
            hit.pmtid = CdID::module(id);
            Hit_buffer.push_back(hit);
        }
        
    }
    pe_sum=pe_sum/1350;
    LogDebug  << "Done to read CalibPMT" << std::endl;


    m_energy->reset();
    m_energy->setValue(pe_sum);
 
    LogDebug<<"energy is put successfully!!!!!!!!!!!!!!"<<pe_sum<<endl;

    float detid=0x10;
    m_detID->reset();
    m_detID->setValue(detid);

 
    //m_timer_event->stop();
    //float n_time = m_timer_event->elapsed();
    //LogDebug<<"spend time "<<n_time<<endl;   
    //m_timer_allevent->stop();
    //float all_time = m_timer_allevent->elapsed();
    //LogDebug<<"spend all time "<<all_time<<endl;
    


    if(pe_sum>100){
        return true;
    }

    JM::OECHeader *tEventHeader=(JM::OECHeader*)nav->getHeader("/Event/OEC");
    JM::OECEvent *tEvent=(JM::OECEvent*)tEventHeader->event("JM::OECEvent");
    tEvent->setEnergy(pe_sum);
    

    //Rec the Vertex X,Y,Z
    std::sort(Hit_buffer.begin(),Hit_buffer.end());
    SimpleRecAlg::ChargeCenterRec();
   
    tEvent->setVertexX(ChaCenRec_x);
    tEvent->setVertexY(ChaCenRec_y);
    tEvent->setVertexZ(ChaCenRec_z);
    //if can  rec Time
    tEvent->setTime(1000);
    //detector type
    LogDebug<<"!!!!!!SimpleRec Vertex XYZ input OECEvent!!!!!!!!"<<std::endl;
    Hit_buffer.clear();
    
    return true;
}
    
bool SimpleRecAlg::ChargeCenterRec()
{
   double x_sum = 0;
   double y_sum = 0;
   double z_sum = 0; 
   double PE_sum = 0;
   ChaCenRec_x=0;
   ChaCenRec_y=0;
   ChaCenRec_z=0;
   double promptRec_x = 0;
   double promptRec_y = 0;
   double promptRec_z = 0;

   int cut_num = 0.3*Hit_buffer.size();
   for (std::vector<Hit>::iterator it=Hit_buffer.begin(); it!=Hit_buffer.end(); ++it){
       if(PE_sum == cut_num) {
              promptRec_x = x_sum/PE_sum; 
              promptRec_y = y_sum/PE_sum;
              promptRec_z = z_sum/PE_sum;
       }
              const Hit& h = *it;
              x_sum = x_sum + ALL_PMT_pos[h.pmtid].X();
              y_sum = y_sum + ALL_PMT_pos[h.pmtid].Y();
              z_sum = z_sum + ALL_PMT_pos[h.pmtid].Z();
              PE_sum = PE_sum+1;
       }

       ChaCenRec_x = x_sum/PE_sum;
       ChaCenRec_y = y_sum/PE_sum;
       ChaCenRec_z = z_sum/PE_sum;
       double promptRec_r = sqrt(promptRec_x*promptRec_x+promptRec_y*promptRec_y+promptRec_z*promptRec_z);
       double ChaCenRec_r = sqrt(ChaCenRec_x*ChaCenRec_x+ChaCenRec_y*ChaCenRec_y+ChaCenRec_z*ChaCenRec_z);
       double  m_theta = TMath::ACos(ChaCenRec_z/ChaCenRec_r);
       double m_phi = TMath::ATan((ChaCenRec_y/ChaCenRec_x));
       m_phi = (m_phi+0.2556)/0.9979;
       m_theta = (m_theta-0.02116)/0.9786;

       if(promptRec_r>18500){
       //   if(true){
            double a = -1.894*1e-5;
            double b = 0.8247;
            promptRec_r = (-b+sqrt(b*b-4*a*(10730-promptRec_r)))/(2*a);
            if(promptRec_r>17700)promptRec_r=17700;
            ChaCenRec_x = TMath::Abs(promptRec_r*TMath::Sin(m_theta)*TMath::Cos(m_phi))*promptRec_x/TMath::Abs(promptRec_x);
            ChaCenRec_y = TMath::Abs(promptRec_r*TMath::Sin(m_phi)*TMath::Sin(m_theta))*promptRec_y/TMath::Abs(promptRec_y);
            ChaCenRec_z = promptRec_r*TMath::Cos(m_theta);
       }else{
            double a1=1.475*1e-6;
            double b1=0.7841;
            ChaCenRec_r = (-b1+sqrt(b1*b1-4*a1*(228.4-ChaCenRec_r)))/(2*a1);
            ChaCenRec_x = TMath::Abs(ChaCenRec_r*TMath::Sin(m_theta)*TMath::Cos(m_phi))*ChaCenRec_x/TMath::Abs(ChaCenRec_x);
            ChaCenRec_y = TMath::Abs(ChaCenRec_r*TMath::Sin(m_phi)*TMath::Sin(m_theta))*ChaCenRec_y/TMath::Abs(ChaCenRec_y);
            ChaCenRec_z = ChaCenRec_r*TMath::Cos(m_theta);
   }
//   std::cout <<"X:" <<ChaCenRec_x<<"  Y: "<<ChaCenRec_y<<"  Z: "<<ChaCenRec_z << std::endl;

   return true;
}

bool SimpleRecAlg::finalize()
{
    LogInfo << objName()
            << "finalized successfully"
            << std::endl;
    return true;
}


void SimpleRecAlg::reset(){
}
