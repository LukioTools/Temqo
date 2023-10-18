

#include <cstdlib>
#include <sys/types.h>
#if !defined(ASCII_IMG)
#define ASCII_IMG 1


#define STB_IMAGE_IMPLEMENTATION

#include "../lib/CImg-3.3.1/CImg.h"

namespace ascii_img
{



    inline u_char* load_image(const char* filename, int x, int y, int interpelation = 1){
        cimg_library::CImg<u_char> image(filename);
        image.resize(x, y, -100,-100, interpelation);
        auto size = x*y*3*sizeof(u_char);
        u_char* out = (u_char*) malloc(size);
        memcpy(out, image.data(), size);
        //image.display();
        return out;
    }
} // namespace ascii_img



#endif // ASCII_IMG

