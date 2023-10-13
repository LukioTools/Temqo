
#include <iostream>
#include <glm/ext.hpp>
#include <thread>

#include <cassert>

// -lncurses
#include <curses.h>
#include <ncurses.h>





WINDOW* root_ptr = nullptr; // window pointer for -> root window (usually not null)
WINDOW* ui_ptr = nullptr; // window pointer for -> user interface (usually not null)
WINDOW* log_ptr = nullptr; // window pointer for -> user interface (usually not null)
WINDOW* list_ptr = nullptr; // window pointer for -> song list pointer (usually not null)
WINDOW* playlists_ptr = nullptr; // window pointer for -> current allocated space for audio visualition (usually not null)
WINDOW* image_ptr = nullptr; // window pointer for -> current allocated space for the image (usually null)
WINDOW* vis_ptr = nullptr; // window pointer for -> current allocated space for audio visualition (usually null)


//please add more usefull macros like time and replacement text 
#include "macros.hpp"
#define LOG_WIN log_ptr
#include "nc_macros.hpp"



//initialize the screen and the pointers
int startup(){
    initscr();
    cbreak(); // Disable line buffering
    noecho(); // Don't echo user input
    keypad(stdscr, TRUE); // Enable function keys
    refresh();

    root_ptr = stdscr; // window pointer for -> root window (usually not null)
    ui_ptr = stdscr; // window pointer for -> user interface (usually not null)
    log_ptr = stdscr; // window pointer for -> user interface (usually not null)
    list_ptr = stdscr; // window pointer for -> song list pointer (usually not null)
    playlists_ptr = stdscr;
    image_ptr = stdscr; // window pointer for -> current allocated space for the image (usually null)
    vis_ptr = stdscr; // window pointer for -> current allocated space for audio visualition (usually null)
    

    get_max(width, height);

    constexpr int text_size = 1;

    log_ptr = subwin(stdscr, text_size, width, 0, 0);
    ui_ptr = subwin(stdscr, text_size, width, height - text_size, 0);
    playlists_ptr = subwin(stdscr, height- 2*text_size, width * 1/6, 1, 0);
    box(playlists_ptr, 0, 0);
    wrefresh(playlists_ptr);


    return 0;
}

//dealloc all the stuff
int stop(){
    endwin();
    return 0;
}


int main(int argc, char const *argv[])
{
    
    check(startup(), "Library initialization failed");
    
    get_max(width, height);

    
    
    nc_log("Hello World (width:%i, height:%i)", width, height);


    wprintw(ui_ptr, "Command: ");
    wrefresh(ui_ptr);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    check(stop(), "Library de-initialization failed");
    return 0;
}
