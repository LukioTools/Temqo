
#include <cstddef>
#include <cstdio>
#include <curses.h>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <new>
#include <vector>
#include <ncurses.h>
#include <glm/ext.hpp>

#include "macros.hpp"
#define LOG_WIN stdscr
#include "nc_macros.hpp"

class Window
{
private:
public:
    WINDOW* window = nullptr;
    Window(const  Window &)= delete;
    Window() : window(nullptr) {}
    Window(WINDOW* win) : window(win) {}
    Window(uint_fast16_t width, uint_fast16_t height, uint_fast16_t x_offset, uint_fast16_t y_offset) : window(nullptr) {
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



    Window& operator=(const  Window&)  = delete;

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


struct Element
{
    Window window;
    size_t size;
};


class Row
{
private:
    
public:
    std::vector<Element*> vec;

    Row(){};
    Row(std::initializer_list<Element*> el):vec(el){};
    ~Row() = default;



    int refresh(long height, size_t height_offset){
        get_max(x,y);
        if(height < 0){
            height = y+height;
        }
        if(vec.size() == 0) return -1;
        size_t 
            percent = 0,
            pos = 0;
        for (auto &&i : vec)
        {
            percent += i->size;
        }
        
        if(!percent) return -1;

        for (auto &&i : vec)
        {
            size_t width = x*(i->size)/percent;
            //logvar(height);
            //logvar(width);
            //logvar(height_offset);
            //logvar(pos);
            //logvar(i->window);
            auto win = newwin(height, width, height_offset, pos);
            //logvar(win)
            i->window = win;
            //logvar(i->window);
            pos += width;
            box(win, 0, 0);
            wrefresh(win);
        }
        return OK;
    }
};

class WindowManager
{
private:
    
public:
    std::vector<Row> rows;
    
    WindowManager() {}
    WindowManager(std::initializer_list<Row> ls): rows(ls) {}
    
    ~WindowManager() {}
};
int startup(){
    initscr();
    cbreak(); // Disable line buffering
    noecho(); // Don't echo user input
    keypad(stdscr, TRUE); // Enable function keys
    refresh();
    return 0;
}

int stop(){
    endwin();
    return 0;
}


int main(int argc, char const *argv[])
{
    startup();
    Row r;
    auto e = new Element{Window(nullptr), 2};
    //logvar(e);
    
    r.vec.push_back(e);
    //r.vec.push_back(new Element{Window(nullptr), 2});
    get_max(x,y);
    auto i = r.refresh(-1,1 );
    auto s = (r.vec[0]->window.exists() ? "True":"False" );

    getch();
    stop();
    return 0;
}
