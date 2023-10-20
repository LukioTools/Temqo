#if !defined(USEFULL_MACROS)
#define USEFULL_MACROS

#include <iostream>
#include <fstream>
std::ofstream different_terminal("/dev/pts/4");
#define LOG_STREAM different_terminal
#if !defined(LOG_STREAM) 
#define LOG_STREAM std::cout
#endif

#if !defined(ERR_STREAM) 
#define ERR_STREAM std::cerr
#endif

//user can define the default error type if he choses so
#if !defined(check_err_type)
#define check_err_type std::runtime_error
#endif // check_err_type

//simple error logging
#define err(variable) \
    std::cerr << variable << std::endl;


#define check_cb(boolean, errormsg, cleanup, errtype) \
if((boolean)){err(#boolean); cleanup; throw errtype(errormsg);};

//check roots
#define __check(boolean, errormsg, errtype, invert_bool) \
if((boolean) != invert_bool){\
    err(#boolean); \
    throw errtype(errormsg);\
};


//positive macros
#define check(boolean, errormsg) \
    __check(boolean, errormsg, check_err_type, false)
#define check_err(boolean, errormsg, errtype) \
    __check(boolean, errormsg, errtype, false)
//inverted macros
#define check_not(boolean, errormsg) \
    __check(boolean, errormsg, check_err_type, true)
#define check_err_not(boolean, errormsg, errtype) \
    __check(boolean, errormsg, errtype, true)

//simple logging
#define logv(variable) \
    LOG_STREAM << variable << std::endl;
//simple variable name and value logging
#define logvar(variable) \
    LOG_STREAM << #variable": " << variable << std::endl;

//ansi shenanignas
//dont use this
#define print_color(r,g,b) "\033[38;2;" << r << ';' << g <<';' << b << 'm'
// Define a macro for setting the background color with RGB variables
#define print_bcolor(r, g, b) ("\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m ").c_str()
#define print_bcolorstr(r, g, b) ("\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m ")

#define rgb_bg(rgb) ("\033[48;2;" + std::to_string(rgb.r) + ";" + std::to_string(rgb.g) + ";" + std::to_string(rgb.b) + "m ").c_str()
#define rgb_bgstr(rgb) ("\033[48;2;" + std::to_string(rgb.r) + ";" + std::to_string(rgb.g) + ";" + std::to_string(rgb.b) + "m ")


// Define a macro to reset the background color
#define print_bcolor_reset "\033[0m"
#define bg_reset "\033[0m"




#endif // USEFULL_MACROS
