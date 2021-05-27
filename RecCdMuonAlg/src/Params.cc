#include "RecCdMuonAlg/Params.h"
#include "SniperKernel/SniperLog.h"

Params::Params()
{
}

Params::~Params()
{
}

    void 
Params::set(std::string key, double value)
{
    m_params[key] = value; 
}

double 
Params::get(std::string key) const
{
    std::map<std::string, double>::const_iterator search = m_params.find(key); 
    if(search == m_params.end())
    {
        LogError << "No value named [" << key << "] in params!!" << std::endl; 
        return 0;
    }
    return search->second; 
}
