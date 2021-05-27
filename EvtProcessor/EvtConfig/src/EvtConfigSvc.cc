#include <boost/python.hpp>
#include "EvtConfig/EvtConfigSvc.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperLog.h"
#include "xercesc/framework/XMLFormatter.hpp"
#include "xercesc/util/PlatformUtils.hpp"
#include "xercesc/util/XMLString.hpp"
#include "xercesc/util/XMLUniDefs.hpp"
#include "xercesc/util/TranscodingException.hpp"
#include "xercesc/parsers/XercesDOMParser.hpp"
//#include "Sequence.h"
//#include "Signature.h"
#include <xercesc/dom/DOM.hpp>
DECLARE_SERVICE(EvtConfigSvc);

EvtConfigSvc::EvtConfigSvc(const std::string& name)
  :SvcBase(name),
    m_Signature_DocNode(NULL), m_Sequence_DocNode(NULL),
    m_SignatureVec(0),m_SequenceVec(0),m_TableVector(0),
    m_idSequence(0),m_idSignature(0),m_idCriteria(0),m_idItem(0),
    m_first(true)
{
    m_count = 0;
    declProp("seqListFile", m_seqListFile = "myseqlist.xml");
    declProp("sigListFile", m_sigListFile = "mysiglist.xml");
    //declProp("tableVector",m_TableVector);
    

}
EvtConfigSvc::~EvtConfigSvc()
{
}

bool
EvtConfigSvc::initialize()
{   
    
   
    LogDebug << " initialized start" << std::endl;
    
    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch) {
        exit(1);
    }
    //DOM解析器实例化 
    XercesDOMParser* signatureParser = new XercesDOMParser;
    signatureParser->setValidationScheme(XercesDOMParser::Val_Always);    // optional.
    signatureParser->setDoNamespaces(true);   // optional
    LogDebug<<"Seq File Path: "<<m_seqListFile<<std::endl; 
    XercesDOMParser* sequenceParser = new XercesDOMParser;
    sequenceParser->setValidationScheme(XercesDOMParser::Val_Always);    // optional.
    sequenceParser->setDoNamespaces(true);   // optional
    //加载xml文件
    try {
        signatureParser->parse(m_sigListFile.c_str());
        sequenceParser->parse(m_seqListFile.c_str());
    }
    catch (const XMLException& toCatch) { 
        exit(1);
    }
    catch (const DOMException& toCatch) {
        XMLPlatformUtils::Terminate();
        exit(1);
    }
    catch (...) {
        XMLPlatformUtils::Terminate();
        exit(1);
    }

    m_Sequence_DocNode  = sequenceParser->getDocument();
    m_Signature_DocNode = signatureParser->getDocument();
    if(m_first){
        m_first=false;
    }
    else{
        for (unsigned int i = 0; i < m_SignatureVec.size(); i++) delete m_SignatureVec[i];
        for (unsigned int j = 0; j < m_SequenceVec.size(); j++) delete m_SequenceVec[j];
        m_SignatureVec.clear();
        m_SequenceVec.clear();
        m_TableVector.clear();
        m_idCriteria=0;
        m_idSignature=0;
        m_idSequence=0;
        m_idItem=0;
    }

    buildSequenceList(m_Sequence_DocNode, true);
    buildSignatureList(m_Signature_DocNode, true);
    delete sequenceParser;
    delete signatureParser;
    //pair<SeqVec,SigVec>
    makeTableVector(); 
    LogDebug << " initialized successfully" << std::endl;
    return true;
}
bool
EvtConfigSvc::execute()
{
    ++m_count;
    LogDebug << "execute: count: " << m_count << std::endl;
    return true;
}
bool
EvtConfigSvc::finalize()
{
    XMLPlatformUtils::Terminate();
    LogDebug << " finalized successfully" << std::endl;
    return true;
}


void EvtConfigSvc::buildSequenceList(const DOMDocument* xmlDoc, bool first)
{
    if ( first ) {
        first=false;

    }
#define XMLCF XMLString::compareString
#define XMLTF XMLString::transcode
    DOMElement* elementRoot = xmlDoc->getDocumentElement();
    DOMNodeList*      children = elementRoot->getChildNodes();
    const  XMLSize_t nodeCount = children->getLength();
    std::string outString, inputString, seq_idString, algoString;
    for( XMLSize_t xx = 0; xx < nodeCount; ++xx )
    {
        DOMNode* currentNode = children->item(xx);
        if( currentNode->getNodeType() &&  // true is not NULL
             currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
        {
            LogDebug<<XMLString::transcode(currentNode->getNodeName())<<std::endl; 
            //解析SEQUENCE中属性值
            if(XMLCF(XMLTF(currentNode->getNodeName()),"SEQUENCE")==0) {
                DOMNamedNodeMap *attributes = currentNode->getAttributes();
                int attributesCount = attributes->getLength();
                for(int i = 0; i < attributesCount; i++) {
	            if(XMLCF(XMLTF(attributes->item(i)->getNodeName()),"sequence_id")==0)
	                seq_idString = XMLTF(attributes->item(i)->getNodeValue());
                    LogDebug<<seq_idString<<std::endl;
                    if(XMLCF(XMLTF(attributes->item(i)->getNodeName()),"input")==0)
	                inputString = XMLTF(attributes->item(i)->getNodeValue());
	            if(XMLCF(XMLTF(attributes->item(i)->getNodeName()),"output")==0)
	                outString = XMLTF(attributes->item(i)->getNodeValue());
                }
                Sequence* m_helpSequence = new Sequence();
                m_helpSequence->setSeqID(seq_idString);
                m_helpSequence->setInputID(inputString);
                m_helpSequence->setOutputID(outString);
                m_SequenceVec.push_back(m_helpSequence);
            }
            DOMNodeList * info = currentNode->getChildNodes();
            int AlgoNum=0;
            for(XMLSize_t yy = 0; yy<info->getLength();yy++)
            {
                DOMNode *info_currentNode = info->item(yy);
                if( info_currentNode->getNodeType() && info_currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
                {
                    //解析每个SEQUENCE下的所有Node
                    if (XMLCF(XMLTF(info_currentNode->getNodeName()),"ESALGORITHM") == 0) {
                        DOMNamedNodeMap *attributes = info_currentNode->getAttributes();
                        int attributesCount = attributes->getLength();
                        for (int i = 0; i < attributesCount; i++) {
	                    if(XMLCF(XMLTF(attributes->item(i)->getNodeName()),"algorithm")==0)
	                      algoString = XMLTF(attributes->item(i)->getNodeValue());
                            LogDebug<<algoString<<std::endl;
                        }
                        AlgoNum++;
                        m_SequenceVec.back()->makeVectors(algoString);
                    }
                }
            }
            m_SequenceVec.back()->setAlgoNum(AlgoNum);
        }
    }
    for (std::vector<Sequence*>::iterator i=m_SequenceVec.begin();i!=m_SequenceVec.end(); ++i) {
        (*i)->print();
    }
}
void EvtConfigSvc::buildSignatureList(const DOMDocument* xmlDoc, bool first)
{
    if ( first ) {
        first=false;

    }
#define XMLCF XMLString::compareString
#define XMLTF XMLString::transcode
    DOMElement* elementRoot = xmlDoc->getDocumentElement();
    DOMNodeList*      children = elementRoot->getChildNodes();
    const  XMLSize_t nodeCount = children->getLength();
    std::string helpString, helpPrescale, behaviour, itemName, itemType;
    int prescale=1;
    float forcedAccept=1, itemValue=0;
    //SIGNATURE循环
    for( XMLSize_t xx = 0; xx < nodeCount; ++xx )
    {   
        static std::string current_sigID = "";
        DOMNode* currentNode = children->item(xx);
        if( currentNode->getNodeType() &&  // true is not NULL
             currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
        {
            LogDebug<<XMLString::transcode(currentNode->getNodeName())<<std::endl; 
            //SIGNATURE
            if(XMLCF(XMLTF(currentNode->getNodeName()),"SIGNATURE")==0) {
                DOMNamedNodeMap *attributes = currentNode->getAttributes();
                int attributesCount = attributes->getLength();
                for(int i = 0; i < attributesCount; i++) {
	            if(XMLCF(XMLTF(attributes->item(i)->getNodeName()),"signature_id")==0){
	                helpString = XMLTF(attributes->item(i)->getNodeValue());
                        LogDebug<<"signature_id: "<<helpString<<std::endl;   
                    }
                    current_sigID = helpString;
                    if(XMLCF(XMLTF(attributes->item(i)->getNodeName()),"prescale")==0){
                        helpPrescale = XMLTF(attributes->item(i)->getNodeValue());
                        prescale = atoi(helpPrescale.c_str());
                    }
                    if(XMLCF(XMLTF(attributes->item(i)->getNodeName()),"forced_accept")==0){
                        std::string helpString=XMLTF(attributes->item(i)->getNodeValue());
                        double returnValue(0);
                        std::istringstream is(helpString);
                        is >> returnValue;
                        forcedAccept = returnValue;
                    }
                }
                //Sig input Vector
                Signature* m_helpSignature = new Signature();
                m_helpSignature->setSigLabel(helpString);
                m_helpSignature->setPrescale(prescale);
                m_helpSignature->setForcedAccept(forcedAccept);
                m_SignatureVec.push_back(m_helpSignature);
            }
            DOMNodeList * info = currentNode->getChildNodes();
            //CRITERIA循环
            for(XMLSize_t yy = 0; yy<info->getLength();yy++)
            {   
                DOMNode *info_currentNode = info->item(yy);
                if( info_currentNode->getNodeType() && info_currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
                {
                    //CRITERIA
                    static EvtCriteria* current_EvtCriteria = NULL;
                    if (XMLCF(XMLTF(info_currentNode->getNodeName()),"CRITERIA") == 0) {
                        EvtCriteria *m_EvtCriteria = new EvtCriteria(m_idCriteria);
                        DOMNamedNodeMap *attributes = info_currentNode->getAttributes();
                        int attributesCount = attributes->getLength();
                        for (int i = 0; i < attributesCount; i++) {
                            if(XMLCF(XMLTF(attributes->item(i)->getNodeName()),"behaviour")==0) {
	                        behaviour = XMLTF(attributes->item(i)->getNodeValue());
	                        if (behaviour == "reject") behaviour += "_reject";
                                LogDebug<<"behaviour: "<<behaviour<<std::endl;
	                    }
	                    if(XMLCF(XMLTF(attributes->item(i)->getNodeName()),"next")==0) {
	                        if (behaviour != "jump") {
                                    LogDebug << "Unconsistent of behaviour and next" << std::endl;
	    
	                            exit(1);
	                        }
	                        std::string nextID = XMLTF(attributes->item(i)->getNodeValue());
                                 
	                        for (unsigned int i = 0; i < m_SequenceVec.size(); i++) {
            
	                            if (m_SequenceVec[i]->getSeqID() == current_sigID) {
	                                while (++i < m_SequenceVec.size()) {
		                            if (m_SequenceVec[i]->getSeqID() == nextID) break;
	                                }
	                                if (i == m_SequenceVec.size()) {
                                            LogDebug<<"i: "<<i<<" size: "<<m_SequenceVec.size()<<std::endl;
                                            std::cout << "Undefined jump target: " << nextID << std::endl;
		                            exit(1);
	                                }
	                                break;
	                            }
	                        }
                                
	                        behaviour = behaviour +'_'+ nextID;
	                    }
	                    if(XMLCF(XMLTF(attributes->item(i)->getNodeName()),"mark")==0) {
	                        if ((behaviour!= "accept")&&(behaviour!= "continue")) {
                                    LogDebug << "Unconsistent of behaviour and mark" << std::endl;
	    
	                            exit(1);
	                           }
	                       behaviour = behaviour +'_'+ XMLTF(attributes->item(i)->getNodeValue());
	                   }
                       }
                       if (behaviour.find('_') == std::string::npos) {
                           LogDebug << "Unvalid behaviour ==> " << behaviour << std::endl;
	
                           exit(1);
                       }
                       //EvtCriteria input Vector
                       m_EvtCriteria->setBehaviour(behaviour);
                       m_EvtCriteria->setScope(getParent()); 
                       //每个sig下的criteria
                       m_SignatureVec.back()->makeVectors(m_EvtCriteria);
                       m_idCriteria++;
                       current_EvtCriteria = m_EvtCriteria;
                    }
                    DOMNodeList * info_info = info_currentNode->getChildNodes();
                    //Item 循环
                    for(XMLSize_t yy = 0; yy<info_info->getLength();yy++)
                    {
                        DOMNode *info_info_currentNode = info_info->item(yy);
                        if( info_info_currentNode->getNodeType() && info_info_currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
                        {
                            //ITEM
                            if (XMLCF(XMLTF(info_info_currentNode->getNodeName()),"ITEM") == 0) {
                                DOMNamedNodeMap *itemAttr = info_info_currentNode->getAttributes();
                                if (itemAttr->getLength() != 3) {
	                            exit(1);
      			        }
                                for (int i = 0; i < 3; i++) {
	                           if(XMLCF(XMLTF(itemAttr->item(i)->getNodeName()), "name") == 0){
	                               itemName = XMLTF(itemAttr->item(i)->getNodeValue());
                                       LogDebug << "item name: "<<itemName << std::endl;
                                   }
	                           if(XMLCF(XMLTF(itemAttr->item(i)->getNodeName()), "value") == 0){
	                               std::string helpString=XMLTF(itemAttr->item(i)->getNodeValue());
                                       double returnValue(0);
                                       std::istringstream is(helpString);
                                       is >> returnValue;
                                       itemValue=returnValue;
                                   }
	                           if(XMLCF(XMLTF(itemAttr->item(i)->getNodeName()), "type") == 0)
	                               itemType = XMLTF(itemAttr->item(i)->getNodeValue());
                               }
                               current_EvtCriteria->addItem(itemName, itemValue, itemType,m_idItem);
                               m_idItem++;
                            }  
                        }
                    }
               }
           }
       }
    }
    for (std::vector<Signature*>::iterator i=m_SignatureVec.begin();i!=m_SignatureVec.end(); ++i) {
        (*i)->print();
    }
}
void EvtConfigSvc::makeTableVector()
{
  for (unsigned int i = 0; i < m_SequenceVec.size(); i++) {
    std::string seqId = m_SequenceVec[i]->getSeqID();
    bool matched = false;
    for (unsigned int j = 0; j < m_SignatureVec.size(); j++) {
      if (m_SignatureVec[j]->label() == m_SequenceVec[i]->getSeqID()) {
	m_TableVector.push_back(std::make_pair(m_SignatureVec[j], m_SequenceVec[i]));
        LogDebug<<"Pair(Sig: "<< m_SignatureVec[j]->label()<<"--Seq: "<<m_SequenceVec[i]->getSeqID()<<")"<<std::endl;
	matched = true;
	break;
      }
    }
    if (!matched) {
      LogDebug<< "No matched Signature to Sequence(" << seqId << ")" << std::endl;
      exit(1);
    }
  }
}
const std::vector<std::pair<Signature*, Sequence*> >& EvtConfigSvc::getTablesVector() const
{
  return m_TableVector;
}
