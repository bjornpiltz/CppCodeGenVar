#include "SymbolPrivate.h"
#include "ConditionCache.h"
#include "ConditionalTree.h"
#include <codegenvar/BooleanEvaluator.h>
#include <symengine/add.h>
#include <symengine/integer.h>
#include <symengine/real_double.h>

namespace codegenvar {
    

namespace internal {
    
using namespace SymEngine;

struct BooleanEvaluatorPrivate
{
    static bool handle(const SymbolPrivate* lhs, const SymbolPrivate* rhs, BooleanOp op);
    bool handle(SymExpr lhs, SymExpr rhs, BooleanOp op);

    ConditionalTree evaluated;
    ConditionalTree::Iterator currentContext;
    ConditionCache evaluatedBools;
    RCP<const Boolean> getCurrentContext()const;
};

} // namespace internal


using namespace internal;
using SymEngine::boolTrue;
using SymEngine::down_cast;
using SymEngine::integer;
using SymEngine::is_a;
using SymEngine::Piecewise;
using SymEngine::PiecewiseVec;
using SymEngine::piecewise;
using SymEngine::RCP;

std::shared_ptr<BooleanEvaluatorPrivate> BooleanEvaluator::p;

BooleanEvaluator::BooleanEvaluator()
{
    if (p)
        ERROR("There can only be one BooleanEvaluator.");
    p = std::make_shared<BooleanEvaluatorPrivate>();
}

std::weak_ptr<BooleanEvaluatorPrivate> BooleanEvaluator::get()
{
    return p;
}


/** @defgroup symbol_logical_ops Logical operators
 *  @{
 */
bool operator ==(const Symbol& lhs, const Symbol& rhs)
{
    return BooleanEvaluatorPrivate::handle(lhs.p.get(), rhs.p.get(), EQ);
}

bool operator < (const Symbol& lhs, const Symbol& rhs)
{
    return BooleanEvaluatorPrivate::handle(lhs.p.get(), rhs.p.get(), LT);
}

bool operator > (const Symbol& lhs, const Symbol& rhs)
{
    return BooleanEvaluatorPrivate::handle(lhs.p.get(), rhs.p.get(), GT);
}

bool operator !=(const Symbol& lhs, const Symbol& rhs){ return !(lhs == rhs);}

bool operator <=(const Symbol& lhs, const Symbol& rhs){ return !(lhs  > rhs);}

bool operator >=(const Symbol& lhs, const Symbol& rhs){ return !(lhs  < rhs);}
/** @} */ // end of symbol_logical_ops


namespace internal {

bool BooleanEvaluatorPrivate::handle(const SymbolPrivate *lhs, const SymbolPrivate *rhs, BooleanOp op)
{
    // See if the expression is constant.
    auto diff = sub(lhs->expression, rhs->expression);
    if (is_a_Number(*diff))
    {
        if (op == EQ)
            return down_cast<const Number &>(*diff).is_zero();

        if (is_a<RealDouble>(*diff))
        {
            double result = down_cast<const RealDouble &>(*diff).as_double();
            return (op == LT && result < 0.0)
                || (op == GT && result > 0.0);
        }
        else if (is_a<Integer>(*diff))
        {
        int result = down_cast<const Integer &>(*diff).as_int();
        return (op == LT && result < 0)
            || (op == GT && result > 0);
        }
        else
            ERROR("Unknown symengine number type.");
    }

    auto evaluator = BooleanEvaluator::get().lock();
    if (!evaluator)
        ERROR("Your code contains conditional branches, but you haven't "
            "created a boolean evaluator.\n"
            "See http://cppcodegenvar.readthedocs.io/en/latest/conditional_example.html for details.");     
    return evaluator->handle(lhs->expression, rhs->expression, op);
}

bool BooleanEvaluatorPrivate::handle(SymExpr lhs, SymExpr rhs, BooleanOp op)
{
    bool result;
    if(evaluatedBools.alreadyEvaluated(lhs, rhs, op, result))
        return result;

    RCP<const Boolean>  condition;
    switch(op)
    {
    default:
    case LT: condition = Lt(lhs, rhs); break;
    case EQ: condition = Eq(lhs, rhs); break;
    case GT: condition = Gt(lhs, rhs); break;
    }
    currentContext.emplace_back(condition, true);
    if (!evaluated.isEvaluated(currentContext))
        result = true;
    else
    {
        currentContext.back().second = false;
        if (!evaluated.isEvaluated(currentContext))
            result = false;
        else
            ASSERT("I shouldn't be here.");
    }
    evaluatedBools.insert(lhs, rhs, op, result);
    return result;
}

RCP<const Boolean> BooleanEvaluatorPrivate::getCurrentContext()const
{
    if (currentContext.empty())
        return {};

    set_boolean set;
    for (auto branch : currentContext)
        set.insert(branch.second ? branch.first : branch.first->logical_not());
    return logical_and(set);
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

    evaluator->evaluated.visit(evaluator->currentContext);

    if (evaluator->evaluated.isFullyEvaluated())
        condition = boolTrue;

    if (is_a<Piecewise>(*p->expression))
    {
        PiecewiseVec vec = down_cast<const Piecewise&>(*p->expression).get_vec();
        ASSERT(vec.size() != 0);
        vec.emplace_back(std::make_pair(symbol.p->expression, condition));
        p->expression = SymEngine::piecewise(std::move(vec));
    }
    else
    {
        auto q = piecewise({ {symbol.p->expression, condition} });
        if (!eq(*p->expression, *integer(0)))
            std::cerr << "Warning: overwriting expression: " << p->expression->__str__() << std::endl;
        std::swap(q, p->expression);
    }
    return *this;
}

bool BooleanEvaluator::isFullyEvaluated()
{
    if(p->evaluated.branch[true].isNull() && p->evaluated.branch[false].isNull())
        return p->currentContext.empty();
    bool result = p->evaluated.isFullyEvaluated();
    p->currentContext = {};
    p->evaluatedBools = {};
    return result;
}
  
} // namespace codegenvar
