#ifndef STEPDECISION_H
#define STEPDECISION_H


#include "EvtSteerData/Signature.h"
//#include "EvtStore/EvtStoreSvc.h"
#include <vector>
#include <string> 
#include "SniperKernel/AlgBase.h"


class StepDecision : public AlgBase
{
 public:

  StepDecision(const std::string& name);
  ~StepDecision();

  virtual bool isEnabled( ) const{return m_isEnabled;};
  bool initialize(){return true;};
  bool execute(){return true;};
  bool finalize(){return true;};

  bool initDecision(Signature* );
  int execDecision(const std::string& sigID);  
  const std::string& getBehaviour() const { return m_behaviour; }
  const std::string& getContinue() const { return m_continue; }
 private:
  //EvtStoreSvc* m_EvtStoreSvc;

  std::string m_behaviour;
  std::string m_continue;
  std::map<std::string, Signature*> m_SigMap;

  bool m_isEnabled;
};
#endif
