#pragma once
#include "Symbol.h"
#include <Eigen/Core>

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

namespace codegenvar
{
    typedef Eigen::Matrix<Symbol, -1, 1> Vec;
    typedef Eigen::Matrix<Symbol, -1, -1> Mat;
    typedef Eigen::Matrix<Symbol, 2, 2> Mat2;
    typedef Eigen::Matrix<Symbol, 2, 3> Mat23;
    typedef Eigen::Matrix<Symbol, 3, 3> Mat3;
    typedef Eigen::Matrix<Symbol, 2, 1> Vec2;
    typedef Eigen::Matrix<Symbol, 3, 1> Vec3;
    

    /* For example:
     * Vec2 x = namedVector<2>("x) 
     * creates a vector with symbols named "x_1" and "x_2".
     */
    template <int n = -1>
    Eigen::Matrix<Symbol, n, 1> namedVector(std::string name, int size = -1)
    {
        Eigen::Matrix<Symbol, n, 1> result(n == -1 ? size : n);
        for (int i= 0; i<n; i++)
            result(i) = Symbol(name + "_" + std::to_string(i+1));
        return result;
    }
    Mat namedMatrix(std::string name, int rows, int cols)
    {
        Mat result(rows, cols);
        for (int r = 0; r<rows; r++)
            for (int c = 0; c<cols; c++)
                result(r, c) = Symbol(name + "_" + std::to_string(r+1) + std::to_string(c+1));
        return result;
    }
    template<typename Derived>
    void operator |=(Eigen::DenseBase<Derived> & dst, const Eigen::DenseBase<Derived> & src)
    {
        if (dst.rows() != src.rows() || dst.cols() != src.cols())
            dst.resize(src.rows(), src.cols());
    
        for (int r = 0; r<src.rows(); r++)
            for (int c = 0; c<src.cols(); c++)
                dst(r, c) |= src(r, c);
    }
}