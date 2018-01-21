#pragma once
#include "Symbol.h"
#include <Eigen/Core>

namespace codegenvar
{
    typedef Eigen::Matrix<Symbol, Eigen::Dynamic, 1> Vec;
    typedef Eigen::Matrix<Symbol, Eigen::Dynamic, Eigen::Dynamic> Mat;
    typedef Eigen::Matrix<Symbol, 2, 2> Mat2;
    typedef Eigen::Matrix<Symbol, 2, 3> Mat23;
    typedef Eigen::Matrix<Symbol, 3, 3> Mat3;
    typedef Eigen::Matrix<Symbol, 2, 1> Vec2;
    typedef Eigen::Matrix<Symbol, 3, 1> Vec3;
}

// The following lets us put Symbols in Eigen matrices:
namespace Eigen
{

template<> 
struct NumTraits<codegenvar::Symbol>
{
    typedef codegenvar::Symbol Symbol; 
    typedef Symbol Real;
    typedef Symbol NonInteger;
    typedef Symbol Nested;
    typedef Symbol Literal;
    
    static Symbol dummy_precision() {return std::move(Symbol(1e-12));}
    static inline Real epsilon() {return Real(std::numeric_limits<double>::epsilon());}
    static inline int digits10() { return NumTraits<double>::digits10(); }
    
    enum {
        IsComplex = 0,
        IsInteger = 0,
        IsSigned,
        ReadCost = 1,
        AddCost = 1,
        MulCost = 3,
        HasFloatingPoint = 1,
        RequireInitialization = 1
    };
    
    template<bool Vectorized>
    struct Div {
        enum {
            AVX = false,
            Cost = 3
        };
    };
};

template <typename BinaryOp>
struct ScalarBinaryOpTraits<codegenvar::Symbol, double, BinaryOp> 
{
    typedef codegenvar::Symbol ReturnType;
};

template <typename BinaryOp>
struct ScalarBinaryOpTraits<double, codegenvar::Symbol, BinaryOp> 
{
    typedef codegenvar::Symbol ReturnType;
};

}  // namespace Eigen
