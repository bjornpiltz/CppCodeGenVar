#include <codegenvar/generator/CodeGenerator.h>
#include <iostream>

template <typename Scalar>
Scalar fun(const Scalar& x, const Scalar& y, const Scalar& theta)
{
    return pow(x, 2)*cos(theta) + 2*y;
}

int main()
{
    using namespace codegenvar;
    const Symbol x("x"), y("y"), theta("theta");
    Symbol z = fun(x, y, theta);
    
    CodeGenerator generate;
    generate.options.varDeclaration  = "float";
    generate.options.tempPrefix  = "v";
    generate.options.varName  = "z";
    
    std::cout << generate(Symbol(2)) << std::endl;
    std::cout << generate(y) << std::endl;
    std::cout << generate(-y) << std::endl;
    std::cout << generate(1/y) << std::endl;
    std::cout << generate(1/(1+y)) << std::endl;
    std::cout << generate(1/(2*y)) << std::endl;
    std::cout << generate(cos(1/y)) << std::endl;
    std::cout << generate(pow(cos(1/y), 3)) << std::endl;
    std::cout << generate(z) << std::endl;
    std::cout << generate(Symbol(2.0)) << std::endl;
    
    return 0;
}