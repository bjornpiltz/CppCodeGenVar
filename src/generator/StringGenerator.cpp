#include <codegenvar/generator/StringGenerator.h>
#include "SymbolPrivate.h"
#include "Error.h"
#include <symengine/printer.h>

namespace codegenvar{

namespace internal{

using SymEngine::StrPrinter;
using SymEngine::BaseVisitor;
using SymEngine::rational;
using SymEngine::integer;
using SymEngine::PrecedenceEnum;
using SymEngine::FunctionSymbol;

// The purpose of this class is to replace some naming conventions
// unusual to coders, like a**x -> pow(a, x).
class MyPrinter : public BaseVisitor<MyPrinter, StrPrinter>
{
public:
    using StrPrinter::apply;
    using StrPrinter::bvisit;

    void _print_pow(std::ostringstream &o, const SymExpr &a, const SymExpr &b)
    {
        if (eq(*a, *SymEngine::E)) {
            o << "exp(" << apply(b) << ")";
        } else if (eq(*b, *rational(1, 2))) {
            o << "sqrt(" << apply(a) << ")";
        } else if (eq(*b, *integer(-1))) 
        {
            o << "1/" << parenthesizeLE(a, PrecedenceEnum::Mul);
        } else {
            o << "pow(" << apply(a) << ", " << apply(b) << ")";
        }
    }
    void bvisit(const FunctionSymbol &x)
    {
        auto name = x.get_name();
        if (name == "ceiling")
            name = "ceil";
        std::ostringstream o;
        o << name << "(" << this->apply(x.get_args()) << ")";
        str_ = o.str();
    }
};

} // namespace internal

std::string StringEvaluator::operator()(const Symbol& symbol)const
{
    const auto& p = symbol.p;
    if (p->expression.is_null())
        return "";
    
    internal::MyPrinter strPrinter;
    return strPrinter.apply(*p->expression);
}

std::string Symbol::toString()const
{
    return StringEvaluator()(*this);
}

}// namespace codegenvar