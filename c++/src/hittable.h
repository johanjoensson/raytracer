#ifndef RAY_HITTABLE_H
#define RAY_HITTABLE_H

#include "ray.h"
#include "vec.h"
#include <optional>
#include <memory>

class Material;

struct Hit_record{
        Point3 p;
        Vec3 normal;
        Material* material_ptr;
        double t;
        bool front_face;

        Hit_record(Point3&& point, Vec3&& outwards_normal, Material* material, const double intersection, const Ray& r)
                : p(std::move(point)), normal(std::move(outwards_normal)), material_ptr(material), t(intersection), front_face(dot(r.direction(), outwards_normal) < 0)
        {
                if(!front_face){
                        normal *= -1;
                }
        }

        Hit_record() :p(), normal(), material_ptr(nullptr), t(), front_face() {}
        Hit_record(const Hit_record&) = delete;
        Hit_record(Hit_record&&) = default;
        ~Hit_record() = default;

        Hit_record& operator=(const Hit_record&) = delete;
        Hit_record& operator=(Hit_record&&) = default;
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
                        double a = 1; // r.direction().norm2();
                        double half_b = dot(oc, r.direction());
                        double c = oc.squaredNorm() - m_radius*m_radius;
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
                        Hit_record rec(r.at(root), std::move(outwards_normal), m_material_ptr.get(), root, r);

                        return rec;
                }

                Sphere(Point3&& center, const double r, std::shared_ptr<Material>&& material)
                        : Hittable(), m_center(std::move(center)), m_radius(r), m_material_ptr(material)
                {}
                Sphere() = default;
        private:
                Point3 m_center;
                double m_radius;
                std::shared_ptr<Material> m_material_ptr;

};
#endif // RAY_HITTABLE_H
