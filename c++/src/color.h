#ifndef RAY_COLOR_H
#define RAY_COLOR_H

#include "vec.h"
#include "utils.h"

class Color3 : public VecD_Base<3>{
        public:
                double& r() {return m_data[0];}
                double& g() {return m_data[1];}
                double& b() {return m_data[2];}
                double r() const {return m_data[0];}
                double g() const {return m_data[1];}
                double b() const {return m_data[2];}
                
                Color3(const double a, const double b, const double c)
                        : VecD_Base({a, b, c})
                        {}
                Color3(const VecD_Base<3>& v) : VecD_Base(v) {}
                Color3() : VecD_Base() {}
};

inline void write_color(std::ostream& out, const Color3& color, const int samples_per_pixel)
{
        double scale = 1./samples_per_pixel;
        out << static_cast<int>(256 * clamp(std::sqrt(scale*color.r()), 0, 0.999)) << " "
            << static_cast<int>(256 * clamp(std::sqrt(scale*color.g()), 0, 0.999)) << " "
            << static_cast<int>(256 * clamp(std::sqrt(scale*color.b()), 0, 0.999)) << "\n";
}

class Color4 : public VecD_Base<4>{
        public:
                double& r() {return m_data[0];}
                double& g() {return m_data[1];}
                double& b() {return m_data[2];}
                double& a() {return m_data[3];}
                double r() const {return m_data[0];}
                double g() const {return m_data[1];}
                double b() const {return m_data[2];}
                double a() const {return m_data[3];}
                
                Color4(const double a, const double b, const double c, const double d)
                        : VecD_Base({a, b, c, d})
                        {}
                Color4(const VecD_Base<4>& v) : VecD_Base(v) {}
                Color4() : VecD_Base() {}
};

inline void write_color(std::ostream& out, const Color4& color, const int samples_per_pixel)
{
        double scale = 1./samples_per_pixel;
        out << static_cast<int>(255.999 * clamp(std::sqrt(scale*color.r()), 0, 0.999)) << " "
            << static_cast<int>(255.999 * clamp(std::sqrt(scale*color.g()), 0, 0.999)) << " "
            << static_cast<int>(255.999 * clamp(std::sqrt(scale*color.b()), 0, 0.999)) << " "
            << static_cast<int>(255.999 * clamp(std::sqrt(scale*color.a()), 0, 0.999)) << "\n";
}

#endif // RAY_COLOR_H
