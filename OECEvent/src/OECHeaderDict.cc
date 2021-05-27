// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dOdOdIsrcdIOECHeaderDict
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
#include "OECHeader.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_JMcLcLOECHeader(void *p = 0);
   static void *newArray_JMcLcLOECHeader(Long_t size, void *p);
   static void delete_JMcLcLOECHeader(void *p);
   static void deleteArray_JMcLcLOECHeader(void *p);
   static void destruct_JMcLcLOECHeader(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JM::OECHeader*)
   {
      ::JM::OECHeader *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JM::OECHeader >(0);
      static ::ROOT::TGenericClassInfo 
         instance("JM::OECHeader", ::JM::OECHeader::Class_Version(), "OECHeader.h", 11,
                  typeid(::JM::OECHeader), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JM::OECHeader::Dictionary, isa_proxy, 4,
                  sizeof(::JM::OECHeader) );
      instance.SetNew(&new_JMcLcLOECHeader);
      instance.SetNewArray(&newArray_JMcLcLOECHeader);
      instance.SetDelete(&delete_JMcLcLOECHeader);
      instance.SetDeleteArray(&deleteArray_JMcLcLOECHeader);
      instance.SetDestructor(&destruct_JMcLcLOECHeader);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JM::OECHeader*)
   {
      return GenerateInitInstanceLocal((::JM::OECHeader*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JM::OECHeader*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace JM {
//______________________________________________________________________________
atomic_TClass_ptr OECHeader::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *OECHeader::Class_Name()
{
   return "JM::OECHeader";
}

//______________________________________________________________________________
const char *OECHeader::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::OECHeader*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int OECHeader::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JM::OECHeader*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *OECHeader::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::OECHeader*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *OECHeader::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JM::OECHeader*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace JM
namespace JM {
//______________________________________________________________________________
void OECHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class JM::OECHeader.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JM::OECHeader::Class(),this);
   } else {
      R__b.WriteClassBuffer(JM::OECHeader::Class(),this);
   }
}

} // namespace JM
namespace ROOT {
   // Wrappers around operator new
   static void *new_JMcLcLOECHeader(void *p) {
      return  p ? new(p) ::JM::OECHeader : new ::JM::OECHeader;
   }
   static void *newArray_JMcLcLOECHeader(Long_t nElements, void *p) {
      return p ? new(p) ::JM::OECHeader[nElements] : new ::JM::OECHeader[nElements];
   }
   // Wrapper around operator delete
   static void delete_JMcLcLOECHeader(void *p) {
      delete ((::JM::OECHeader*)p);
   }
   static void deleteArray_JMcLcLOECHeader(void *p) {
      delete [] ((::JM::OECHeader*)p);
   }
   static void destruct_JMcLcLOECHeader(void *p) {
      typedef ::JM::OECHeader current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JM::OECHeader

namespace {
  void TriggerDictionaryInitialization_OECHeaderDict_Impl() {
    static const char* headers[] = {
"OECHeader.h",
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
#line 1 "OECHeaderDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace JM{class __attribute__((annotate("$clingAutoload$OECHeader.h")))  OECHeader;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "OECHeaderDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "OECHeader.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"JM::OECHeader", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("OECHeaderDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_OECHeaderDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_OECHeaderDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_OECHeaderDict() {
  TriggerDictionaryInitialization_OECHeaderDict_Impl();
}
