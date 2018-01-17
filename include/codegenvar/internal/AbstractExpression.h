#pragma once
#include "codegenvar/Symbol.h"
#include <set>
#include <string>
#include <memory>

namespace codegenvar {
namespace internal {

enum Type// The order of the following also defines the operators precedence
{
    TypeUnaryFunction_abs,
    TypeUnaryFunction_log,
    TypeUnaryFunction_exp,
    TypeUnaryFunction_sqrt,
    TypeUnaryFunction_cos,
    TypeUnaryFunction_acos,
    TypeUnaryFunction_sin,
    TypeUnaryFunction_asin,
    TypeUnaryFunction_tan,
    TypeUnaryFunction_atan,
    TypeUnaryFunction_sinh,
    TypeUnaryFunction_cosh,
    TypeUnaryFunction_tanh,
    TypeUnaryFunction_floor,
    TypeUnaryFunction_ceil,
    LastTypeUnaryFunction= TypeUnaryFunction_ceil,
    
    TypeConstantExpression,
    TypeVariableExpression,
    
    TypeBinaryFunctionPow,
    
    TypeUnaryMinus,
    TypeUnaryDiv,
    
    TypeTernaryMul,
    TypeTernaryPlus
};

typedef std::shared_ptr<const class AbstractExpression> ConstPtr;
typedef std::shared_ptr<      class AbstractExpression> Ptr;

class AbstractExpression
{
public:
    virtual ~AbstractExpression(){}
    virtual Type type()const=0;
    virtual bool isZero()const { return false; }
    virtual bool isOne()const { return false; }
    virtual AbstractExpression* clone()const=0;
    virtual std::set<std::string> getVariableNames()const;
    
    virtual int childCount()const{ return 0;}
    virtual ConstPtr child(int num=0)const;
    virtual const std::vector<ConstPtr>& children()const;
    
    virtual void append(ConstPtr){}
    virtual void prepend(ConstPtr){}
    
    virtual ConstPtr resolved(const Symbol::Map&)const;
};

}// namespace internal
}// namespace codegenvar
