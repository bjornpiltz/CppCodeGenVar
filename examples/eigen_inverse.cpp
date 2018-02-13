#include <codegenvar/Eigen>
#include <codegenvar/BooleanEvaluator.h>
#include <Eigen/Dense>
#include <iostream>

using namespace codegenvar;

int main()
{    
    Mat2 m2 = namedMatrix("m", 2, 2);
    std::cerr << m2.inverse() << std::endl << std::endl;

    BooleanEvaluator evaluator;
    Mat m = namedMatrix("m", 2, 2);
    Mat inv(2, 2);
    do
    {
        inv |= m.inverse().eval();
    } while (!evaluator.isFullyEvaluated());

    std::cerr << inv(0,0) << std::endl;
    return 0;
}
