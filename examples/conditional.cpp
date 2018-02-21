#include <codegenvar/Symbol.h>
#include <codegenvar/BooleanEvaluator.h>
#include <iostream>

using namespace codegenvar;

template <typename Scalar>
Scalar foo(Scalar x, Scalar y)
{
    if (y == 0)
        return x;

    return x/y < 0 ? -x/y : x/y;
}

// main:
int main()
{    
    Symbol x("x"), y("y");
    try
    {
        Symbol z = foo(x, y);
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
    
    // Fix:
    BooleanEvaluator evaluator;
    Symbol z;
    do
    {
        std::cout << ".";
        z |= foo(x, y);
    }
    while (!evaluator.isFullyEvaluated());
    std::cout << std::endl << "z = " << z << std::endl;
    return 0;
}