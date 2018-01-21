#include "codegenvar/Symbol.h"
#include "Expression.h"
#include "ConstantExpression.h"
#include "VariableExpression.h"
#include "Error.h"

namespace codegenvar{

using namespace internal;

Symbol::Symbol(const std::string& name)
    : expr(std::make_shared<VariableExpression>(name))
{    
}

std::vector<Symbol> Symbol::variables(std::initializer_list<std::string> names)
{
    std::vector<Symbol> result;
    for (auto name: names)
        result.emplace_back(name);
    return result;    
}

Symbol::Symbol(ConstPtr expr)
    : expr(expr)
{
}  

void Symbol::swap(Symbol& other)
{
    std::swap(expr, other.expr);
}

Symbol::Symbol(const Number& value)
    : expr(std::make_shared<ConstantExpression>(value))
{
}

Symbol::Symbol(int value)
    : expr (std::make_shared<ConstantExpression>(value))
{    
}

Symbol::Symbol(long int value)
    : expr (std::make_shared<ConstantExpression>(value))
{    
}

Symbol::Symbol(long long int value)
    : expr (std::make_shared<ConstantExpression>(value))
{    
}

Symbol::Symbol(float value)
    : expr (std::make_shared<ConstantExpression>(value))
{    
}

Symbol::Symbol(double value)
    : expr (std::make_shared<ConstantExpression>(value))
{    
}

std::set<std::string> Symbol::getVariableNames()const
{
    CONDITION(expr, "Symbol is uninitialized");
    return expr->getVariableNames();
}

Symbol& Symbol::operator += (const Symbol& other)
{
    Symbol copy(*this + other);
    std::swap(copy.expr, expr);
    return *this;
}

Symbol& Symbol::operator -= (const Symbol& other)
{
    Symbol copy(*this - other);
    std::swap(copy.expr, expr);
    return *this;
}

Symbol& Symbol::operator *= (const Symbol& other)
{
    Symbol copy(*this * other);
    std::swap(copy.expr, expr);
    return *this;
}

Symbol& Symbol::operator /= (const Symbol& other)
{
    Symbol copy(*this / other);
    std::swap(copy.expr, expr);
    return *this;
}

// Unary div
Symbol Symbol::inverse()const 
{
    const Symbol& value = *this;
    CONDITION(value.expr, "Passed null ptr");
    if (value.expr->isOne())
        return value;
    if (value.expr->type()==TypeUnaryDiv)
        return Symbol(value.expr->child());
    return ConstPtr(new Expression(TypeUnaryDiv, { value.expr }));
}

Symbol Symbol::resolved(const Symbol::Map& symbolMap) const
{
    auto names = expr->getVariableNames();
    for (auto name: names)
        if (symbolMap.count(name))
            return Symbol(expr->resolved(symbolMap));
    return *this;
}

Symbol operator + (const Symbol& lhs, const Symbol& rhs)
{
    if(lhs.expr->isZero())
        return Symbol(rhs.expr);
    else if(rhs.expr->isZero())
        return Symbol(lhs.expr);
    else if(lhs.expr->type()==TypeTernaryPlus)
    {
        Ptr copy(lhs.expr->clone());
        
        if(rhs.expr->type()==TypeTernaryPlus)
            for (int i = 0; i<rhs.expr->childCount(); i++)
                copy->append(rhs.expr->child(i));
        else
            copy->append(rhs.expr);
        return Symbol(copy);
    }
    else if(rhs.expr->type()==TypeTernaryPlus)
    {
        Ptr copy(rhs.expr->clone());
        copy->prepend(lhs.expr);
        return Symbol(copy);
    }
    else 
        return ConstPtr(new Expression(TypeTernaryPlus, { lhs.expr, rhs.expr }));
}

Symbol operator - (const Symbol& lhs, const Symbol& rhs)
{
    return lhs + (-rhs);
}

Symbol operator * (const Symbol& lhs, const Symbol& rhs)
{
    if(lhs.expr->isZero())
        return Symbol(0);
    else if(rhs.expr->isZero())
        return Symbol(0);
    else if(lhs.expr->isOne())
        return Symbol(rhs.expr);
    else if(rhs.expr->isOne())
        return Symbol(lhs.expr);
    else if(lhs.expr->type()==TypeTernaryMul)
    {
        Ptr copy(lhs.expr->clone());
        if(rhs.expr->type()==TypeTernaryMul)
            for (int i = 0; i<rhs.expr->childCount(); i++)
                copy->append(rhs.expr->child(i));
        else
            copy->append(rhs.expr);
        return ConstPtr(copy);
    }
    else if(rhs.expr->type()==TypeTernaryMul)
    {
        Ptr copy(rhs.expr->clone());
        copy->prepend(lhs.expr);
        return ConstPtr(copy);
    }
    return ConstPtr(new Expression(TypeTernaryMul, { lhs.expr, rhs.expr }));
}

Symbol operator / (const Symbol& lhs, const Symbol& rhs)
{
    if(rhs.expr->isOne())
        return Symbol(lhs.expr);
    return lhs * rhs.inverse();
}

// Symbol op Number and Number op Symbol overloads
Symbol operator + (const Number& lhs, const Symbol& rhs){ return Symbol(lhs) + rhs;}
Symbol operator + (const Symbol& lhs, const Number& rhs){ return lhs + Symbol(rhs);}
Symbol operator - (const Number& lhs, const Symbol& rhs){ return Symbol(lhs) - rhs;}
Symbol operator - (const Symbol& lhs, const Number& rhs){ return lhs - Symbol(rhs);}
Symbol operator * (const Number& lhs, const Symbol& rhs){ return Symbol(lhs) * rhs;}
Symbol operator * (const Symbol& lhs, const Number& rhs){ return lhs * Symbol(rhs);}
Symbol operator / (const Number& lhs, const Symbol& rhs){ return Symbol(lhs) / rhs;}
Symbol operator / (const Symbol& lhs, const Number& rhs){ return lhs / Symbol(rhs);}

Symbol& Symbol::operator  = (const Number& other){ return *this = Symbol(other);}
Symbol& Symbol::operator += (const Number& other){ return *this += Symbol(other);}
Symbol& Symbol::operator -= (const Number& other){ return *this -= Symbol(other);}
Symbol& Symbol::operator *= (const Number& other){ return *this *= Symbol(other);}
Symbol& Symbol::operator /= (const Number& other){ return *this /= Symbol(other);}

Symbol pow(const Symbol&x, const Number& y){return pow(x, Symbol(y));}


bool operator ==(const Symbol& lhs, const Number& rhs){ return (lhs) == Symbol(rhs); }
bool operator ==(const Number& lhs, const Symbol& rhs){ return Symbol(lhs) == (rhs); }
bool operator < (const Symbol& lhs, const Number& rhs){ return (lhs) <  Symbol(rhs); }
bool operator < (const Number& lhs, const Symbol& rhs){ return Symbol(lhs) <  (rhs); }
bool operator > (const Symbol& lhs, const Number& rhs){ return (lhs) >  Symbol(rhs); }
bool operator > (const Number& lhs, const Symbol& rhs){ return Symbol(lhs) >  (rhs); }
bool operator !=(const Symbol& lhs, const Number& rhs){ return (lhs) != Symbol(rhs); }
bool operator !=(const Number& lhs, const Symbol& rhs){ return Symbol(lhs) != (rhs); }
bool operator <=(const Symbol& lhs, const Number& rhs){ return (lhs) <= Symbol(rhs); }
bool operator <=(const Number& lhs, const Symbol& rhs){ return Symbol(lhs) <= (rhs); }
bool operator >=(const Symbol& lhs, const Number& rhs){ return (lhs) >= Symbol(rhs); }
bool operator >=(const Number& lhs, const Symbol& rhs){ return Symbol(lhs) >= (rhs); }


#define DEFINE_UNARY_FUNCTION(functionName)\
Symbol functionName(const Symbol& value)\
{\
    return ConstPtr(new Expression(TypeUnaryFunction_ ## functionName, { value.expr } ));\
}
DEFINE_UNARY_FUNCTION(abs)
DEFINE_UNARY_FUNCTION(log)
DEFINE_UNARY_FUNCTION(exp)
DEFINE_UNARY_FUNCTION(sqrt)
DEFINE_UNARY_FUNCTION(cos)
DEFINE_UNARY_FUNCTION(acos)
DEFINE_UNARY_FUNCTION(sin)
DEFINE_UNARY_FUNCTION(asin)
DEFINE_UNARY_FUNCTION(tan)
DEFINE_UNARY_FUNCTION(atan)
DEFINE_UNARY_FUNCTION(sinh)
DEFINE_UNARY_FUNCTION(cosh)
DEFINE_UNARY_FUNCTION(tanh)
DEFINE_UNARY_FUNCTION(floor)
DEFINE_UNARY_FUNCTION(ceil)

Symbol pow(const Symbol& lhs, const Symbol& rhs)
{
    return ConstPtr(new Expression(TypeBinaryFunctionPow, { lhs.expr, rhs.expr }));
}

// Unary minus
Symbol operator -(const Symbol& value)
{
    CONDITION(value.expr, "Passed null ptr");
    if (value.expr->isZero())
        return value;
    if (value.expr->type()==TypeUnaryMinus)
        return value.expr->child();
    return ConstPtr{new Expression(TypeUnaryMinus, { value.expr })};
}

// TODO: implement boolean logic.
bool operator ==(const Symbol& lhs, const Symbol& rhs){ ERROR("Boolean logic is not supported yet.");}
bool operator < (const Symbol& lhs, const Symbol& rhs){ ERROR("Boolean logic is not supported yet.");}
bool operator > (const Symbol& lhs, const Symbol& rhs){ ERROR("Boolean logic is not supported yet.");}

bool operator !=(const Symbol& lhs, const Symbol& rhs){ return !(lhs == rhs);}
bool operator <=(const Symbol& lhs, const Symbol& rhs){ return !(lhs  > rhs);}
bool operator >=(const Symbol& lhs, const Symbol& rhs){ return !(lhs  < rhs);}

std::ostream& operator<<(std::ostream& os, const Symbol& a)
{
    return os << a.toString();
}

}// namespace codegenvar