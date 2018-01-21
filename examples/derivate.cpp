#include "SnavelyReprojectionError.h"
#include <codegenvar/Symbol.h>
#include <codegenvar/generator/DerivateGenerator.h>
#include <codegenvar/generator/CodeGenerator.h>
#include <iostream>

int main()
{
    using namespace codegenvar;
    
    SnavelyReprojectionError<Symbol> function(Symbol("obs_x"), Symbol("obs_y"));
    const auto vars = Symbol::variables(
    {
          "r1", "r2", "r3", 
          "t1", "t2", "t3",
          "f" , "l1", "l2",
          "p1", "p2", "p3"
    });
    
    Symbol residuals[2];
    function(&vars[0], &vars[9], residuals);
    
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
