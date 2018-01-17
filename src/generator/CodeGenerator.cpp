#include "generator/CodeGenerator.h"
#include "internal/Error.h"
#include "internal/AbstractExpression.h"
#include "internal/ConstantExpression.h"
#include "internal/VariableExpression.h"
#include <algorithm>

namespace codegenvar{

using namespace internal;

namespace codegenerator {

struct Exp
{
    std::string string;
    std::set<std::string> vars;
};

void evaluate(
        const CodeGenerator::Options& options, 
        const std::string& varName, 
        std::map<std::string, Exp>& assignments, 
        internal::ConstPtr expr)
{
    switch(expr->type())
    {
    case TypeConstantExpression:
    {
        std::string value = std::dynamic_pointer_cast<const ConstantExpression>(expr)->toString();
        assignments[varName] = {value, {}};
        return;
    }
        
    case TypeVariableExpression: 
    {
        std::string var = std::dynamic_pointer_cast<const VariableExpression>(expr)->toString();
        assignments[varName] = {var, {var}};
        return;
    }
        
    default:
        break;
    };
    
    static int inc = 1;//TODO: not global   
    std::string arg1 = options.tempPrefix + std::to_string(inc++);
    evaluate(options, arg1, assignments, expr->child(0));
            
    switch(expr->type())
    {
    // The Unary operators:
    case TypeUnaryMinus:            assignments[varName] = {     "-" + arg1,       {arg1} }; return;
    case TypeUnaryDiv:              assignments[varName] = {  "1.0/" + arg1,       {arg1} }; return;
        
    // The Unary functions:
    case TypeUnaryFunction_abs:     assignments[varName] = {  "abs(" + arg1 + ")", {arg1} }; return;
    case TypeUnaryFunction_log:     assignments[varName] = {  "log(" + arg1 + ")", {arg1} }; return;
    case TypeUnaryFunction_exp:     assignments[varName] = {  "exp(" + arg1 + ")", {arg1} }; return;
    case TypeUnaryFunction_sqrt:    assignments[varName] = { "sqrt(" + arg1 + ")", {arg1} }; return;
    case TypeUnaryFunction_cos:     assignments[varName] = {  "cos(" + arg1 + ")", {arg1} }; return;
    case TypeUnaryFunction_acos:    assignments[varName] = { "acos(" + arg1 + ")", {arg1} }; return;
    case TypeUnaryFunction_sin:     assignments[varName] = {  "sin(" + arg1 + ")", {arg1} }; return;
    case TypeUnaryFunction_asin:    assignments[varName] = { "asin(" + arg1 + ")", {arg1} }; return;
    case TypeUnaryFunction_tan:     assignments[varName] = {  "tan(" + arg1 + ")", {arg1} }; return;
    case TypeUnaryFunction_atan:    assignments[varName] = { "atan(" + arg1 + ")", {arg1} }; return;
    case TypeUnaryFunction_sinh:    assignments[varName] = { "sinh(" + arg1 + ")", {arg1} }; return;
    case TypeUnaryFunction_cosh:    assignments[varName] = { "cosh(" + arg1 + ")", {arg1} }; return;
    case TypeUnaryFunction_tanh:    assignments[varName] = { "tanh(" + arg1 + ")", {arg1} }; return;
    case TypeUnaryFunction_floor:   assignments[varName] = {"floor(" + arg1 + ")", {arg1} }; return;
    case TypeUnaryFunction_ceil:    assignments[varName] = { "ceil(" + arg1 + ")", {arg1} }; return;
        
        
    // The n-ary operators:
    case TypeTernaryMul:
    {
        Exp expression;
        expression.string = arg1;
        expression.vars.insert(arg1);
        for (int i = 1; i<expr->childCount(); i++)
        {
            std::string argn = options.tempPrefix + std::to_string(inc++);
            if(expr->child(i)->type()==TypeUnaryDiv)
            {
                evaluate(options, arg1, assignments, expr->child(i)->child());
                expression.string = "/"+argn;
                expression.vars.insert(argn);
            }
            else
            {
                evaluate(options, arg1, assignments, expr->child(i));
                expression.string = "*"+argn;
                expression.vars.insert(argn);
            }
        }
        assignments[varName]  = expression;    
        return;
    }

    case TypeTernaryPlus:
    {
        Exp expression;
        expression.string = arg1;
        expression.vars.insert(arg1);
        for (int i = 1; i<expr->childCount(); i++)
        {
            std::string argn = options.tempPrefix + std::to_string(inc++);
            if(expr->child(i)->type()==TypeUnaryMinus)
            {
                evaluate(options, arg1, assignments, expr->child(i)->child());
                expression.string = "-" + argn;
                expression.vars.insert(argn);
            }
            else
            {
                evaluate(options, arg1, assignments, expr->child(i));
                expression.string = "+" + argn;
                expression.vars.insert(argn);
            }
        }
        assignments[varName]  = expression;    
        return;
    }
        
        
    // The binary functions:
    case TypeBinaryFunctionPow:
    {
        std::string arg2 = "tmp_" + std::to_string(inc++);
        evaluate(options, arg2, assignments, expr->child(1));
        assignments[varName] = {"pow(" + arg1 + ", " + arg2 + ")", {arg1, arg2} }; return;
        return;
    }
        
    default:
        ERROR("Unhandled case:");
    }
}

}// namespace codegenerator

std::string CodeGenerator::operator()(const Symbol& symbol)const
{
    using namespace codegenerator;

    // Generate all assignments by calling the recursive evaluate():
    std::map<std::string, Exp> assignments;
    evaluate(options, options.varName, assignments, symbol.expr);
    
    // Sort them in the correct order: ("y = x" must come before "z = y", since z is dependent on y)
    std::vector<std::string> keys;
    for(auto const& it: assignments)
        keys.push_back(it.first);
    
    auto isDependentOn = [&](const std::string& a, const std::string& b)
    {
        std::set<std::string> stack{a};
        while (!stack.empty())
        {
            auto string = *stack.begin();
            if(string==b)
                return true;
            stack.erase(stack.begin());
            if(assignments.count(string))
                for (auto dependent: assignments.at(string).vars)
                    stack.insert(dependent);
        }
        return false;
    };
    std::sort(keys.begin(), keys.end(), [&](const std::string& a, const std::string& b)
    {
        if (isDependentOn(a, b))
            return false;
        if (isDependentOn(b, a))
            return true;
        return a<b;
    });
    
    // Format the output:
    std::string result;
    for (auto key: keys)
        result += options.varDeclaration + key + " = " + assignments.at(key).string + ";\n";
    return result;
}

}// namespace codegenvar
