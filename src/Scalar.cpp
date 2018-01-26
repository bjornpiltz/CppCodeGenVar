#include <codegenvar/Scalar.h>
#include <cmath>

namespace codegenvar {

Scalar::Scalar(double d)
    : state(Float)
{
    val.d = d;
}

Scalar::Scalar(int i)
    : state(Int)
{
    val.i = i;
}

Scalar::Scalar(long int i)
    : state(Int)
{
    val.i = i;
}

Scalar::Scalar(long long int i)
    : state(Int)
{
    val.i = i;
}

long long Scalar::toInt()const
{
    return state==Int ? val.i : std::llround(val.d);
}

double Scalar::toDouble()const
{
    return state==Int ? val.i : val.d;
}

bool Scalar::isInt()const
{
    return state == Int;
}

bool Scalar::isFloat()const
{
    return state == Float;
}

}// namespace codegenvar
