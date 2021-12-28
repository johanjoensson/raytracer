#ifndef RAY_COLOR_H
#define RAY_COLOR_H
#include "vec.h"

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

void write_color(std::ostream& out, const Color3& color)
{
        out << static_cast<int>(255.999 * color.r()) << " "
            << static_cast<int>(255.999 * color.g()) << " "
            << static_cast<int>(255.999 * color.b()) << "\n";
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

void write_color(std::ostream& out, const Color4& color)
{
        out << static_cast<int>(255.999 * color.r()) << " "
            << static_cast<int>(255.999 * color.g()) << " "
            << static_cast<int>(255.999 * color.b()) << " "
            << static_cast<int>(255.999 * color.a()) << "\n";
}

#endif // RAY_COLOR_H
