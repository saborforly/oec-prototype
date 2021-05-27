#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: liuyan

# using the new Mgr.

import Sniper
def get_parser():
    import argparse

    parser=argparse.ArgumentParser(description='run RecQT')
    parser.add_argument("--loglevel",default="Info",choices=["Test","Debug","Info","Warn","Error","Fatal"],help="set the log level")
    parser.add_argument("--evtmax",type=int,default=1,help="events to be processed")
    parser.add_argument("--inputdir",default="",help="input file name")
    parser.add_argument("--inputlist", action='append', help="the file which contains input file names")
    parser.add_argument("--rate", action="append", help="rate for TAG: Calib:1.0")
    parser.add_argument("--toyMC",dest="toyMC",action="store_true",help="generate toy MC data for OEC")
    parser.add_argument("--disabletoyMC",dest="toyMC",action="store_false",help="disable toy MC")
    parser.set_defaults(toyMC=False)

    parser.add_argument("--selectEvent", default='False', action='store_true', help='run select Event or not')   

 
    parser.add_argument("--enableloop",dest="loop",action="store_true",help="enable loop on input file")
    parser.add_argument("--disableloop",dest="loop",action="store_false",help="disable loop in input file")
    
    parser.add_argument("--seq",default="/junofs/users/liuyan/OEC/oec-prototype/seq.xml",help="input seq.xml")
    parser.add_argument("--sig",default="/junofs/users/liuyan/OEC/oec-prototype/sig.xml",help="input sig.xml")
    parser.set_defaults(loop=False)
    
    return parser

DATA_LOG_MAP = {"Test":0, "Debug":2, "Info":3, "Warn":4, "Error":5, "Fatal":6}
import RootIOSvc
import BufferMemMgr

if __name__ == "__main__":
    parser=get_parser()
    args = parser.parse_args()
    
    task_top = Sniper.TopTask("OECTask")
    task_top.setEvtMax(args.evtmax)
    task_top.setLogLevel(0)
    #subTask buffer calibEvent
    if args.rate is None:
        args.rate = [ "1.0" ]

    filelist=[]
    if args.inputdir:
        import os
        files =os.listdir(args.inputdir)
        for file_ in files:
            if not  os.path.isdir(args.inputdir+file_):
                f_name = args.inputdir+str(file_)
                fsize = os.path.getsize(f_name)
                fsize = fsize/float(1024*1024)
                if fsize>70:
                    filelist.append(f_name)
    if args.inputlist:
        for infile in args.inputlist:
            filelist.append(str(infile)) 
    print(filelist)
    subTask={}
    readin={}
    subTaskId = 0
    rateMap={}
    #Input File in subTask
    for infile in filelist:
    #if 1==1:
        print(args.inputlist)
        subTask[subTaskId] = task_top.createTask("Task/subTask_"+str(subTaskId))    #taskList[2]: sample2
        subTask[subTaskId].setLogLevel(0)
        bufMgrTmp=subTask[subTaskId].createSvc("BufferMemMgr")
        bufMgrTmp.property("TimeWindow").set([-100000000, 100000000])


        readin[subTaskId]=subTask[subTaskId].createSvc("RootInputSvc/InputSvc")
        readin[subTaskId].property("InputFile").set(infile)

        rateMap["subTask_"+str(subTaskId)] = float(args.rate[subTaskId])

        subTaskId+=1

    #buffer OECEvent
    import BufferMemMgr
    bufMgr = task_top.createSvc("BufferMemMgr")
    bufMgr.property("TimeWindow").set([-100, 100]);


    #read xml 
    import EvtConfig
    evtConf = task_top.createSvc("EvtConfigSvc")
    evtConf.property("seqListFile").set(args.seq)
    evtConf.property("sigListFile").set(args.sig)    
    
    #store energy and time for tag
    import EvtStore
    task_top.property("svcs").append("EvtStoreSvc")

    #store result
    import EvtResult
    task_top.property("svcs").append("EvtResultSvc")

    #for fast reconstruction
    import Geometry
    geosvc=task_top.createSvc("RecGeomSvc")
    geosvc.property("GeomFile").set("/cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J20v1r0-Pre1/offline/Examples/Tutorial/share/sample_detsim.root")
    geosvc.property("GeomPathInRoot").set("JunoGeom")
    geosvc.property("FastInit").set(True)
    import JunoTimer
    task_top.createSvc("JunoTimerSvc")


    #init OECEvent
    import OECBuf
    print(rateMap)
    
    bufalg=task_top.createAlg("OECBufAlg")
    bufalg.property("RateMap").set(rateMap)
    #run Event Filter
    import EvtSteering
    #alg will input OECEvent value
    import EvtAlgorithms

    
    import SimpleRecAlg
    
    import RecCdMuonAlg 
    #subTask RecCdMuonAlg
    subTask_RecMuon = task_top.createTask("Task/subTask_RecCdMuonAlg")
    import SpmtMuonRecTool
    recMuonAlg = RecCdMuonAlg.createAlg(subTask_RecMuon)
    recMuonAlg.property("Use3inchPMT").set(True)
    recMuonAlg.useRecTool("SpmtMuonRecTool")

    

    task_top.property("algs").append("StepHandler")
    
    #For test get Result
    #task.property("algs").append("ExampleGetResult")
    task_top.show()
    task_top.run()


