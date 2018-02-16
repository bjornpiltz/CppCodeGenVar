#include "ConditionCache.h"
#include <symengine/add.h>
#include <symengine/mul.h>
#include <symengine/integer.h>
#include <bitset>

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

static inline bool isMathematicallyEquivalent(RCP<const Basic> lhs, RCP<const Basic> rhs)
{
    return eq(*expand(sub(lhs, rhs)), *integer(0));
}

void ConditionCache::insert(MathExpression lhs, MathExpression rhs, BooleanOp op, bool on)
{
    const auto key = sub(lhs, rhs);
    if (allowedStates.count(key)==0)
        allowedStates[key] = Unknown;
    allowedStates[key] &= mask(op, on);
}

void ConditionCache::insert(MathExpression lhs, MathExpression rhs, CompoundBooleanOp op, bool on)
{
    const auto key = sub(lhs, rhs);
    if (allowedStates.count(key)==0)
        allowedStates[key] = Unknown;
    allowedStates[key] &= mask(op, on);
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
    const auto key = sub(lhs, rhs);
    for (auto pair : allowedStates)
    {
        if (isMathematicallyEquivalent(pair.first, key))
        {
            int allowedState = pair.second;
            value = allowedState&op;
            if(value && std::bitset<3>(allowedState).count()>1)
                return false;
            return true; 
        }
        else if (isMathematicallyEquivalent(pair.first, neg(key)))
        {
            int allowedState = pair.second ^ (LT|GT);
            value = allowedState&op;
            if(value && std::bitset<3>(allowedState).count()>1)
                return false;
            return true; 
        }
    }
    return false;
}

} // namespace internal

} // namespace codegenvar
