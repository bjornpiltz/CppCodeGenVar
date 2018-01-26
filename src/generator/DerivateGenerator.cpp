#include <codegenvar/generator/DerivateGenerator.h>
#include "Error.h"
#include "SymbolPrivate.h"
#include <symengine/derivative.h>

namespace codegenvar{

using namespace internal;
using SymEngine::diff;
using SymEngine::symbol;

std::map<std::string, Symbol> DerivateEvaluator::operator()(const Symbol& s)const
{
    std::map<std::string, Symbol> result;
    for (auto var: s.getVariableNames())
        result[var] = SymbolPrivate::ctor(diff(s.p->expression, symbol(var)));
    return result;
}

}// namespace codegenvar
