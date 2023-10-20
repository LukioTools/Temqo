#if !defined(USEFULL_NC_MACROS)
#define USEFULL_NC_MACROS

#if !defined(LOG_ELEMENT)
//#error LOG_WIN needs to be defined before including nc_macros 
#endif // LOG_WIN

// -lncurses
#include <curses.h>
#include <ncurses.h>

#define nc_logl(format,...) \
    if(LOG_ELEMENT){\
    wclear(LOG_ELEMENT);\
    wprintw(LOG_ELEMENT->window, format, __VA_ARGS__); \
    mvwhline(LOG_ELEMENT->window, 1, 0, ACS_HLINE, width); \
    wrefresh(LOG_ELEMENT->window);}

#define nc_log(format,...) \
    if(LOG_ELEMENT){\
    wclear(LOG_ELEMENT->window);\
    wprintw(LOG_ELEMENT->window, format, __VA_ARGS__); \
    wrefresh(LOG_ELEMENT->window);}

#define disp(window, format, ...) \
    if(window){\
    wclear(window); \
    mvwprintw(window, 0, 0, format, __VA_ARGS__); \
    wrefresh(window);}

#define get_max(x, y ) int x, y; getmaxyx(stdscr, y, x)

#define wisset(window) (window != nullptr && window != stdscr);

#endif // USEFULL_NC_MACROS
