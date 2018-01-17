#pragma once
#include <string>

namespace codegenvar {

class Number
{
public:
    Number(double d);
    Number(int i);
    Number(long int i);
    Number(long long i);
    
    long long toInt()const;
    double toDouble()const;
    std::string toString()const;
    
    bool isValid()const;
    bool isZero()const;
    bool isOne()const;
    friend Number operator-(const Number& x);

private:
    enum State{NaN, Float, Int};
    State state = NaN;
    union {double d; long long i;}val;
};

}// namespace codegenvar