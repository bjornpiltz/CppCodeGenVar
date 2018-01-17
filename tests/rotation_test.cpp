#include "SnavelyReprojectionError.h"
#include "Symbol.h"
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
    
    Symbol residuals[2];;
    function(&vars[0], &vars[9], residuals);
    
    std::cerr << "res1 := " << residuals[0].toString() << std::endl << std::endl;
    std::cerr << "res2 := " << residuals[1].toString() << std::endl;
    return 0;
}