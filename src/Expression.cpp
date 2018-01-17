#include "internal/Expression.h"
#include "internal/Expression.h"
#include "internal/Error.h"

namespace codegenvar{
namespace internal{

Expression::Expression(Type _type)
    : _type(_type)
{
}

Expression::Expression(Type type, const std::vector<ConstPtr>& exps)
    : _type(type)
    , exps(exps)
{
}

Type Expression::type()const 
{
    return _type;
}

int Expression::childCount()const
{ 
    return static_cast<int>(exps.size());
}

std::set<std::string> Expression::getVariableNames()const
{
    std::set<std::string> result;
    for (const auto& expr: exps)
    {
        auto names = expr->getVariableNames();
        result.insert(names.begin(), names.end());
    }
    return result;
}

ConstPtr Expression::child(int num)const
{
    CONDITION(num < exps.size(), "num out of range.");
    return exps[num];
}

const std::vector<ConstPtr>& Expression::children()const
{
    return exps;
}

void Expression::append(ConstPtr expr)
{
    CONDITION(type() == TypeTernaryMul||type() == TypeTernaryPlus, "Appending to a fixed size function");
    exps.insert(exps.end(), expr);
}

void Expression::prepend(ConstPtr expr)
{
    CONDITION(type() == TypeTernaryMul||type() == TypeTernaryPlus, "Prepending to a fixed size function");
    exps.insert(exps.begin(), expr);
}

Expression* Expression::clone()const  
{
    std::vector<ConstPtr> copy;
    for(const auto& expr: exps)
        copy.emplace_back(expr->clone());
    return new Expression( type(), copy);
}

ConstPtr Expression::resolved(const Symbol::Map& symbolMap)const
{
    std::vector<ConstPtr> copies;
    for (auto expr: exps)
        copies.push_back(expr->resolved(symbolMap));
    return ConstPtr{new Expression(type(), copies)};
}

}// namespace internal
}// namespace codegenvar
