#ifndef RAY_CAMERA_H
#define RAY_CAMERA_H

#include "utils.h"
#include "vec.h"
#include "ray.h"

class Camera{
        public:
                Ray get_ray(const double u, const double v) const
                {
                        Vec3 rd = m_lens_radius * random_in_unit_disk();
                        Vec3 offset = m_x*rd.x() + m_y*rd.y();
                        return Ray(m_origin + offset,
                                   m_lower_left_corner + u*m_horizontal + v*m_vertical - m_origin - offset);
                }

                Camera() : Camera(Vec3(0,0,0), Vec3(0, 0, -1), Vec3(0, 1, 0), 16./9, 90, 0, 1) {}
                Camera(Vec3&& origin, const Vec3& lookat, const Vec3& vup, const double aspect_ratio, const double vertical_fov, const double aperture, const double focus_distance)
                        : m_origin(std::move(origin)), m_lower_left_corner(), m_horizontal(), m_vertical(), m_x(), m_y(), m_z(), m_lens_radius(aperture/2)
                {
                        double theta = degree_to_radian(vertical_fov);
                        double h = std::tan(theta/2);
                        double viewport_height = 2*h;
                        double viewport_width = aspect_ratio*viewport_height;

                        m_z = unit_vector(m_origin - lookat);
                        m_x = unit_vector(cross(vup, m_z));
                        m_y = cross(m_z, m_x);

                        m_horizontal = focus_distance*viewport_width*m_x;
                        m_vertical = focus_distance*viewport_height*m_y;
                        m_lower_left_corner = m_origin - m_horizontal/2 - m_vertical/2 - focus_distance*m_z;
                }
        private:
                Point3 m_origin;
                Point3 m_lower_left_corner;
                Vec3 m_horizontal;
                Vec3 m_vertical;
                Vec3 m_x, m_y, m_z;
                double m_lens_radius;
};

#endif // RAY_CAMERA_H
