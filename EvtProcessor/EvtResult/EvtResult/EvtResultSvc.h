#ifndef EvtRESULT_H
#define EvtRESULT_H

#include <vector>
#include <string>
#include <stdint.h>
#include "SniperKernel/SvcBase.h"
#include "SniperKernel/SvcFactory.h" 
#include "SniperKernel/SniperLog.h"

class EvtResultSvc:public SvcBase
{
      
 public:
  EvtResultSvc(const std::string& name);

  ~EvtResultSvc();

  bool initialize ( );
  bool execute ( );
  bool finalize ( );

  bool push_back(uint32_t val);

  void setAnswer(std::string &answer);
  void setMark(std::string &mark);
  void setMidAnswer(std::string &answer);
  void reset();

  /* Method to retrieve the Evt answer string */
  const std::string getAnswer() const;
  const std::string getMidAnswer() const;
  const std::string getMark() const;

  /* Method to retrieve the Evt answer (string vector) index */
  const int getMidAnswerIndex() const;
  
 
 private:
  std::vector<std::string> m_midAnswerVector; 
  std::string m_mark;
  std::string m_answer; 
  int m_midanswerIndex;
  std::vector<uint32_t> m_efVec;
};
  
#endif
