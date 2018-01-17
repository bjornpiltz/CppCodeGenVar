#pragma once
#include "Symbol.h"

namespace codegenvar{

struct NumericalEvaluator
{
    double operator()(const Symbol&)const;
};

}// namespace codegenvar