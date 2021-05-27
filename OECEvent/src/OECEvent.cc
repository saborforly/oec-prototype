#include "Event/OECEvent.h"

ClassImp(JM::OECEvent);
namespace JM{
    OECEvent::OECEvent(){
        m_time=TTimeStamp(0,0);
    }

    OECEvent::~OECEvent(){
    }

    string OECEvent::getIDA(){
        return "CD";
    }

    bool OECEvent::addsTag(string tag){
        i_tag=0;
        //int i_temptag=0;
        //if (!tag.compare("singles_LE")){
        //    temptag=0x1040;
        //}
        //else if (!tag.compare("singles_ME")){
        //    temptag=0x1040;
        //}
        //else if (!tag.compare("singles_HE")){
        //    temptag=0x1040;
        //}
        //else if (!tag.compare("singles_XE")){
        //    temptag=0x1040;
        //}
        //else if (!tag.compare("muWP")){
        //    temptag=0x2010;
        //}
        //else if (!tag.compare("muTT")){
        //    temptag=0x4010;
        //}
        //else if (!tag.compare("muShower")){
        //    temptag=0x1010;
        //}
        //else if (!tag.compare("muShrTrack")){
        //    temptag=0x1010;
        //}
        //else if (!tag.compare("muStop")){
        //    temptag=0x1010;
        //}
        //else if (!tag.compare("muStpTrack")){
        //    temptag=0x1010;
        //}
        //else if (!tag.compare("muSglTrack")){
        //    temptag=0x1010;
        //}
        //else if (!tag.compare("muDblTrack")){
        //    temptag=0x1010;
        //}
        //else if (!tag.compare("muNoTrack")){
        //    temptag=0x1010;
        //}
        //else {
        //    LogError<<"no tag named:"<<tag<<endl;
        //    return false;
        //}
        if (!strcmp(tag.c_str(),"singles_LE")) i_tag=1;
        else if (!strcmp(tag.c_str(),"singles_ME")) i_tag=2;
        else if (!strcmp(tag.c_str(),"singles_HE")) i_tag=3;
        else if (!strcmp(tag.c_str(),"singles_XE")) i_tag=4;
        return true;
    }
}
