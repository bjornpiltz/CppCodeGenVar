#include "codegenvar/Symbol.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace codegenvar;

#define COMPARE( a, result) EXPECT_EQ(result, (a).toString())
    /*std::cout << "Testing: " << result << std::endl;*/

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
}

GTEST_TEST(BasicTest, booleans)
{
    const Symbol a("a"), b("b"), c("c");
    EXPECT_EQ(a+b+c,  c+a+b);
    EXPECT_NE(a+b+c,  c+a);
}

GTEST_TEST(BasicTest, substitutions)
{
    const Symbol a("a"), b("b"), c("c"), d("d");
    
    // 'a' can't be numerically evaluated:
    EXPECT_ANY_THROW( a.toDouble() );
    
    EXPECT_DOUBLE_EQ(a.resolved({{"a", 2}}).toDouble(), 2.0);
    
    const Symbol::Map symbolMap{ {"a", 2}, {"b", 3}, {"c", 7}, {"d", 11} };
    EXPECT_DOUBLE_EQ(((a+b) * (c+d)).resolved(symbolMap).toDouble(), 90.0);
}