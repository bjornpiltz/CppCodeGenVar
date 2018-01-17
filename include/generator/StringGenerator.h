#pragma once
#include "Symbol.h"

namespace codegenvar {

struct StringEvaluator
{
    std::string operator()(const Symbol&)const;
    
private:
    //std::string evaluate(internal::ConstPtr)const;
    //std::string evaluateAndAddBraces(internal::ConstPtr, int type)const;
};

}// namespace codegenvar
