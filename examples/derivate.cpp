#include "SnavelyReprojectionError.h"
#include <codegenvar/Symbol.h>
#include <codegenvar/generator/DerivateGenerator.h>
#include <codegenvar/generator/CodeGenerator.h>
#include <codegenvar/BooleanEvaluator.h>
#include <iostream>
#include <vector>

using namespace codegenvar;

int main()
{
    
    SnavelyReprojectionError<Symbol> function(Symbol("obs_x"), Symbol("obs_y"));
    std::vector<Symbol> vars;
    for(auto v: {
          "r1", "r2", "r3", 
          "t1", "t2", "t3",
          "f" , "l1", "l2",
          "p1", "p2", "p3"}
    )
    vars.emplace_back(v);
    
    Symbol residuals[2];
    std::map<std::string, TBA> result;
    BooleanEvaluator evaluator;
    do
    {
        function(&vars[0], &vars[9], residuals);
        result["x"] = residuals[0];
        result["y"] = residuals[1];
    }
    while (!evaluator.done(result));
    
    DerivateEvaluator evaluate;
    evaluate.options.constants.insert({"obs_x", "obs_y"});
    
    auto dx = evaluate(residuals[0]);
    for(auto var: residuals[0].getVariableNames())
        std::cerr << "dx/d" << var << " :\n" << CodeGenerator("dx_d" + var)(dx[var]) << std::endl << std::endl;
    std::cerr << std::endl;

    auto dy = evaluate(residuals[1]);
    for(auto var: residuals[1].getVariableNames())
        std::cerr << "dy/d" << var << " :\n" << CodeGenerator("dy_d" + var)(dy[var]) << std::endl << std::endl;
    
    return 0;
}
