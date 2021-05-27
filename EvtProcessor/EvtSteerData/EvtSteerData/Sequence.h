#ifndef SEQUENCE_H
#define SEQUENCE_H
#include <utility>
#include <map>
#include <string>
#include <vector>

    
class Sequence {

  public:
    Sequence();
    virtual ~Sequence();
    std::vector<std::string>& algoVector();

    int algoNum() const;
    int inputNum() const;
    void setInputNum(int inputNum);
    void setAlgoNum(int algoNum);
    void setSeqID(std::string seqID) {
      m_sequenceID = seqID;
    }
    const std::string& getSeqID() const {
      return m_sequenceID;
    }
    void setInputID(std::string inputID) {
      m_inputID = inputID;
    }
    const std::string& getInputID() const {
      return m_inputID;
    }
    void setOutputID(std::string outputID) {
      m_outputID = outputID;
    }
    const std::string& getOutputID() const {
      return m_outputID;
    }
    void makeVectors(std::string algoString);
    void print();
    
  private:
    std::vector<std::string> m_sequenceAlgoVector ;
    std::string m_sequenceID;
    std::string m_inputID;
    std::string m_outputID;
    int m_sequenceInputNum;
    int m_sequenceAlgoNum;
    
  };
 

#endif
