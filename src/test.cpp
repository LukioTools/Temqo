
#include <cstddef>
#include <cstdio>
#include <curses.h>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <new>
#include <stdexcept>
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
    std::vector<Element*> vec;
    
public:

    Row(){};
    Row(std::initializer_list<Element*> el):vec(el){};
    ~Row() = default;


    void append(Element* el){
        vec.push_back(el);
    }
    void append(size_t relative_size){
        vec.push_back(new Element{Window(nullptr), relative_size});
    }
    void insert(Element* el, size_t position){
        auto it = vec.begin();
        std::advance(it, position);
        if(it >= vec.end()) throw std::out_of_range("Insert was out of range");
        vec.insert(it, el);
    }

    void insert(Element** el, size_t n, size_t position){
        auto it = vec.begin();
        std::advance(it, position);
        if(it >= vec.end()) throw std::out_of_range("Insert was out of range");
        vec.insert(it, el, el+n);
    }

    void resize(size_t idx, long pm){
        vec[idx]->size+=pm;
    }

    Element* operator[](size_t idx){
        return vec[idx];
    }



    int refresh(long height, size_t height_offset){
        uint x,y;
        getmaxyx(stdscr, y,x);
        if(height < 0){
            height = y+height;
        }
        

        //logvar(height)
        //logvar(height_offset)
        //logvar(y);
        



        
        
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
            size_t width = round((double)(x*(i->size))/percent);
            if(width+pos > x){width = x-pos;}

            i->window = newwin(height, width, height_offset, pos);
            char
                a='|'       ,
                b='|'       ,
                c=' '       ,
                d='-'       ,
                e='|'       ,
                f='|'       ,
                g='+'       ,
                h='+'       
            ;

            if(height_offset == 0){ // first
                c='-';
                e='+'; f='+';
            }
            if(width+pos < x){
                
            }
            //else if(height_offset+height >= y){ // last 
            //
            //}
            else{
            }
            wborder(i->window, a, b, c, d, e, f, g, h);
            wrefresh(i->window);
            pos += width;
        }
        return OK;
    }
};
struct RowElement{
    Row row;
    uint size;
    uint mod;
};

//aka just a bunch of rows
class WindowManager
{
private:
    std::vector<RowElement*> rows;
public:


    void append(RowElement* re){
        rows.push_back(re);
    }

    void refresh(){
        uint x,y;
        getmaxyx(stdscr, y,x);
        size_t pos = 0, percent = 0; 
        for (auto &&i : rows)
        {
            percent+=i->size;
        }
        for (auto &&i : rows)
        {
            size_t height = round((double)(y*i->size)/percent);
            if(height+pos > y){height = y-pos;}
            i->row.refresh(height, pos);
            pos+=height;
        }
        
    }

    void resize(size_t idx, long pm){
        rows[idx]->size+=pm;
    }

    RowElement* operator[](size_t idx){
        return rows[idx];
    }

    size_t size(){
        return rows.size();
    }
    
    WindowManager() {}
    WindowManager(std::initializer_list<RowElement*> ls): rows(ls) {}
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
    WindowManager wm;
    wm.append(new RowElement{Row(), 1});
    wm.append(new RowElement{Row(), 2});
    wm.append(new RowElement{Row(), 2});

    wm[0]->row.append(2);
    wm[0]->row.append(7);

    wm[1]->row.append(6);
    wm[1]->row.append(3);
    wm[1]->row.append(2);

    wm[2]->row.append(1);

    wm.refresh();

    getch();
    stop();
    return 0;
}
