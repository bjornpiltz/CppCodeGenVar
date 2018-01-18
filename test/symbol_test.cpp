#include "codegenvar/Symbol.h"
#include <gtest/gtest.h>
#include <cmath>
#include <iostream>

using namespace codegenvar;

#define COMPARE( a, result) EXPECT_EQ(result, (a).toString())

GTEST_TEST(BasicTest, ctors_and_assignment)
{
    // Test a default constructed object:
    Symbol a;
    EXPECT_EQ(a.toString(), "");
    EXPECT_ANY_THROW(a.toDouble());

    // Test empty assignments:
    a = Symbol();
    a = std::move(Symbol());
    
    // Test copy constructor:
    Symbol b(a);
    EXPECT_EQ(b.toString(), "");

    // Test assignments:
    b = Symbol("b");
    EXPECT_EQ(b.toString(), "b");
    
    Symbol c(b);
    c = Symbol("c");

    // Check aliasing:
    EXPECT_EQ(a.toString(), "");
    EXPECT_EQ(b.toString(), "b");
    EXPECT_EQ(c.toString(), "c");

    auto vars = Symbol::variables({ "x", "y", "z" });
    EXPECT_EQ(vars[0].toString(), "x");
    EXPECT_EQ(vars[1].toString(), "y");
    EXPECT_EQ(vars[2].toString(), "z");

    // Test swapping:
    std::swap(vars[0], vars[1]);
    EXPECT_EQ(vars[0].toString(), "y");
    EXPECT_EQ(vars[1].toString(), "x");

    vars[0].swap(vars[1]);
    EXPECT_EQ(vars[0].toString(), "x");
    EXPECT_EQ(vars[1].toString(), "y");
    
    // Test integer ctors:
    Symbol var_i((int)1);
    Symbol var_li((long int)2);
    Symbol var_lli((long long int)3);
    Symbol var_f((float)4);
    Symbol var_d((double)-8);

    EXPECT_EQ(var_i.toString(), "1");
    EXPECT_EQ(var_li.toString(), "2");
    EXPECT_EQ(var_lli.toString(), "3");
    EXPECT_EQ(var_f.toString(), "4.000000");
    EXPECT_EQ(var_d.toString(), "-8.000000");
    
    // Test getVariableNames();
    EXPECT_TRUE(var_i.getVariableNames().empty());
    EXPECT_TRUE(var_li.getVariableNames().empty());
    EXPECT_TRUE(var_lli.getVariableNames().empty());
    EXPECT_TRUE(var_f.getVariableNames().empty());
    EXPECT_TRUE(var_d.getVariableNames().empty());

    EXPECT_TRUE(c.getVariableNames().size()==1 &&
               *c.getVariableNames().begin()== "c");

    // TODO: plus assignments, etc
}

GTEST_TEST(BasicTest, simple_expressions)
{
    const Symbol a("a"), b("b"), c("c"), d("d"), C3(3), C3_0(3.0);
    
    COMPARE(a, "a");
    COMPARE(-a, "-a");
    COMPARE(- -a, "a");
    COMPARE(- - -a, "-a");
    
    COMPARE(b, "b");
    COMPARE(c, "c");
    COMPARE(d, "d");
    COMPARE(Symbol(1), "1");
    COMPARE(C3, "3");
    COMPARE(C3_0, "3.000000");
    COMPARE(a + c, "a+c");
    COMPARE(a + c*b, "a+c*b");
    COMPARE(a + b*c + d, "a+b*c+d");
    COMPARE((a+b) * (c+d), "(a+b)*(c+d)");

    COMPARE(a + (b + c + d), "a+b+c+d");
    COMPARE((a + b) + (c + d), "a+b+c+d");
    COMPARE((a + b + c) + d, "a+b+c+d");
    COMPARE(a + b + c + d, "a+b+c+d");
    
    COMPARE(a * (b * c * d), "a*b*c*d");
    COMPARE((a * b) * (c * d), "a*b*c*d");
    COMPARE((a * b * c) * d, "a*b*c*d");
    COMPARE(a * b * c * d, "a*b*c*d");
    
    COMPARE(1/a, "1/a");
    COMPARE(3/a, "3/a");
    COMPARE(cos(a), "cos(a)");
    COMPARE(cos(a + c), "cos(a+c)");
    COMPARE(cos(a + c), "cos(a+c)");
    COMPARE(pow(a, c), "pow(a,c)");
    COMPARE(pow(a, 2), "pow(a,2)");
    COMPARE(pow(a, 2*b), "pow(a,2*b)");
    COMPARE(pow(a, 2*b)+c, "pow(a,2*b)+c");
    COMPARE(-pow(a, 2), "-pow(a,2)");
    COMPARE(pow(-a, 2), "pow(-a,2)");
    COMPARE(-sin(a), "-sin(a)");
    
    COMPARE(a+(b-c), "a+b-c");
    COMPARE(a+b-c, "a+b-c");
    COMPARE(a-(b-c), "a-(b-c)");
    COMPARE(a/(b*c), "a/(b*c)");
    COMPARE(a/(b/c), "a/(b/c)");
    COMPARE(a*(b/c), "a*b/c");
    COMPARE(a*(b-c), "a*(b-c)");
    COMPARE(a-b*c, "a-b*c");
    COMPARE(-a, "-a");
}
GTEST_TEST(BasicTest, unary_functions)
{
    Symbol x("x");

    COMPARE(abs(x), "abs(x)");
    COMPARE(log(x), "log(x)");
    COMPARE(exp(x), "exp(x)");
    COMPARE(sqrt(x), "sqrt(x)");
    COMPARE(cos(x), "cos(x)");
    COMPARE(acos(x), "acos(x)");
    COMPARE(sin(x), "sin(x)");
    COMPARE(asin(x), "asin(x)");
    COMPARE(tan(x), "tan(x)");
    COMPARE(atan(x), "atan(x)");
    COMPARE(sinh(x), "sinh(x)");
    COMPARE(cosh(x), "cosh(x)");
    COMPARE(tanh(x), "tanh(x)");
    COMPARE(floor(x), "floor(x)");
    COMPARE(ceil(x), "ceil(x)");

    
    const Symbol c1(1.0);

#define EVALUATE_UNARY_FUNCTION(fun)\
    EXPECT_DOUBLE_EQ(fun(1.0), fun(c1).toDouble())

    EXPECT_DOUBLE_EQ(fabs(1.0), abs(c1).toDouble())
    EVALUATE_UNARY_FUNCTION(log);
    EVALUATE_UNARY_FUNCTION(exp);
    EVALUATE_UNARY_FUNCTION(sqrt);
    EVALUATE_UNARY_FUNCTION(cos);
    EVALUATE_UNARY_FUNCTION(acos);
    EVALUATE_UNARY_FUNCTION(sin);
    EVALUATE_UNARY_FUNCTION(asin);
    EVALUATE_UNARY_FUNCTION(tan);
    EVALUATE_UNARY_FUNCTION(atan);
    EVALUATE_UNARY_FUNCTION(sinh);
    EVALUATE_UNARY_FUNCTION(cosh);
    EVALUATE_UNARY_FUNCTION(tanh);
    EVALUATE_UNARY_FUNCTION(floor);
    EVALUATE_UNARY_FUNCTION(ceil);

}
GTEST_TEST(BasicTest, simplifications)
{
    const Symbol a("a"), b("b"), c("c"), C1(1);
    
    COMPARE(-(-a), "a");
    COMPARE(a+-b, "a-b");
    COMPARE(a+-b, "a-b");
    COMPARE(C1*a, "a");
    COMPARE(1*a, "a");
    COMPARE(a*1, "a");
    COMPARE(0*a, "0");
    COMPARE(a*0, "0");
    COMPARE(a/C1, "a");
    
    COMPARE(0+a, "a");
    COMPARE(a+0, "a");
    
    COMPARE(1/a, "1/a");
    COMPARE(1/(1/a), "a");
    COMPARE(C1/1, "1");
    COMPARE(1/C1, "1");
    COMPARE(C1/C1, "1");
    COMPARE(a/1, "a");
}

GTEST_TEST(BasicTest, booleans)
{
    const Symbol a("a"), b("b"), c("c");

    //Boolean logic is not supported yet.
    EXPECT_ANY_THROW(a+b+c == a+b+c);
    EXPECT_ANY_THROW(a+b+c != a+b+c);
    EXPECT_ANY_THROW(a+b+c ==  c+a);
    EXPECT_ANY_THROW(a+b+c !=  c+a);
}

GTEST_TEST(BasicTest, substitutions)
{
    const Symbol a("a"), b("b"), c("c"), d("d");
    
    // 'a' can't be numerically evaluated:
    EXPECT_ANY_THROW( a.toDouble() );
    
    EXPECT_DOUBLE_EQ(a.resolved({{"a", 2}}).toDouble(), 2.0);
    
    const Symbol::Map symbolMap{ {"a", 2}, {"b", 3}, {"c", 7}, {"d", 11} };
    EXPECT_DOUBLE_EQ(((a+b) * (c+d)).resolved(symbolMap).toDouble(), 90.0);
    
    EXPECT_EQ(a.toString(), "a");
    EXPECT_EQ(a.resolved({ {"b", 0} }).toString(), "a");
    EXPECT_EQ(a.resolved({ {"a", 0} }).toString(), "0");
}
