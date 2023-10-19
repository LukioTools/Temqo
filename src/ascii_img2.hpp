

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <thread>

#if !defined(ASCII_IMG)
#define ASCII_IMG 1

#define STB_IMAGE_IMPLEMENTATION

#include "macros.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stbi/stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../lib/stbi/stb_image_resize2.h"



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
            size_t x;
            size_t y;
            u_char channels;
            u_char* data;

            RGB<> get(size_t index){
                auto ptr = &data[index*channels];
                return {ptr[0], ptr[1], ptr[2]};
            }

            RGB<> operator()(size_t index){
                return get(index);
            }

            RGB<> operator[](size_t index){
                return get(index);
            }

            size_t size(){
                return x*y;
            }

            load_image_t(size_t _x, size_t _y, u_char* _data = nullptr, u_char _channels = 4) :x(_x), y(_y), data( _data), channels(_channels){}

            ~load_image_t(){
                free(data);
            }
    };



    inline load_image_t* load_image(std::string filename, int x, int y){
        
        int cx,cy,n_channels;

        u_char* data = stbi_load(filename.c_str(), &cx, &cy, &n_channels, 0);
        check(!data, ("could not load image: " + filename).c_str());

        unsigned char* out_data = (unsigned char*)malloc(x * y * n_channels);
        check(!out_data, "could not allocate memory for image");

        stbir_resize_uint8(data, cx, cy, 0, out_data, x, y, 0, n_channels);
        
        
        free(data);

        return new load_image_t({
            static_cast<size_t>(x),static_cast<size_t>(y),
            out_data,static_cast<u_char>(n_channels)
        });;
    }
} // namespace ascii_img



#endif // ASCII_IMG

