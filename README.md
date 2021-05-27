# oec-prototype  
Online Event Classification prototype  
### source env  
source /cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc447/Release/J16v1r1/setup.sh  
### you should CMT the code in the order of what follows:  
*1. OECEvent  
*2. EvtUtilities/EvtDataTypes, EvtUtilities/EvtStore, EvtUtilities/EvtAlgorithms  
*3. EvtProcessor/EvtSteerData, EvtProcessor/EvtResult, EvtProcessor/EvtConfig, EvtProcessor/EvtSteering  
*4. SimpleRecAlg  
*5. RecCdMuonAlg  
*6. OECAlg  
*7. cmt  
### How to run:  
source cmt/setup.sh  

<<<<<<< HEAD   SimpleRec  
use --input to give input cabli.root  
use --rate to give the rate of sample  
use --evtmax to give the maximum number of events  

a sample of how to use:
cd oec-prototype/OECAlg  
python share/otherrun.py --evtmax 10 --input share/sample_calib.root --rate 1  
use --inputlist to give input cabli.root  
use --rate to give the rate of sample  
use --evtmax to give the maximum number of events  

python oec-prototype/run.py --inputlist oec-prototype/RecCdMuonAlg/share/calib_2000002.root --rate 1  


## Updata OECBuf  
add the timestamp of OECEvent  
add the subtask  
----buffer manager for calibEvent  
----give how to fire the subtask  
----give the event rate
add the calib and OECEvent to toptask buffer. 6.4  


## Updata SimpleRecAlg  and RecCdMuonAlg  
----revise the method of put CirterialItemValue into storeSvc. 6.4  
###Updata SimpleRecAlg  
revise  
----change read calib way from subtask to topTask. 6.4   
----correct the way to get PMT Position. 6.4  

###Updata RecCdMuonAlg  
add the muon recAlg  
----give the cluster for get mult-mu  
----change the seq.xml and sig.xml  
----default the Event is muon  
revise  
----correct the Alg for passing parameters. 6.4  


##Update run.py  
----correct the subtask name RecCdMuonAlg to subTask_RecCdMuonAlg. 6.4  

##conclusion  
1.OEC can run complete operation. 6.4  

