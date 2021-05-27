#include "EvtAlgorithms/IEFAlgorithm.h"
#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "EvtStore/EvtStoreSvc.h"

IEFAlgorithm::IEFAlgorithm(const std::string& name):
  AlgBase(name){
  //declProp("OutputLevel",m_output = 7777777);
}

bool
IEFAlgorithm::initialize(){
  LogInfo << "in initialize()" << std::endl;
 
  m_run=0;
  //bool sc = m_EvtStoreSvc->get("EFResult", m_ef);
  //if (!sc) {
  //  LogError << "m_EvtStoreSvc->get(EFResult) wrong" << std::endl;
  //  return sc;
  //}  

  
  LogInfo << "finish initialize()" << std::endl;
  return true;
}
