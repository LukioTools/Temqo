#if !defined(USEFULL_MACROS)
#define USEFULL_MACROS

#include <iostream>

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



#endif // USEFULL_MACROS
