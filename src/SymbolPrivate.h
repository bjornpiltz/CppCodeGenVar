#pragma once 
#include <symengine/symbol.h>

namespace codegenvar{

namespace internal
{

typedef SymEngine::RCP<const SymEngine::Basic> SymExpr;

class SymbolPrivate
{
public:
    SymExpr expression; 
    
    static Symbol ctor(SymExpr ptr)
    {
        Symbol result;
        result.p->expression = ptr;
        return result;
    }
};

}// namespace internal

}// namespace codegenvar