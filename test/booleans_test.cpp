#include "test_helper.h"
#include <codegenvar/Symbol.h>
#include <codegenvar/BooleanEvaluator.h>

#define COMPARE( a, result) EXPECT_EQ(wo_ws(result), wo_ws((a).toString()))

using namespace codegenvar;

Symbol testFun(const Symbol& x, const Symbol& y)
{
    if (x==y)
        return x;
    if (x<3)
        return x+y+3;
    if (x>3)
        return Symbol(0.0);
    
    return x-y;
}

GTEST_TEST(booleans, uninitialized)
{
    Symbol x("x"), y("y"), c1(1), c2(2);
    bool dummy = false;
    EXPECT_ANY_THROW(Symbol z = testFun(x, y));
    EXPECT_ANY_THROW(dummy = (1 != x));
    EXPECT_ANY_THROW(dummy = (x != 1));
    EXPECT_ANY_THROW(dummy = (1 == x));
    EXPECT_ANY_THROW(dummy = (x == 1));
    EXPECT_ANY_THROW(dummy = (x != y));
    EXPECT_ANY_THROW(dummy = (x <= y));
    EXPECT_ANY_THROW(dummy = (x >= y));
    EXPECT_ANY_THROW(dummy = (x <  y));
    EXPECT_ANY_THROW(dummy = (x >  y));
    
    EXPECT_EQ(c1 == c1, true);
    EXPECT_EQ(c1 == c2, false);
    
    EXPECT_EQ(c1 != c1, false);
    EXPECT_EQ(c1 != c2, true);
    
    EXPECT_EQ(c1 < c1, false);
    EXPECT_EQ(c1 < c2, true);
    EXPECT_EQ(c2 < c1, false);
    
    EXPECT_EQ(c1 > c1, false);
    EXPECT_EQ(c1 > c2, false);
    EXPECT_EQ(c2 > c1, true);
    
    EXPECT_EQ(c1 <= c1, true);
    EXPECT_EQ(c1 <= c2, true);
    EXPECT_EQ(c2 <= c1, false);
    
    EXPECT_EQ(c1 >= c1, true);
    EXPECT_EQ(c1 >= c2, false);
    EXPECT_EQ(c2 >= c1, true);
    
    EXPECT_EQ(x == x, true);
    EXPECT_EQ(x < x, false);
    EXPECT_EQ(x > x, false);
    EXPECT_EQ(x <= x, true);
    EXPECT_EQ(x >= x, true);    
}

GTEST_TEST(booleans, a)
{
    Symbol x("x"), y("y");

    Symbol tmp;
    tmp |= x+y+3;
    COMPARE(tmp, "3+x+y");
    BooleanEvaluator evaluator;
    EXPECT_TRUE(evaluator.isFullyEvaluated());
    EXPECT_TRUE(tmp.equals(x+y+3));
    Symbol z;
    do
    {
        z |= testFun(x, y);
    }while (!evaluator.isFullyEvaluated());
    
    COMPARE(z,STRINGIFY(
     x == y
     ? x
     : x < 3 && x != y
       ? 3 + x + y
       : 3 <= x && 3 < x && x != y
         ? 0.0
         : x - y));
}
