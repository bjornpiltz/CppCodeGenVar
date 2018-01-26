#pragma once
#include <string>

namespace codegenvar {

/*
 * This class serves solely as a thin wrapper for scalars. 
 */
class Scalar
{
public:
    Scalar()=delete;
    Scalar(double d);
    Scalar(int i);
    Scalar(long int i);
    Scalar(long long i);
    
    long long toInt()const;
    double toDouble()const;

    std::string toString()const;
    
    bool isInt()const;
    bool isFloat()const;

private:
    enum State{Float, Int};
    State state;
    union {double d; long long i;}val;
};

}// namespace codegenvar