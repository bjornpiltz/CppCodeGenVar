#include <codegenvar/generator/CodeGenerator.h>
#include "Error.h"
#include <algorithm>
//#include <symengine/codegen.h>

namespace codegenvar{
    
using namespace internal;

namespace codegenerator {


}// namespace codegenerator

CodeGenerator::CodeGenerator(std::string varName)
{
    options.varName = varName;
}

std::string CodeGenerator::operator()(const Symbol& symbol)const
{
    
    return symbol.toString();//TODO: WIP
    using namespace codegenerator;
    //SymEngine::ccode(symbo)
    std::string result;
#if 0
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
#endif
    return result;
}

}// namespace codegenvar
