#include <codegenvar/Eigen>
#include <Eigen/Geometry>
#include <iostream>

template<class T, int n>
using Vec = Eigen::Matrix<T, n, 1>;

// https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
template <class T>
void rotate_rodrigues(const Vec<T, 3>& k, const T& theta, const Vec<T, 3>& v, Vec<T, 3>& v_rot)
{
    v_rot = v*cos(theta) + k.cross(v)*sin(theta) + k*k.dot(v)*(1-cos(theta));
}

using namespace codegenvar; 

bool equals(const Vec3& a, const Vec3& b)
{
    for(int i = 0; i<3; i++)
        if (!a(i).equals(b(i)))
            return false;
    return true;
}

// main:
int main()
{
    Symbol angle("theta");
    Vec3 axis = namedVector<3>("k"), point = namedVector<3>("v");
    
    Vec3 r1;

    // Use Rodrigues' rotation formula to calculate the rotation of the point:
    rotate_rodrigues(axis, angle, point, r1);
    std::cerr << "Rodrigues:\n" << r1 << std::endl;

    // Use Eigen to calculate the rotation of the point:
    Vec3 r = Eigen::AngleAxis<Symbol>(angle, axis)*point;
    std::cerr << "\nEigen:\n"  << r << std::endl;
    
    // Check for mathematical equality:
    std::cerr << "\nThe two expressions are " 
              << (equals(r1, r) ? "equal" : "not equal") 
              << "." << std::endl;

    // If we convert all expressions to their expanded form, we can see that they are identical.
    std::cerr << "\nExpanded:\n";
    for (int i = 0; i < 3; i++)
    {
        r1(i).expand();
        r(i).expand();
    }

    std::cerr << "\nRodrigues:\n" << r1 << std::endl;
    std::cerr << "\nEigen:\n"  << r << std::endl;

    return 0;
}
