#include "test_helper.h"
#include <codegenvar/Symbol.h>
#include <codegenvar/BooleanEvaluator.h>

#define COMPARE( a, result) EXPECT_EQ(wo_ws(result), wo_ws((a).toString()))

using namespace codegenvar;

void v1 (double x, double y, double z)
{
    bool a = (x==y);
    bool b = (x<3);
    bool c = (x>3);
    
    if (a)
    {
        z = x;
    }
    else
    {
        if (b)
        {
            z = x+y+3;
        }
        else
        {
            
            if (c)
            {
                z = 0.0;
            }
            else
            {
                z =  x-y;
            }
        }
    }
}

void v2 (double x, double y, double z)
{
    bool a = (x==y);
    bool b = !(x==y) && (x<3);
    bool c = !(x==y) &&!(x<3) &&(x>3);
    
    if (a)
    {
        z = x;
    }
    else if (b)
    {
        z = x+y+3;
    }
    else if (c)
    {
        z = 0.0;
    }
    else
    {
        z =  x-y;
    }
}

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
    Symbol x("x"), y("y"), c1(1);

    EXPECT_TRUE(c1.isFullyEvaluated());
    EXPECT_TRUE(x.isFullyEvaluated());
    EXPECT_TRUE((x+y+3).isFullyEvaluated());
    
    Symbol tmp;
    EXPECT_TRUE((tmp|=(x+y+3)).isFullyEvaluated());
    BooleanEvaluator evaluator;
    EXPECT_TRUE((tmp|=(x+y+3)).isFullyEvaluated());
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
