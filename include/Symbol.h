#pragma once
#include "Number.h"
#include <map>
#include <vector>
#include <memory>
#include <set>
#include <string>
#include <iosfwd>

namespace codegenvar {

namespace internal {class AbstractExpression;}

class Symbol
{
public:        
    typedef std::map<std::string, Number> Map;

public:        
    // Constructors creating constants.
    explicit Symbol(int value);
    explicit Symbol(long int value);
    explicit Symbol(long long int value);
    explicit Symbol(float value);
    explicit Symbol(double value);
    explicit Symbol(const Number& value);
    
    // Constructors creating variables or named constants.
    explicit Symbol(const std::string&);
    static std::vector<Symbol> variables(std::initializer_list<std::string> names);
    
    std::string toString()const;
    
    Symbol resolved(const Map& symbolMap)const;
    
    std::set<std::string> getVariableNames()const;
    
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

    // Symbol op Number and Number op Symbol overloads
    friend Symbol operator + (const Number& lhs, const Symbol& rhs);
    friend Symbol operator + (const Symbol& lhs, const Number& rhs);
    friend Symbol operator - (const Number& lhs, const Symbol& rhs);
    friend Symbol operator - (const Symbol& lhs, const Number& rhs);
    friend Symbol operator * (const Number& lhs, const Symbol& rhs);
    friend Symbol operator * (const Symbol& lhs, const Number& rhs);
    friend Symbol operator / (const Number& lhs, const Symbol& rhs);
    friend Symbol operator / (const Symbol& lhs, const Number& rhs);
    
    Symbol& operator=(const Number& other);
    Symbol& operator+=(const Number& other);
    Symbol& operator-=(const Number& other);
    Symbol& operator*=(const Number& other);    
    Symbol& operator/=(const Number& other);

    friend Symbol pow(const Symbol&, const Number&);
    
    friend bool operator ==(const Symbol& lhs, const Number& rhs);
    friend bool operator ==(const Number& lhs, const Symbol& rhs);
    friend bool operator < (const Symbol& lhs, const Number& rhs);
    friend bool operator < (const Number& lhs, const Symbol& rhs);
    friend bool operator > (const Symbol& lhs, const Number& rhs);
    friend bool operator > (const Number& lhs, const Symbol& rhs);
    friend bool operator !=(const Symbol& lhs, const Number& rhs);
    friend bool operator !=(const Number& lhs, const Symbol& rhs);
    friend bool operator <=(const Symbol& lhs, const Number& rhs);
    friend bool operator <=(const Number& lhs, const Symbol& rhs);
    friend bool operator >=(const Symbol& lhs, const Number& rhs);
    friend bool operator >=(const Number& lhs, const Symbol& rhs);

    friend std::ostream& operator<<(std::ostream& os, const Symbol& a);
    
public:
    Symbol()=default;
    Symbol(const Symbol&)= default;
    Symbol(Symbol&&)=default;
    Symbol& operator=(const Symbol&)= default;
    Symbol& operator=(Symbol&&)= default;
    void swap(Symbol& other);

private:
    // The data pointer:
    std::shared_ptr<const class internal::AbstractExpression> expr;
    Symbol(std::shared_ptr<const class internal::AbstractExpression>);
    
private:
    friend struct CodeGenerator;
    friend struct DerivateEvaluator;
    friend struct NumericalEvaluator;
    friend struct StringEvaluator;
};

}// namespace codegenvar