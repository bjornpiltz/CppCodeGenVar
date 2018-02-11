#include "test_helper.h"
#include <codegenvar/Symbol.h>

using codegenvar::Symbol;

#define COMPARE( a, result) EXPECT_EQ(trim(result), trim((a).toCode()))

GTEST_TEST(code, a)
{
    Symbol null, x("x"), y("y"), i(2), d(3.0);
   
    COMPARE(null, "out = 0;");
    COMPARE(x, "out = x;");
    COMPARE(i, "out = 2;");
    COMPARE(d, "out = 3.0;");
   
    // TODO: Re-enable these when the symengine version is known.
    // EXPECT_ANY_THROW((x/null).toCode());
    // EXPECT_ANY_THROW((d/null).toCode());
    // EXPECT_ANY_THROW((i/null).toCode());
   
    COMPARE(d*i, "out = 6.0;");
    COMPARE(x*i, "out = 2*x;");
    COMPARE(x*d, "out = 3.0*x;");
    COMPARE(y*x*d, "out = 3.0*x*y;");
}

GTEST_TEST(code, mul)
{
    Symbol null, x("x"), y("y"), i(2), d(3.0);
    COMPARE(y*(x*i), STRINGIFY(
        out = 2*x*y;
    ));
    COMPARE(y*(x*i*x), STRINGIFY(
        const double tmp_0 = pow(x, 2);
        out = 2*tmp_0*y;
    ));
    COMPARE(y*(i/x), STRINGIFY(
        out = 2*y/x;
   ));
    COMPARE(y*(i/sqrt(x)), STRINGIFY(
        const double tmp_0 = sqrt(x);
        out = 2*y/tmp_0;
    ));
}
