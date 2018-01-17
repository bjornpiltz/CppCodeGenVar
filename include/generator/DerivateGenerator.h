#pragma once
#include "Symbol.h"

namespace codegenvar{

struct DerivateEvaluator
{
    struct Options
    {
        std::set<std::string> constants;
    }options;

    std::map<std::string, Symbol> operator()(const Symbol&)const;
};

}// namespace codegenvar
