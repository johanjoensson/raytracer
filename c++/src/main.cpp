#include <iostream>
#include "vec.h"
#include "color.h"
#include "ray.h"
#include "hittable.h"

double hit_sphere(const Point3& center, double radius, const Ray& r)
{
        Vec3 oc = r.origin() - center;
        double a = r.direction().norm2();
        double half_b = dot(oc, r.direction());
        double c = oc.norm2() - radius*radius;
        double discriminant = half_b*half_b - a*c;
        if (discriminant < 0){
                return -1;
        }else{
                return (-half_b - sqrt(discriminant)) / a;
        }
}

Color3 ray_color(const Ray& r)
{
        double t = hit_sphere(Point3(0,0,-1), 0.5, r);
        if (t > 0){
                Vec3 N(unit_vector(r.at(t) - Vec3(0,0,-1)));
                return 0.5*Color3(N.x() + 1, N.y() + 1, N.z() + 1);
        }
        Vec3 unit_direction {unit_vector(r.direction())};
        t = 0.5*(unit_direction.y() + 1.0);
        return (1. - t)*Color3(1., 1., 1.) + t*Color3(0.5, 0.7, 1.);
}

int main(){
        const double aspect_ratio = 16./9;
        const int image_width = 400;
        const int image_height = static_cast<int>(image_width/aspect_ratio);

        double viewport_height = 2;
        double viewport_width = aspect_ratio * viewport_height;
        double focal_length = 1;

        Point3 origin{0,0,0};
        Vec3 horizontal(viewport_width, 0, 0);
        Vec3 vertical(0, viewport_height, 0);
        Point3 lower_left_corner = origin - horizontal/2 - vertical/2 - Vec3(0, 0, focal_length);


        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

        for(int j = image_height -1; j >= 0; j--){
                std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
                for(int i = 0; i < image_width; i++){
                        double u = double(i) / (image_width - 1);
                        double v = double(j) / (image_height - 1);
                        Ray r (origin, lower_left_corner + u*horizontal + v*vertical - origin);
                        Color3 pixel_color(ray_color(r));
                        write_color(std::cout, pixel_color);
                }
        }
        std::cerr << "\nDone\n";
        return 0;
}
