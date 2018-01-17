#pragma once
#include "codegenvar/Symbol.h"

namespace codegenvar{

struct NumericalEvaluator
{
    double operator()(const Symbol&)const;
};

}// namespace codegenvar