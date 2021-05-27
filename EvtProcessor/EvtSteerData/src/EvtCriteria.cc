#include <cmath>
#include "EvtSteerData/EvtCriteria.h"
#include <boost/python.hpp>
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/Task.h"
#include "EvtDataTypes/CriteriaItemValue.h"
#include "EvtStore/EvtStoreSvc.h"
EvtCriteria::EvtCriteria(int id)
  :m_idCriteria(id)
{}

bool EvtCriteria::satisfyCriteria() const {
 
  CriteriaItemValue* p_itemValue;
  float              itemValue;
  std::vector<CriteriaItem>::const_iterator it = m_items.begin();

  bool judge=true;
  SniperPtr<EvtStoreSvc> hsSvc(*m_scope,"EvtStoreSvc");
  if( hsSvc.invalid()) {
    LogError << "Unable to locate EvtStoreSvc" << std::endl;
    return false;
  }

  LogInfo << " items size: " << m_items.size() << std::endl;

  while (it != m_items.end()) {
    LogInfo << "item_name: " << it->name << std::endl;

    bool status = hsSvc->get(it->name, p_itemValue);
    if (!status) return false;
    status = p_itemValue->getValue(itemValue);
    if (!status) return false;

    LogInfo << " items value: " << itemValue << std::endl;

    if (it->type == "MIN") {
      if (itemValue <= it->value) judge= false;
    } else if (it->type == "MAX") {
      if (itemValue >= it->value) judge= false;
    } else if (it->type == "TRUE") {
      if (fabs(itemValue - it->value) > 0.000001) judge= false;
    } else if (it->type == "FALSE") {
      if (fabs(itemValue - it->value) < 0.000001) judge= false;
    }else if (it->type == "STREQ"){
      if(itemValue != it->value) judge= false;
    } else {
      std::cout << "In EvtCriteria::satisfyCriteria() ==> Unvalid CriteriaItemType!" << std::endl;
      exit(1);
    }

    if(it->name =="nmuc"){

    }
    else if(it->name =="nmdc"){

    }
    else if(it->name=="etot"&&it->type=="MIN"){
      
    }
    else if(it->name=="etot"&&it->type=="MAX"){
      
    }
    else if(it->name=="acop"&&it->type=="MAX"){
      
    }
    it++;
  }

  //std::cout << "EvtCriteria::satisfyCriteria() return true" << std::endl;
  return judge;
}

void EvtCriteria::addItem(const std::string& name, float value, const std::string& type,int id) {
  CriteriaItem it = {name, value, type,id};
  m_items.push_back(it);
}
void EvtCriteria::print(){ 
    for(std::vector< CriteriaItem>::iterator i =m_items.begin();i!=m_items.end(); ++i){
        LogInfo<<"CriteriaItem: "<<(*i).name<<" - "<<(*i).value<<" - "<<(*i).type<<" - "<<(*i).id<<std::endl;
    }
}
