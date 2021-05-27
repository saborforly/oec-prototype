#include "SniperKernel/AlgFactory.h"
#include "EventBuf/OECBufAlg.h"
#include "SniperKernel/AlgBase.h"
#include <strstream>
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "EvtDataTypes/CriteriaItemValue.h"

#include "Event/CalibHeader.h"
#include "EvtNavigator/NavBuffer.h"
#include "TTimeStamp.h"
#include "TSystem.h"

DECLARE_ALGORITHM(OECBufAlg);

OECBufAlg::OECBufAlg( const std::string& name) 
  : AlgBase(name) 
{
    i_currentTime_sec=0;
    i_currentTime_nanosec=0;
    f_totRate=0;
    declProp("RateMap", m_rateMap);
}

OECBufAlg::~OECBufAlg() { 
}
bool
OECBufAlg::initialize()
{
    //m_rateMap={{"1",1},{"2",1}};

    f_totRate=0;
    SniperPtr<EvtStoreSvc>  stsvc(getParent(), "EvtStoreSvc");
    if ( stsvc.invalid() ) {
        LogError << "cannot get the EvtStoreSvc." << std::endl;
        return false;
    }
    m_storeSvc= stsvc.data();

    SniperPtr<IDataMemMgr> mMgr(*getParent(), "BufferMemMgr");
    if ( mMgr.invalid() ) {
        LogError << "cannot get the BufferMemMgr." << std::endl;
        return false;
    }
    m_mMgr=mMgr.data();    

    int inputNo=-1;
    for (map<string,float>::iterator it=m_rateMap.begin();it!=m_rateMap.end();it++){
        inputNo++;
        f_totRate+=it->second;
        InputReviser* aInci = new InputReviser(*getRoot(),Form("subTask_%d",inputNo), false);
        m_inputReviser.push_back(aInci);
    }
    LogInfo<<"total rate is:"<<f_totRate<<endl;

    LogInfo << "Initialize OECBuf!" << std::endl;
    m_timer_all = new JunoTimer("oecbuf");
    m_timer = new JunoTimer("getOneEvent");

    return true;
}

bool 
OECBufAlg::execute()
{
   ProcInfo_t info;
   gSystem->GetProcInfo(&info);
   float m_memory = info.fMemResident/1024;
   LogDebug<<"Get memory "<<m_memory<<endl;

   m_timer_all->start();
   m_timer->start();

   if (!getOneEvent()){
       LogError<<"fail to get one event!!!!"<<endl;
       return false;
   }
   m_timer->stop();

   m_thisEvent = new JM::OECEvent();     //init event
   m_thisHeader = new JM::OECHeader();  //init header 
 
   m_nav = new JM::EvtNavigator();
   TTimeStamp thistime(i_currentTime_sec,i_currentTime_nanosec);
   m_nav->setTimeStamp(thistime);

   m_nav->addHeader("/Event/Calib", m_calHeader);
   m_mMgr->adopt(m_nav, "/Event");
   m_thisHeader->setEvent(m_thisEvent);
   m_nav->addHeader("/Event/OEC", m_thisHeader);

   
   m_thisEvent->setTime(thistime);

   m_timer_all->stop();
   float timer = m_timer->elapsed();
   float timer_all = m_timer_all->elapsed();
  
   LogInfo << "getOneEvent spend time " <<timer <<std::endl;
   LogInfo << "all spend time " <<timer_all <<std::endl;
  
   LogInfo << "Put init Event in Buffer!" << std::endl;
   return true;
}

bool OECBufAlg::getOneEvent(){
    //get time
    double deltaT=m_myrandom.Uniform(1./(double)f_totRate);
    int deltaT_sec=(int)deltaT;
    int deltaT_nanosec=1e9*(deltaT-(double)deltaT_sec);
    i_currentTime_sec+=deltaT_sec;
    i_currentTime_nanosec+=deltaT_nanosec;

    //get sample category
    int tempNo=-1;
    float tempsum=0;
    double temprand=m_myrandom.Uniform(f_totRate);
    for (map<string, float>::iterator it=m_rateMap.begin();it!=m_rateMap.end();it++){
        tempNo++;
        tempsum+=it->second;
        if (temprand<tempsum){
            break;
        }
    }
    bool m_fire = m_inputReviser[tempNo]->fire(*getRoot());
    if(!m_fire){
         LogInfo<<"!!!! --- fire failed--- !!!! "<<endl;
    }
    

    //store for reconstruction algorithm to use
    //For Test
    
    Task* task = dynamic_cast<Task*>(getParent()->find(Form("subTask_%d",(int)tempNo)));
    SniperDataPtr<JM::NavBuffer>  navBuf(task, "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    JM::NavBuffer* buf = navBuf.data();
    JM::EvtNavigator* nav = buf->curEvt(); 
    m_calHeader =(JM::CalibHeader*) nav->getHeader("/Event/Calib"); 
    
    return true;
    

}
