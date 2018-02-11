#include "SymbolPrivate.h"
#include "BooleanEvaluatorPrivate.h"
#include <codegenvar/BooleanEvaluator.h>
#include <symengine/add.h>
#include <symengine/integer.h>
#include <symengine/real_double.h>

namespace codegenvar {

using internal::BooleanEvaluatorPrivate;
using SymEngine::boolTrue;
using SymEngine::down_cast;
using SymEngine::integer;
using SymEngine::is_a;
using SymEngine::Piecewise;
using SymEngine::PiecewiseVec;
using SymEngine::piecewise;
using SymEngine::RCP;

std::shared_ptr<internal::BooleanEvaluatorPrivate> BooleanEvaluator::p;

BooleanEvaluator::BooleanEvaluator()
{
    if (p)
        ERROR("There can only be one BooleanEvaluator.");
    p = std::make_shared<BooleanEvaluatorPrivate>();
}

std::weak_ptr<internal::BooleanEvaluatorPrivate> BooleanEvaluator::get()
{
    return p;
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
    auto evaluator = BooleanEvaluator::get().lock();
    if (evaluator)
        return evaluator->handle(lhs->expression, rhs->expression, op);
    
    // See if the expression is constant.
    auto diff = sub(lhs->expression, rhs->expression);
    
    if (!is_a_Number(*diff))
        ERROR("Your code contains conditional branches, but you haven't "
              "created a boolean evaluator. See <doc> for details.");     // TODO: link
    
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
    
    currentContext.emplace_back(condition, True);
    if (!evaluated.isEvaluated(currentContext))
        return true;

    currentContext.back().value = False;
    if (!evaluated.isEvaluated(currentContext))
        return false;
    
std::cerr << "TODO: Should i be here?" << std::endl;

    return false;
}

RCP<const Boolean> BooleanEvaluatorPrivate::getCurrentContext()const
{
    if (currentContext.empty())
        return {};

    set_boolean set;
    for (auto branch : currentContext)
        set.insert(branch.value==True 
                 ? branch.condition
                 : branch.condition->logical_not());
    return logical_and(set);
}

bool ConditionalTree::isFullyEvaluated()const
{
    return branch[True].isFullyEvaluated() && branch[False].isFullyEvaluated();
}

bool ConditionalTree::isEvaluated(const Iterator& address)const
{
    ASSERT(!address.empty());
    const auto& child = branch[address.front().value];
    if (address.size()==1)
        return child.isFullyEvaluated();
    return child.node && child.node->isEvaluated(Iterator(address.begin()+1, address.end()));
}

void ConditionalTree::insert(const Iterator& address, SymExpr expr)
{
    ASSERT(!address.empty());
    auto& child = branch[address.front().value];
    if (address.size()==1)
    {
        CONDITION(!child.node, "It seems the function tested is not deterministic. (1)");
        if (!child.leaf.is_null())
            CONDITION(expr->__eq__(*child.leaf), "It seems the function tested is not deterministic. (2)");
        else
            child.leaf = expr;
        return;
    }
    if(!child.node)
        child.node = std::unique_ptr<ConditionalTree>(new ConditionalTree);
    child.node->insert(Iterator(address.begin()+1, address.end()), expr);
}

} // namespace internal

Symbol& Symbol::operator|=(const Symbol& symbol)
{
    auto evaluator = BooleanEvaluator::get().lock();
    if (!evaluator)
        return *this = symbol;
     
    auto condition = evaluator->getCurrentContext();
    if (condition.is_null())
        return *this = symbol;
    

    evaluator->evaluated.insert(evaluator->currentContext, symbol.p->expression);    
    
    if (evaluator->evaluated.isFullyEvaluated())
        condition = boolTrue;

    auto q = piecewise({ {symbol.p->expression, condition} });
    if (is_a<Piecewise>(*p->expression))
    {
        const Piecewise& piecewise = down_cast<const Piecewise&>(*p->expression);
        PiecewiseVec vec = piecewise.get_vec();
        ASSERT(vec.size()!=0);
        // TODO: check if vec already contains condition
        vec.emplace_back(std::make_pair(symbol.p->expression, condition));
        p->expression = SymEngine::piecewise(std::move(vec));
    }
    else
    {
        if (!p->expression->__eq__(*integer(0)))
            std::cerr << "Warning: overwriting expression: " << p->expression->__str__() << std::endl;
        std::swap(q, p->expression);
    }
    evaluator->currentContext = {};
    return *this;
}

bool BooleanEvaluator::isFullyEvaluated()const
{
    return p->evaluated.isFullyEvaluated();
}

bool Symbol::isFullyEvaluated()const
{
    if (is_a<Piecewise>(*p->expression))
    {
        const Piecewise& piecewise = down_cast<const Piecewise&>(*p->expression);
        ASSERT(piecewise.get_vec().size()!=0);
        auto pair = piecewise.get_vec().back();
        return pair.second->__eq__(*boolTrue);
    }
    return true;
}

} // namespace codegenvar
