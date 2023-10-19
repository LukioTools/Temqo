#include <chrono>
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
#define print_bcolor(r, g, b) ("\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m ").c_str()
#define print_bcolorstr(r, g, b) ("\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m ")

// Define a macro to reset the background color
#define print_bcolor_reset "\033[0m"

//#include "ascii_img.hpp"
#include "ascii_img2.hpp"

struct RGB{
    u_char r = 0;
    u_char g = 0;
    u_char b = 0;
};

/*rgb colors*/
//lenght in bytes
std::string generate_ansi_img(ascii_img::load_image_t* pixels){
    std::string str = "";
    auto x = pixels->x;
    for (size_t i = 0; i < pixels->size(); i++)
    {
        auto ptr = pixels->get(i);
        //logvar((int) r) logvar((int) g)logvar((int) b)
        if(i%x == 0){
            str += '\n';
        }
        str += print_bcolorstr(ptr.r,ptr.g,ptr.b)  /*+ print_bcolor_reset*/;
    }
    
    return (str =+ print_bcolor_reset);
}

int main(int argc, char const *argv[])
{
    //8
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    //int x=w.ws_col,y=w.ws_row;
    int x=1000,y=1000;
 
    logvar(w.ws_row);
    logvar(w.ws_col);
    logvar(w.ws_xpixel);
    logvar(w.ws_ypixel);


    auto beg = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    //ascii_img::load_image("test.png", x, y, 1);1
    auto out = ascii_img::load_image("test_mnm.jpg", x, y);
    auto str_out = generate_ansi_img(out);

    auto end = std::chrono::high_resolution_clock::now().time_since_epoch().count();


    delete out;
    logv(str_out);



    logvar(end - beg)


    return 0;
}
