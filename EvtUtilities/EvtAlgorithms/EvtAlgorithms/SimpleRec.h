#ifndef SimpleRec_H
#define SimpleRec_H

#include "EvtDataTypes/CriteriaItemValue.h"
//#include "FastRecAlg/FastRecAlg.h"
#include "IEFAlgorithm.h"

#include "TFile.h"
#include "TTree.h"

class SimpleRec: public IEFAlgorithm{
 public:
  SimpleRec(const std::string& name);
  virtual ~SimpleRec();
  virtual bool initialize();
  bool execute();
  bool finalize();
  void reset();
 private:
  CriteriaItemValue* m_energy;
  CriteriaItemValue* m_detID;

  //use fast reconsruction algorithm
  //FastRecAlg *m_fastAlg;
  //input for test
  TFile *m_InputFile;
  TTree *m_InputTree;
  float f_energy;
  float f_time;
  int i_detID;
  int i_totevt;

};
#endif //SimpleRec_H
