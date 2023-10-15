
#include <cmath>
#include <stdexcept>
#include <sys/types.h>
#if !defined(ASCII_IMG)
#define ASCII_IMG 1

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "macros.hpp"

namespace ascii_img
{
    struct RGB{
        u_char r = 0;
        u_char g = 0;
        u_char b = 0;
    };

    inline RGB* load_image(const char* filename, int x, int y ){
        auto out = new RGB[x*y];
        int ix = 0, iy = 0, channels = 0;
        stbi_uc* img = stbi_load(filename, &ix, &iy, &channels, 0);
        logvar(filename);
        logvar(ix);
        logvar(iy);
        logvar(channels);

        if(!img){
            auto err =  stbi_failure_reason();
            logvar(err);
            throw std::runtime_error(err);
        }

        if(!ix || !iy){
            auto err = "Loaded image was interpeted incorrectly (x, y were zero)";
            logvar(err);
            throw std::runtime_error(err);
        } 
        auto x_step = (double) ix/x;
        auto y_step = (double) iy/y;

        


        size_t iteration =0;


        for (size_t j = 0; j < y; j++)
        {
            for (size_t i = 0; i < x; i++)
            {
                double e = i*x_step+j*iy*y_step;
                double idx = (double) e*4;
                auto ptr = &img[(int) std::floor(idx)];
                logvar(i);
                logvar(ix);
                logvar(iy);
                logvar(x_step);
                logvar(y_step);
                logvar(e)
                logvar(idx)
                logvar(channels)
                logvar(std::floor(idx));
                out[iteration] = {
                    ptr[0],
                    ptr[1],
                    ptr[2],
                };
                iteration ++;
            }
        }
        
        
        
        
        
        exit:

        logvar(iteration);


        

        stbi_image_free(img);
        return out;
    }
} // namespace ascii_img



#endif // ASCII_IMG

