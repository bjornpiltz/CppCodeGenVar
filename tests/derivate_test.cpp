#include "SnavelyReprojectionError.h"
#include "Symbol.h"
#include "generator/DerivateGenerator.h"
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
    evaluate.addConstants({"obs_x", "obs_y"});
    
    auto dx = evaluate(residuals[0]);
    for(auto var: residuals[0].getVariableNames())
        std::cerr << "dx/d" << var << " = " << dx[var] << std::endl << std::endl;
    std::cerr << std::endl;
    
    auto dy = evaluate(residuals[1]);
    for(auto var: residuals[1].getVariableNames())
        std::cerr << "dy/d" << var << " = " << dy[var] << std::endl << std::endl;
    
    return 0;
}