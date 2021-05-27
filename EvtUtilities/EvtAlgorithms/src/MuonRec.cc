#include "EvtAlgorithms/MuonRec.h"
#include "SniperKernel/SniperLog.h"
#include "EvtStore/EvtStoreSvc.h"
#include "SniperKernel/SniperPtr.h"

#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "Event/OECHeader.h"
#include "TTimeStamp.h"

DECLARE_ALGORITHM(MuonRec);

MuonRec::MuonRec(const std::string& name) :
  IEFAlgorithm(name) {
  //m_trackNo = new CriteriaItemValue;
}

MuonRec::~MuonRec() {
  delete m_trackNo;
  delete m_muType;
}

bool
MuonRec::initialize(){

  IEFAlgorithm::initialize();
  //int32_t nshower=m_ef->getEFVec(38);
  //if(nshower<2)  return true;

  //float trackNo=1;

  //m_trackNo->setValue(trackNo);
  //LogInfo <<"Number of trackNo is: "<<trackNo<<endl;
  m_run=1;
  LogInfo << "MuonRec initialize()" << std::endl;
  return true;
}

bool 
MuonRec::execute() {
  SniperPtr<EvtStoreSvc> hsSvc(*getParent(),"EvtStoreSvc");
  if( hsSvc.invalid()) {
      LogError << "Unable to locate EvtStoreSvc" << std::endl;
      return false;
  }
  //For Test get event 
  SniperDataPtr<JM::NavBuffer> navbuf(getParent(), "/Event");
  JM::NavBuffer* m_buf=navbuf.data();
  JM::EvtNavigator* tnav=m_buf->curEvt();
  JM::OECHeader *tEventHeader=(JM::OECHeader*)tnav->getHeader("/Event/OEC");
  JM::OECEvent *tEvent=(JM::OECEvent*)tEventHeader->event("JM::OECEvent");
  LogDebug<<"For Test, the current event energy is:"<<tEvent->getEnergy()<<endl;

  m_muType=new CriteriaItemValue();
  m_trackNo=new CriteriaItemValue();

  m_muType->reset();
  f_muType=1;
  m_muType->setValue(f_muType);
  if (!hsSvc->put("muType",m_muType)){
      LogError<<"error when put muType!!!!!!"<<endl;
      return false;
  }
  LogInfo<<"muType is put successfully!!!!!!!!!!!!!!"<<endl;
  
  m_trackNo->reset();
  f_trackNo=1;
  m_trackNo->setValue(f_trackNo);
  if (!hsSvc->put("trackNo",m_trackNo)){
      LogError<<"error when put trackNo!!!!!!"<<endl;
      return false;
  }
  LogInfo<<"trackNo is put successfully!!!!!!!!!!!!!!"<<endl;

  //float              itemValue;
  //CriteriaItemValue* p_itemValue;
  //hsSvc->get("trackNo", p_itemValue);
  //p_itemValue->getValue(itemValue);
  //LogInfo << "get(trackNo): "<<itemValue << std::endl;
  //LogInfo << "put(trackNo) right"<< std::endl;

  return true;
}

bool 
MuonRec::finalize() {
  LogInfo<< "MuonRec finalize()" << std::endl;
  return true;
}

void MuonRec::reset() {

  if(m_run){
    m_trackNo->reset();
    m_run=0;
  }
  return;
}

