#ifndef Params_h
#define Params_h
#include <map>
#include <string>
class Params
{
public:
    Params ();
    virtual ~Params ();

    void set(std::string key, double value); 
    double get(std::string key) const; 

private:
    std::map<std::string, double> m_params; 
};
#endif
