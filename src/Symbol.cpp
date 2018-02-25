#include <codegenvar/Symbol.h>
#include "SymbolPrivate.h"
#include <symengine/add.h>
#include <symengine/functions.h>
#include <symengine/visitor.h>
#include <symengine/eval_double.h>
#include <symengine/derivative.h>

/**
 * All classes and function of CppCodeGenVar are contained int the namespace codegenvar.
 */
namespace codegenvar{

using SymEngine::symbol;
using SymEngine::integer;
using SymEngine::real_double;
using SymEngine::free_symbols;

namespace internal
{

// Convert a scalar to a symengine expression:
static SymExpr scalar(const Scalar& value)
{
    if (value.isInt())
        return integer(value.toInt());
    else
        return real_double(value.toDouble());
}

} // namespace internal

using namespace internal;

/** @defgroup symbol_ctors Symbol Constructors
 *  @{
 */
Symbol::Symbol(const std::string& name)
    : Symbol()
{           
    if (!name.empty())
        p->expression = symbol(name);
}

 /// Constructors creating a constant.
Symbol::Symbol(int value)
    : Symbol()
{    
    p->expression = integer(value);
}

 /// Constructors creating a constant.
Symbol::Symbol(long int value)
    : Symbol()
{    
    p->expression = integer(value);
}

 /// Constructors creating a constant.
Symbol::Symbol(long long int value)
    : Symbol()
{    
    p->expression = integer(value);
}

 /// Constructors creating a constant.
Symbol::Symbol(float value)
    : Symbol()
{    
    p->expression = real_double(value);
}

 /// Constructors creating a constant.
Symbol::Symbol(double value)
    : Symbol()
{    
    p->expression = real_double(value);
}

/** @} */ // end of symbol_ctors

Symbol::Symbol(const Scalar& value)
    : Symbol()
{
    p->expression = scalar(value);
}

void Symbol::swap(Symbol& other)
{
    std::swap(p, other.p);
}

std::set<std::string> Symbol::getVariableNames()const
{
    std::set<std::string> result;
    if (!p->expression.is_null())
        for (auto symbol: free_symbols(*p->expression))
            result.insert(symbol->__str__());
    return result;
}

void Symbol::expand()
{
    *this = expanded();
}

Symbol Symbol::expanded()const
{
    return SymbolPrivate::ctor(SymEngine::expand(p->expression));
}

bool Symbol::equals(const Symbol& other)const
{
    if (other.p->expression.is_null())
        return p->expression.is_null();
    else if (p->expression.is_null())
        return false;
    return SymEngine::eq(*other.p->expression, *other.p->expression);
}

Symbol Symbol::resolved(const Symbol::Map& symbolMap) const
{
    CONDITION(!p->expression.is_null(), "Symbol is uninitialized");
    SymEngine::map_basic_basic map;
    for (auto pair: symbolMap)
        map[symbol(pair.first)] = scalar(pair.second);
    return SymbolPrivate::ctor(p->expression->subs(map));
}

double Symbol::toDouble()const
{
    CONDITION(!p->expression.is_null(), "Symbol is uninitialized");
    return SymEngine::eval_double(*p->expression);
}

Symbol Symbol::diff(const std::string& varName)const
{
    return SymbolPrivate::ctor(SymEngine::diff(p->expression, symbol(varName)));
}

/** @defgroup symbol_math_ops Mathematical operations
 *  @{
 */
Symbol& Symbol::operator += (const Symbol& other)
{
    Symbol copy(*this + other);
    std::swap(copy.p, p);
    return *this;
}

Symbol& Symbol::operator -= (const Symbol& other)
{
    Symbol copy(*this - other);
    std::swap(copy.p, p);
    return *this;
}

Symbol& Symbol::operator *= (const Symbol& other)
{
    Symbol copy(*this * other);
    std::swap(copy.p, p);
    return *this;
}

Symbol& Symbol::operator /= (const Symbol& other)
{
    Symbol copy(*this / other);
    std::swap(copy.p, p);
    return *this;
}

Symbol operator + (const Symbol& lhs, const Symbol& rhs)
{
    return SymbolPrivate::ctor(add(lhs.p->expression, rhs.p->expression));
}

Symbol operator - (const Symbol& lhs, const Symbol& rhs)
{
    return lhs + (-rhs);
}

Symbol operator * (const Symbol& lhs, const Symbol& rhs)
{
    return SymbolPrivate::ctor(mul(lhs.p->expression, rhs.p->expression));
}

Symbol operator / (const Symbol& lhs, const Symbol& rhs)
{
    return SymbolPrivate::ctor(div(lhs.p->expression, rhs.p->expression));
}

// Unary minus
Symbol operator -(const Symbol& value)
{
    return SymbolPrivate::ctor(sub(integer(0), value.p->expression));
}
/** @} */ // end of symbol_math_ops

// Symbol op Scalar and Scalar op Symbol overloads
Symbol operator + (const Scalar& lhs, const Symbol& rhs){ return Symbol(lhs) + rhs;}
Symbol operator + (const Symbol& lhs, const Scalar& rhs){ return lhs + Symbol(rhs);}
Symbol operator - (const Scalar& lhs, const Symbol& rhs){ return Symbol(lhs) - rhs;}
Symbol operator - (const Symbol& lhs, const Scalar& rhs){ return lhs - Symbol(rhs);}
Symbol operator * (const Scalar& lhs, const Symbol& rhs){ return Symbol(lhs) * rhs;}
Symbol operator * (const Symbol& lhs, const Scalar& rhs){ return lhs * Symbol(rhs);}
Symbol operator / (const Scalar& lhs, const Symbol& rhs){ return Symbol(lhs) / rhs;}
Symbol operator / (const Symbol& lhs, const Scalar& rhs){ return lhs / Symbol(rhs);}

Symbol& Symbol::operator  = (const Scalar& other){ return *this = Symbol(other);}
Symbol& Symbol::operator += (const Scalar& other){ return *this += Symbol(other);}
Symbol& Symbol::operator -= (const Scalar& other){ return *this -= Symbol(other);}
Symbol& Symbol::operator *= (const Scalar& other){ return *this *= Symbol(other);}
Symbol& Symbol::operator /= (const Scalar& other){ return *this /= Symbol(other);}

bool operator ==(const Symbol& lhs, const Scalar& rhs){ return (lhs) == Symbol(rhs); }
bool operator ==(const Scalar& lhs, const Symbol& rhs){ return Symbol(lhs) == (rhs); }
bool operator < (const Symbol& lhs, const Scalar& rhs){ return (lhs) <  Symbol(rhs); }
bool operator < (const Scalar& lhs, const Symbol& rhs){ return Symbol(lhs) <  (rhs); }
bool operator > (const Symbol& lhs, const Scalar& rhs){ return (lhs) >  Symbol(rhs); }
bool operator > (const Scalar& lhs, const Symbol& rhs){ return Symbol(lhs) >  (rhs); }
bool operator !=(const Symbol& lhs, const Scalar& rhs){ return (lhs) != Symbol(rhs); }
bool operator !=(const Scalar& lhs, const Symbol& rhs){ return Symbol(lhs) != (rhs); }
bool operator <=(const Symbol& lhs, const Scalar& rhs){ return (lhs) <= Symbol(rhs); }
bool operator <=(const Scalar& lhs, const Symbol& rhs){ return Symbol(lhs) <= (rhs); }
bool operator >=(const Symbol& lhs, const Scalar& rhs){ return (lhs) >= Symbol(rhs); }
bool operator >=(const Scalar& lhs, const Symbol& rhs){ return Symbol(lhs) >= (rhs); }

#define DEFINE_UNARY_FUNCTION(functionName)\
Symbol functionName(const Symbol& value)\
{\
    return SymbolPrivate::ctor( functionName (value.p->expression));\
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

Symbol ceil(const Symbol& value)
{
    return SymbolPrivate::ctor(SymEngine::ceiling(value.p->expression));
}

Symbol pow(const Symbol& lhs, const Symbol& rhs)
{
    return SymbolPrivate::ctor(pow(lhs.p->expression, rhs.p->expression));
}

Symbol pow(const Symbol&x, const Scalar& y){return pow(x, Symbol(y));}
Symbol pow(const Scalar&x, const Symbol& y){return pow(Symbol(x), y);}

std::ostream& operator<<(std::ostream& os, const Symbol& a)
{
    return os << a.toString();
}

Symbol::Symbol(std::unique_ptr<SymbolPrivate>&& other)
    : p(std::move(other))
{
}

Symbol::Symbol()
    : Symbol(std::unique_ptr<SymbolPrivate>(new SymbolPrivate))
{   
    p->expression = integer(0);
}

Symbol::Symbol(const Symbol& other)
    : Symbol(std::unique_ptr<SymbolPrivate>(new SymbolPrivate(*other.p)))
{
}

Symbol::Symbol(Symbol&& other)
    : Symbol(std::move(other.p))
{   
}

Symbol& Symbol::operator=(const Symbol& other)
{
    return *this=Symbol(other);
}

Symbol& Symbol::operator=(Symbol&& other)
{
    std::swap(other.p, p);
    return *this;
}

Symbol::~Symbol()
{   
}

}// namespace codegenvar