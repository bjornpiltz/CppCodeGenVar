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
    auto vars = Symbol::Array(
          "r1", "r2", "r3", 
          "t1", "t2", "t3",
          "f" , "l1", "l2",
          "p1", "p2", "p3");
    
    Symbol residuals[2];
    BooleanEvaluator evaluator;
    do
    {
        Symbol tmp[2];
        function(&vars[0], &vars[9], tmp);
        residuals[0] |= tmp[0];
        residuals[1] |= tmp[1];
    }
    while (!evaluator.isFullyEvaluated());

    DerivateEvaluator evaluate;
    evaluate.options.constants.insert({"obs_x", "obs_y"});
    
    auto dx = evaluate(residuals[0]);
    for(auto var: residuals[0].getVariableNames())
        std::cout << "dx/d" << var << " :\n" << CodeGenerator("dx_d" + var)(dx[var]) << std::endl << std::endl;
    std::cout << std::endl;

    auto dy = evaluate(residuals[1]);
    for(auto var: residuals[1].getVariableNames())
        std::cout << "dy/d" << var << " :\n" << CodeGenerator("dy_d" + var)(dy[var]) << std::endl << std::endl;
    
    return 0;
}
