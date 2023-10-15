///ai shit 




#include <cstdint>
#include <cstdio>
#include <glm/fwd.hpp>
#include <initializer_list>
#include <iostream>
#include <glm/ext.hpp>
#include <thread>
#include <locale.h>

#include <glm/ext.hpp>

#include <cassert>

// -lncurses
#include <curses.h>
#include <ncurses.h>
#include <vector>

class Window
{
private:
public:
    WINDOW* window = nullptr;
    Window() : window(stdscr) {}
    Window(uint_fast16_t width, uint_fast16_t height, uint_fast16_t x_offset, uint_fast16_t y_offset) : window(stdscr) {
        window = subwin(stdscr, width, height, x_offset, y_offset);
    }

    glm::ivec2 size(){
        int x, y;
        getmaxyx(window, y, x );
        return {x,y};
    }

    operator glm::ivec2() {
        int x, y;
        getmaxyx(window, y, x );
        return {x,y};
    }
    
    operator WINDOW*() {
        return window;
    }

    Window& operator=(WINDOW* wind){
        if(exists()){
            delwin(window);
        }
        window = wind;
        return *this;
    }

    bool exists(){
        return (window != nullptr && window != stdscr);
    }
    ~Window() {
        delwin(window);
    }
};


class Row
{
private:
    
public:
    Window* windows; //null terminated?
    ushort relative_height;
    Row() {
    }
    ~Row() {}
};

class WindowManager
{
private:
    
public:
    std::vector<Window*> windows;
    
    WindowManager() {}
    WindowManager(std::initializer_list<Window*> ls): windows(ls) {}
    
    ~WindowManager() {}
};

Window root_ptr;        // window pointer for -> root window (usually not null)
Window ui_ptr;          // window pointer for -> user interface (usually not null)
Window log_ptr;         // window pointer for -> user interface (usually not null)
Window list_ptr;        // window pointer for -> song list pointer (usually not null)
Window playlists_ptr;   // window pointer for -> playlist pointer (usually not null)
Window image_ptr;       // window pointer for -> current allocated space for the image (usually null)
Window vis_ptr;         // window pointer for -> current allocated space for audio visualition (usually null)


//please add more usefull macros like time and replacement text 
#include "macros.hpp"
#define LOG_WIN log_ptr
#include "nc_macros.hpp"






class Border{
public:
    wchar_t 
        left, 
        right, 
        top, 
        bottom, 
        top_left, 
        top_right, 
        bottom_left, 
        bottom_right

    ;

    Border(chtype _left = 0,chtype _right = 0,chtype _top = 0,chtype _bottom = 0,chtype _top_left = 0,chtype _top_right = 0,chtype _bottom_left = 0,chtype _bottom_right = 0): 
    left(_left), right(_right), top(_top), bottom(_bottom), top_left(_top_left), top_right(_top_right), bottom_left(_bottom_left), bottom_right(_bottom_right) 
    {
    }

    void draw(Window& win){
        //if(!win.exists()) {
        //    std::cerr << "Window didnt exist\n" << std::endl;
        //    return;
        //};
//
        //glm::ivec2 size = win.size();

    }
    
};

Border left_border(0,0,0,0,0, 0,0, 0);




//initialize the screen and the pointers
int startup(){
    initscr();
    cbreak(); // Disable line buffering
    noecho(); // Don't echo user input
    keypad(stdscr, TRUE); // Enable function keys
    refresh();

    root_ptr = stdscr;      
    ui_ptr = stdscr;        
    log_ptr = stdscr;       
    list_ptr = stdscr;      
    playlists_ptr = stdscr; 
    image_ptr = stdscr;     
    vis_ptr = stdscr;       
    

    get_max(width, height);

    constexpr int text_size = 1;

    log_ptr = subwin(stdscr, text_size, width, 0, 0);
    ui_ptr = subwin(stdscr, text_size, width, height - text_size, 0);
    playlists_ptr = subwin(stdscr, height- 2*text_size, width * 1/9, 1, 0);
    list_ptr = subwin(stdscr, height- 2*text_size, width * 1/9, 1, 0);
    

    left_border.draw(playlists_ptr);
    //nc_log("Exists: %s (%p) [%p] {%p}\n", playlists_ptr.exists() ? "True":"False", playlists_ptr.window, ptr,stdscr);

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
    
    
    //nc_log("Hello World (width:%i, height:%i)", width, height);

    wprintw(ui_ptr.window, "Command: ");
    wrefresh(ui_ptr.window);

    getch();

    check(stop(), "Library de-initialization failed");


    return 0;
}
