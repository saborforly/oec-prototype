//the root script to generate OEC toy MC input
using namespace std;

struct myEvent{
    float energy;//MeV
    int time_second;
    int time_nanosecond;
    float x;
    float y;
    float z;
    int isPDS;//0:prompt  1:delayed  2:single
};

//define the function to insert an event
bool insertEvent(list<myEvent>& thelist, myEvent &theevent){
    if (thelist.empty()) thelist.push_back(theevent);
    else {
        for (list<myEvent>::reverse_iterator rit=thelist.rbegin();rit!=thelist.rend();rit++){
            if (rit==(--thelist.rend())){
                thelist.insert(thelist.begin(),theevent);
                break;
            }
            if ((*it).time_second>theevent.time_second) continue;
            if ((*rit).time_nanosecond<theevent.time_nanosecond){
                thelist.insert(rit.base(),theevent);
                break;
            }
        }//insert theevent 
    }
    return true;
}

void bkgenToyMC(){
    int i_evtmax=100;
    string s_evtType="IBD";
    int f_currentTime_second=0;
    int f_currentTime_nanosecond=0;
    float f_frequency[2];//Hz
    float f_pdInterval[2];//ms
    TFile *m_toySpecFile[2];
    TH1D *m_toySpec[2];

    //set constant value
    f_frequency[0]=0.01;//IBD
    f_frequency[1]=3;//muon
    f_pdInterval[0]=200000;//200us
    f_pdInterval[1]=1e9/f_frequency[1];
    m_toySpecFile[0]=new TFile("/junofs/users/huangx/huangxinihep/oec-prototype/OECAlg/share/background.root","READ");
    m_toySpec[0]=(TH1D*)m_toySpecFile[0]->Get("TrueEventHistogramAD1");//unit of energy is MeV
    m_toySpecFile[1]=new TFile("/junofs/users/huangx/huangxinihep/oec-prototype/OECAlg/share/muE.root","READ");
    m_toySpec[1]=(TH1D*)m_toySpecFile[1]->Get("h");//unit of energy is GeV

    list<myEvent> eventList;
    TRandom m_myrandom(5);


    if (!s_evtType.compare("IBD")){
        cout<<"event type:IBD"<<endl;
        float pE=-1;
        float tE=-1;
        myEvent theEvent;
        theEvent.x=0;
        theEvent.y=0;
        theEvent.z=0;
        for (int i=0;i<i_evtmax;i++){
            pE=m_toySpec[0]->GetRandom();
            tE=2.2;//m_myrandom.Gaus(2.2,TMath::Sqrt(2.2)*0.03);
            double tT=m_myrandom.Exp(1e9/f_frequency[0]);
            int tT_second=int(tT/1e9);
            int tT_nanosecond=int(tT-tT_second*1e9);
            tT_second+=f_currentTime_second;
            tT_second+=f_currentTime_nanosecond;
            double deltaT=m_myrandom.Exp(f_pdInterval[0]);
            int deltaT_second=int(deltaT/1e9);
            int deltaT_nanosecond=int(deltaT-deltaT_second*1e9);
            int pT_second=tT_second-deltaT_second;
            int pT_nanosecond=tT_nanosecond-deltaT_nanosecond;
            theEvent.energy=pE;
            theEvent.time_second=pT_second;
            theEvent.time_nanosecond=pT_nanosecond;
            theEvent.isPDS=0;
            insertEvent(eventList,theEvent);//insert prompt event
            theEvent.energy=tE;
            theEvent.time_second=tT_second;
            theEvent.time_nanosecond=tT_nanosecond;
            theEvent.isPDS=1;
            insertEvent(eventList,theEvent);//insert delayed event
            f_currentTime_second=tT_second;
            f_currentTime_nanosecond=tT_nanosecond;
        }
    }
    else if (!s_evtType.compare("muon")){
        cout<<"event type:muon"<<endl;
        float tE=-1;
        myEvent theEvent;
        theEvent.x=0;
        theEvent.y=0;
        theEvent.z=0;
        theEvent.isPDS=2;
        for (int i=0;i<i_evtmax;i++){
            tE=1000*m_toySpec[1]->GetRandom();
            double tT=m_myrandom.Exp(1e9/f_frequency[1]);
            int tT_second=int(tT/1e9);
            int tT_nanosecond=int(tT-tT_second*1e9);
            tT_second+=f_currentTime_second;
            tT_second+=f_currentTime_nanosecond;
            theEvent.energy=tE;
            theEvent.time_second=tT_second;
            theEvent.time_nanosecond=tT_nanosecond;
            insertEvent(eventList,theEvent);//insert prompt event
            f_currentTime_second=tT_second;
            f_currentTime_nanosecond=tT_nanosecond;
        }
    }
    else if (!s_evtType.compare("mixed")){
        cout<<"event type:mixed"<<endl;
        int i_totevt=0;
        myEvent theEvent;
        theEvent.x=0;
        theEvent.y=0;
        theEvent.z=0;
        for (;i_totevt<i_evtmax;){
            if (m_myrandom.Uniform(0,f_frequency[0]+f_frequency[1])<f_frequency[0]){//IBD event
                cout<<"IBD"<<endl;
                float pE=-1;
                float tE=-1;
                pE=m_toySpec[0]->GetRandom();
                tE=2.2;//m_myrandom.Gaus(2.2,TMath::Sqrt(2.2)*0.03);
                float tT=f_currentTime+m_myrandom.Exp(1e9/f_frequency[0]);
                float pT=tT-m_myrandom.Exp(f_pdInterval[0]);
                theEvent.energy=pE;
                theEvent.time=pT;
                theEvent.isPDS=0;
                insertEvent(eventList,theEvent);//insert prompt event
                i_totevt++;
                theEvent.energy=tE;
                theEvent.time=tT;
                theEvent.isPDS=1;
                insertEvent(eventList,theEvent);//insert delayed event
                f_currentTime=tT;
                i_totevt++;
            }
            else {//muon event
                cout<<"muon"<<endl;
                float tE=-1;
                theEvent.isPDS=2;
                tE=1000*m_toySpec[1]->GetRandom();
                float tT=f_currentTime+m_myrandom.Exp(1e9/f_frequency[1]);
                theEvent.energy=tE;
                theEvent.time=tT;
                insertEvent(eventList,theEvent);//insert prompt event
                f_currentTime=tT;
                i_totevt++;
            }
        }
    }
    else {
        cout<<"event type error!"<<endl;
        return;
    }

    ////output
    TFile *outputFile=new TFile("/junofs/users/huangx/huangxinihep/oec-prototype/OECAlg/share/fackOECInput.root","RECREATE");
    TTree *outputTree=new TTree("OECInput","the input for OEC");
    float f_E=0;float f_x=0;float f_y=0;float f_z=0;int i_isPDS=0;
    int i_T_second=0;int i_T_nanosecond=0;
    outputTree->Branch("E",&f_E,"E/F");
    outputTree->Branch("T_second",&i_T_second,"T_second/I");
    outputTree->Branch("T_nanosecond",&i_T_nanosecond,"T_nanosecond/I");
    outputTree->Branch("x",&f_x,"x/F");
    outputTree->Branch("y",&f_y,"y/F");
    outputTree->Branch("z",&f_z,"z/F");
    outputTree->Branch("isPDS",&i_isPDS,"isPDS/I");
    for (list<myEvent>::iterator it=eventList.begin();it!=eventList.end();it++){
        f_E=(*it).energy;
        i_T_second=(*it).time_second;
        i_T_nanosecond=(*it).time_nanosecond;
        f_x=(*it).x;
        f_y=(*it).y;
        f_z=(*it).z;
        i_isPDS=(*it).isPDS;
        cout<<" energy:"<<f_E
            <<" time:"<<i_T_second<<"s"<<i_T_nanosecond<<"ns"
            <<" isPDS:"<<i_isPDS
            <<endl;
        outputTree->Fill();
    }
    outputTree->Write();
    outputFile->Close();
}
