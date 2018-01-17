#include "codegenvar/generator/CodeGenerator.h"
#include "codegenvar/internal/Error.h"
#include "codegenvar/internal/AbstractExpression.h"
#include "codegenvar/internal/ConstantExpression.h"
#include "codegenvar/internal/VariableExpression.h"
#include <algorithm>

namespace codegenvar{
    
using namespace internal;

namespace codegenerator {

struct Expression
{
    std::string string;
    std::set<std::string> deps;
};

Expression evaluate(
        const CodeGenerator::Options& options, 
        std::map<std::string, Expression>& assignments,
        internal::ConstPtr expr,
        int& variableCounter
    )
{
    switch(expr->type())
    {
    case TypeConstantExpression:
    {
        std::string value = std::dynamic_pointer_cast<const ConstantExpression>(expr)->toString();
        return Expression{value, {}};
    }
        
    case TypeVariableExpression: 
    {
        std::string var = std::dynamic_pointer_cast<const VariableExpression>(expr)->toString();
        return Expression{var, {var}};
    }
        
    default:
        break;
    };
    
    auto evalChild = [&](internal::ConstPtr expr, int childNum)
    {
        Expression expression = evaluate(options, assignments, expr->child(childNum), variableCounter);

        if (expression.deps.empty())
        {
            // A constant needs no substitution.
        }
        else if (expression.deps.size() == 1 && *expression.deps.begin() == expression.string)
        {
            // A variable needs no substitution.
        }
        else
        {
            // See if we can reuse a temporary:
            for (const auto& it: assignments)
                if (it.second.string == expression.string)
                {
                    expression.string = it.first;
                    expression.deps = it.second.deps;
                    return expression;
                }

            // Come up with a variable name:
            std::string varname = options.tempPrefix + std::to_string(++variableCounter);

            // Substitute the expression with the name
            assignments[varname] = expression;
            expression.string = varname;
            expression.deps.insert(varname);
        }
        return expression;
    };
    Expression arg1 = evalChild(expr, 0);

    switch(expr->type())
    {
    // The Unary operators:
    case TypeUnaryMinus:            return Expression{     "-" + arg1.string,       arg1.deps };
    case TypeUnaryDiv:              return Expression{  "1.0/" + arg1.string,       arg1.deps };
        
    // The Unary functions:
    case TypeUnaryFunction_abs:     return Expression{  "abs(" + arg1.string + ")", arg1.deps };
    case TypeUnaryFunction_log:     return Expression{  "log(" + arg1.string + ")", arg1.deps };
    case TypeUnaryFunction_exp:     return Expression{  "exp(" + arg1.string + ")", arg1.deps };
    case TypeUnaryFunction_sqrt:    return Expression{ "sqrt(" + arg1.string + ")", arg1.deps };
    case TypeUnaryFunction_cos:     return Expression{  "cos(" + arg1.string + ")", arg1.deps };
    case TypeUnaryFunction_acos:    return Expression{ "acos(" + arg1.string + ")", arg1.deps };
    case TypeUnaryFunction_sin:     return Expression{  "sin(" + arg1.string + ")", arg1.deps };
    case TypeUnaryFunction_asin:    return Expression{ "asin(" + arg1.string + ")", arg1.deps };
    case TypeUnaryFunction_tan:     return Expression{  "tan(" + arg1.string + ")", arg1.deps };
    case TypeUnaryFunction_atan:    return Expression{ "atan(" + arg1.string + ")", arg1.deps };
    case TypeUnaryFunction_sinh:    return Expression{ "sinh(" + arg1.string + ")", arg1.deps };
    case TypeUnaryFunction_cosh:    return Expression{ "cosh(" + arg1.string + ")", arg1.deps };
    case TypeUnaryFunction_tanh:    return Expression{ "tanh(" + arg1.string + ")", arg1.deps };
    case TypeUnaryFunction_floor:   return Expression{"floor(" + arg1.string + ")", arg1.deps };
    case TypeUnaryFunction_ceil:    return Expression{ "ceil(" + arg1.string + ")", arg1.deps };
        
    // The n-ary operators:
    
    case TypeTernaryMul:
    {
        Expression expression;
        expression.string = arg1.string;
        expression.deps = arg1.deps;
        for (int i = 1; i < expr->childCount(); i++)
        {
            bool isDiv = expr->child(i)->type() == TypeUnaryDiv;
            Expression argn = isDiv ? evalChild(expr->child(i), 0) : evalChild(expr, 1);
            expression.string += (isDiv ? "/" : "*") + argn.string;
            expression.deps.insert(argn.deps.begin(), argn.deps.end());
        }
        return expression;
    }
    
    case TypeTernaryPlus:
    {
        Expression expression;
        expression.string = arg1.string;
        expression.deps = arg1.deps;
        for (int i = 1; i < expr->childCount(); i++)
        {
            bool isNeg = expr->child(i)->type() == TypeUnaryMinus;
            Expression argn = isNeg ? evalChild(expr->child(i), 0) : evalChild(expr, 1);
            expression.string += (isNeg ? " - " : " + ") + argn.string;
            expression.deps.insert(argn.deps.begin(), argn.deps.end());
        }
        return expression;
    }

    // The binary functions:
    case TypeBinaryFunctionPow:
    {
        auto deps = arg1.deps;
        Expression arg2 = evalChild(expr, 1);
        deps.insert(arg2.deps.begin(), arg2.deps.end());
        return Expression{ "pow(" + arg1.string + ", " + arg2.string + ")", deps };
    }
        
    default:
        ERROR("Unhandled case:");
    }
}

}// namespace codegenerator

CodeGenerator::CodeGenerator(std::string varName)
{
    options.varName = varName;
}

std::string CodeGenerator::operator()(const Symbol& symbol)const
{
    using namespace codegenerator;

    // Generate all assignments by calling the recursive evaluate():
    std::map<std::string, Expression> assignments;
    int variableCounter = 0;
    assignments[options.varName] = evaluate(options, assignments, symbol.expr, variableCounter);
    
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
                for (auto dependent: assignments.at(string).deps)
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
