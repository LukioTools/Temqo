

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
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
#include "rgb.hpp"

namespace ascii_img
{
    
    
    class load_image_t {
        public:
            size_t x;
            size_t y;
            u_char channels;
            u_char* data;

            /** 
             * @param index index of the value to be gotten
             * @throws std::out_of_range throws out of range if index is bigger than x*y (size)
             * @return u_char* returns pointer to the start of the values
             * @note recommended to use since you can check if there are the channels necessary and 
            */
            inline u_char* get_ptr(size_t index){
                auto sz = size();
                if(index >= sz)
                    throw std::out_of_range("load_image_t::get(index) index out of range: (index)" + std::to_string(index) + ": (size)" + std::to_string(sz));
                auto ptr = &data[index*channels];
                return ptr;
            }

            /** 
             * @param index index of the value to be gotten
             * @return RGB<> returns rgb values of that specific pointer
             * @note doesnt check bounds or if the channels exist, not recommended use, unless performance is absolutely necessary
             * @deprecated so dangerous so it shall be marked forevermore
            */
            inline RGB<> get_unsafe_dangerous(size_t index){
                auto ptr = &data[index*channels];
                return {
                    ptr[0], 
                    ptr[1], 
                    ptr[2]
                };
            }

            /** 
             * @param index index of the value to be gotten
             * @throws {std::out_of_range} throws out of range if index is bigger than x*y (size)
             * @return {RGB<>} returns rgb values of that specific pointer
             * @note doesnt check if there are actually 3 channels in the data set, can set wrong data or segfault if there are not 3 or more channels
            */
            inline RGB<> get_unsafe(size_t index){
                auto sz = size();
                if(index >= sz)
                    throw std::out_of_range(
                        "load_image_t::get(index) index out of range: (index)" 
                        + std::to_string(index) 
                        + ": (size)" 
                        + std::to_string(sz)
                    );
                auto ptr = &data[index*channels];
                return {
                    ptr[0], 
                    ptr[1], 
                    ptr[2]
                };
            }

            /** 
             * @param index index of the value to be gotten
             * @throws {std::out_of_range} throws out of range if index is bigger than x*y (size)
             * @return {RGB<>} returns rgb values of that specific pointer
             * @note checks if there are actually 3 channels in the data set, sets the values to 0 if it doesnt exist
            */
            inline RGB<> get(size_t index){
                auto sz = size();
                if(index >= sz)
                    throw std::out_of_range(
                        "load_image_t::get(index) index out of range: (index)" 
                        + std::to_string(index) 
                        + ": (size)" 
                        + std::to_string(sz)
                    );
                auto ptr = &data[index*channels];
                return {
                    channels > 0 ? ptr[0] : static_cast<unsigned char>(0), 
                    channels > 1 ? ptr[1] : static_cast<unsigned char>(0), 
                    channels > 2 ? ptr[2] : static_cast<unsigned char>(0)
                };
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
            size_t bsize(){
                return x*y*channels;
            }

            u_char* begin(){
                return data;
            }

            u_char* end(){
                return data+bsize();
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

