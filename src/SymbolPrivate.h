#pragma once
#include <codegenvar/Symbol.h>
#include "Error.h"

#if defined(_MSC_VER)
# pragma warning( push )  
# pragma warning( disable : 4244 )  
#endif

#include <symengine/symbol.h>
#include <symengine/logic.h>

#if defined(_MSC_VER)
# pragma warning( pop ) 
#endif

namespace codegenvar {

namespace internal {

class SymbolPrivate;

typedef SymEngine::RCP<const SymEngine::Basic> SymExpr;
typedef SymEngine::RCP<const SymEngine::Boolean> Condition;

class SymbolPrivate
{
public:
    SymExpr expression; 
    
    static std::unique_ptr<SymbolPrivate> ctor(SymExpr ptr)
    {
        auto p = std::unique_ptr<SymbolPrivate>(new internal::SymbolPrivate);
        p->expression = ptr;
        return p;
    }
};

}// namespace internal

}// namespace codegenvar