/*************************
 *author: liuyan
 * ***********************/
#include "EvtSteering/StepDecision.h"
// INCLUDE GAUDI HEADER FILES:
#include "SniperKernel/AlgFactory.h" 
#include "SniperKernel/AlgBase.h" 
#include "EvtSteerData/Signature.h"                                  
#include <string> 
#include <vector> 
#include <algorithm> 
DECLARE_ALGORITHM(StepDecision);
////////////////////////////////////////////////////////////////
//  CONSTRUCTOR:
StepDecision::StepDecision(const std::string& name): 
  AlgBase(name) {
  m_isEnabled = false;
}
                                  
// DESTRUCTOR:
StepDecision::~StepDecision() { }
                                  
/////////////////////////////////////////////////////////////////
// INITIALIZE METHOD:
bool
StepDecision::initDecision(Signature* signature) {

  /*
  //StatusCode sc = service( "EvtStoreSvc", m_EvtStoreSvc);
  if( sc.isFailure() ) {
    log << MSG::FATAL << name() << ": Unable to locate Service EvtStoreSvc" << endreq;
    return sc;
  }
  */
  std::string sigID = signature->label();
  m_SigMap[sigID] = signature;

  LogDebug << " Find Signature with ID: " << sigID << std::endl;

  return true;
}

/////////////////////////////////////////////////////////////////
int StepDecision::execDecision(const std::string& sigID) {
  LogDebug << " Decision execute: " << sigID << std::endl;

  Signature* sig = m_SigMap[sigID];
  int decision = sig->isCriteriaMatched();
  
  m_continue =sig->midresult();
  m_behaviour = sig->result();
  //cout << "----"<< m_behaviour<< "  "<<m_continue<<endl;
  return decision;
}
