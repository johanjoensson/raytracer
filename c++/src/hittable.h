#ifndef RAY_HITTABLE_H
#define RAY_HITTABLE_H

#include "ray.h"
#include "vec.h"
#include <optional>

struct Hit_record{
        Point3 p;
        Vec3 normal;
        double t;
        bool front_face;

        Hit_record(const Point3& point, const Vec3& outwards_normal, const double intersection, const Ray& r)
                : p(point), normal(outwards_normal), t(intersection), front_face(dot(r.direction(), outwards_normal) < 0)
        {
                if(!front_face){
                        normal *= -1;
                }
        }
};

class Hittable{
        public:
                virtual std::optional<Hit_record> hit(const Ray& r, const double t_min, const double t_max) const = 0;
                Hittable() = default;
                Hittable(const Hittable&) = default;
                Hittable(Hittable&&) = default;
                virtual ~Hittable() = default;
                Hittable& operator=(const Hittable&) = default;
                Hittable& operator=(Hittable&&) = default;
};

class Sphere : public Hittable{
        public:
                virtual std::optional<Hit_record> hit(const Ray& r, const double t_min, const double t_max) const override
                {
                        Vec3 oc = r.origin() - m_center;
                        double a = r.direction().norm2();
                        double half_b = dot(oc, r.direction());
                        double c = oc.norm2() - m_radius*m_radius;
                        double discriminant = half_b*half_b - a*c;
                        if (discriminant < 0){
                                return std::nullopt;
                        }
                        double sqrtd = sqrt(discriminant);

                        double root = (-half_b - sqrtd)/a;
                        if (root < t_min || t_max < root){
                                root = (-half_b + sqrtd)/a;
                                if(root < t_min || t_max < root){
                                        return std::nullopt;
                                }
                        }
                        Vec3 outwards_normal = (r.at(root) - m_center)/m_radius;
                        Hit_record rec(r.at(root), outwards_normal, root, r);

                        return rec;
                }

                Sphere(const Point3& center, const double r)
                        : Hittable(), m_center(center), m_radius(r)
                {}
                Sphere() = default;
        private:
                Point3 m_center;
                double m_radius;

};
#endif // RAY_HITTABLE_H
