//the OEC realize
#include <iostream>
using namespace std;




struct cSelection{
    bool initialize(){
        // dump the table

        std::cout << "Initializing the tables. " << std::endl;
        for (TagConditionPair* p = tables; p->selection != 0xFFFFFFFF; ++p) {
            std::cout //<< "0x" << std::setfill('0') << std::setw(8) << std::hex << p->selection
                      << " => "
                      << TagName[p->tag]
                      << std::endl;
        }
        std::cout << "Initialize the tables done. " << std::endl;

        return true;
    }

    bool execute(){
        // the quick selection flow: 
        // 1) tE (MeV)
        // 2) pE (MeV)
        // 3) dist (mmm)
        // 4) time (ns)
        double tE;
        double pE;
        double dist;
        double time;

        ////for test
        //int testEventNum=100000;
        //int tagNum=0;
        //TRandom myrandom;
        //for (int i=0;i<testEventNum;i++){
        //    tE=myrandom.Uniform(0.75,1.6);
        //    pE=myrandom.Uniform(0.75,4);
        //    dist=500;
        //    time=myrandom.Uniform(0,10000);
        //    Tag tag=get_tag(tE,pE,dist,time);
        //    if (tag==tagBiPo) tagNum++;
        //}
        //cout<<tagNum<<endl;

        ////for test:IBD
        //TRandom *myrandom=new TRandom();
        //TFile *IBDinput=new TFile("/junofs/users/huangx/OEC/Others/EventRate/signals/background.root","READ");
        //TH1D *IBDSpec=(TH1D*)IBDinput->Get("TrueEventHistogramAD1");
        //int testEventNum=100000;
        //int IBDtagNum=0;
        //for (int i=0;i<testEventNum;i++){
        //    tE=myrandom->Gaus(2.2,0.03*TMath::Sqrt(2.2));
        //    pE=myrandom->Gaus(IBDSpec->GetRandom(),0.03*TMath::Sqrt(IBDSpec->GetRandom()));
        //    dist=1000;
        //    time=myrandom->Exp(200000);
        //    Tag tag=tagOther;
        //    if (time>=1500000){
        //        continue;
        //    }
        //    tag = get_tag(tE, pE, dist, time);
        //    if (tag==tagIBD){
        //        IBDtagNum++;
        //    }
        //}
        //cout<<"total IBD:"<<IBDtagNum<<endl;
        
        //for test:radioactivity
        int IBDNum=0;int OtherNum=0;int BiPoNum=0;int unknownNum=0;
        TFile *radioInputFile=new TFile("/junofs/users/huangx/OEC/Others/EventRate/signals/radioSpec.root","READ");
        TH2D *totalSingle=(TH2D*)radioInputFile->Get("totalSingle");
        for (int i=0;i<totalSingle->GetNbinsX();i++){
            totalSingle->SetBinContent(i+1,1,0);
        }
        int i_totalTagNum=100000000;
        TRandom myrandom;
        for (int i=0;i<i_totalTagNum;i++){
            double tR;double pR;
            totalSingle->GetRandom2(tR,tE);
            totalSingle->GetRandom2(pR,pE);
            tR=1000*TMath::Power(tR,1/3.);
            pR=1000*TMath::Power(pR,1/3.);
            double costheta=myrandom.Uniform(-1,1);
            //cout<<"pR:"<<pR<<"  "<<"tR:"<<tR<<"  "<<"costheta:"<<costheta<<endl;
            dist=TMath::Sqrt(tR*tR+pR*pR-2*tR*pR*costheta);
            time=myrandom.Uniform(0,1500000);
            Tag tag=get_tag(tE,pE,dist,time);
            if (tag==tagIBD) IBDNum++;
            else if (tag==tagOther) OtherNum++;
            else if (tag==tagBiPo) BiPoNum++;
            else unknownNum++;
        }
        cout<<"IBD:"<<IBDNum<<endl<<"Other:"<<OtherNum<<endl<<"BiPo:"<<BiPoNum<<endl<<"unknow:"<<unknownNum<<endl;
        cout<<"total:"<<i_totalTagNum<<endl;

        return true;
    }

    bool finalize(){
        return true;
    }

    Tag get_tag(double tE, double pE, double dist, double time){
        tECut _tECut=get_tECut(tE);
        pECut _pECut=get_pECut(pE);
        distCut _distCut=get_distCut(dist);
        timeCut _timeCut=get_timeCut(time);

        Tag tag=tagOther;
        
        //std::cout << " tE: " << tE
        //          << " pE: " << pE
        //          << " dist: " << dist
        //          << " time: " << time
        //          << std::endl;
        //std::cout << " tECut: " << _tECut
        //          << " pECut: " << _pECut
        //          << " distCut: " << _distCut
        //          << " timeCut: " << _timeCut
        //          << std::endl;

        for (TagConditionPair* p = tables; p->selection != 0xFFFFFFFF; ++p) {
            if (p->_tECut != tEAny && _tECut != p->_tECut) {
                continue;
            }

            if (p->_pECut != pEAny && _pECut != p->_pECut) {
                continue;
            }

            if (p->_distCut != distAny && _distCut != p->_distCut) {
                continue;
            }

            if (p->_timeCut != timeAny && _timeCut != p->_timeCut) {
                continue;
            }

            // found the tag
            tag = p->tag;

            break;

        }

        //std::cout << " found Tag: " << TagName[tag] << std::endl;

        return tag;
    }
        
    tECut get_tECut(double tE){
        tECut _tECut=tEAny;
        for (int i=0;i<tENum;i++){
            if (tE<tECutRange[i].low) continue;
            if (tECutRange[i].high!=0 && tE>tECutRange[i].high){
                continue;
            }
            _tECut=tECut(i);
            break;
        }
        return _tECut;
    }
    
    pECut get_pECut(double pE){
        pECut _pECut=pEAny;
        for (int i=0;i<pENum;i++){
            if (pE<pECutRange[i].low) continue;
            if (pECutRange[i].high!=0 && pE>pECutRange[i].high){
                continue;
            }
            _pECut=pECut(i);
            break;
        }
        return _pECut;
    }
        
    distCut get_distCut(double dist){
        distCut _distCut=distAny;
        for (int i=0;i<distNum;i++){
            if (dist<distCutRange[i].low) continue;
            if (distCutRange[i].high!=0 && dist>distCutRange[i].high){
                continue;
            }
            _distCut=distCut(i);
            break;
        }
        return _distCut;
    }
        
    timeCut get_timeCut(double time){
        timeCut _timeCut=timeAny;
        for (int i=0;i<timeNum;i++){
            if (time<timeCutRange[i].low) continue;
            if (timeCutRange[i].high!=0 && time>timeCutRange[i].high){
                continue;
            }
            _timeCut=timeCut(i);
            break;
        }
        return _timeCut;
    }
};

int correctedSelection(){
    cSelection selAlg;

    selAlg.initialize();
    selAlg.execute();
    selAlg.finalize();
    return 0;
}
