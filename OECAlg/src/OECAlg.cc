#include "OECAlg.h"
#include "EvtDataTypes/CriteriaItemValue.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "Event/OECHeader.h"
#include "TMath.h"
//#include "OECEvent.cc"

#include "SniperKernel/AlgFactory.h"
DECLARE_ALGORITHM(OECAlg);

OECAlg::OECAlg(const string &name):AlgBase(name),
    tECutRange({
            {0,0.75},
            {0,1.6},
            {0,2},
            {0,2.6},
            {0,100},
            {100,0}
            }),
    pECutRange({
            {0,0.75},
            {0,4},
            {0,12},
            {0,100},
            {0,1000},
            {1000,0}
            }),
    distCutRange({
            {0,2500},
            {2500,0}
            }),
    timeCutRange({
            {0,10000},
            {10000,0}
            }),
    TagName({
            "tagIBD",
            "tagBiPo",
            "tagProtonDecay",
            "tagSpallationNeutron",
            "tagOther",
            })
{
    //for toy MC
    m_toyInputFile=NULL;
    m_toyInputTree=NULL;
    i_toyEvts=0;
    i_toyEvtsMax=0;
    f_E=0;f_T=0;f_x=0;f_y=0;f_z=0;

    b_toyMCOpt=false;
    declProp("toyMCOpt",b_toyMCOpt=false);
    b_loop=false;
    declProp("loopOpt",b_loop=false);
}

OECAlg::~OECAlg(){
}

bool OECAlg::initialize(){
    LogDebug<<"toyMCOpt:"<<b_toyMCOpt<<endl;
    SniperDataPtr<JM::NavBuffer> navbuf(getParent(), "/Event");
    if (navbuf.invalid()){
        LogError<<"can not get navbuffer!!!"<<endl;
        return false;
    }
    m_buf=navbuf.data();

    //m_reviser=new InputReviser(*getRoot(),"default",b_loop);


    //for toy MC
    if (b_toyMCOpt){
        SniperPtr<EvtStoreSvc>  stsvc(getParent(), "EvtStoreSvc");
        if ( stsvc.invalid() ) {
            LogError << "cannot get the EvtStoreSvc." << std::endl;
            return false;
        }
        m_storeSvc= stsvc.data();
        m_toyInputFile=new TFile("/junofs/users/huangx/huangxinihep/oec-prototype/OECAlg/share/fackOECInput.root","READ");
        m_toyInputTree=(TTree*)m_toyInputFile->Get("OECInput");
        i_toyEvtsMax=m_toyInputTree->GetEntries();
        m_toyInputTree->SetBranchAddress("E",&f_E);
        m_toyInputTree->SetBranchAddress("T",&f_T);
        m_toyInputTree->SetBranchAddress("x",&f_x);
        m_toyInputTree->SetBranchAddress("y",&f_y);
        m_toyInputTree->SetBranchAddress("z",&f_z);
        i_toyEvts=0;

        //for single tag result
        SniperPtr<EvtResultSvc> singleResultSvc(getParent(),"EvtResultSvc");
        if (singleResultSvc.invalid()){
            LogError<<"Fail to get EvtResultSvc!"<<std::endl;
            return false;
        }
        m_singleResultSvc=singleResultSvc.data();

        return true;
    }
#define MASK(TE, PE, DI, TI) ((TE<<24) | (PE<<16) | (DI<<8) | (TI<<0)), (TE), (PE), (DI), (TI)
    LogInfo<<"Initializing tables..."<<endl;
    tables.push_back({MASK(tEltLow,pEAny,distAny,timeAny),tagOther});
    tables.push_back({MASK(tEltnHLow,pEltBiPoHigh,distlt,timeAny),tagBiPo});
    tables.push_back({MASK(tEltPDLow,pEltBiPoHigh,distlt,timeAny),tagIBD});
    tables.push_back({MASK(tEltPDLow,pEltIBDHigh,distlt,timeAny),tagIBD});
    tables.push_back({MASK(tEltnHHigh,pEltBiPoHigh,distlt,timeAny),tagIBD});
    tables.push_back({MASK(tEltnHHigh,pEltIBDHigh,distlt,timeAny),tagIBD});
    tables.push_back({MASK(tEltnHHigh,pEltPDHigh,distAny,timeltPDcut),tagProtonDecay});
    tables.push_back({MASK(tEltPDHigh,pEltPDHigh,distAny,timeltPDcut),tagProtonDecay});
    tables.push_back({MASK(tEltPDLow,pEltPDHigh,distAny,timeAny),tagSpallationNeutron});
    tables.push_back({MASK(tEltPDLow,pEgtPDHigh,distAny,timeAny),tagSpallationNeutron});
    tables.push_back({MASK(tEltnHHigh,pEltPDHigh,distAny,timeAny),tagSpallationNeutron});
    tables.push_back({MASK(tEltnHHigh,pEgtPDHigh,distAny,timeAny),tagSpallationNeutron});
    tables.push_back({MASK(tEAny,pEAny,distAny,timeAny),tagOther});
    
    for (vector<TagConditionPair>::iterator it=tables.begin();it!=tables.end();it++){
        LogInfo<<"==>"<<TagName[(*it).tag]<<endl;
    }
    LogInfo<<"Initializing tables done."<<endl;

    return true;
}

bool OECAlg::finalize(){
    LogInfo<<"finalizing OECAlg..."<<endl;

    //for toy MC
    if (b_toyMCOpt){
        if (m_toyInputFile!=NULL) m_toyInputFile->Close();
        return true;
    }

    return true;
}

bool OECAlg::execute(){
    //JM::EvtNavigator* nav=m_buf->curEvt();

    //for toy MC
    if (b_toyMCOpt){
        JM::OECEvent *thisEvent;
        CriteriaItemValue *p_itemValue;
        if (i_toyEvts>=i_toyEvtsMax){//execeed events in input file, store a fake OECEvent
            thisEvent=new JM::OECEvent;
            thisEvent->setEnergy(-10);
            thisEvent->setTime(0);
            thisEvent->setVertexX(0);
            thisEvent->setVertexY(0);
            thisEvent->setVertexZ(0);
            thisEvent->addsTag(0);
            LogInfo<<m_singleResultSvc->getMark()<<endl;
            m_currentTimeStamp=TTimeStamp(0,0);
            if (!registerData(thisEvent)){
                return false;
            }
            i_toyEvts++;
        }
        else{
            m_toyInputTree->GetEntry(i_toyEvts);
            thisEvent=new JM::OECEvent();

            m_storeSvc->get("energy",p_itemValue);
            p_itemValue->getValue(f_E);
            thisEvent->setEnergy(f_E);

            m_storeSvc->get("time",p_itemValue);
            p_itemValue->getValue(f_T);
            thisEvent->setTime(f_T);

            m_storeSvc->get("vertexX",p_itemValue);
            p_itemValue->getValue(f_x);
            thisEvent->setVertexX(f_x);

            m_storeSvc->get("vertexY",p_itemValue);
            p_itemValue->getValue(f_y);
            thisEvent->setVertexY(f_y);

            m_storeSvc->get("vertexZ",p_itemValue);
            p_itemValue->getValue(f_z);
            thisEvent->setVertexZ(f_z);

            thisEvent->addsTag(m_singleResultSvc->getMark());
            LogInfo<<"energy:"<<f_E
                <<"   time:"<<f_T
                <<"   vertexX:"<<f_x
                <<"   vertexY:"<<f_y
                <<"   vertexZ:"<<f_z
                <<"   tag:"<<m_singleResultSvc->getMark()
                <<endl;
            int i_time=(int)f_T;
            int i_second=i_time/1000000000;//get second
            int i_nanosecond=i_time%1000000000;//get nanosecond
            m_currentTimeStamp=TTimeStamp(i_second,i_nanosecond);
            if (!registerData(thisEvent)){
                return false;
            }
            i_toyEvts++;
        }
        //if (i_toyEvts>=i_toyEvtsMax){//execeed events in input file, store a fake OECEvent
        //    i_toyEvts=0;
        //}
        //m_toyInputTree->GetEntry(i_toyEvts);
        //thisEvent=new JM::OECEvent();
        //thisEvent->setEnergy(f_E);
        //thisEvent->setTime(f_T);
        //thisEvent->setVertexX(f_x);
        //thisEvent->setVertexY(f_y);
        //thisEvent->setVertexZ(f_z);
        //m_currentTimeStamp=TTimeStamp();
        //if (!registerData(thisEvent)){
        //    return false;
        //}
        //i_toyEvts++;
        return true;
    }

    //event selection

    //m_reviser->fire(*getRoot());
    //SniperDataPtr<JM::NavBuffer> navbuf(dynamic_cast<Task*>(getRoot()->find("default")), "/Event");
    //if (navbuf.invalid()){
    //    LogError<<"can not get navbuffer!!!"<<endl;
    //    return false;
    //}
    //m_buf=navbuf.data();
    
    //get this event
    JM::EvtNavigator* tnav=m_buf->curEvt();
    JM::OECHeader *tEventHeader=(JM::OECHeader*)tnav->getHeader("/Event/OEC");
    JM::OECEvent *tEvent=(JM::OECEvent*)tEventHeader->event("JM::OECEvent");
    LogDebug<<"the current event time is:"<<tEvent->getTime()<<endl;

    //get prompt event
    vector<JM::OECEvent*> v_pEvents;
    DataBuffer<JM::EvtNavigator>::Iterator tit=m_buf->current();
    DataBuffer<JM::EvtNavigator>::Iterator rbeginit=m_buf->begin();
    //for test
    if (tit==rbeginit){
        LogInfo<<"01 proceeding events found!"<<endl;
        return true;
    }
    tit--;
    rbeginit--;
    for (DataBuffer<JM::EvtNavigator>::Iterator it=tit;it!=rbeginit;it--){
        JM::EvtNavigator *pnav=(*(it)).get();
        JM::OECHeader *pEventHeader=(JM::OECHeader*)pnav->getHeader("/Event/OEC");
        JM::OECEvent *pEvent=(JM::OECEvent*)pEventHeader->event("JM::OECEvent");
        if (tEvent->getTime()-pEvent->getTime()<1500000){
            v_pEvents.push_back(pEvent);
        }
        else break;
    }
    LogInfo<<v_pEvents.size()<<" proceeding events found!"<<endl;
    //JM::EvtNavigator *pnav=(*(m_buf->current()--)).get();
    //JM::OECHeader *pEventHeader=(JM::OECHeader*)pnav->getHeader("/Event/OEC");
    //JM::OECEvent *pEvent=(JM::OECEvent*)pEventHeader->event("JM::OECEvent");

    // the quick selection flow: 
    // 1) tE (MeV)
    // 2) pE (MeV)
    // 3) dist (mm)
    // 4) time (ns)
    for (vector<JM::OECEvent*>::iterator it=v_pEvents.begin();it!=v_pEvents.end();it++){
        double tE = tEvent->getEnergy();
        double pE = (*it)->getEnergy();
        double dx=tEvent->getVertexX()-(*it)->getVertexX();
        double dy=tEvent->getVertexY()-(*it)->getVertexY();
        double dz=tEvent->getVertexZ()-(*it)->getVertexZ();
        double dist=TMath::Sqrt(dx*dx+dy*dy+dz*dz);
        double time = tEvent->getTime()-(*it)->getTime(); // 10 us
        Tag tag=get_tag(tE,pE,dist,time);
    }

    return true;
}

OECAlg::Tag OECAlg::get_tag(double tE,double pE,double dist,double time){
    tECut _tECut=get_tECut(tE);
    pECut _pECut=get_pECut(pE);
    distCut _distCut=get_distCut(dist);
    timeCut _timeCut=get_timeCut(time);

    Tag tag=tagOther;
    LogDebug << " tE: " << tE
              << " pE: " << pE
              << " dist: " << dist
              << " deltaT: " << time
              << std::endl;
    LogDebug << " tECut: " << _tECut
              << " pECut: " << _pECut
              << " distCut: " << _distCut
              << " timeCut: " << _timeCut
              << std::endl;

    for (vector<TagConditionPair>::iterator it=tables.begin();it!=tables.end();it++){
        if ((*it)._tECut!=tEAny && _tECut!=(*it)._tECut){
            continue;
        }
        if ((*it)._pECut!=pEAny && _pECut!=(*it)._pECut){
            continue;
        }
        if ((*it)._distCut!=distAny && _distCut!=(*it)._distCut){
            continue;
        }
        if ((*it)._timeCut!=timeAny && _timeCut!=(*it)._timeCut){
            continue;
        }

        tag=(*it).tag;
        break;
    }

    LogInfo<< " found Tag: " << TagName[tag] << std::endl;
    return tag;
}

OECAlg::tECut OECAlg::get_tECut(double tE){
    tECut _tECut=tEAny;
    for (int i=0;i<(int)tENum;i++){
        if (tE<tECutRange[i].low) continue;
        if (tECutRange[i].high!=0 && tE>tECutRange[i].high){
            continue;
        }
        _tECut=tECut(i);
        break;
    }
    return _tECut;
}

OECAlg::pECut OECAlg::get_pECut(double pE){
    pECut _pECut=pEAny;
    for (int i=0;i<(int)pENum;i++){
        if (pE<pECutRange[i].low) continue;
        if (pECutRange[i].high!=0 && pE>pECutRange[i].high){
            continue;
        }
        _pECut=pECut(i);
        break;
    }
    return _pECut;
}

OECAlg::distCut OECAlg::get_distCut(double dist){
    distCut _distCut=distAny;
    for (int i=0;i<(int)distNum;i++){
        if (dist<distCutRange[i].low) continue;
        if (distCutRange[i].high!=0 && dist>distCutRange[i].high){
            continue;
        }
        _distCut=distCut(i);
        break;
    }
    return _distCut;
}

OECAlg::timeCut OECAlg::get_timeCut(double time){
    timeCut _timeCut=timeAny;
    for (int i=0;i<(int)timeNum;i++){
        if (time<timeCutRange[i].low) continue;
        if (timeCutRange[i].high!=0 && time>timeCutRange[i].high){
            continue;
        }
        _timeCut=timeCut(i);
        break;
    }
    return _timeCut;
}

bool OECAlg::registerData(JM::OECEvent *thisevent){
    JM::EvtNavigator* nav = new JM::EvtNavigator();
    LogDebug << "time stamp: '" << m_currentTimeStamp << "'." << std::endl;
    nav->setTimeStamp(m_currentTimeStamp);

    SniperPtr<IDataMemMgr> mMgr(*getParent(), "BufferMemMgr");
    mMgr->adopt(nav, "/Event");

    JM::OECHeader* thisHeader = new JM::OECHeader();
    //JM::OECEvent* theEvent = new JM::OECEvent;
    //theEvent->setEvent(thisevent);

    thisHeader->setEvent(thisevent);
    nav->addHeader("/Event/OEC", thisHeader);
    return true;
}
