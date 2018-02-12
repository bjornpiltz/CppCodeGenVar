#include "SnavelyReprojectionError.h"
#include <codegenvar/Symbol.h>
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
    
    std::cerr << "res1 := " << residuals[0].toString() << std::endl << std::endl;
    std::cerr << "res2 := " << residuals[1].toString() << std::endl;
    
    return 0;
}