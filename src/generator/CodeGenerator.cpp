#include <codegenvar/generator/CodeGenerator.h>
#include "SymbolPrivate.h"
#include <symengine/codegen.h>
#include <algorithm>

#define DEBUG_OUTPUT(a) 

namespace SymEngine
{
namespace internal
{

class CodePrinter2 : public BaseVisitor<CodePrinter2, CodePrinter>
{
public:
    std::string tempPrefix;
    
    using CodePrinter::str_;
    using CodePrinter::apply;
    using CodePrinter::bvisit;
    
    int varCounter = 0;
    std::map<std::string, std::string> exp2var;
    std::map<std::string , std::string> var2code;
    
    void assignToVar(const Basic& x, const char* name)
    {
        std::string expression = x.__str__();
        std::string var;
        auto it = exp2var.find(expression);
        if (it == exp2var.end())
        {
            exp2var[expression] = var = tempPrefix + std::to_string(varCounter++);
            DEBUG_OUTPUT(std::cerr << "ASSIGN " << var << ": " << str_ << " (" << name << ")" << std::endl);
        }
        else
        {
            var = it->second;
            DEBUG_OUTPUT(std::cerr << "Found " << var << " = " << expression << " (" << name << ")" << std::endl);
        }
        var2code[var] = str_;
        str_ = var;
    }
    void _print_pow(std::ostringstream &o, const RCP<const Basic> &a,
                    const RCP<const Basic> &b)
    {
        std::ostringstream o2;
        if (eq(*b, *rational(1, 2))) {
            o2 << "sqrt(" << apply(a) << ")";
        } else {
            o2 << "pow(" << apply(a) << ", " << apply(b) << ")";
        }
        str_ = o2.str();
        assignToVar(Pow(a, b), "_print_pow");
        o << str_;
    }
#define PROCESS(name) void bvisit(const name &x)\
    {\
        CodePrinter:: bvisit(x);\
        assignToVar(x, #name);\
    }
#define IGNORE(name) void bvisit(const name &x)\
    { CodePrinter:: bvisit(x); }
    
#define NOT_IMPL(name) void bvisit(const name &x)\
    { throw SymEngineException("Not supported"); }
    
    IGNORE(Integer)
    IGNORE(Symbol)
    IGNORE(RealDouble)
    IGNORE(Infty)
    IGNORE(NaN)
    IGNORE(Pow)
    
    PROCESS(Rational)
    PROCESS(ComplexDouble)
    PROCESS(Equality)
    PROCESS(Unequality)
    PROCESS(LessThan)
    PROCESS(StrictLessThan)
    PROCESS(Interval)
    PROCESS(BooleanAtom)
    PROCESS(And)
    PROCESS(Or)
    PROCESS(Xor)
    PROCESS(Not)
    PROCESS(Contains)
    PROCESS(Piecewise)
    PROCESS(Union)
    PROCESS(Complement)
    PROCESS(ImageSet)
    PROCESS(ConditionSet)
    PROCESS(RealMPFR)
    PROCESS(ComplexMPC)
    PROCESS(Add)
    PROCESS(Mul)
    PROCESS(UIntPoly)
    PROCESS(URatPoly)
    PROCESS(UExprPoly)
    PROCESS(Log)
    PROCESS(Constant)
    PROCESS(Function)
    PROCESS(FunctionSymbol)
    PROCESS(NumberWrapper)
    PROCESS(MIntPoly)
    PROCESS(MExprPoly)
    
    NOT_IMPL(Basic)
    NOT_IMPL(Complex)
    NOT_IMPL(EmptySet)
    NOT_IMPL(FiniteSet)
    NOT_IMPL(UniversalSet)
    NOT_IMPL(UnivariateSeries)
    NOT_IMPL(Derivative)
    NOT_IMPL(Subs)
    NOT_IMPL(GaloisField)
};

} // namespace internal

} // namespace SymEngine

namespace codegenvar{

CodeGenerator::CodeGenerator(std::string varName)
{
    options.varName = varName;
}

std::string CodeGenerator::operator()(const Symbol& symbol)const
{
    SymEngine::internal::CodePrinter2 printer;
    printer.tempPrefix = options.tempPrefix;
    auto output = printer.apply(symbol.p->expression);

    auto it = printer.var2code.find(output);
    if (it != printer.var2code.end())
    {
        output = printer.var2code.at(output);
        printer.var2code.erase(it);
    }
        
    std::string result;    
    for (auto it: printer.var2code)
        result += options.varDeclaration + " " + it.first + " = " + it.second + ";\n";
    
    result += options.varName + " = " + output + ";\n";
    return result;
}

std::string Symbol::toCode()const
{
    return CodeGenerator()(*this);
}

}// namespace codegenvar
