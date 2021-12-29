#ifndef RAY_VEC_H
#define RAY_VEC_H
#include "utils.h"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <execution>
#include <array>
#include <iostream>

template<std::size_t D>
class VecD_Base{
        public:
                double length() const {return norm();}
                double length_squared() const {return norm2();}
                double norm() const {return std::sqrt(norm2());}
                double norm2() const 
                {
                        return std::transform_reduce(std::execution::par, std::cbegin(m_data), std::cend(m_data), std::cbegin(m_data), 0.);
                }

                VecD_Base& operator/=(const double s) 
                { 
                        std::transform(std::execution::par, std::cbegin(m_data), std::cend(m_data), std::begin(m_data),
                                        [=](const double a){ return a/s;}
                                        );
                        return *this;
                }
                VecD_Base& operator*=(const double s) 
                { 
                        std::transform(std::execution::par, std::cbegin(m_data), std::cend(m_data), std::begin(m_data),
                                        [=](const double a){ return a*s;}
                                        );
                        return *this;
                }
                VecD_Base& operator*=(const VecD_Base& other) 
                { 
                        std::transform(std::execution::par, std::cbegin(m_data), std::cend(m_data), std::cbegin(other.m_data), std::begin(m_data),
                                        [](const double a, const double b){ return a*b;}
                                        );
                        return *this;
                }
                VecD_Base& operator-=(const VecD_Base& other) 
                { 
                        std::transform(std::execution::par, std::cbegin(m_data), std::cend(m_data), std::cbegin(other.m_data), std::begin(m_data),
                                        [](const double a, const double b){ return a-b;}
                                        );
                        return *this;
                }
                VecD_Base& operator+=(const VecD_Base& other) 
                { 
                        std::transform(std::execution::par, std::cbegin(m_data), std::cend(m_data), std::cbegin(other.m_data), std::begin(m_data),
                                        [](const double a, const double b){ return a+b;}
                                        );
                        return *this;
                }
                double operator[](const size_t i) const { return m_data[i];}
                double& operator[](const size_t i){ return m_data[i];}
                VecD_Base operator-() const 
                {
                        VecD_Base res = *this;
                        std::transform(std::execution::par, std::cbegin(res.m_data), std::cend(res.m_data), std::begin(res.m_data),
                                        [](const double v){return -v;}
                                        );
                        return res;
                }

                friend VecD_Base operator+(const VecD_Base& a, const VecD_Base& b)
                {
                        VecD_Base res = a;
                        return res += b;
                }

                friend VecD_Base operator-(const VecD_Base& a, const VecD_Base& b)
                {
                        VecD_Base res = a;
                        return res -= b;
                }

                friend VecD_Base operator*(const VecD_Base& a, const VecD_Base& b)
                {
                        VecD_Base res = a;
                        return res *= b;
                }

                friend VecD_Base operator*(const VecD_Base& a, const double s)
                {
                        VecD_Base res = a;
                        return res *= s;
                }

                friend VecD_Base operator*(const double s, const VecD_Base& a)
                {
                        return a*s;
                }

                friend VecD_Base operator/(const VecD_Base& a, const double s)
                {
                        VecD_Base res = a;
                        return res /= s;
                }

                friend double dot(const VecD_Base& a, const VecD_Base& b)
                {
                        return std::transform_reduce(std::execution::par, std::cbegin(a.m_data), std::cend(a.m_data), std::cbegin(b.m_data), 0.);
                }

                friend VecD_Base unit_vector(const VecD_Base& a)
                {
                        return a/a.norm();
                }

                friend std::ostream& operator<<(std::ostream& out, const VecD_Base& v)
                {
                        std::for_each(std::cbegin(v.m_data), std::cend(v.m_data),
                                        [&](const double d){ out << d << " ";}
                                     );
                        return out;
                }

                VecD_Base()
                        : m_data()
                {
                        m_data.fill(0);
                }

                VecD_Base(std::initializer_list<double> l)
                        : m_data()
                {
                        std::transform(std::begin(l), std::end(l), std::begin(m_data), 
                                        [](const double v){ return v;}
                                        );
                }

                VecD_Base(const VecD_Base&) = default;
                VecD_Base(VecD_Base&&) = default;
                virtual ~VecD_Base() = default;

                VecD_Base& operator=(const VecD_Base&) = default;
                VecD_Base& operator=(VecD_Base&&) = default;
        protected:
                std::array<double, D> m_data;
};

class Vec2 : public VecD_Base<2>{
        public:
                double& x() {return m_data[0];}
                double& y() {return m_data[1];}
                double x() const {return m_data[0];}
                double y() const {return m_data[1];}
                
                Vec2(const double a, const double b)
                        : VecD_Base({a, b})
                        {}
                Vec2(const VecD_Base<2>& v) : VecD_Base(v) {}
                Vec2() : VecD_Base() {}
                Vec2(const Vec2&) = default;
                Vec2(Vec2&&) = default;
                ~Vec2() = default;
                Vec2& operator=(const Vec2&) = default;
                Vec2& operator=(Vec2&&) = default;
};
class Vec3 : public VecD_Base<3>{
        public:
                double& x() {return m_data[0];}
                double& y() {return m_data[1];}
                double& z() {return m_data[2];}
                double x() const {return m_data[0];}
                double y() const {return m_data[1];}
                double z() const {return m_data[2];}
                
                friend Vec3 cross(const Vec3& a, const Vec3& b)
                {
                        return Vec3(
                                        a[1]*b[2] - a[2]*b[1],
                                        a[2]*b[0] - a[0]*b[2],
                                        a[0]*b[1] - a[1]*b[0]
                                    );
                }

                inline static Vec3 random(const double min = 0, const double max = 1)
                {
                        return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
                }

                Vec3(const double a, const double b, const double c)
                        : VecD_Base({a, b, c})
                        {}
                Vec3(const VecD_Base<3>& v) : VecD_Base(v) {}
                Vec3() : VecD_Base() {}
                Vec3(const Vec3&) = default;
                Vec3(Vec3&&) = default;
                ~Vec3() = default;
                Vec3& operator=(const Vec3&) = default;
                Vec3& operator=(Vec3&&) = default;
};
using Point3 = Vec3;

Vec3 random_inside_unit_sphere() {
        Vec3 v(Vec3::random(-1, 1));
        while(v.norm2() > 1){
                v = Vec3::random(-1, 1);
        }
        return v;
}

Vec3 random_unit_vector(){
        return unit_vector(random_inside_unit_sphere());
}

Vec3 random_in_hemisphere(const Vec3& normal)
{
        Vec3 in_unit_sphere = random_inside_unit_sphere();
        if(dot(in_unit_sphere, normal) > 0){
                return in_unit_sphere;
        }else{
                return -in_unit_sphere;
        }
}
class Vec4 : public VecD_Base<4>{
        public:
                double& x() {return m_data[0];}
                double& y() {return m_data[1];}
                double& z() {return m_data[2];}
                double& w() {return m_data[3];}
                double x() const {return m_data[0];}
                double y() const {return m_data[1];}
                double z() const {return m_data[2];}
                double w() const {return m_data[3];}
                
                Vec4(const double a, const double b, const double c, const double d)
                        : VecD_Base({a, b, c, d})
                        {}
                Vec4(const VecD_Base<4>& v) : VecD_Base(v) {}
                Vec4() : VecD_Base() {}
                Vec4(const Vec4&) = default;
                Vec4(Vec4&&) = default;
                ~Vec4() = default;
                Vec4& operator=(const Vec4&) = default;
                Vec4& operator=(Vec4&&) = default;
};

#endif //RAY_VEC_H
