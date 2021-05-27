#include <iostream>
#include "EvtResult/EvtResultSvc.h"
DECLARE_SERVICE(EvtResultSvc);

EvtResultSvc::EvtResultSvc( const std::string& name) 
  : SvcBase(name) 
{ }

EvtResultSvc::~EvtResultSvc(){}
bool
EvtResultSvc::initialize ( ) 
{

  LogInfo << "Start to run initialisation" << std::endl; 
  reset();
  return true;
}
bool
EvtResultSvc::execute ( )
{
  LogInfo << "Start to run executen" << std::endl;
  return true;
}

bool
EvtResultSvc::finalize ( ) 
{
  LogInfo << "!!! finalize !!!" << std::endl;
  m_efVec.clear();
  m_midAnswerVector.clear();
  return true;
}

bool EvtResultSvc::push_back(uint32_t val){
  if(m_efVec.size()>0&&m_efVec.size()<54) return false;
  if(m_efVec.size()==0) m_efVec.resize(54,0);
  m_efVec.push_back(val);
  return true;
}

void EvtResultSvc::setMidAnswer(std::string &answer)
{
  m_midanswerIndex++;
  m_midAnswerVector.push_back(answer);
}

void EvtResultSvc::reset()
{
  m_answer = "Unknown";
  m_mark = "Unknown";
  m_midanswerIndex = -1;
  
}

void EvtResultSvc::setAnswer(std::string &answer){ m_answer=answer; }
const std::string EvtResultSvc::getAnswer() const { return m_answer; }

void EvtResultSvc::setMark(std::string &mark){ m_mark=mark; }
const std::string EvtResultSvc::getMark() const { return m_mark; }


const std::string EvtResultSvc::getMidAnswer() const { return m_midAnswerVector[m_midanswerIndex]; }
const int EvtResultSvc::getMidAnswerIndex() const { return m_midanswerIndex; }
