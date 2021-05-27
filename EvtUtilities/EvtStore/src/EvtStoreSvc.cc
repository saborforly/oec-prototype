#include "SniperKernel/SvcFactory.h"
#include "EvtStore/EvtStoreSvc.h"
#include "SniperKernel/AlgBase.h"
#include <strstream>
#include <algorithm>

DECLARE_SERVICE(EvtStoreSvc);

EvtStoreSvc::EvtStoreSvc( const std::string& name) 
  : SvcBase(name) 
{ }

EvtStoreSvc::~EvtStoreSvc() { }
/*
StatusCode EvtStoreSvc::queryInterface (const InterfaceID& riid, void** ppvIF ) 
{
  if ( IID_IEvtStoreSvc.versionMatch(riid) ) { 
//    *ppvIF = static_cast<IEvtStoreSvc*> (this); 
    *ppvIF = (EvtStoreSvc*)this; 
  } else { 
    return Service::queryInterface(riid, ppvIF) ; 
  }
  return StatusCode::SUCCESS;
}
*/
bool
EvtStoreSvc::initialize ( ) 
{

  LogInfo << "Start of run initialisation" << std::endl;

  
  return true;
}
bool
EvtStoreSvc::execute ( )
{
  LogInfo << "Start of run executen" << std::endl;
}

bool
EvtStoreSvc::finalize ( ) 
{
  ContainerType::iterator pos;
  for (pos = m_map.begin(); pos != m_map.end(); ++pos) {
//    delete (*pos).second;
  }
  m_map.erase(m_map.begin(),m_map.end());
  return true;
}

  //
  // method to print out all keys in the store
  //
void EvtStoreSvc::printKeys(){
  ContainerType::iterator pos;
  for (pos = m_map.begin(); pos != m_map.end(); ++pos) {
    cout << pos->first << endl;
  }
}

int EvtStoreSvc::size(){
  return m_map.size();
}

int EvtStoreSvc::max_size(){
  return m_map.max_size();
}

//
// returns string that contains the length of the Store+1, or, if
//that key is already taken in the Store, length+n with n>1 and n the 
//lowest value for which the key is not already taken
//
string EvtStoreSvc::sListLength(){
  unsigned int iListLength = this->size();
  iListLength += 1;
  ostrstream ListLength;
  
  //convert integer into std::string
  ListLength <<  iListLength;
  ListLength << '\0'; 
  string stringListLength = ListLength.str();          
  if (this->exists(stringListLength)){
    stringListLength = "unset";
    for (int i = 0; i < this->max_size(); i++){
      iListLength += 1;
      ListLength <<  iListLength; 
      ListLength << '\0'; 
      stringListLength = ListLength.str();
      if (!(this->exists(stringListLength))) {
        ListLength.freeze(0);
        return stringListLength;
      }
    }
  }

  ListLength.freeze(0);
  return stringListLength;
}

// return true if named object exists

bool EvtStoreSvc::exists(const std::string& name) {
  return (m_map.find(name) != m_map.end());
}

