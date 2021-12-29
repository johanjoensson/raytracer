#include <iostream>
#include "vec.h"
#include "color.h"
#include "ray.h"
#include "hittable.h"
#include "hittable_list.h"
#include "camera.h"

#include "utils.h"

Color3 ray_color(const Ray& r, const Hittable& world, const int depth)
{
        if(depth <= 0){
                return Color3(0, 0, 0);
        }
        auto hit = world.hit(r, 1e-10, infinity);
        if(hit){
                //Point3 target = hit->p + hit->normal + random_in_hemisphere(hit->normal);
                Point3 target = hit->p + hit->normal + random_unit_vector();
                return 0.5*ray_color(Ray(hit->p, target - hit->p), world, depth - 1);
        }        
        Vec3 unit_direction {unit_vector(r.direction())};
        double t = 0.5*(unit_direction.y() + 1.0);
        return (1. - t)*Color3(1., 1., 1.) + t*Color3(0.5, 0.7, 1.);
}

int main(){
        const double aspect_ratio = 16./9;
        const int image_width = 400;
        const int image_height = static_cast<int>(image_width/aspect_ratio);
        const int samples_per_pixel = 100;
        const int max_depth = 50;

        Hittable_list world;
        world.add(std::make_unique<Sphere>(Point3(0, 0, -1), 0.5));
        world.add(std::make_unique<Sphere>(Point3(0, -100.5, -1), 100));

        Camera cam;

        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

        for(int j = image_height -1; j >= 0; j--){
                std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
                for(int i = 0; i < image_width; i++){
                        Color3 pixel_color(0, 0, 0);
                        for(int s = 0; s < samples_per_pixel; s++){
                                double u = (i + random_double()) / (image_width - 1);
                                double v = (j + random_double()) / (image_height - 1);
                                Ray r(cam.get_ray(u, v));
                                pixel_color += ray_color(r, world, max_depth);
                        }
                        write_color(std::cout, pixel_color, samples_per_pixel);
                }
        }
        std::cerr << "\nDone\n";
        return 0;
}
