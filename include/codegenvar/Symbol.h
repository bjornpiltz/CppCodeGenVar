#pragma once
#include "Scalar.h"
#include <map>
#include <memory>
#include <set>
#include <string>
#include <iosfwd>

namespace codegenvar {

namespace internal {class SymbolPrivate;}

class Symbol
{
public:        
    typedef std::map<std::string, Scalar> Map;

public:        
    // Constructors creating constants.
    explicit Symbol(int value);
    explicit Symbol(long int value);
    explicit Symbol(long long int value);
    explicit Symbol(float value);
    explicit Symbol(double value);
    explicit Symbol(const Scalar& value);
    
    // Constructors creating variables or named constants.
    explicit Symbol(const std::string&);
    
    std::string toString()const;
    std::string toCode()const;
    double toDouble()const;
    
    Symbol resolved(const Map& symbolMap)const;
    
    std::set<std::string> getVariableNames()const;
    
    void expand();
    Symbol expanded()const;

    bool equals(const Symbol& other)const;
    
    // Unary '-':
    friend Symbol operator-(const Symbol& x);
    
    // Binary operators:
    friend Symbol operator + (const Symbol& lhs, const Symbol& rhs);
    friend Symbol operator - (const Symbol& lhs, const Symbol& rhs);
    friend Symbol operator * (const Symbol& lhs, const Symbol& rhs);
    friend Symbol operator / (const Symbol& lhs, const Symbol& rhs);
    
    Symbol& operator+=(const Symbol& other);
    Symbol& operator-=(const Symbol& other);
    Symbol& operator*=(const Symbol& other);
    Symbol& operator/=(const Symbol& other);
        
    Symbol inverse()const;
    
    // Unary functions:
    
    friend Symbol abs(const Symbol&);
    friend Symbol log(const Symbol&);
    friend Symbol exp(const Symbol&);
    friend Symbol sqrt(const Symbol&);
    friend Symbol cos(const Symbol&);
    friend Symbol acos(const Symbol&);
    friend Symbol sin(const Symbol&);
    friend Symbol asin(const Symbol&);
    friend Symbol tan(const Symbol&);
    friend Symbol atan(const Symbol&);
    friend Symbol sinh(const Symbol&);
    friend Symbol cosh(const Symbol&);
    friend Symbol tanh(const Symbol&);
    friend Symbol floor(const Symbol&);
    friend Symbol ceil(const Symbol&);
    friend Symbol pow(const Symbol&, const Symbol&);
    
    // Booleans:
    friend bool operator ==(const Symbol& lhs, const Symbol& rhs);
    friend bool operator !=(const Symbol& lhs, const Symbol& rhs);
    friend bool operator < (const Symbol& lhs, const Symbol& rhs);
    friend bool operator <=(const Symbol& lhs, const Symbol& rhs);
    friend bool operator > (const Symbol& lhs, const Symbol& rhs);
    friend bool operator >=(const Symbol& lhs, const Symbol& rhs);

    // Symbol op Scalar and Scalar op Symbol overloads
    friend Symbol operator + (const Scalar& lhs, const Symbol& rhs);
    friend Symbol operator + (const Symbol& lhs, const Scalar& rhs);
    friend Symbol operator - (const Scalar& lhs, const Symbol& rhs);
    friend Symbol operator - (const Symbol& lhs, const Scalar& rhs);
    friend Symbol operator * (const Scalar& lhs, const Symbol& rhs);
    friend Symbol operator * (const Symbol& lhs, const Scalar& rhs);
    friend Symbol operator / (const Scalar& lhs, const Symbol& rhs);
    friend Symbol operator / (const Symbol& lhs, const Scalar& rhs);
    
    Symbol& operator=(const Scalar& other);
    Symbol& operator+=(const Scalar& other);
    Symbol& operator-=(const Scalar& other);
    Symbol& operator*=(const Scalar& other);    
    Symbol& operator/=(const Scalar& other);
    
    friend Symbol pow(const Symbol&, const Scalar&);
    friend Symbol pow(const Scalar&, const Symbol&);
    
    friend bool operator ==(const Symbol& lhs, const Scalar& rhs);
    friend bool operator ==(const Scalar& lhs, const Symbol& rhs);
    friend bool operator < (const Symbol& lhs, const Scalar& rhs);
    friend bool operator < (const Scalar& lhs, const Symbol& rhs);
    friend bool operator > (const Symbol& lhs, const Scalar& rhs);
    friend bool operator > (const Scalar& lhs, const Symbol& rhs);
    friend bool operator !=(const Symbol& lhs, const Scalar& rhs);
    friend bool operator !=(const Scalar& lhs, const Symbol& rhs);
    friend bool operator <=(const Symbol& lhs, const Scalar& rhs);
    friend bool operator <=(const Scalar& lhs, const Symbol& rhs);
    friend bool operator >=(const Symbol& lhs, const Scalar& rhs);
    friend bool operator >=(const Scalar& lhs, const Symbol& rhs);

    friend std::ostream& operator<<(std::ostream& os, const Symbol& a);
    
public:
    Symbol();
    ~Symbol();
    Symbol(const Symbol&);
    Symbol(Symbol&&);
    Symbol& operator=(const Symbol&);
    Symbol& operator=(Symbol&&);
    void swap(Symbol& other);

private:
    friend class internal::SymbolPrivate;
    std::unique_ptr<class internal::SymbolPrivate> p;
    Symbol(std::unique_ptr<class internal::SymbolPrivate>&&);

private:
    friend struct CodeGenerator;
    friend struct DerivateEvaluator;
    friend struct NumericalEvaluator;
    friend struct StringEvaluator;
    friend struct BooleanEvaluator;
    friend struct TBA;
};

}// namespace codegenvar