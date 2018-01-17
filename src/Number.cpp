#include "codegenvar/Number.h"
#include "codegenvar/internal/Error.h"
#include <cmath>

namespace codegenvar {

Number::Number(double d)
    : state(Float)
{
    val.d = d;
}

Number::Number(int i)
    : state(Int)
{
    val.i = i;
}

Number::Number(long int i)
    : state(Int)
{
    val.i = i;
}

Number::Number(long long int i)
    : state(Int)
{
    val.i = i;
}

long long Number::toInt()const
{
    CONDITION(state!=NaN, "NaN");
    return state==Int ? val.i : std::llround(val.d);
}

double Number::toDouble()const
{
    CONDITION(state!=NaN, "NaN");
    return state==Int ? val.i : val.d;
}

std::string Number::toString()const
{
    if(state==NaN)
        return "NaN";
    return state==Int ? std::to_string(val.i) : std::to_string(val.d);
}

bool Number::isZero()const
{
    CONDITION(state!=NaN, "NaN");
    return state==Int ? val.i==0 : std::abs(val.d)<1e-16;
}

bool Number::isOne()const
{
    CONDITION(state!=NaN, "NaN");
    return state==Int ? val.i==1 : std::abs(val.d-1.0)<1e-16;
}

bool Number::isValid()const
{
    return state!=NAN;
}

Number operator-(const Number& x)
{
    CONDITION(x.state!=Number::NaN, "NaN");
    if(x.state==Number::Int)
        return Number(-x.val.i);
    else
        return Number(-x.val.d);
}

}// namespace codegenvar
