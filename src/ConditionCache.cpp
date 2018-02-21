#include "ConditionCache.h"
#include <symengine/add.h>
#include <symengine/mul.h>
#include <symengine/integer.h>
#include <bitset>

#define DEBUGGING_CONDITIONCACHE(a)

namespace codegenvar {

namespace internal {

using namespace SymEngine;

static inline int mask(int op, bool on)
{
    switch (op)
    {
    default:
    case LT: return (on ? LT : GE);
    case EQ: return (on ? EQ : NE);
    case LE: return (on ? LE : GT);
    case GT: return (on ? GT : LE);
    case NE: return (on ? NE : EQ);
    case GE: return (on ? GE : LT);
    }
}

static inline std::string toString(int mask)
{
    switch (mask)
    {
    default:
    case Invalid: return "?";
    case Unknown: return "?";
    case LT: return "<";
    case EQ: return "==";
    case LE: return "<=";
    case GT: return ">";
    case NE: return "!=";
    case GE: return ">=";
    }
}

void ConditionCache::insert(MathExpression lhs, MathExpression rhs, int op, bool on)
{
    const auto key = expand(sub(lhs, rhs));
    if (allowedStates.count(key)==0)
        allowedStates[key] = Unknown;
    allowedStates[key] &= mask(op, on);
    DEBUGGING_CONDITIONCACHE(std::cerr << "Setting " << key->__str__() << " " << toString(mask(op, on)) << " 0"<< std::endl);
}

bool ConditionCache::alreadyEvaluated(MathExpression lhs, MathExpression rhs, CompoundBooleanOp op, bool& value)const
{
    bool retval, tmp;
    switch (op)
    {
    default:
    case LE: retval = alreadyEvaluated(lhs, rhs, GT, tmp); break; // a<=b -> !(a >b)
    case NE: retval = alreadyEvaluated(lhs, rhs, EQ, tmp); break; // a!=b -> !(a==b)
    case GE: retval = alreadyEvaluated(lhs, rhs, LT, tmp); break; // a>=b -> !(a <b)
    }
    if(retval)
        value = !tmp;
    return retval;
}

bool ConditionCache::alreadyEvaluated(MathExpression lhs, MathExpression rhs, BooleanOp op, bool& value)const 
{
    const auto key = expand(sub(lhs, rhs));
    DEBUGGING_CONDITIONCACHE(std::cerr << "Checking " << key->__str__() << " " << toString(op) << " 0");
    for (auto pair : allowedStates)
    {
        int allowedState = 0;
        if (isMathematicallyEquivalent(pair.first, key))
            allowedState = pair.second;
        else if (isMathematicallyEquivalent(pair.first, neg(key)))
            allowedState = pair.second ^ (LT|GT);
        else
            continue;

        value = allowedState&op;
        DEBUGGING_CONDITIONCACHE(std::cerr << " : true" << std::endl);
        return !(value && std::bitset<3>(allowedState).count()>1);
    }
    DEBUGGING_CONDITIONCACHE(std::cerr << " : false" << std::endl);
    return false;
}

bool ConditionCache::isMathematicallyEquivalent(MathExpression lhs, MathExpression rhs)
{
    // a == b iff a-b == 0
    return eq(*expand(sub(lhs, rhs)), *integer(0));
}

} // namespace internal

} // namespace codegenvar
