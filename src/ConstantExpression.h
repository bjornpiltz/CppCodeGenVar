#pragma once
#include "AbstractExpression.h"

namespace codegenvar{
namespace internal {

class ConstantExpression : public AbstractExpression
{
    Number value;
public:
    ConstantExpression(const Number& value)
        : value(value)
    {
    }
    std::string toString()const{ return value.toString(); }
    Number toNumber()const{ return value; }
    double toDouble()const{ return value.toDouble(); }
    
    Type type()const override{ return TypeConstantExpression;}
    bool isZero()const override { return value.isZero(); }
    bool isOne()const override { return value.isOne(); }
    ConstantExpression* clone()const override { return new ConstantExpression(*this); }
};

}// namespace internal
}// namespace codegenvar
