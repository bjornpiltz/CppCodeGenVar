#pragma once
#include "Symbol.h"

namespace codegenvar {

struct CodeGenerator
{
    struct Options
    {
        std::string varName = "out";
        std::string varDeclaration = "const double ";
        std::string tempPrefix = "tmp_";
    }options;
    
    std::string operator()(const Symbol&)const;
};

}// namespace codegenvar
