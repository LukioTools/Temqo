

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <thread>

#if !defined(ASCII_IMG)
#define ASCII_IMG 1

#define STB_IMAGE_IMPLEMENTATION



#include "../lib/CImg-3.3.1/CImg.h"


namespace ascii_img
{
    template <typename T = u_char>
    struct RGB
    {
        T r;
        T g;
        T b;
    };
    
    class load_image_t {
        public:
            size_t x = 0;
            size_t y = 0;
            u_char* r;
            u_char* g;
            u_char* b;

            RGB<> get(size_t index){
                return {r[index], g[index], b[index]};
            }

            RGB<> operator()(size_t index){
                return {r[index], g[index], b[index]};
            }

            RGB<> operator[](size_t index){
                return {r[index], g[index], b[index]};
            }

            size_t size(){
                return x*y;
            }

            load_image_t(size_t _x, size_t _y, u_char* _r = nullptr, u_char* _g = nullptr, u_char* _b = nullptr) :x(_x), y(_y),  r(_r), g(_g), b(_b) {}

            ~load_image_t(){
                free(r);
                free(g);
                free(b);
            }
    };



    inline load_image_t* load_image(const char* filename, int x, int y, int interpelation = 1){
        cimg_library::CImg<u_char> image(filename);
        image.resize(x, y, -100,-100, interpelation);
        
        auto xy = x*y;

        u_char* r = image.data();
        u_char* g = &r[xy];
        u_char* b = &r[xy*2];
        
        u_char* rout = (u_char*) malloc(xy);;
        u_char* gout = (u_char*) malloc(xy);
        u_char* bout = (u_char*) malloc(xy);
        memcpy(rout, r, xy);
        memcpy(gout, g, xy);
        memcpy(bout, b, xy);
        //image.display();

        return new load_image_t({
            static_cast<size_t>(x),static_cast<size_t>(y),
            rout,
            gout,
            bout,
        });;
    }
} // namespace ascii_img



#endif // ASCII_IMG

