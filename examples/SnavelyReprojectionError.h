#pragma once
#include <Eigen/Core>
#include <Eigen/Geometry>

// This class takes a camera, modeled with 9 parameters, and
// projects a 3 parameter world point into image space.
//
// Adapted from https://ceres-solver.googlesource.com/ceres-solver/+/master/examples/snavely_reprojection_error.h
template<typename Scalar>
struct SnavelyReprojectionError
{
    typedef Eigen::Matrix<Scalar, 3, 1> Vec3;
    typedef Eigen::Matrix<Scalar, 2, 1> Vec2;
    
    typedef typename Vec3::ConstMapType Vec3Ref;
  
    Vec2 observed;
    
    SnavelyReprojectionError(const Vec2& obs)
        : observed(obs)
    {
    }
    Vec3 rotate(const Vec3Ref& R, const Vec3Ref& v)const
    {
        const Scalar theta2 = R.squaredNorm();
        // We only evaluate the square root if the norm of the angle_axis vector is 
        // greater than zero. Otherwise we get a division by zero.

        if (theta2 > std::numeric_limits<double>::epsilon()) 
        {
            // Away from zero, use the rodriguez formula:
            const Scalar theta = sqrt(theta2);
            const Vec3 k = R/theta;
            return v*cos(theta) + k.cross(v)*sin(theta) + k*k.dot(v)*(1.0-cos(theta));
        } 
        // Switching to the Taylor expansion near zero provides meaningful derivatives 
        return  v + R.cross(v);
    }
    // projectPoint:
    Vec2 projectPoint(
              const Scalar R[3], // The compact angle-axis rotation of the camera.
              const Scalar T[3], // The translation of the camera.
              const Scalar& f,  // The focal length of the camera.
              const Scalar& l1, // The first distortion parameter of the camera.
              const Scalar& l2, // The second distortion parameter of the camera.
              const Scalar P[3]  // The world point.
        ) const 
    {
        // Rotate P around R and translate by T:    
        const Vec3 p = rotate(Vec3::Map(R), Vec3::Map(P)) + Vec3::Map(T);
  
        // Compute the center of distortion.
        const Vec2 cx = -p.hnormalized();
  
        // Apply second and fourth order radial distortion.
        const Scalar r2 = cx.squaredNorm();
        const Scalar distortion = 1 + r2*(l1 + r2*l2);
  
        // Compute final projected point position.
        return f * distortion * cx;
    }
    
    bool operator()(const Scalar camera[9],
                    const Scalar point[3], 
                          Scalar residuals[2]) const 
    {
        const auto R = camera+0; // camera[0,1,2] are the compact angle-axis rotation.
        const auto T = camera+3; // camera[3,4,5] are the translation.
        const auto& f  = camera[6];         // camera[6] is the focal length
        const auto& l1 = camera[7];         // camera[7] is the first distortion parameter.
        const auto& l2 = camera[8];         // camera[8] is the second distortion parameter.
        
        // The error is the difference between the predicted and observed position.      
        Vec2::Map(residuals) = projectPoint(R, T, f, l1, l2, point) - observed;
        return true;
    }
};
