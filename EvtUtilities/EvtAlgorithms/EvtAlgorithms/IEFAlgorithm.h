#ifndef IEF_ALGORITHM_H
#define IEF_ALGORITHM_H

#include "EvtStore/EvtStoreSvc.h"
#include "SniperKernel/AlgFactory.h" 
#include "SniperKernel/AlgBase.h"
#include "SniperKernel/Task.h"
#include "Event/OECHeader.h"

class IEFAlgorithm : public AlgBase {
  public:
    IEFAlgorithm(const std::string& name);
    virtual ~IEFAlgorithm() {};
    virtual bool initialize();
    virtual void reset() = 0;

    void setScope(Task* scope) {m_scope = scope;}
    Task* getScope() {return m_scope;}
  protected:
    int m_output;
    float m_beam;
    bool m_run;
    Task* m_scope;
};
#endif
