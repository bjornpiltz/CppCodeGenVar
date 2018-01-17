#pragma once
#include "codegenvar/Symbol.h"

namespace codegenvar {

struct StringEvaluator
{
    std::string operator()(const Symbol&)const;
};

}// namespace codegenvar
