


#include <fstream>
#include <iostream>



std::ofstream different_terminal("/dev/pts/3");


#define LOG_STREAM different_terminal
#include "macros.hpp"
#define LOG_WIN log_scr
#include "nc_macros.hpp"

#include "ascii_img.hpp"


int main(int argc, char const *argv[])
{
    constexpr int x=30, y=60;
    auto values = ascii_img::load_image("src/test.png", x, y);
    for (size_t i = 0; i < x*y; i++)
    {
        logv( "Values: " << (int) values[i].r<< " "<<(int) values[i].g<< " " << (int) values[i].b)
        break;
    }

    delete[] values;
    
    return 0;
}
