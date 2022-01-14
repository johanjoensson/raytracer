#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "vec.h"
#include "color.h"
#include "ray.h"
#include "hittable.h"
#include "hittable_list.h"
#include "camera.h"

#include "material.h"
#include "utils.h"

Color3 ray_color(const Ray& r, const Hittable& world, const int depth)
{
        if(depth <= 0){
                return Color3(0, 0, 0);
        }
        auto hit = world.hit(r, 1e-3, infinity);
        if(hit){
                auto attenuate_and_scatter = hit->material_ptr->scatter(r, hit.value());
                if(attenuate_and_scatter){
                        const Color3 attenuation = std::move(attenuate_and_scatter->first);
                        const Ray scattered = std::move(attenuate_and_scatter->second);
                        return attenuation.cwiseProduct(ray_color(scattered, world, depth - 1));
                }
                return Color3(0, 0, 0);
        }        
        // Vec3 unit_direction = r.direction();
        double t = 0.5*(r.direction().y() + 1.0);
        return (1. - t)*Color3(1., 1., 1.) + t*Color3(0.5, 0.7, 1.);
}

Hittable_list random_scene() {
    Hittable_list world;

    auto ground_material = std::make_shared<Lambertian>(Color3(0.5, 0.5, 0.5));
    world.add(std::make_unique<Sphere>(Point3(0,-10000,0), 10000, std::move(ground_material)));

    for (int a = -21; a < 21; a++) {
        for (int b = -21; b < 21; b++) {
            Point3 center(a + 0.9*random_double(), 0.2 + 0.6*random_double(), b + 0.9*random_double());
            while ((center - Point3(4, 0.2, 0)).norm() < 0.9){
                center = Point3(a + 0.9*random_double(), 0.2 + 0.6*random_double(), b + 0.9*random_double());
            }

            std::shared_ptr<Material> sphere_material;
            double choose_mat = random_double();
            if (choose_mat < 0.5) {
                    // diffuse
                    Vec3 albedo = random(0, 1).cwiseProduct(random(0, 1));
                    sphere_material = std::make_shared<Lambertian>(std::move(albedo));
                    world.add(std::make_unique<Sphere>(std::move(center), 0.2, std::move(sphere_material)));
            } else if (choose_mat < 0.80) {
                    // metal
                    auto albedo = random(0.2, 1);
                    auto fuzz = random_double(0, 0.75);
                    sphere_material = std::make_shared<Metal>(std::move(albedo), fuzz);
                    world.add(std::make_unique<Sphere>(std::move(center), 0.2, std::move(sphere_material)));
            } else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(random_double(1.2, 2.5));
                    world.add(std::make_unique<Sphere>(std::move(center), 0.2, std::move(sphere_material)));
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_unique<Sphere>(Point3(0, 1, 0), 1.0, std::move(material1)));
    material1 = std::make_shared<Dielectric>(2.5);
    world.add(std::make_unique<Sphere>(Point3(0, 1, 0), 0.7, std::move(material1)));

    auto material2 = std::make_shared<Lambertian>(Color3(0.4, 0.2, 0.1));
    world.add(std::make_unique<Sphere>(Point3(-4, 1, 0), 1.0, std::move(material2)));

    auto material3 = std::make_shared<Metal>(Color3(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_unique<Sphere>(Point3(4, 1, 0), 1.0, std::move(material3)));

    return world;
}

int main(){
        const double aspect_ratio = 16./9;
        const int image_width = 1920;
        const int image_height = static_cast<int>(image_width/aspect_ratio);
        const int samples_per_pixel = 10;
        const int max_depth = 50;

        if(SDL_Init(SDL_INIT_VIDEO) < 0){
                std::cerr << "Error initialising SDL\n";
                return -1;
        }
        SDL_Surface* img = SDL_CreateRGBSurfaceWithFormat(0, image_width, image_height, 32, SDL_PIXELFORMAT_RGBA32);
        if(img == NULL){
                std::cerr << "Error creating SDL surface\n";
                return -1;
        }

        Hittable_list world(random_scene());

        Point3 cam_pos(13, 2, 3), lookat(0, 0, 0);
        Vec3 vup(0,1,0);
        double dist_to_focus = 10;
        double aperture = 0.1;
        Camera cam(std::move(cam_pos), lookat, vup, aspect_ratio, 20, aperture, dist_to_focus);

        //std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
        std::cout << "Image width x height : " << image_width << " x " << image_height << "\n";

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
                        // write_color(std::cout, pixel_color, samples_per_pixel);
                        write_color(*img, i, image_height - 1 - j, pixel_color, samples_per_pixel);
                }
        }
        IMG_SavePNG(img, "image.png");
        SDL_FreeSurface(img);
        std::cerr << "\nDone\n";
        return 0;
}
