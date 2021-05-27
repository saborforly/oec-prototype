// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dOdOdIsrcdIOECEventDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// The generated code does not explicitly qualifies STL entities
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "OECEvent.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_JMcLcLOECEvent(void *p = 0);
   static void *newArray_JMcLcLOECEvent(Long_t size, void *p);
   static void delete_JMcLcLOECEvent(void *p);
   static void deleteArray_JMcLcLOECEvent(void *p);
   static void destruct_JMcLcLOECEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::OECEvent*)
   {
      ::JM::OECEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::OECEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("JM::OECEvent", ::JM::OECEvent::Class_Version(), "OECEvent.h", 11,
                  typeid(::JM::OECEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::OECEvent::Dictionary, isa_proxy, 4,
                  sizeof(::JM::OECEvent) );
      instance.SetNew(&new_JMcLcLOECEvent);
      instance.SetNewArray(&newArray_JMcLcLOECEvent);
      instance.SetDelete(&delete_JMcLcLOECEvent);
      instance.SetDeleteArray(&deleteArray_JMcLcLOECEvent);
      instance.SetDestructor(&destruct_JMcLcLOECEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::OECEvent*)
   {
      return GenerateInitInstanceLocal((::JM::OECEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::OECEvent*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr OECEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *OECEvent::Class_Name()
{
   return "JM::OECEvent";
}

//______________________________________________________________________________
const char *OECEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::OECEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int OECEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::OECEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *OECEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::OECEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *OECEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::OECEvent*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void OECEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::OECEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::OECEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::OECEvent::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLOECEvent(void *p) {
      return  p ? new(p) ::JM::OECEvent : new ::JM::OECEvent;
   }
   static void *newArray_JMcLcLOECEvent(Long_t nElements, void *p) {
      return p ? new(p) ::JM::OECEvent[nElements] : new ::JM::OECEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLOECEvent(void *p) {
      delete ((::JM::OECEvent*)p);
   }
   static void deleteArray_JMcLcLOECEvent(void *p) {
      delete [] ((::JM::OECEvent*)p);
   }
   static void destruct_JMcLcLOECEvent(void *p) {
      typedef ::JM::OECEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::OECEvent

namespace {
  void TriggerDictionaryInitialization_OECEventDict_Impl() {
    static const char* headers[] = {
"OECEvent.h",
0
    };
    static const char* includePaths[] = {
"/junofs/users/liuyan/OEC/oec-prototype/OECEvent",
"/cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J20v1r0-Pre1/offline/DataModel/BaseEvent",
"/cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J20v1r0-Pre1/offline/DataModel/EDMUtil",
"/cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J20v1r1-branch/ExternalLibs/ROOT/6.20.02/include/",
"/junofs/users/liuyan/OEC/oec-prototype/OECEvent/Event/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "OECEventDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$OECEvent.h")))  OECEvent;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "OECEventDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "OECEvent.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::OECEvent", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("OECEventDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_OECEventDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_OECEventDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_OECEventDict() {
  TriggerDictionaryInitialization_OECEventDict_Impl();
}
