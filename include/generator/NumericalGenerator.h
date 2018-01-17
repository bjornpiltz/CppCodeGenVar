#pragma once
#include "Symbol.h"

namespace codegenvar{

struct NumericalEvaluator
{
    double operator()(const Symbol&)const;
    
private:
    //double evaluate(internal::ConstPtr)const;
};

}// namespace codegenvar