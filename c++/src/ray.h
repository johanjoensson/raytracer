#ifndef RAY_RAY_H
#define RAY_RAY_H

#include "vec.h"

class Ray {
        public:
                Point3 at(const double t) const {return m_origin + t*m_direction;}
                Vec3 direction() const {return m_direction;}
                Point3 origin() const {return m_origin;}

                Ray() = default;
                Ray(const Point3& origin, const Vec3& direction)
                        : m_origin(origin), m_direction(direction)
                {}

                Ray(const Ray&) = default;
                Ray(Ray&&) = default;

                Ray& operator=(const Ray&) = default;
                Ray& operator=(Ray&&) = default;
                ~Ray() = default;
        private:
                Point3 m_origin;
                Vec3 m_direction;
};
#endif // RAY_RAY_H
