

#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)

    // crap
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x600
    #endif

    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <iphlpapi.h>
    #pragma comment(lib, "ws2_32.lib")
    #pragma comment(lib, "iphlpapi.lib")

    #define crap_os
    #define valid_socket(sock_no) (sock_no != INVALID_SOCKET)
    #define close_socket(sock_no) (closesocket(sock_no))
    #define get_socket_errno() (WSAGetLastError())

    typedef SOCKET sock;

#else

    // unix
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <errno.h>

    #include <ifaddrs.h> // for listing adapters

    #define unix_os
    #define valid_socket(sock_no) (sock_no >= 0)
    #define close_socket(sock_no) (close(sock_no))
    #define get_socket_errno() (errno)

#endif


#include <iostream>
#include <map>
#include <vector>