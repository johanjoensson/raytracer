#ifndef RAY_HITTABLE_LIST_H
#define RAY_HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

class Hittable_list : public Hittable {
        public:
                virtual std::optional<Hit_record> hit(const Ray& r, const double t_min, const double t_max) const override
                {
                        std::optional<Hit_record> res;
                        double closest_so_far = t_max;
                        for(const auto& sphere : m_spheres){
                                auto temp_res = sphere->hit(r, t_min, closest_so_far);
                                if(temp_res){
                                        closest_so_far = temp_res->t;
                                        res = std::move(temp_res);
                                }
                        }
                        return res;
                }
                void add(std::unique_ptr<Sphere>&& sphere) {m_spheres.push_back(std::move(sphere));}
                void clear() {m_spheres.clear();}
        
                Hittable_list(std::vector<std::unique_ptr<Sphere>>&& spheres) : m_spheres(std::move(spheres)) {}
                
                Hittable_list() : m_spheres() {}
                Hittable_list(const Hittable_list&) = default;
                Hittable_list(Hittable_list&&) = default;
                ~Hittable_list() = default;

                Hittable_list& operator=(const Hittable_list&) = default;
                Hittable_list& operator=(Hittable_list&&) = default;
        private:
                std::vector<std::unique_ptr<Sphere>> m_spheres;
};
#endif // RAY_HITTABLE_LIST_H
