

#include <wx/wx.h>


 #if defined(_WIN16) or defined(_WIN32) or defined(_WIN64) or defined(__WIN32__) or defined(__TOS_WIN__) or defined(__WINDOWS__)

    #if not defined(crap_os)
        #define crap_os
    #endif

    #if not defined(_INC_CONIO)
        #include <conio.h>
    #endif

#else

    #if not defined(unix_os)
        #define unix_os
    #endif

    #if defined(__APPLE__) or defined(__APPLE_CC__)
        #define mac_os
    #else
        #define linux_os
    #endif

    #if (defined(mac_os) and not defined(_UNISTD_H_)) or (defined(linux_os) and not defined(_UNISTD_H))
        #include <unistd.h>
    #endif

#endif



#if (defined(mac_os) and not defined(_LIBCPP_CTIME)) or (defined(linux_os) and not defined(_GLIBCXX_CTIME)) or (defined(crap_os) and not defined(_CTIME_))
    #include <ctime>
#endif

#if (defined(mac_os) and not defined(_LIBC_CSTRING)) or (defined(linux_os) and not defined(_GLIBCXX_STRING)) or (defined(crap_os) and not defined(_CSTRING_))
    #include <cstring>
#endif

#if (defined(mac_os) and not defined(_LIBCPP_STRING)) or (defined(linux_os) and not defined(_GLIBCXX_STRING)) or (defined(crap_os) and not defined(_STRING_))
    #include <string>
#endif

#if (defined(mac_os) and not defined(_LIBCPP_VECTOR)) or (defined(linux_os) and not defined(_GLIBCXX_VECTOR)) or (defined(crap_os) and not defined(_VECTOR_))
    #include <vector>
#endif

#if (defined(mac_os) and not defined(_LIBCPP_MAP)) or (defined(linux_os) and not defined(_GLIBCXX_MAP)) or (defined(crap_os) and not defined(_MAP_))
    #include <map>
#endif

#if (defined(mac_os) and not defined(_LIBCPP_SET)) or (defined(linux_os) and not defined(_GLIBCXX_SET)) or (defined(crap_os) and not defined(_SET_))
    #include <set>
#endif

#if (defined(mac_os) and not defined(_SYS_IOCTL_H_)) or (defined(linux_os) and not defined(_SYS_IOCTL_H))
    #include <sys/ioctl.h>
#endif



#define is ==
#define less <
#define greater >
#define IN_RANGE_INCLUDE(a, b, c) (a <= b and b <= c)
#define CASE_DIFF 'a' - 'A'
#define prev -1
#define NOT !=





