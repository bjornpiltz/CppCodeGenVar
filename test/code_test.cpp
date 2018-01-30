#include <codegenvar/Symbol.h>
#include <gtest/gtest.h>  
#include <cctype>

using codegenvar::Symbol;

inline std::string wo_ws(std::string a)
{
    while (!a.empty() && std::isspace(a[0]))
        a = a.substr(1);
    
    while (!a.empty() && std::isspace(a[a.size()-1]))
        a = a.substr(0, a.size()-1);    
    for (auto it = a.begin(); it != a.end(); ++it)
        if (std::iscntrl(*it))
            *it = ' ';
    return a;
}

#define COMPARE( a, result) EXPECT_EQ(wo_ws(result), wo_ws((a).toCode()))
#define STRINGIFY(code)  #code

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
