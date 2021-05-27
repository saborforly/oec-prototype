#include "EvtSteerData/Signature.h"
#include <boost/python.hpp>
#include "SniperKernel/SniperLog.h"
#include "EvtSteerData/EvtCriteria.h"
Signature::Signature(){}

Signature::Signature(std::vector<EvtCriteria*> vec) :
  m_criteriaVec(vec),
  m_SignaturePrescale(1),
  m_SignatureForcedRate(1.),
  m_Id(0),
  m_Predecessor_Id(0),
  m_Successor_Id(0)
{}
Signature::~Signature() {
  int size = m_criteriaVec.size();
  for (int i = 0; i < size; i++) delete m_criteriaVec[i];
}

/// Print method. //
void Signature::print()
{
  LogInfo<<"start signature"<<std::endl;
  LogInfo<< "signature_id: " << m_Sig_label 
       << "; prescale: " << m_SignaturePrescale 
       << "; forcedAccept: " << m_SignatureForcedRate<<std::endl;
  for(std::vector<EvtCriteria *>::iterator i =m_criteriaVec.begin();i!=m_criteriaVec.end(); ++i){
      std::string behaviour=(*i)->getBehaviour();
      LogInfo<<"EvtCriteria Behavior: "<< behaviour<<std::endl;
      LogInfo<<"CriteriaItem: ";
      (*i)->print();
  }
  LogInfo<<"end signature"<<std::endl;
}

void Signature::makeVectors(EvtCriteria* criteria) {
  m_criteriaVec.push_back(criteria);
}

/// Returns the id //
int Signature::id() const { return m_Id; }

/// set the id //
void Signature::setId(int id){ m_Id=id; }

/// Get the forced accept rate. //
float Signature::forcedAccept() const
{
  return m_SignatureForcedRate;
}

/// Set the forced accept rate. //
void Signature::setForcedAccept(float value)
{
  m_SignatureForcedRate = value;
}

/// Get prescale. //
int Signature::prescale() const
{
  return m_SignaturePrescale;
}

/// Set prescale. //
void Signature::setPrescale(int value)
{
  m_SignaturePrescale = value;
}

/// 获取是否veto
bool Signature::isVeto() const
{
  return m_SignatureVeto;
}

/// set veto. //
void Signature::setVeto(bool value)
{
  m_SignatureVeto = value;
}

int Signature::predecessorId(){
  return m_Predecessor_Id;
}

int Signature::successorId(){
  return m_Successor_Id;
}

void Signature::setPredecessorId(int id){
  m_Predecessor_Id=id;
}

void Signature::setSuccessorId(int id){
  m_Successor_Id=id;
}

void Signature::setSigLabel(std::string label) {
  m_Sig_label = label;
}

std::string Signature::label() const {
  return m_Sig_label;
}

int Signature::isCriteriaMatched() {
  int size = m_criteriaVec.size();

  LogInfo << " criteriaVec size: " << size << std::endl;

  m_midMark="";
  for (int i = 0; i < size; i++) {
    //satisfyCriteria 
    if (m_criteriaVec[i]->satisfyCriteria()) {
      m_result = m_criteriaVec[i]->getBehaviour();
      std::string::size_type separator=m_result.find('_');
      std::string perform = m_result.substr(0, separator);
      if(perform=="continue"){
	m_midMark = m_result.substr(separator+1);
      }
      else {
	return m_criteriaVec[i]->idCriteria();
      }
    }
  }
  m_result = "";
  return -1;
}

