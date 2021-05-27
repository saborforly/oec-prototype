#include "NonUserIf/DLEFactory.h"
#include "EvtAlgorithms/SimpleRec.h"
#include "SniperKernel/SniperLog.h"
#include "EvtStore/EvtStoreSvc.h"

#include "SniperKernel/SniperPtr.h"

#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "Event/OECHeader.h"
#include "TTimeStamp.h"

DECLARE_ALGORITHM(SimpleRec);

SimpleRec::SimpleRec(const std::string& name) :
  IEFAlgorithm(name) {
  //m_energy = NULL;//new CriteriaItemValue;
  //m_time= NULL;//new CriteriaItemValue;
}

SimpleRec::~SimpleRec() {
  delete m_energy;
  delete m_detID; 
}

bool
SimpleRec::initialize(){

  IEFAlgorithm::initialize();
  //if(nshower<2)  return true;

  //use fast reconstruction algorithm
  //DLElement* objalg = DLEFactory::instance().create("FastRecAlg");
  //objalg->setParent(getParent());
  //m_fastAlg = dynamic_cast<FastRecAlg*>(objalg);
  //m_fastAlg->initialize();

  //input for test
  m_InputFile=new TFile("/junofs/users/huangx/huangxinihep/oec-prototype/OECAlg/share/fackOECInput.root","READ");
  //m_InputFile=new TFile("/junofs/users/huangx/huangxinihep/EventFilter/EvtUtilities/EvtAlgorithms/share/fackOECInput.root","READ");
  m_InputTree=(TTree*)m_InputFile->Get("OECInput");
  m_InputTree->SetBranchAddress("E",&f_energy);
  m_InputTree->SetBranchAddress("T",&f_time);
  i_totevt=-1;

  //m_energy->setValue(4.);
  //m_time->setValue(0);
  m_run=1;
  LogInfo << "SimpleRec initialize()" << std::endl;
  return true;
}

bool 
SimpleRec::execute() {
  SniperPtr<EvtStoreSvc> hsSvc(*getParent(),"EvtStoreSvc");
  if( hsSvc.invalid()) {
      LogError << "Unable to locate EvtStoreSvc" << std::endl;
      return false;
  }

  //put event into buffer
  SniperDataPtr<JM::NavBuffer> navbuf(getParent(), "/Event");
  JM::NavBuffer *m_buf=navbuf.data();
  JM::EvtNavigator* tnav=m_buf->curEvt();

  int i_time=(int)f_time;
  int i_second=i_time/1000000000;//get second
  int i_nanosecond=i_time%1000000000;//get nanosecond
  TTimeStamp m_currentTimeStamp=TTimeStamp(i_second,i_nanosecond);
  tnav->setTimeStamp(m_currentTimeStamp);

  JM::OECHeader *tEventHeader=(JM::OECHeader*)tnav->getHeader("/Event/OEC");
  JM::OECEvent *tEvent=(JM::OECEvent*)tEventHeader->event("JM::OECEvent");    
  //set event value
  float k_energy = 100;
  tEvent->setEnergy(k_energy);
  tEvent->setTime(f_time);
  tEvent->setVertexX(0);
  tEvent->setVertexY(0);
  tEvent->setVertexZ(0);
  LogInfo << "k_energy: "<<k_energy << std::endl; 


  //use fast reconstruction algorithm
  //m_fastAlg->execute();
  i_totevt++;
  m_InputTree->GetEntry(i_totevt);
  m_energy = new CriteriaItemValue;
  m_detID= new CriteriaItemValue;
  //storeSvc detID
  i_detID=0x10;
  m_detID->reset();
  m_detID->setValue(i_detID);
  if (!hsSvc->put("detectorID",m_detID)){
      LogError<<"error when put ID!!!!!!"<<endl;
      return false;
  }

  //storeSvc energy
  m_energy->reset();
  //m_energy->setValue(f_energy);
  m_energy->setValue(180);
  if (!hsSvc->put("energy",m_energy)){
      LogError<<"error when put energy!!!!!!"<<endl;
      return false;
  }


  //for test
  float              itemValue;
  CriteriaItemValue* p_itemValue;
  hsSvc->get("energy", p_itemValue);
  p_itemValue->getValue(itemValue);
  LogInfo << "!get(energy): "<<itemValue << std::endl;

  return true;
}

bool 
SimpleRec::finalize() {
  LogInfo<< "SimpleRec finalize()" << std::endl;
  //use fast reconstruction algorithm
  //m_fastAlg->finalize();
  return true;
}

void SimpleRec::reset() {

  if(m_run){
    m_energy->reset();
    m_run=0;
  }
  return;
}

