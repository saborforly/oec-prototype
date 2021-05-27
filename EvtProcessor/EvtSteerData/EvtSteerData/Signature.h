#ifndef _SIGNATURE_
#define _SIGNATURE_

#include <utility>
#include <map>
#include <vector>
#include <string>
#include <list>
#include "EvtCriteria.h"

class Signature
{
 public:
  Signature();
  Signature(std::vector<EvtCriteria*>);
  virtual ~Signature();
  int id() const;
  void setId(int); 
  void print();
  void makeVectors(EvtCriteria* criteria);
  //inline const std::vector<EvtCriteria*>& criteriaVector() const{
  //  return m_criteriaVec;
  //}
  bool isVeto() const;
  void setVeto(bool);
  float forcedAccept() const;
  void setForcedAccept(float);
  int prescale() const;
  void setPrescale(int);
  int predecessorId();
  int successorId();
  void setPredecessorId(int);
  void setSuccessorId(int);
  void setSigLabel(std::string label);
  std::string label() const;
 
  /// For trigger
  int isCriteriaMatched();
  const std::string& result() const {
    return m_result;
  }
  const std::string& midresult() const {
    return m_midMark;
  }

 private:
  std::vector<EvtCriteria *> m_criteriaVec;
  int m_SignaturePrescale;
  float m_SignatureForcedRate;
  bool m_SignatureVeto;
  std::string m_Sig_label;
  std::string m_result;
  std::string m_midMark;

  int m_Id; 
  int m_Predecessor_Id; 
  int m_Successor_Id; 
};
#endif
