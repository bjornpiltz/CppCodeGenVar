#pragma once
#include <symengine/basic.h>

namespace codegenvar {

namespace internal {

enum BooleanOp { LT=1, EQ=2, GT=4 };
enum CompoundBooleanOp { Invalid=0, LE=LT|EQ, GE=EQ|GT, NE=LT|GT, Unknown=7 };

/*
 * A ConditionCache keeps track of mathematical expressions wich have already been 
 * assigned values. For example:

    auto a = symbol("a"), b = symbol("b");

    ConditionCache cache;

    std::cerr << "Assert that a <= b." << std::endl;
    cache.insert(a, b, LE, true); 

    bool b1;
    if (cache.alreadyEvaluated(a, b, GT, b1))
        std:: cerr << "We know that a " << (b1 ? "is" : "is not") << " larger than b." << std::endl;
    else
        std:: cerr << "We don't know if b is larger than a." << std::endl;
        
    bool b2;
    if (cache.alreadyEvaluated(a, b, EQ, b2))
        std::cerr << "We know that a " << (b2 ? "equals" : "doesn't equal") << " b." << std::endl;
    else
        std::cerr << "We don't know if a equals b." << std::endl;

    * outputs:
    *   Assert that a <= b.
    *   We know that a is not larger than b.
    *   We don't know if a equals b.
 */
class ConditionCache
{
    typedef SymEngine::RCP<const SymEngine::Basic> MathExpression;
public:
    void insert(MathExpression lhs, MathExpression rhs, int op, bool on);

    bool alreadyEvaluated(MathExpression lhs, MathExpression rhs, BooleanOp op, bool& value)const;
    bool alreadyEvaluated(MathExpression lhs, MathExpression rhs, CompoundBooleanOp op, bool& value)const;
    
    static bool isMathematicallyEquivalent(MathExpression lhs, MathExpression rhs);
private:
    std::map<MathExpression, int, SymEngine::RCPBasicKeyLess> allowedStates;
};


} // namespace internal

} // namespace codegenvar
