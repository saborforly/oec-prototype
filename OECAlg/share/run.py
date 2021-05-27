#!/usr/bin/env python
#-*- coding: utf-8 -*-

import Sniper

def get_parser():
    import argparse

    parser=argparse.ArgumentParser(description='run RecQT')
    parser.add_argument("--loglevel",default="Info",choices=["Test","Debug","Info","Warn","Error","Fatal"],help="set the log level")
    parser.add_argument("--evtmax",type=int,default=1,help="events to be processed")
    parser.add_argument("--input",default="",help="input file name")
    parser.add_argument("--input-list",default=None,help="the file which contains input file names")

    parser.add_argument("--toyMC",dest="toyMC",action="store_true",help="generate toy MC data for OEC")
    parser.add_argument("--disabletoyMC",dest="toyMC",action="store_false",help="disable toy MC")
    parser.set_defaults(toyMC=False)

    parser.add_argument("--enableloop",dest="loop",action="store_true",help="enable loop on input file")
    parser.add_argument("--disableloop",dest="loop",action="store_false",help="disable loop in input file")
    parser.set_defaults(loop=False)
    
    return parser

DATA_LOG_MAP = {"Test":0, "Debug":2, "Info":3, "Warn":4, "Error":5, "Fatal":6}

if __name__ == "__main__":
    parser=get_parser()
    args = parser.parse_args()

    task=Sniper.Task("OECtask")
    task.setEvtMax(args.evtmax)
    task.setLogLevel(DATA_LOG_MAP[args.loglevel])
    
    #import DataRegistritionSvc
    #task.createSvc("DataRegistritionSvc")
    
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    bufMgr.property("TimeWindow").set([-100, 100])
    
    import RootIOSvc
    inputsvc=task.createSvc("RootInputSvc/InputSvc")
    inputfiles=[]
    if args.input_list:
        import sys
        import os.path
        if not os.path.exists(args.input_list):
            sys.exit(-1)
        with open(args.input_list) as f:
            for line in f:
                line=line.strip()
                inputfiles.append(line)
    else:
        inputfiles.append(args.input)
    inputsvc.property("InputFile").set(inputfiles)

    if args.toyMC:
        outputsvc=task.createSvc("RootOutputSvc/OutputSvc")
        outputsvc.property("OutputStreams").set({"/Event/OEC":"toyMCdata.root"})

        import EvtConfig
        #from EvtConfig import EvtConfigSvc
        hltConf = task.createSvc("EvtConfigSvc")
        #hltConf.property("seqListFile").set("/junofs/users/liuyan/OEC/EventFilter/EvtProcessor/EvtConfig/share/seq.xml")
        #hltConf.property("sigListFile").set("/junofs/users/liuyan/OEC/EventFilter/EvtProcessor/EvtConfig/share/sig.xml")    
        hltConf.property("seqListFile").set("/junofs/users/huangx/huangxinihep/oec-prototype/seq.xml")
        hltConf.property("sigListFile").set("/junofs/users/huangx/huangxinihep/oec-prototype/sig.xml")    
        
        import EvtStore
        task.property("svcs").append("EvtStoreSvc")

        import EvtResult
        task.property("svcs").append("EvtResultSvc")

        #for fast reconstruction
        import Geometry
        geosvc=task.createSvc("RecGeomSvc")
        geosvc.property("GeomFile").set("/cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J20v1r0-Pre1/offline/Examples/Tutorial/share/sample_detsim.root")
        geosvc.property("GeomPathInRoot").set("JunoGeom")
        geosvc.property("FastInit").set(True)
        print("geo successful")
        import JunoTimer
        task.createSvc("JunoTimerSvc")
        import SimpleRecAlg

        import EvtSteering
        import EvtAlgorithms
        handleralg=task.createAlg("StepHandler")
        #task.property("algs").append("StepHandler")

    import OECAlg
    oecalg=task.createAlg("OECAlg")
    oecalg.property("toyMCOpt").set(args.toyMC)
    oecalg.property("loopOpt").set(args.loop)
    
    #recqtsvc.createTool("PDFTool")
    
    task.run()



