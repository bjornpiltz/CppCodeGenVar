#include "generator/NumericalGenerator.h"
#include "generator/StringGenerator.h"
#include <iostream>
#include <cmath>

using namespace codegenvar;

void test()
{
    const Symbol a("a");
    const Symbol b("b");
    const Symbol c("c");
    const Symbol d("d");

    const Symbol::Map symbolMap{ {"a", 2}, {"b", 3}, {"c", 7}, {"d", 11} };
    
    const Symbol C1(1);
    const Symbol C3(3);
    const Symbol C3_0(3.0);
    {
        Symbol tmp;
        Symbol tmp2(tmp);
        Symbol tmp3(d);
        tmp = tmp2;

    }
    auto evalAndCompare = [](const Symbol& symbol, const Symbol::Map& symbolMap, double result)
    {
        NumericalEvaluator toDouble;
        double value = toDouble(symbol.resolved(symbolMap));
        if(std::abs(value-result)>1e-16)
        {
            std::string mapString;
            for (auto it: symbolMap)
                mapString += it.first + "=" + it.second.toString() + ",";
            
            StringEvaluator toString;
            std::cerr << "Error: " << toString(symbol)
                      << " with " << mapString
                      << " evaluates to "<< value
                      << " not " << result << std::endl;
        }
    };
    auto compare = [](const Symbol& a, const std::string& result)
    {
        auto str = a.toString();
        if(str!=result)
            std::cerr << "Error: \"";
        
//if(str!=result)
            std::cerr << str;

        if(str != result)
            std::cerr << "\" != \"" << result << "\"";
//if(str!=result)
            std::cerr << std::endl;
    };
    
    compare(a, "a");
    evalAndCompare(a, symbolMap, 2 );

    compare(b, "b");
    compare(c, "c");
    compare(d, "d");
    compare(C1, "1");
    compare(C3, "3");
    compare(C3_0, "3.000000");
    compare(a + c, "a+c");
    compare(a + c*b, "a+c*b");
    compare(a + b*c + d, "a+b*c+d");
    compare((a+b) * (c+d), "(a+b)*(c+d)");
    evalAndCompare((a+b) * (c+d), symbolMap, 90 );
    
    compare(1/a, "1/a");
    compare(3/a, "3/a");
    compare(cos(a), "cos(a)");
    compare(cos(a + c), "cos(a+c)");
    compare(cos(a + c), "cos(a+c)");
    compare(pow(a, c), "pow(a,c)");
    compare(pow(a, 2), "pow(a,2)");
    compare(pow(a, 2*b), "pow(a,2*b)");
    compare(pow(a, 2*b)+c, "pow(a,2*b)+c");
    compare(-pow(a, 2), "-pow(a,2)");
    compare(pow(-a, 2), "pow(-a,2)");
    compare(-sin(a), "-sin(a)");
    compare(-a, "-a");
    
    compare(a+(b-c), "a+b-c");
    compare(a+b-c, "a+b-c");
    compare(a-(b-c), "a-b+c");
    compare(a*(b-c), "a*(b-c)");
    compare(a-b*c, "a-b*c");
    compare(-a, "-a");

    compare(- (-a), "a");
    compare(a+-b, "a-b");
    compare(a+-b, "a-b");
    compare(C1*a, "a");
    compare(1*a, "a");
    compare(a*1, "a");
    compare(0*a, "0");
    compare(a*0, "0");
    compare(a/C1, "a");

    if(a+b+c == c+a+b)
        std::cerr << "Error: !=";
}

int main(int argc, char* argv[])
{
    try {
        test();
    }
    catch (const std::exception& err)
    {
        std::cerr << err.what() << std::endl;
    }
    return 1;
}