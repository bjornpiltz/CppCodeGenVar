#include "codegenvar/Symbol.h"
#include <cmath>
#include <iostream>

int main()
{
    using namespace codegenvar;
    
    Symbol a("a"), b("b"), c("c");
    Symbol x = sqrt(a + b*c) * pow(b, 2) / c;
    std::cout << x.toString();
    
    return 0;
}