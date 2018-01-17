#pragma once
#include "codegenvar/Symbol.h"

namespace codegenvar {

struct CodeGenerator
{
    CodeGenerator()=default;
    CodeGenerator(std::string varName);
    struct Options
    {
        std::string varName = "out";
        std::string varDeclaration = "const double ";
        std::string tempPrefix = "tmp_";
    }options;
    
    std::string operator()(const Symbol&)const;
};

}// namespace codegenvar
