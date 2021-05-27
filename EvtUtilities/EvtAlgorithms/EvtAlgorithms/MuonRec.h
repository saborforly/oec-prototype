#ifndef MuonRec_H
#define MuonRec_H

#include "EvtDataTypes/CriteriaItemValue.h"
#include "IEFAlgorithm.h"

class MuonRec: public IEFAlgorithm{
 public:
  MuonRec(const std::string& name);
  virtual ~MuonRec();
  virtual bool initialize();
  bool execute();
  bool finalize();
  void reset();
 private:
  CriteriaItemValue* m_trackNo;
  CriteriaItemValue* m_muType;

  float f_trackNo;
  float f_muType;//1:normal muon   3:shower muon   4:stop muon

};
#endif //MuonRec_H
