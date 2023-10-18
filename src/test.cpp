

#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <vector>

std::ofstream different_terminal("/dev/pts/3");

#include "ascii_img.hpp"


int main(int argc, char const *argv[])
{
    constexpr int x=20, y=40;

    auto values = ascii_img::load_image("src/test.png", x, y, 1);


    for (size_t i = 0; i < x*y; i++)
    {
        u_char* val = &values[i]; 
        different_terminal <<  (int) val[0] << ':' <<(int) val[1] << ':' <<  (int) val[2] <<  std::endl;
    }

    cimg_library::CImg<u_char> img(values, x,y,1,3);

    
    img.display();

    delete[] values;
    
    return 0;
}
