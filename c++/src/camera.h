#ifndef RAY_CAMERA_H
#define RAY_CAMERA_H

#include "utils.h"
#include "vec.h"
#include "ray.h"

class Camera{
        public:
                Ray get_ray(const double u, const double v) const
                {
                        return Ray(m_origin, m_lower_left_corner + u*m_horizontal + v*m_vertical - m_origin);
                }

                Camera() : Camera(Vec3(0,0,0), 16./9, 2, 1) {}
                Camera(const Vec3& origin, const double aspect_ratio, const double viewport_height, double focal_length)
                        : m_origin(origin), m_lower_left_corner(origin - Vec3(aspect_ratio*viewport_height, 0, 0)/2 - Vec3(0, viewport_height, 0)/2 - Vec3(0, 0, focal_length)), m_horizontal(aspect_ratio*viewport_height, 0, 0), m_vertical(0, viewport_height, 0)
                {}
        private:
                Point3 m_origin;
                Point3 m_lower_left_corner;
                Vec3 m_horizontal;
                Vec3 m_vertical;
};

#endif // RAY_CAMERA_H
