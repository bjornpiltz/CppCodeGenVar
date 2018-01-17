#pragma once
#include "Symbol.h"
#include <string>
#include <set>
#include <map>

namespace codegenvar{

struct DerivateEvaluator
{
    void addConstants(const std::set<std::string>&);
    std::map<std::string, Symbol> operator()(const Symbol&);
    
private:
    std::set<std::string> constants;
};

}// namespace codegenvar
