#include <cmath>
#include <iostream>
#include <locale>
#include <sys/types.h>
#include "macros.hpp"

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

#define print_color(r,g,b) "\033[38;2;" << r << ';' << g <<';' << b << 'm'
// Define a macro for setting the background color with RGB variables
#define print_bcolor(r, g, b) ("\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m").c_str()
#define print_bcolorstr(r, g, b) ("\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m")

// Define a macro to reset the background color
#define print_bcolor_reset "\033[0m"

#include "ascii_img.hpp"

struct RGB{
    u_char r = 0;
    u_char g = 0;
    u_char b = 0;
};

/*rgb colors*/
//lenght in bytes
std::string generate_ansi_row(RGB* colors, size_t lenght){
    std::string str;
    ulong sz = std::floor(lenght/3);
    for (size_t i = 0; i < sz; i++)
    {
        auto ptr = colors[i];
        auto r = ptr.r;
        auto g = ptr.g;
        auto b = ptr.b;
        str = str + print_bcolorstr(r,g,b) + ' ' + print_bcolor_reset;
        //logv((int)r << ':' << (int) g<<':' << (int) b);
        //logvar(sz);

    }
    
    return str;
}

int main(int argc, char const *argv[])
{
    //8
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int x=10,y=10;
 
    auto chars = ascii_img::load_image("test.png", x, y);

    cimg_library::CImg<u_char> image(chars, x,y,1,3);

    //image.display();

    logvar(w.ws_row);
    logvar(w.ws_col);
    logvar(w.ws_xpixel);
    logvar(w.ws_ypixel);


    for (size_t i = 0; i < x*y; i+=3)
    {
        std::cout << (int) chars[i] <<':' << (int) chars[i+1] <<':' <<(int) chars[i+2] << std::endl;
        auto img_px = image.data(i);
        std::cout << "Sample: " << (int) img_px[i] <<':' << (int) img_px[i+1] <<':' <<(int) img_px[i+2] << std::endl;

    }
    
    std::string out = "";
    out += generate_ansi_row((RGB*)chars, x *y* 3) + '\n';
    std::cout << out << std::endl;


    return 0;
}
