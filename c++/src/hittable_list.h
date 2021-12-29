#ifndef RAY_HITTABLE_LIST_H
#define RAY_HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

class Hittable_list : public Hittable {
        public:
                virtual std::optional<Hit_record> hit(const Ray& r, const double t_min, const double t_max) const override
                {
                        std::optional<Hit_record> res (std::nullopt);
                        double closest_so_far = t_max;
                        for(const auto& object : m_objects){
                                auto temp_res = object->hit(r, t_min, closest_so_far);
                                if(temp_res){
                                        closest_so_far = temp_res->t;
                                        res = std::move(temp_res);
                                }
                        }
                        return res;
                }
                void add(std::unique_ptr<Hittable> object) {m_objects.push_back(std::move(object));}
                void clear() {m_objects.clear();}
        
                Hittable_list(std::vector<std::unique_ptr<Hittable>>&& objects) : m_objects(std::move(objects)) {}
                
                Hittable_list() : m_objects() {}
                Hittable_list(const Hittable_list&) = default;
                Hittable_list(Hittable_list&&) = default;
                ~Hittable_list() = default;

                Hittable_list& operator=(const Hittable_list&) = default;
                Hittable_list& operator=(Hittable_list&&) = default;
        private:
                std::vector<std::unique_ptr<Hittable>> m_objects;
};
#endif // RAY_HITTABLE_LIST_H
