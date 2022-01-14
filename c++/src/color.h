#ifndef RAY_COLOR_H
#define RAY_COLOR_H

#include "vec.h"
#include "utils.h"
#include <SDL2/SDL.h>

inline void write_color(std::ostream& out, const Color3& color, const int samples_per_pixel)
{
        double scale = 1./samples_per_pixel;
        out << static_cast<int>(256 * clamp(std::sqrt(scale*color[0]), 0, 0.999)) << " "
            << static_cast<int>(256 * clamp(std::sqrt(scale*color[1]), 0, 0.999)) << " "
            << static_cast<int>(256 * clamp(std::sqrt(scale*color[2]), 0, 0.999)) << "\n";
}

inline void write_color(SDL_Surface& surf, int x, int y, const Color3& color, const int samples_per_pixel)
{
        double scale = 1./samples_per_pixel;
        SDL_LockSurface(&surf);
        uint32_t r = static_cast<uint8_t>(256 * clamp(std::sqrt(scale*color[0]), 0, 0.999));
        uint32_t g = static_cast<uint8_t>(256 * clamp(std::sqrt(scale*color[1]), 0, 0.999));
        uint32_t b = static_cast<uint8_t>(256 * clamp(std::sqrt(scale*color[2]), 0, 0.999));
        uint32_t a = 255;
        
        SDL_PixelFormat* format = surf.format;
        uint32_t* pixel_ptr = reinterpret_cast<uint32_t*>(surf.pixels) + y*surf.w + x;
        *pixel_ptr = (r << format->Rshift) + (g << format->Gshift) + (b << format->Bshift) + (a << format->Ashift);
        SDL_UnlockSurface(&surf);
}
#endif // RAY_COLOR_H
