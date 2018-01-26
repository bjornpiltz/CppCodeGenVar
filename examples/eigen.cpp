#include <codegenvar/SymbolWithEigenBindings.h>
#include <Eigen/Geometry>
#include <iostream>

using namespace codegenvar;

int main()
{
    const Symbol x("x"), y("y"), dx("dx"), dy("dy"), a("a");
    
    const Vec2 p(x, y);
    const Vec3 P = p.homogeneous();
    const Vec2 diff(dx, dy);

    std::cout << "p = " << p.transpose() << std::endl;
    std::cout << "translate by " << diff.transpose() << std::endl;
    std::cout << "rotate by " << a << std::endl << std::endl;
    std::cout << "P = " << P.transpose() << std::endl << std::endl;

    Mat23 t;
    t << Symbol(1), Symbol(0), diff(0),
         Symbol(0), Symbol(1), diff(1);

    std::cout << "p + diff = " << (p+diff).transpose() << std::endl << std::endl;
    std::cout << "t = " << std::endl << t << std::endl << std::endl;
    std::cout << "t*P = " << (t*P).transpose() << std::endl << std::endl;

    Mat3 T;
    T << t(0, 0), t(0, 1), t(0, 2),
         t(1, 0), t(1, 1), t(1, 2),
         Symbol(0), Symbol(0), Symbol(1);

    std::cout << "T = " << std::endl << T << std::endl << std::endl;
    std::cout << "T*P = " << (T*P).transpose() << std::endl << std::endl;

    Mat2 r;
    r <<  cos(a), -sin(a),
          sin(a),  cos(a);

    std::cout << "r = " << std::endl << r << std::endl << std::endl;
    std::cout << "r*p = " << (r*p).transpose() << std::endl << std::endl;

    Mat3 R;
    R << r(0, 0) , r(0, 1), Symbol(0),
         r(1, 0) , r(1, 1), Symbol(0),
         Symbol(0), Symbol(0), Symbol(1);

    std::cout << "R = " << std::endl << R << std::endl << std::endl;
    std::cout << "R*P = " << (R*P).transpose() << std::endl << std::endl;

    std::cout << "T*R = " << std::endl << T*R << std::endl << std::endl;
    std::cout << "T*R*P = " << (T*R*P).transpose() << std::endl << std::endl;

    std::cout << "R*T = " << std::endl << R*T << std::endl << std::endl;
    std::cout << "R*T*P = " << (R*T*P).transpose() << std::endl << std::endl;

    return 0;
}