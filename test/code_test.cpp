#include <codegenvar/Symbol.h>
#include <gtest/gtest.h>  

using codegenvar::Symbol;

inline std::string wo_ws(std::string a)
{
    while (!a.empty() && std::isspace(a[0]))
        a = a.substr(1);
    
    while (!a.empty() && std::isspace(a[a.size()-1]))
        a = a.substr(0, a.size()-1);
    return a;
}

#define COMPARE( a, result) EXPECT_EQ(wo_ws(result), wo_ws((a).toCode()))

GTEST_TEST(code, a)
{
   Symbol null, x("x"), y("y"), i(2), d(3.0);
   
   COMPARE(null, "out = 0;");
   COMPARE(x, "out = x;");
   COMPARE(i, "out = 2;");
   COMPARE(d, "out = 3.0;");
   
   EXPECT_ANY_THROW((x/null).toCode());
   EXPECT_ANY_THROW((d/null).toCode());
   EXPECT_ANY_THROW((i/null).toCode());
   
   COMPARE(d*i, "out = 6.0;");
   COMPARE(x*i, "out = 2*x;");
   COMPARE(x*d, "out = 3.0*x;");
   COMPARE(y*x*d, "out = 3.0*x*y;");
}

#if 0
WIP:
template<typename T>
T fun(T r1, T r2, T r3, T p1, T p2, T p3)
{
    const T tmp_0 = pow(r1, 2);
    const T tmp_1 = pow(r2, 2);
    const T tmp_2 = pow(r3, 2);
    const T tmp_3 = tmp_0 + tmp_1 + tmp_2;
    //const T tmp_4 = sqrt(tmp_3);
    //const T tmp_5 = cos(tmp_4);
    //const T tmp_6 = p2*tmp_5;
    const T tmp_7 = p1*r3/sqrt(tmp_3);
    const T tmp_8 = p3*r1/sqrt(tmp_3);
    const T tmp_9 = tmp_7 - tmp_8;
    return tmp_9;
}


GTEST_TEST(code, b)
{
    return;
    using codegenvar::Symbol;
    
    Symbol r1("r1"), r2("r2"), r3("r3");
    Symbol p1("p1"), p2("p2"), p3("p3");
    Symbol x = fun(r1, r2, r3, p1, p2, p3);
    std::cerr << x.toString() << std::endl << std::endl;
    std::cerr << x.toCode() << std::endl << std::endl;
}
#endif