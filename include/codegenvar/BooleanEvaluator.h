#pragma once
#include <codegenvar/Symbol.h>
#include <memory>

namespace codegenvar {

namespace internal {class BooleanEvaluatorPrivate;}

struct BooleanEvaluator
{
    BooleanEvaluator();
    Symbol getSymbol(const std::string name)const;
    bool done(const Symbol&)const;
    
private:
    std::shared_ptr<internal::BooleanEvaluatorPrivate> p;
};

} // namespace codegenvar
