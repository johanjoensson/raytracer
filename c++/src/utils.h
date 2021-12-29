#ifndef RAY_UTILS_H
#define RAY_UTILS_H

#include <limits>
#include <random>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degree_to_radian(const double degrees)
{
        return degrees*pi/180;
}

inline double random_double(const double min = 0, const double max = 1.0)
{
        static std::random_device rd;
        static std::mt19937 generator(rd());

        std::uniform_real_distribution<double> distribution(min, max);
        return distribution(generator);
}

inline double clamp(const double x, const double min, const double max)
{
        if(x < min){
                return min;
        } else if(x > max){
                return max;
        }else{
                return x;
        }
}

#endif // RAY_UTILS_H
