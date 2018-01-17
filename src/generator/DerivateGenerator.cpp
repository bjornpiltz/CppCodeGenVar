#include "generator/DerivateGenerator.h"
#include "internal/Expression.h"
#include "internal/ConstantExpression.h"
#include "internal/VariableExpression.h"
#include "internal/Error.h"
#include <ceres/jet.h>

namespace codegenvar{

using namespace internal;

namespace derivateevaluator{

ceres::Jet<Symbol, -1> evaluate(ConstPtr expr, const std::set<std::string>& constants, std::vector<std::string>& variableNames)
{
    typedef ceres::Jet<Symbol, -1> Jet;
    typedef Eigen::Matrix<Symbol, -1, 1> Vec;
    
    switch(expr->type())
    {
    case TypeConstantExpression:
    {
        const auto number = std::dynamic_pointer_cast<const ConstantExpression>(expr)->toNumber();
        return Jet(Symbol(number), Vec::Zero(variableNames.size()));
     }   
    case TypeVariableExpression: 
    {
        const std::string name = std::dynamic_pointer_cast<const VariableExpression>(expr)->toString();
        Vec infPart = Vec::Zero(variableNames.size());

        // We don't need to track constants like "pi".
        if(!constants.count(name))
        {
            auto it = std::find(variableNames.begin(), variableNames.end(), name);
            CONDITION (it != variableNames.end(), "Couldn't find variable:"+name);
            
            auto index = std::distance(variableNames.begin(), it);
            CONDITION (index>-1 && index < variableNames.size(), "Index out of bounds.");
            
            if (it != variableNames.end())
                infPart[index] = Symbol(1);
        }
        return Jet(Symbol(name), infPart);
     }   
    default:
        break;
    };
        
    ceres::Jet<Symbol, -1> result = derivateevaluator::evaluate(expr->child(0), constants, variableNames);
    switch(expr->type())
    {
    case TypeUnaryMinus:            return -result;
    case TypeUnaryDiv:              return Symbol(1)/result;
        
    case TypeUnaryFunction_abs:     return abs(result);
    case TypeUnaryFunction_log:     return log(result);
    case TypeUnaryFunction_exp:     return exp(result);
    case TypeUnaryFunction_sqrt:    return sqrt(result);
    case TypeUnaryFunction_cos:     return cos(result);
    case TypeUnaryFunction_acos:    return acos(result);
    case TypeUnaryFunction_sin:     return sin(result);
    case TypeUnaryFunction_asin:    return asin(result);
    case TypeUnaryFunction_tan:     return tan(result);
    case TypeUnaryFunction_atan:    return atan(result);
    case TypeUnaryFunction_sinh:    return sinh(result);
    case TypeUnaryFunction_cosh:    return cosh(result);
    case TypeUnaryFunction_tanh:    return tanh(result);
    case TypeUnaryFunction_floor:   return floor(result);
    case TypeUnaryFunction_ceil:    return ceil(result);
        
    case TypeTernaryMul:
        for (int i = 1; i<expr->childCount(); i++)
            result *= evaluate(expr->child(i), constants, variableNames);
        return result;
        
    case TypeTernaryPlus:
        for (int i = 1; i<expr->childCount(); i++)
            result += evaluate(expr->child(i), constants, variableNames);
        return result;
    
    case TypeBinaryFunctionPow:
        return pow(evaluate(expr->child(0), constants, variableNames), evaluate(expr->child(1), constants, variableNames));
        
    default:
        ERROR("Unhandled case:");
    }
}

}// namespace derivateevaluator

std::map<std::string, Symbol> DerivateEvaluator::operator()(const Symbol& symbol)
{
    std::vector<std::string> variableNames;
    std::map<std::string, Symbol> result;
    auto tmp = symbol.getVariableNames();
    variableNames.clear();
    variableNames.insert(variableNames.begin(), tmp.begin(), tmp.end());
    
    if(variableNames.empty())
        return {};
    auto jet = derivateevaluator::evaluate(symbol.expr, constants, variableNames);
    
    if (jet.v.size()==0)
        return result;
    CONDITION(jet.v.size()==variableNames.size(), "Wrong size of jets!!");
    for (int  i = 0; i< variableNames.size(); i++)
        result[variableNames[i]] = jet.v(i);
    return result;
}

void DerivateEvaluator::addConstants(const std::set<std::string>& v)
{
    constants.insert(v.begin(), v.end());
}

}// namespace codegenvar
