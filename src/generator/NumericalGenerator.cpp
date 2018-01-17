#include "generator/NumericalGenerator.h"
#include "internal/Expression.h"
#include "internal/ConstantExpression.h"
#include "internal/VariableExpression.h"
#include "internal/Error.h"
#include <cmath>

namespace codegenvar{

using namespace internal;

namespace numericalevaluator{

double evaluate(ConstPtr expr)
{
    switch(expr->type())
    {
    case TypeConstantExpression:
        return std::dynamic_pointer_cast<const ConstantExpression>(expr)->toDouble();
        
    case TypeVariableExpression: 
        ERROR("Trying to numerically evaluate a variable");

    default:
        break;
    };
    
    double result = evaluate(expr->child(0));
    switch(expr->type())
    {
    case TypeUnaryMinus:            return -result;
    case TypeUnaryDiv:              return 1.0/result;
        
    case TypeUnaryFunction_abs:     return std::abs(result);
    case TypeUnaryFunction_log:     return std::log(result);
    case TypeUnaryFunction_exp:     return std::exp(result);
    case TypeUnaryFunction_sqrt:    return std::sqrt(result);
    case TypeUnaryFunction_cos:     return std::cos(result);
    case TypeUnaryFunction_acos:    return std::acos(result);
    case TypeUnaryFunction_sin:     return std::sin(result);
    case TypeUnaryFunction_asin:    return std::asin(result);
    case TypeUnaryFunction_tan:     return std::tan(result);
    case TypeUnaryFunction_atan:    return std::atan(result);
    case TypeUnaryFunction_sinh:    return std::sinh(result);
    case TypeUnaryFunction_cosh:    return std::cosh(result);
    case TypeUnaryFunction_tanh:    return std::tanh(result);
    case TypeUnaryFunction_floor:   return std::floor(result);
    case TypeUnaryFunction_ceil:    return std::ceil(result);
        
    case TypeTernaryMul:
        for (int i = 1; i<expr->childCount(); i++)
            result *= evaluate(expr->child(i));
        return result;
        
    case TypeTernaryPlus:
        for (int i = 1; i<expr->childCount(); i++)
            result += evaluate(expr->child(i));
        return result;
    
    case TypeBinaryFunctionPow:
        return std::pow(evaluate(expr->child(0)), evaluate(expr->child(1)));
        
    default:
        ERROR("Unhandled case:");
    }
}

}// namespace numericalevaluator

double NumericalEvaluator::operator()(const Symbol& symbol)const
{
    return numericalevaluator::evaluate(symbol.expr);
}

double Symbol::toDouble()const
{
    NumericalEvaluator toDouble;
    return toDouble(*this);
}

}// namespace codegenvar
