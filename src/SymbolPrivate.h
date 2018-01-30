#pragma once 

#if defined(_MSC_VER)
# pragma warning( push )  
# pragma warning( disable : 4244 )  
#endif

#include <symengine/symbol.h>

#if defined(_MSC_VER)
# pragma warning( pop ) 
#endif

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