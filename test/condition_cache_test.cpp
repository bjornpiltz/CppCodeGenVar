#include "test_helper.h"
#include "../src/ConditionCache.h"
#include <symengine/symbol.h>
#include <symengine/mul.h>
#include <symengine/add.h>
#include <symengine/integer.h>
#include <iostream>

using namespace codegenvar;
using namespace codegenvar::internal;
using namespace SymEngine;

GTEST_TEST(condition_cache, simple)
{
    ConditionCache cache;
    auto a = symbol("a"), b = symbol("b");
    cache.insert(a, b, LT, true);// a<b
    bool value;
    EXPECT_TRUE(cache.alreadyEvaluated(a, b, LT, value) &&  value); // a  < b -> true
    EXPECT_TRUE(cache.alreadyEvaluated(a, b, GT, value) && !value); // a  > b -> false
    EXPECT_TRUE(cache.alreadyEvaluated(a, b, EQ, value) && !value); // a == b -> false
    EXPECT_TRUE(cache.alreadyEvaluated(a, b, GE, value) && !value); // a >= b -> false
    EXPECT_TRUE(cache.alreadyEvaluated(a, b, LE, value) &&  value); // a <= b -> true
    EXPECT_TRUE(cache.alreadyEvaluated(a, b, NE, value) &&  value); // a != b -> true

    EXPECT_TRUE(cache.alreadyEvaluated(b, a, LT, value) && !value); // b  < a -> false
    EXPECT_TRUE(cache.alreadyEvaluated(b, a, GT, value) &&  value); // b  > a -> true
    EXPECT_TRUE(cache.alreadyEvaluated(b, a, EQ, value) && !value); // b == a -> false
    EXPECT_TRUE(cache.alreadyEvaluated(b, a, GE, value) &&  value); // b >= a -> true
    EXPECT_TRUE(cache.alreadyEvaluated(b, a, LE, value) && !value); // b <= a -> false
    EXPECT_TRUE(cache.alreadyEvaluated(b, a, NE, value) &&  value); // b != a -> true

    cache = {};
        
    cache.insert(a, b, LE, true);// a<=b

    EXPECT_FALSE(cache.alreadyEvaluated(a, b, LT, value));           // a  < b -> ?
    EXPECT_TRUE(cache.alreadyEvaluated(a, b, GT, value) &&  !value); // a  > b -> false
    EXPECT_FALSE(cache.alreadyEvaluated(a, b, EQ, value));           // a == b -> ?
    EXPECT_FALSE(cache.alreadyEvaluated(a, b, GE, value));           // a >= b -> ?
    EXPECT_TRUE(cache.alreadyEvaluated(a, b, LE, value) &&  value);  // a <= b -> true
    EXPECT_FALSE(cache.alreadyEvaluated(a, b, NE, value));           // a != b -> ?

    EXPECT_TRUE(cache.alreadyEvaluated(b, a, LT, value) && !value);  // b  < a -> false
    EXPECT_FALSE(cache.alreadyEvaluated(b, a, GT, value));           // b  > a -> ?
    EXPECT_FALSE(cache.alreadyEvaluated(b, a, EQ, value));           // b == a -> ?
    EXPECT_TRUE(cache.alreadyEvaluated(b, a, GE, value) &&  value);  // b >= a -> true
    EXPECT_FALSE(cache.alreadyEvaluated(b, a, LE, value));           // b <= a -> ?
    EXPECT_FALSE(cache.alreadyEvaluated(b, a, NE, value));           // b != a -> ?

}

GTEST_TEST(condition_cache, example)
{
    auto a = symbol("a"), b = symbol("b");

    ConditionCache cache;

    std::cerr << "Assert that a <= b." << std::endl;
    cache.insert(a, b, LE, true); 

    bool b1;
    if (cache.alreadyEvaluated(a, b, GT, b1))
        std:: cerr << "We know that a " << (b1 ? "is" : "is not") << " larger than b." << std::endl;
    else
        std:: cerr << "We don't know if b is larger than a." << std::endl;
        
    bool b2;
    if (cache.alreadyEvaluated(a, b, EQ, b2))
        std::cerr << "We know that a " << (b2 ? "equals" : "doesn't equal") << " b." << std::endl;
    else
        std::cerr << "We don't know if a equals b." << std::endl;
}
