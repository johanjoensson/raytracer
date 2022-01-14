#ifndef RAY_VEC_H
#define RAY_VEC_H
#include "utils.h"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <execution>
#include <array>
#include <iostream>

#include <Eigen/Dense>

using Vec3 = Eigen::Vector3d;
using Point3 = Eigen::Vector3d;
using Color3 = Eigen::Vector3d;

inline double dot(const Vec3& a, const Vec3& b)
{
        return a.dot(b);
}

inline Vec3 cross(const Vec3& a, const Vec3& b)
{
        return a.cross(b);
}

inline Vec3 unit_vector(const Vec3& a)
{
        return a.normalized();
}

inline Vec3 random(double min, double max)
{
        // Eigen Random creates random doubles in the range [-1, 1]
        Vec3 offset(min + 1, min + 1, min + 1);
        double scale = max/2;
        return scale*(Vec3::Random() + offset);
}

inline Vec3 random_in_unit_sphere() {
        Vec3 p = random(-1,1);
        while (p.squaredNorm() >= 1){
                p = random(-1, 1);
        }
        return p;
}

inline Vec3 random_unit_vector()
{
        return unit_vector(random_in_unit_sphere());
}

inline Vec3 random_in_hemisphere(const Vec3& normal)
{
        Vec3 in_unit_sphere = random_in_unit_sphere();
        if (dot(in_unit_sphere, normal) > 0){
                return in_unit_sphere;
        }else{
                return -in_unit_sphere;
        }
}

inline bool near_zero(const Vec3& v)
{
        return v.squaredNorm() < 1e-12;
}

inline Vec3 reflect(const Vec3& v, const Vec3& n)
{
        return v - 2*dot(v, n)*n;
}

inline Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat)
{
        double cos_theta = std::min(dot(-uv, n), 1.0);
        Vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
        Vec3 r_out_parallel = -std::sqrt(std::abs(1 - r_out_perp.squaredNorm()))*n;
        return r_out_perp + r_out_parallel;
}

inline Vec3 random_in_unit_disk()
{
        Vec3 p = Vec3(random_double(-1, 1), random_double(-1, 1), 0);
        while(p.squaredNorm() > 1){
                p = Vec3(random_double(-1, 1), random_double(-1, 1), 0);
        }
        return p;
}

#endif //RAY_VEC_H
