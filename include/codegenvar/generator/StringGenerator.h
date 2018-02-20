#pragma once
#include <codegenvar/Symbol.h>

namespace codegenvar {

struct StringGenerator
{
    std::string operator()(const Symbol&)const;
};

}// namespace codegenvar
