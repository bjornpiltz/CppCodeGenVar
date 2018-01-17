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
            result *= evaluate(expr->child(i));
        return result;
        
    case TypeTernaryPlus:
        for (int i = 1; i<expr->childCount(); i++)
            result += evaluate(expr->child(i));
        return result;
    
    case TypeBinaryFunctionPow:
        return pow(evaluate(expr->child(0)), evaluate(expr->child(1)));
        
    default:
        ERROR("Unhandled case:");
    }
}

}// namespace numericalevaluator

double NumericalEvaluator::operator()(const Symbol& symbol)const
{
    return numericalevaluator::evaluate(symbol.expr);
}

}// namespace codegenvar
