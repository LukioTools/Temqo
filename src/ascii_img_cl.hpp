
#include <CL/opencl.hpp>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#if !defined(ASCII_IMG_CL)
#define ASCII_IMG_CL

#define CL_HPP_TARGET_OPENCL_VERSION 300

namespace ascii_img
{
    
    template <typename T = unsigned char>
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
            unsigned char* r = nullptr;
            unsigned char* g = nullptr;
            unsigned char* b = nullptr;

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

            load_image_t(){}

            load_image_t(size_t _x, size_t _y, unsigned char* _r = nullptr, unsigned char* _g = nullptr, unsigned char* _b = nullptr) :x(_x), y(_y),  r(_r), g(_g), b(_b) {}

            ~load_image_t(){
                free(r);
                free(g);
                free(b);
            }
    };


    inline load_image_t* load_image_cl(const char* filename, int x, int y, int interpelation = 1){
        

        return new load_image_t;

    }

} // namespace ascii_img


#endif // ASCII_IMG_CL
