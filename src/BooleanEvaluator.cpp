#include "SymbolPrivate.h"
#include <codegenvar/BooleanEvaluator.h>
#include <symengine/add.h>
#include <symengine/integer.h>
#include <symengine/real_double.h>
#include <symengine/logic.h>

namespace codegenvar {

using namespace SymEngine;

namespace internal {

enum Bool{True=1, False=0};

struct YADAYADAYADA
{
    YADAYADAYADA(const RCP<const Boolean>& condition, Bool value)
        : condition(condition)
        , value(value)
    {
    }
    RCP<const Boolean> condition;//TODO: do we need condition?
    Bool value;
};
typedef std::vector<YADAYADAYADA> TreeAddress;

struct TreeNode
{
    RCP<const Boolean> condition;
    struct{
        std::unique_ptr<TreeNode> child;
        SymExpr eval;
    }data[2];
    
    bool isFullyEvaluated()const
    {
        return (!data[True].eval.is_null() || data[True].child->isFullyEvaluated())
            && (!data[False].eval.is_null()|| data[False].child->isFullyEvaluated());
    }
    bool isEvaluated(const TreeAddress& address)const
    {
        ASSERT(!address.empty());
        auto yada = address.front();
        const auto& tba = data[yada.value];
        if (address.size()==1)
        {
            if (!tba.eval.is_null())
                return true;
            if (!tba.child)
                return false;
            return tba.child->isFullyEvaluated();
        }
        ASSERT(yada.condition->__eq__(*condition));
        
        const auto& child = tba.child;
        ASSERT(child);
        return child->isEvaluated(TreeAddress(address.begin()+1, address.end()));
    }
    void insert(const TreeAddress& address, SymExpr expr)
    {
        ASSERT(!address.empty());
        auto yada = address.front();
        auto& tba = data[yada.value];
        if (address.size()==1)
        {
            CONDITION(!tba.child, "It seems the function tested is not deterministic. (1)");
            if (!tba.eval.is_null())
                CONDITION(expr->__eq__(*tba.eval), "It seems the function tested is not deterministic. (2)");
            else
                tba.eval = expr;
            return;
        }
        ASSERT(yada.condition->__eq__(*condition));
        
        const auto& child = tba.child;
        ASSERT(child);
        child->insert(TreeAddress(address.begin()+1, address.end()), expr);
    }
};

struct BooleanEvaluatorPrivate
{
    enum Op{LT, EQ, GT};
    
    static bool handle(const SymbolPrivate* lhs, const SymbolPrivate* rhs, Op op);
    bool handle(SymExpr lhs, SymExpr rhs, Op op);
    bool done(SymExpr expr);
    
    typedef std::unique_ptr<TreeNode> EvaluatedBooleans;
    TreeNode evaluated;
    TreeAddress currentContext;
};

} // namespace internal

using internal::BooleanEvaluatorPrivate;

BooleanEvaluator::BooleanEvaluator()
    : p(std::make_shared<BooleanEvaluatorPrivate>())
{
}

Symbol BooleanEvaluator::getSymbol(const std::string name)const
{
    Symbol symbol(name);
    symbol.p->booleanEvaluator = p;
    return symbol;
}

bool BooleanEvaluator::done(const Symbol& symbol) const
{
    CONDITION(!symbol.p->expression.is_null(), "Invalid expression");
    if (auto evaluator = symbol.p->booleanEvaluator.lock())
        return evaluator->done(symbol.p->expression);
    return true;
}

bool operator ==(const Symbol& lhs, const Symbol& rhs)
{
    return BooleanEvaluatorPrivate::handle(lhs.p.get(), rhs.p.get(), BooleanEvaluatorPrivate::EQ);
}

bool operator < (const Symbol& lhs, const Symbol& rhs)
{
    return BooleanEvaluatorPrivate::handle(lhs.p.get(), rhs.p.get(), BooleanEvaluatorPrivate::LT);
}

bool operator > (const Symbol& lhs, const Symbol& rhs)
{
    return BooleanEvaluatorPrivate::handle(lhs.p.get(), rhs.p.get(), BooleanEvaluatorPrivate::GT);
}

namespace internal {

bool BooleanEvaluatorPrivate::handle(const SymbolPrivate *lhs, const SymbolPrivate *rhs, Op op)
{
    using namespace SymEngine;
    auto evaluator = lhs->booleanEvaluator.lock();
    if (!evaluator)
        evaluator = rhs->booleanEvaluator.lock();    
    else if (auto other = rhs->booleanEvaluator.lock())
        if (other && other!=evaluator)
            ERROR("There are two different boolean evaluators");        
    
    if (evaluator)
        return evaluator->handle(lhs->expression, rhs->expression, op);
    
    // See if the expression is constant.
    auto diff = sub(lhs->expression, rhs->expression);
    
    if (!is_a_Number(*diff))
        ERROR("Your code contains conditional branches, but you haven't "
              "created a boolean evaluator. See <doc> for details.");     
    
    if (op == EQ)
        return down_cast<const Number &>(*diff).is_zero();
    
    if (is_a<RealDouble>(*diff))
    {
         double result = down_cast<const RealDouble &>(*diff).as_double();
         return (op==LT  && result < 0.0)  
             || (op==GT  && result > 0.0);
    }
    else if (is_a<Integer>(*diff))
    {
         int result = down_cast<const Integer &>(*diff).as_int();
         return (op==LT  && result < 0)  
             || (op==GT  && result > 0);
    }
    else
        ERROR("Unknown symengine number type.");    
}

bool BooleanEvaluatorPrivate::handle(SymExpr lhs, SymExpr rhs, Op op)
{
    RCP<const Boolean>  condition;
    switch(op)
    {
    default:
    case LT: condition = Lt(lhs, rhs); break;
    case EQ: condition = Eq(lhs, rhs); break;
    case GT: condition = Gt(lhs, rhs); break;
    }
    
std::cerr << "Boolean logic goes here. " << condition->__str__() << std::endl;
    
    currentContext.emplace_back(condition, True);
    if (!evaluated.isEvaluated(currentContext))
        return true;
    currentContext.back().value = False;
    if (!evaluated.isEvaluated(currentContext))
        return false;
    
std::cerr << "TODO: Should i be here?" << std::endl;

    return false;
}

bool BooleanEvaluatorPrivate::done(SymExpr expr)
{
    if (!currentContext.empty())
    {
        evaluated.insert(currentContext, expr);
        currentContext = {};
    }
    
    return evaluated.isFullyEvaluated();
}

} // namespace internal

} // namespace codegenvar
