#include "codegenvar/Symbol.h"
#include <cmath>
#include <iostream>

using namespace codegenvar;

int main()
{    
    Symbol a("a"), b("b"), c("c");
    Symbol x = sqrt(a + b*c) * pow(b, 2) / c;
 
    std::cout << x.toString() << std::endl;
    return 0;
}