#ifndef RAY_RAY_H
#define RAY_RAY_H

#include "vec.h"

class Ray {
        public:
                Point3 at(const double t) const {return m_origin + t*m_direction;}
                const Vec3& direction() const {return m_direction;}
                const Point3& origin() const {return m_origin;}

                Ray() = default;
                Ray(Point3 origin, Vec3 direction)
                        : m_origin(std::move(origin)), m_direction(unit_vector(direction))
                {}

                Ray(const Ray&) = delete;
                Ray(Ray&&) = default;

                Ray& operator=(const Ray&) = delete;
                Ray& operator=(Ray&&) = default;
                ~Ray() = default;
        private:
                Point3 m_origin;
                Vec3 m_direction;
};
#endif // RAY_RAY_H
