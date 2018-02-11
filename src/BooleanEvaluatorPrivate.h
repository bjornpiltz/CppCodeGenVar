#pragma once
#include <codegenvar/BooleanEvaluator.h>
#include "Error.h"
#include <symengine/logic.h>

namespace codegenvar {

namespace internal {
    
using namespace SymEngine;

enum Bool{True=1, False=0};

struct ConditionalBranch
{
    ConditionalBranch(const RCP<const Boolean>& condition, Bool value)
        : condition(condition)
        , value(value)
    {
    }
    RCP<const Boolean> condition;
    Bool value;
};

/*
 * {
 *   if(x<y)
 *     return 0.;
 *   if(x>0)
 *     return x;
 *  return -x;
 * }
 * 
 *   condition       
 *   +--------+  branch[True]
 *   |   x<y  | -------------->  0.0  
 *   +--------+    
 *        | branch[False]
 *        V
 *   condition         
 *   +--------+  branch[True]  
 *   |   x>0  | -------------->  x 
 *   +--------+    
 *        | branch[False]
 *        V
 *       -x   
 */
struct ConditionalTree
{
    typedef std::vector<ConditionalBranch> Iterator;

    struct Child{
        std::unique_ptr<ConditionalTree> node;
        RCP<const Basic> leaf;
        bool isFullyEvaluated()const
        {
            if (!leaf.is_null())
                return true;
            if (node)
                return node->isFullyEvaluated();
            return false;
        }
    }branch[2];
    
    bool isFullyEvaluated()const;
    bool isEvaluated(const Iterator& address)const;
    void insert(const Iterator& address, RCP<const Basic> expr);
};

struct BooleanEvaluatorPrivate
{
    enum Op { LT, EQ, GT };

    static bool handle(const SymbolPrivate* lhs, const SymbolPrivate* rhs, Op op);
    bool handle(RCP<const Basic> lhs, RCP<const Basic> rhs, Op op);

    ConditionalTree evaluated;
    ConditionalTree::Iterator currentContext;
    
    RCP<const Boolean> getCurrentContext()const;
};

}

}