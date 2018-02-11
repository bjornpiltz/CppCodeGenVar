#pragma once
#include <codegenvar/Symbol.h>
#include <memory>

namespace codegenvar {
    
namespace internal {struct BooleanEvaluatorPrivate;}

struct BooleanEvaluator
{
    BooleanEvaluator();
    bool isFullyEvaluated()const;
    
private:
    friend struct internal::BooleanEvaluatorPrivate;
    friend class Symbol;
    static std::shared_ptr<internal::BooleanEvaluatorPrivate> p;
    static std::weak_ptr<internal::BooleanEvaluatorPrivate> get();
};

} // namespace codegenvar
