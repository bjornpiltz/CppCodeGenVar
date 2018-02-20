#include "SnavelyReprojectionError.h"
#include <codegenvar/Eigen>
#include <codegenvar/BooleanEvaluator.h>
#include <iostream>

using namespace codegenvar;

int main()
{
    const Vec2 obs = namedVector<2>("obs");
    const Vec3 r = namedVector<3>("r"), t=namedVector<3>("t"), p=namedVector<3>("p");
    const Symbol f("f"), l1("l1"), l2("l2");

    SnavelyReprojectionError<Symbol> snavely(obs);
    
    Vec2 residuals;
    BooleanEvaluator evaluator;
    do
    {
        residuals |= snavely.projectPoint(r.data(), t.data(), f, l1, l2, p.data());
    }
    while (!evaluator.isFullyEvaluated());
    
    std::cout << "res1 := " << residuals[0].toString() << std::endl << std::endl;
    std::cout << "res2 := " << residuals[1].toString() << std::endl;
    
    return 0;
}