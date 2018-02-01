#include "SymbolPrivate.h"
#include "BooleanEvaluatorPrivate.h"
#include <codegenvar/BooleanEvaluator.h>
#include <symengine/add.h>
#include <symengine/integer.h>
#include <symengine/real_double.h>

namespace codegenvar {

using internal::BooleanEvaluatorPrivate;
    
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

bool BooleanEvaluator::done(std::map<std::string, TBA>& map) const
{
    for (auto& pair : map)
    {
        const auto& key = pair.first;
        TBA& symbol = pair.second;
    }

    if (p->currentContext.empty())
        return true;
     
    p->evaluated.insert(p->currentContext, SymEngine::integer(1));

    p->currentContext = {};
    bool isDone = p->evaluated.isFullyEvaluated();
    if (isDone)
    {
        int sdfsdf  = 0;
    }
    return isDone;
    /*
    
    */
    return false;
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
    
for(int i = 0; i< currentContext.size(); i++)
    std::cerr << "    ";

    currentContext.emplace_back(condition, True);
    if (!evaluated.isEvaluated(currentContext))
    {
// std::cerr << "if( " << condition->__str__() << " == true)" << std::endl;
        return true;
    }
    currentContext.back().value = False;
    if (!evaluated.isEvaluated(currentContext))
    {
// std::cerr << "if( " << condition->__str__() << " != true)" << std::endl;
        return false;
    }
    
std::cerr << "TODO: Should i be here?" << std::endl;

    return false;
}

bool BooleanEvaluatorPrivate::done(SymExpr expr)
{
    if (currentContext.empty())
        return true;
     
    evaluated.insert(currentContext, expr);

    
for(int i = 0; i< currentContext.size(); i++)
    std::cerr << "    ";
std::cerr << expr->__str__() << std::endl;

    currentContext = {};
    bool isDone = evaluated.isFullyEvaluated();
    if (isDone)
    {
        int sdfsdf  = 0;
    }
    return isDone;
}

RCP<const Boolean> BooleanEvaluatorPrivate::getCurrentContext()const
{
    if (currentContext.empty())
        return {};

    set_boolean set;
    for (auto branch : currentContext)
    {
        if (branch.value==True)
            set.insert(branch.condition);
        else
            set.insert(branch.condition->logical_not());
    }
    // TODO: std::cerr << logical_and(set)->__str__() << std::endl;
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

void TBA::operator=(const Symbol& other) 
{
    //std::cerr << "if (" << other.p->condition->__str__() << ")" << std::endl; 
    //std::cerr << "    "<< other.p->expression->__str__() << std::endl; ; 
}

} // namespace codegenvar
