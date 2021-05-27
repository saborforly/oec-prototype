/********************************************************************
NAME:     StepSequencer.cc
author:   liuyan
********************************************************************/
#include "EvtSteering/StepSequencer.h"
#include "SniperKernel/Task.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/AlgBase.h"
#include "SniperKernel/SvcBase.h"
#include "SniperKernel/DataMemSvc.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/DeclareDLE.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SniperLog.h"
#include "NonUserIf/TaskProperty.h"
#include "NonUserIf/DLEFactory.h"
#include <algorithm>
#include "NonUserIf/DLEFactory.h"

DECLARE_ALGORITHM(StepSequencer);

StepSequencer::StepSequencer(const std::string& name):
  AlgBase(name) {
  m_isEnabled = false;
}

StepSequencer::~StepSequencer() {}

 
inline void findAlgTypeName(const std::string& property,
		  std::string& SubAlg_type,
		  std::string& SubAlg_NameParam) 
{
  int slash_pos = property.find_first_of("/");
  SubAlg_type = property.substr( 0, slash_pos );
  SubAlg_NameParam = (slash_pos > 0) ? property.substr( slash_pos + 1) : SubAlg_type;
}

inline void findParamSet(const std::string& property,
		  std::string& SubAlg_name,
		  std::string& SubAlg_param) 
{
  int slash_pos = property.find_first_of("/");
  SubAlg_name = property.substr( 0, slash_pos );
  SubAlg_param = (slash_pos > 0) ? property.substr( slash_pos + 1) : SubAlg_name;
}


void StepSequencer::reset()
{
  //TODO
  //AlgBase reset()
  /*
  std::map<std::string, std::vector<AlgBase*> >::iterator it = m_AlgMap.begin();
  LogDebug << "DEBUG " << "the size of algorithm map is " << m_AlgMap.size() << std::endl;
  for ( ; it != m_AlgMap.end(); it++) {
    LogDebug << "DEBUG " << "reset() of " << it->first << std::endl;
    const std::vector<AlgBase*>& algs = it->second;
    int n = algs.size();
    
    for (int i = 0; i < n; i++) {
      if (IEFAlgBase* ialg = dynamic_cast<IEFAlgBase*>(algs[i])) {
	//std::cout << " @reset(): " << algs[i]->name() << "  p: " << ialg << std::endl;
	ialg->reset();
      }
    }
    */
  
}

// INITIALIZE METHOD:
bool
StepSequencer::initSequencer( Sequence* sequence) {

  std::string SubAlg_type, SubAlg_name, SubAlg_param, SubAlg_NameParam,AlgNameFromConfig ;
  AlgBase* pAlg;
  AlgBase* bAlg;
  
  //read in algorithms in sequence and initialize them
  LogDebug<< "DEBUG " << "Sequence " << sequence->getSeqID()
     <<" has " << sequence->algoNum() << " algorithms " << std::endl;

  std::vector<AlgBase*> helpAlgVec;
  std::vector<std::string>::iterator algI = sequence->algoVector().begin();

  for ( ; algI != sequence->algoVector().end(); algI++){
      std::string alg_def = *algI;
      LogDebug << "DEBUG " << "algorithm    : " << alg_def << std::endl;     
      findAlgTypeName(alg_def,SubAlg_type,SubAlg_NameParam);
      findParamSet(SubAlg_NameParam,AlgNameFromConfig,SubAlg_param);
      SubAlg_name=AlgNameFromConfig+"_"+SubAlg_param;
      
      //check if the name exists, don't create again the subalgorithm, just added to the AlgoTePairs vector
      bool subAlgExist = doesExistAlready(AlgNameFromConfig);
      if(!subAlgExist) {   
         //StatusCode sc = createSubAlgBase(SubAlg_type, SubAlg_name, pAlg);
	 LogDebug << "DEBUG " << "created algorithm:" << AlgNameFromConfig <<"!"<< std::endl;
	 //bAlg = createAlg(SubAlg_name); 
         DLElement* obj = DLEFactory::instance().create(AlgNameFromConfig);
         obj->setParent(getParent());
         bAlg = dynamic_cast<AlgBase*>(obj);
         bAlg->initialize();
	 //bAlg->beginRun();
         //bAlg->setProperty("ParamSetFile", SubAlg_param);

	 helpAlgVec.push_back(bAlg);
      } else {
	 pAlg = existingAlg(AlgNameFromConfig);
	 helpAlgVec.push_back(pAlg);
      }
  }
  m_AlgMap[sequence->getSeqID()] = helpAlgVec;
  
  LogDebug <<"Initialization successfully" << std::endl;
  return true;
}

bool StepSequencer::execSequencer(const std::string& seqID)
{
  const std::vector<AlgBase*>& algVec = m_AlgMap[seqID];

  LogDebug << "DEBUG " << "=============================" << std::endl;

  std::vector<AlgBase*>::const_iterator it = algVec.begin();
  for(; it != algVec.end(); it++) {
    bool sc = (*it)->execute();
    if (!sc) {
      LogError << "ERROR " << (*it)->objName() << " execute failed" << std::endl;
      return false;
    }
  }

  return true;
}
/////////////////////////////////////////////////////////////////

AlgBase* StepSequencer::existingAlg (std::string subAlgName)
{
  std::vector<AlgBase*>::iterator it = helpAlgVec.begin();
  for ( ; it != helpAlgVec.end(); it++) {
    if(subAlgName==(*it)->objName()) {
       LogDebug << subAlgName<<" already created, return pointer to it "<< std::endl;
       return *it;
    }
  }
 
  std::string subTask = "subTask_"+subAlgName;
  Task* task = dynamic_cast<Task*>(getParent()->find(subTask)); 
  AlgBase* exAlg = dynamic_cast<AlgBase*>(task->find(subAlgName));

 return exAlg;
}

/////////////////////////////////////////////////////////////////
bool StepSequencer::doesExistAlready(std::string aName) 
{  
  
  bool doesExist=false;
  if(helpAlgVec.size()==0) {
    doesExist=false;
  }
  else {
    std::vector<AlgBase*>::iterator it = helpAlgVec.begin();
    for ( ; it != helpAlgVec.end(); it++) {
      if(aName==(*it)->objName()) {
        LogDebug <<" sub-algorithm already created" << std::endl;
	doesExist=true;
	break;
      }
    }
  }
  if(doesExist) return true;

  std::string subTask = "subTask_"+aName;

  Task* task = dynamic_cast<Task*>(getParent()->find(subTask));
  if ( !task ) {
    return false;
  }
  AlgBase* exAlg = dynamic_cast<AlgBase*>(task->find(aName));
  if ( !exAlg ) {
    return false;
  }else{
    LogDebug << "subTask alg  have existence  "<<aName << std::endl;
    return true;
  }

}

////////////////////////////////////////////////////////////
/*
std::vector<AlgBase*> StepSequencer::searchAlgForEachTE_o( Sequence* seq ) 
{

  std::string SubAlg_type, SubAlg_name, SubAlg_param, SubAlg_NameParam,AlgNameFromConfig;

  vMatchAlg.clear();
     std::vector<std::string>::iterator It = seq->algoVector().begin();
     for ( ; It < seq->algoVector().end(); It++) {
       std::string alg_defin = *It;
       findAlgTypeName(alg_defin,SubAlg_type,SubAlg_NameParam);
       findParamSet(SubAlg_NameParam,AlgNameFromConfig,SubAlg_param);
       SubAlg_name=AlgNameFromConfig+"_"+SubAlg_param;
      }   
  return vMatchAlg;
}
*/
