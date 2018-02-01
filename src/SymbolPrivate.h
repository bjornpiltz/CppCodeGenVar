#pragma once
#include <codegenvar/Symbol.h>
#include "Error.h"

#if defined(_MSC_VER)
# pragma warning( push )  
# pragma warning( disable : 4244 )  
#endif

#include <symengine/symbol.h>

#if defined(_MSC_VER)
# pragma warning( pop ) 
#endif

namespace codegenvar {

namespace internal {

class SymbolPrivate;
struct BooleanEvaluatorPrivate;

typedef SymEngine::RCP<const SymEngine::Basic> SymExpr;

class SymbolPrivate
{
public:
    SymExpr expression; 
    std::weak_ptr<internal::BooleanEvaluatorPrivate> booleanEvaluator;
    
    static Symbol ctor(SymExpr ptr)
    {
        Symbol result;
        result.p->expression = ptr;
        return result;
    }
};

}// namespace internal

}// namespace codegenvar