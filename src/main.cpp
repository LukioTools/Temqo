
#include <iostream>
#include <glm/ext.hpp>

#include <cassert>



#include <curses.h>
#include <ncurses.h>


//please add more usefull macros like time and replacement text 
#include "macros.hpp"


WINDOW* root_ptr = nullptr; // window pointer for -> root window (usually not null)
WINDOW* ui_ptr = nullptr; // window pointer for -> user interface (usually not null)
WINDOW* list_ptr = nullptr; // window pointer for -> song list pointer (usually not null)
WINDOW* image_ptr = nullptr; // window pointer for -> current allocated space for the image (usually null)
WINDOW* vis_ptr = nullptr; // window pointer for -> current allocated space for audio visualition (usually null)


//initialize the screen and the pointers
int startup(){
    initscr();
    return 0;
}

//dealloc all the stuff
int stop(){
    endwin();
    return 0;
}

#define rte ;

int main(int argc, char const *argv[])
{
    
    check_err_not(1 != 1, "hello", std::logic_error);

    log("Hello World\n");
    

    stop();
    return 0;
}
