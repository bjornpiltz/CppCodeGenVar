#include <codegenvar/generator/StringGenerator.h>
#include "SymbolPrivate.h"
#include <symengine/printer.h>

namespace codegenvar{

namespace internal{

using SymEngine::And;
using SymEngine::Or;
using SymEngine::boolTrue;
using SymEngine::StrPrinter;
using SymEngine::BaseVisitor;
using SymEngine::rational;
using SymEngine::integer;
using SymEngine::is_a;
using SymEngine::PrecedenceEnum;
using SymEngine::FunctionSymbol;
using SymEngine::Function;
using SymEngine::Basic;
using SymEngine::Piecewise;

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
    void bvisit(const Function &x)
    {
        auto name = names_[x.get_type_code()];
        if (name == "ceiling")
            name = "ceil";
        std::ostringstream o;
        o << name << "("  << this->apply(x.get_args()) << ")";
        str_ = o.str();
    }
    void bvisit(const Piecewise &x)
    { 
        std::string tabs(" ");
        std::ostringstream s;
        auto vec = x.get_vec();
        bool hasElse = vec.back().second->__eq__(*boolTrue);
        int size =  hasElse ? vec.size()-1 : vec.size();
        for (int i = 0; i< size; i++)
        {
            s << apply(vec[i].second);
            s << "\n" << tabs << "? ";
            s << apply(vec[i].first);
            s << "\n" << tabs << ": ";
            tabs += "  ";
        }
        if (hasElse)
            s << apply(vec.back().first);
        else
            s << "<undefined>";
        str_ = s.str(); 
    }
    void bvisit(const And &x)
    {
        std::ostringstream s;
        auto container = x.get_container();
        s << apply(*container.begin());
        for (auto it = ++(container.begin()); it != container.end(); ++it)
        {
            if (is_a<Or>(**it))
                s << " && " << "(" << apply(*it) << ")";
            else
                s << " && " << apply(*it);
        }
        str_ = s.str();
    }
    
    void bvisit(const Or &x)
    {
        std::ostringstream s;
        auto container = x.get_container();
        s << apply(*container.begin());
        for (auto it = ++(container.begin()); it != container.end(); ++it)
            s << " || " << apply(*it);
        str_ = s.str();
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