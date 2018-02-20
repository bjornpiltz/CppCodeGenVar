#include <codegenvar/Eigen>
#include <codegenvar/BooleanEvaluator.h>
#include <Eigen/Dense>
#include <iostream>

using namespace codegenvar;

int main()
{    
    Mat2 m2 = namedMatrix("m", 2, 2);
    std::cout << "m' = " << std::endl << m2.inverse() << std::endl << std::endl;

    BooleanEvaluator evaluator;
    Mat m = namedMatrix("m", 2, 2);
    Mat inv(2, 2);
    do
    {
        inv |= m.inverse();
    } while (!evaluator.isFullyEvaluated());

    std::cout << "m11' = " << std::endl << inv(0,0) << std::endl;
    return 0;
}
