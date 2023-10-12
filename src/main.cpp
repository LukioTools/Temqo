
#include <iostream>
#include <glm/ext.hpp>

#include <curses.h>
#include <ncurses.h>

#include "macros.hpp"

int startup(){
    initscr();
    return 0;
}

int stop(){
    endwin();
    return 0;
}


int main(int argc, char const *argv[])
{
    startup();

    log("Hello World\n");
    

    stop();
    return 0;
}
