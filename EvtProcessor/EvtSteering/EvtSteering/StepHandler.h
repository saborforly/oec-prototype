#ifndef STEPHANDLER_H
#define STEPHANDLER_H

#include <vector>
#include <string>
#include "SniperKernel/AlgBase.h" 
#include "EvtConfig/EvtConfigSvc.h"
#include "EvtStore/EvtStoreSvc.h"
#include "EvtResult/EvtResultSvc.h"
#include "StepSequencer.h"
#include "StepDecision.h"
#include "SniperKernel/SniperPtr.h"
#include "EvtConfig/EvtConfigSvc.h"
#include "EvtStore/EvtStoreSvc.h"
#include <map>
//#ifdef TIME_MEASUREMENT  ////// Timer
#include "JunoTimer/JunoTimer.h"
#include "JunoTimer/IJunoTimerSvc.h"
//#include <iomanip>  //setw()
//#endif

class StepHandler : public AlgBase
{
 public:

  StepHandler(const std::string& name);
  ~StepHandler();

  bool initialize();
  bool finalize();
  bool execute();
  bool beginRun();
  std::map<std::string,std::string> getEvtResult(){
    return m_evtResult;
  }

 private:

  std::map<std::string,std::string> m_evtResult;

  std::vector<std::string> m_stepIdList;
  StepSequencer* sequencerAlg;
  StepDecision* decisionAlg;
  std::vector<std::string> m_tmp_evtType;
 
 
  JunoTimer* m_timer[5];
#ifdef TIME_MEASUREMENT   //// Timer
  //IBesTimerSvc* m_timersvc;
  //JunoTimer* m_timer[5];

  //NTuple::Tuple* m_tuple;

  //NTuple::Item<float> g_StepHandlerTime;
  //NTuple::Item<float> g_1stStepExecTime;
  //NTuple::Item<float> g_1stStepDeciTime;
  //NTuple::Item<float> g_2ndStepExecTime;
  //NTuple::Item<float> g_2ndStepDeciTime;
  unsigned long round;
#endif
};

#endif
