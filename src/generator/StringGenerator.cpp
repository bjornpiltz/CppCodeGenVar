#include "codegenvar/generator/StringGenerator.h"
#include "Expression.h"
#include "ConstantExpression.h"
#include "VariableExpression.h"
#include "Error.h"

namespace codegenvar{

using namespace internal;

std::string Symbol::toString()const
{
    StringEvaluator evaluate;
    return expr ? evaluate(*this) : "";
}

namespace stringevaluator {

std::string evaluate(ConstPtr expr);
std::string evaluateAndAddBraces(ConstPtr expr, int parentType)
{
    std::string str = evaluate(expr);
    if (parentType < expr->type()
    || (parentType <= LastTypeUnaryFunction && expr->type() <= LastTypeUnaryFunction))
        str =  "(" +  str + ")";
    return str;
}

std::string evaluate(ConstPtr expr)
{
    switch(expr->type())
    {
    case TypeConstantExpression:
        return std::dynamic_pointer_cast<const ConstantExpression>(expr)->toString();
        
    case TypeVariableExpression: 
        return std::dynamic_pointer_cast<const VariableExpression>(expr)->toString();

    default:
        break;
    };
    std::string result = evaluateAndAddBraces(expr->child(0), expr->type());
    switch(expr->type())
    {
    case TypeUnaryMinus:            return "-"  + result;
    case TypeUnaryDiv:              return "1/" + result;
        
    case TypeUnaryFunction_abs:     return "abs" + result;
    case TypeUnaryFunction_log:     return "log" + result;
    case TypeUnaryFunction_exp:     return "exp" + result;
    case TypeUnaryFunction_sqrt:    return "sqrt" + result;
    case TypeUnaryFunction_cos:     return "cos" + result;
    case TypeUnaryFunction_acos:    return "acos" + result;
    case TypeUnaryFunction_sin:     return "sin" + result;
    case TypeUnaryFunction_asin:    return "asin" + result;
    case TypeUnaryFunction_tan:     return "tan" + result;
    case TypeUnaryFunction_atan:    return "atan" + result;
    case TypeUnaryFunction_sinh:    return "sinh" + result;
    case TypeUnaryFunction_cosh:    return "cosh" + result;
    case TypeUnaryFunction_tanh:    return "tanh" + result;
    case TypeUnaryFunction_floor:   return "floor" + result;
    case TypeUnaryFunction_ceil:    return "ceil" + result;
        
    case TypeTernaryMul:
        for (int i = 1; i<expr->childCount(); i++)
        {
            if(expr->child(i)->type()==TypeUnaryDiv)
                result += "/" + evaluateAndAddBraces(expr->child(i)->child(), TypeUnaryDiv);
            else
                result += "*" + evaluateAndAddBraces(expr->child(i), expr->type());
        }
        break;
        
    case TypeTernaryPlus:
        for (int i = 1; i<expr->childCount(); i++)
        {
            if(expr->child(i)->type()==TypeUnaryMinus)
                result += "-" + evaluateAndAddBraces(expr->child(i)->child(), TypeUnaryMinus);
            else
                result += "+" + evaluateAndAddBraces(expr->child(i), expr->type());
        }
        break;
    
    case TypeBinaryFunctionPow:
        result = "pow(" + evaluate(expr->child(0)) + "," + evaluate(expr->child(1)) + ")";
        break;
        
    default:
        ERROR("Unhandled case:");
    }
    return result;
}
}// anonymous namespace

std::string StringEvaluator::operator()(const Symbol& symbol)const
{
    return stringevaluator::evaluate(symbol.expr);
}

}// namespace codegenvar