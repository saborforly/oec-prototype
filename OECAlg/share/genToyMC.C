//root script to generate OEC toy MC input
using namespace std;

void genToyMC(){
    int i_evtmax=100;
    string s_evtType="IBD";
    float f_currentTime=0;
    float f_frequency;//Hz
    float f_deltaTInterval;//ns

    TFile *m_toySpecFile[3];
    TH1D *m_toySpec[3];

    if (!s_evtType.compare("IBD")){
        cout<<"IBD"<<endl;
        f_frequency=10;
        f_deltaTInterval=200000;//200us
        //get input spectrum
        m_toySpecFile[0]=new TFile("/junofs/users/huangx/huangxinihep/oec-prototype/OECAlg/share/background.root","READ");
        m_toySpec[0]=(TH1D*)m_toySpecFile[0]->Get("TrueEventHistogramAD1");//unit of energy is MeV
    }
    else if (!s_evtType.compare("muon")){
        cout<<"muon"<<endl;
        f_frequency=4;
        f_deltaTInterval=1e9/4.;//ns
        //get input spectrum
        m_toySpecFile[1]=new TFile("/junofs/users/huangx/huangxinihep/oec-prototype/OECAlg/share/muE.root","READ");
        m_toySpec[1]=(TH1D*)m_toySpecFile[1]->Get("h");//unit of energy is GeV
    }

    struct myEvent{
        float energy;
        float time;
        float x;
        float y;
        float z;
    };
    list<myEvent> eventList;
    ////generate data and sort
    TRandom m_myrandom;
    for (int i=0;i<i_evtmax;i++){
        //generate pEvent and tEvent
        float pE=-1;
        float tE=-1;
        if (!s_evtType.compare("muon")){
            pE=1000*m_toySpec[1]->GetRandom();
            tE=1000*m_toySpec[1]->GetRandom();
        }
        else if (!s_evtType.compare("IBD")){
            pE=m_toySpec[0]->GetRandom();
            tE=2.2;//m_myrandom.Gaus(2.2,TMath::Sqrt(2.2)*0.03);
        }
        float tT=f_currentTime+m_myrandom.Exp(1e9/f_frequency);
        float pT=tT-m_myrandom.Exp(f_deltaTInterval);

        //float pE=m_myrandom.Uniform(100,1000);//MeV
        //float tE=m_myrandom.Uniform(2,100);//m_myrandom.Gaus(2.2,TMath::Sqrt(2.2)*0.03);
        //float tT=f_currentTime+4000000;//m_myrandom.Exp(1./f_frequency)*1e9;
        //float pT=tT-5000;//m_myrandom.Exp(f_deltaTInterval);
        cout<<" pE:"<<pE
            <<" tE:"<<tE
            <<" pT:"<<pT
            <<" tT:"<<tT<<endl;
        myEvent pEvent;
        pEvent.energy=pE;
        pEvent.time=pT;
        pEvent.x=0;
        pEvent.y=0;
        pEvent.z=0;
        myEvent tEvent;
        tEvent.energy=tE;
        tEvent.time=tT;
        tEvent.x=0;
        tEvent.y=0;
        tEvent.z=0;

        //store in the list, pEvent first
        if (eventList.empty()){
            eventList.push_back(pEvent);
            eventList.push_back(tEvent);
        }
        else{
            for (list<myEvent>::reverse_iterator rit=eventList.rbegin();rit!=eventList.rend();rit++){
                if ((*rit).time<pT){
                    eventList.insert(rit.base(),pEvent);
                    break;
                }
                if (rit==(--eventList.rend())){
                    eventList.insert(eventList.begin(),pEvent);
                    break;
                }
            }//insert pEvent
            for (list<myEvent>::reverse_iterator rit=eventList.rbegin();rit!=eventList.rend();rit++){
                if ((*rit).time<tT){
                    eventList.insert(rit.base(),tEvent);
                    break;
                }
                if (rit==(--eventList.rend())){
                    eventList.insert(eventList.begin(),tEvent);
                    break;
                }
            }//insert tEvent
        }
        f_currentTime=tT;
    }
    ////output
    TFile *outputFile=new TFile("/junofs/users/huangx/huangxinihep/oec-prototype/OECAlg/share/fackOECInput.root","RECREATE");
    TTree *outputTree=new TTree("OECInput","the input for OEC");
    float f_E=0;float f_T=0;float f_x=0;float f_y=0;float f_z=0;
    outputTree->Branch("E",&f_E,"E/F");
    outputTree->Branch("T",&f_T,"T/F");
    outputTree->Branch("x",&f_x,"x/F");
    outputTree->Branch("y",&f_y,"y/F");
    outputTree->Branch("z",&f_z,"z/F");
    for (list<myEvent>::iterator it=eventList.begin();it!=eventList.end();it++){
        f_E=(*it).energy;
        f_T=(*it).time;
        f_x=(*it).x;
        f_y=(*it).y;
        f_z=(*it).z;
        //cout<<f_E<<"  "<<f_T<<"  "<<f_x<<"  "<<f_y<<"  "<<f_z<<endl;
        outputTree->Fill();
    }
    outputTree->Write();
    outputFile->Close();
}
