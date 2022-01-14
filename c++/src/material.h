#ifndef RAY_MATERIAL_H
#define RAY_MATERIAL_H

#include "utils.h"
#include "hittable.h"
#include "color.h"
#include "ray.h"
#include "vec.h"

#include <optional>
#include <utility>

class Material{
        protected:
                using attenuate_and_scatter = std::pair<Color3, Ray>;

        public:
                virtual std::optional<attenuate_and_scatter> scatter(const Ray& r_in, const Hit_record& rec) const = 0;

                Material() = default;
                Material(const Material&) = default;
                Material(Material&&) = default;
                virtual ~Material() = default;

                Material& operator=(const Material&) = default;
                Material& operator=(Material&&) = default;
};

class Lambertian : public Material{
        public:
                virtual std::optional<attenuate_and_scatter> scatter(const Ray& , const Hit_record& rec) const override
                {
                        // Vec3 scatter_direction = rec.normal + random_in_hemisphere(rec.normal);
                        Vec3 scatter_direction = rec.normal + random_unit_vector();
                        if(near_zero(scatter_direction)){
                                scatter_direction = rec.normal;
                        }
                        return attenuate_and_scatter(m_albedo, Ray(rec.p, std::move(scatter_direction)));
                }

                Lambertian(Color3&& albedo) : m_albedo(std::move(albedo)) {}

                Lambertian() = default;
                Lambertian(const Lambertian&) = delete;
                Lambertian(Lambertian&&) = default;
                virtual ~Lambertian() override = default;

                Lambertian& operator=(const Lambertian&) = delete;
                Lambertian& operator=(Lambertian&&) = default;

        private:
                Color3 m_albedo;
};

class Metal : public Material{
        public:
                virtual std::optional<attenuate_and_scatter> scatter(const Ray& r_in, const Hit_record& rec) const override
                {
                        Vec3 reflected = reflect(r_in.direction(), rec.normal);
                        Ray scattered(rec.p, reflected + m_fuzziness*random_in_unit_sphere());
                        if(dot(scattered.direction(), rec.normal) > 0){
                                return attenuate_and_scatter(m_albedo, std::move(scattered));
                        }else{
                                return std::nullopt;
                        }
                }

                Metal(Color3&& albedo, double fuzziness = 0) : m_albedo(std::move(albedo)), m_fuzziness(fuzziness) {}
        private:
                Color3 m_albedo;
                double m_fuzziness;
};

class Dielectric : public Material{
        public:
                virtual std::optional<attenuate_and_scatter> scatter(const Ray& r_in, const Hit_record& rec) const override
                {
                        const Color3 attenuation(1, 1, 1);
                        double refraction_ratio = rec.front_face ? 1./m_ri : m_ri;

                        // Vec3 unit_direction(r_in.direction());
                        double cos_theta = std::min(dot(-r_in.direction(), rec.normal), 1.0);
                        double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

                        bool cannot_refract = refraction_ratio*sin_theta > 1.0;


                        Vec3 direction;
                        if(cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()){
                                direction = reflect(r_in.direction(), rec.normal);
                        }else{
                                direction = refract(r_in.direction(), rec.normal, refraction_ratio);
                        }
                        return attenuate_and_scatter(attenuation, Ray(rec.p, std::move(direction)));
                }

                Dielectric(const double refractive_index) : m_ri(refractive_index) {}
                Dielectric() = default;
                Dielectric(const Dielectric&) = default;
                Dielectric(Dielectric&&) = default;
                virtual ~Dielectric() = default;

                Dielectric& operator=(const Dielectric&) = default;
                Dielectric& operator=(Dielectric&&) = default;
        private:
                double m_ri;
                static double reflectance(double cosine, double refractive_index)
                {
                        double r0 = (1 - refractive_index) / (1 + refractive_index);
                        r0 *= r0;
                        return r0 + (1 - r0)*std::pow(1 - cosine, 5);
                }
};
#endif // RAY_MATERIAL_H
