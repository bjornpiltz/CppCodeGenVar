#pragma once
#include "internal/AbstractExpression.h"
#include "internal/ConstantExpression.h"

namespace codegenvar{
namespace internal {

class VariableExpression : public AbstractExpression
{
    const std::string name;
public:
    VariableExpression(const std::string& name)
        : name(name)
    {
    }
    Type type()const override{ return TypeVariableExpression;}
    const std::string toString()const{ return name; }
    VariableExpression* clone()const override { return new VariableExpression(*this); }
    std::set<std::string> getVariableNames()const override { return {name}; }    
    ConstPtr resolved(const Symbol::Map& symbolMap)const override
    {
        if(symbolMap.count(name))
            return ConstPtr{new ConstantExpression(symbolMap.at(name))};
        return ConstPtr{clone()};
    }
};

}// namespace internal
}// namespace codegenvar