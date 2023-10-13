#if !defined(USEFULL_NC_MACROS)
#define USEFULL_NC_MACROS

#if !defined(LOG_WIN)
//#error LOG_WIN needs to be defined before including nc_macros 
#endif // LOG_WIN

// -lncurses
#include <curses.h>
#include <ncurses.h>

#define nc_logl(format,...) \
    wclear(LOG_WIN);\
    wprintw(LOG_WIN, format, __VA_ARGS__); \
    mvwhline(LOG_WIN, 1, 0, ACS_HLINE, width); \
    wrefresh(LOG_WIN);

#define nc_log(format,...) \
    wclear(LOG_WIN);\
    wprintw(LOG_WIN, format, __VA_ARGS__); \
    wrefresh(LOG_WIN);

#define get_max(x, y ) int x, y; getmaxyx(stdscr, y, x)

#define wisset(window) (window != nullptr && window != stdscr);

#endif // USEFULL_NC_MACROS
