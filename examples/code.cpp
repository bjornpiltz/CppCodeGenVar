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
    
    std::cerr << generate(Symbol(2)) << std::endl;
    std::cerr << generate(y) << std::endl;
    std::cerr << generate(-y) << std::endl;
    std::cerr << generate(1/y) << std::endl;
    std::cerr << generate(cos(1/y)) << std::endl;
    std::cerr << generate(pow(cos(1/y), 3)) << std::endl;
    std::cerr << generate(z) << std::endl;
    
    return 0;
}