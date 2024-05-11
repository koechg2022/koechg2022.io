
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)

    // crap
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x600
    #endif

    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")

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
// #include "../../libraries/structures.h"

#define buffer_size 100


namespace Network_Management {

    namespace {

        const std::string ip_4 = "IP Version 4";
        const std::string ip_6 = "IP Version 6";

        void fill_buffer_with(char to_fill, char* buffer, unsigned long length) {
            unsigned long index;
            for (index= 0; index < length; index = index + 1) {
                buffer[index] = to_fill;
            }
        }

        void initialize_network() {
            #if defined(crap_os)
                WSADATA d;

                if (WSAStartup(MAKEWORD(2, 2), &d)) {
                    std::fprintf(stderr, "Failed to initialize startup for Crap Operating System. Maybe you should consider a better OS\n");
                    std::exit(EXIT_FAILURE);
                }
            #endif
        }

        void clean_up_network() {
            #if defined(crap_os)
                WSACleanup();
            #endif
        }

    }

    namespace Host {

        const std::map<std::string, std::map<std::string, std::vector<std::string> > > get_network_adapters() {
            std::string adapter_name, ip_address_version, ip_address;
            std::vector<std::string> ip_addresses;
            std::map<std::string, std::map<std::string, std::vector<std::string> > > the_answer;
            char ap_buffer[buffer_size];
            
            #if defined(crap_os)
                
                initialize_network();

                DWORD size = 20000;
                PIP_ADAPTER_ADDRESSES adapter, adapters;
                PIP_ADAPTER_UNICAST_ADDRESS this_address;

                do {
                    adapters = (PIP_ADAPTER_ADDRESSES) malloc(size);

                    if (!adapters) {
                        std::fprintf(stderr, "Couldn't allocate %d bytes for adapters.\n");
                        std::exit(EXIT_FAILURE);
                    }

                    int r = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, adapters, &size);

                    if (r == ERROR_BUFFER_OVERFLOW) {
                        std::printf("GetAdaptersAddresses wants %ld bytes.\n");
                        free(adapters);
                    }

                    else if (r == ERROR_SUCCESS) {
                        break;
                    }

                    else {
                        std::fprintf(stderr, "Error retrieving adapter information for Crap Operating System. Maybe you should get a better OS\n");
                        free(adapters);
                        // WSACleanup();
                        clean_up_network();
                        std::exit(EXIT_FAILURE);
                    }

                } while (!adapters);

                
                while (adapter) {
                    
                    adapter_name = std::string(adapter->FriendlyName);
                    this_address = adapter->FirstUnicastAddress;
                    adapter_name = std::string(this_address->Address.lpSockaddr->sa_family == AF_INET ? "IP Version 4" : "IP version 6");
                    std::memset(ap_buffer, 0, buffer_size);
                    getnameinfo(address->Address.lpSockaddr, iSOckaddrLength, ap_buffer, buffer_size, 0, 0, NI_NUMERICHOST);
                    ip_address = std::string(ap_buffer);
                    // add adapter_name if it's not present.
                    if (the_answer.find(adapter_name) == the_answer.end()) {
                        
                        // create the internal map with ip_address mapped to a vector, if not present
                        std::map<std::string, std::vector<std::string> > internal_map;
                        std::vector<std::string> new_list;
                        new_list.push_back(ip_address);
                        internal_map.insert(std::make_pair(ip_address_version, new_list));
                        the_answer.insert(std::make_pair(adapter_name, internal_map));

                    }

                    else {
                        the_answer[adapter_name][ip_address_version].push_back(ip_address);
                    }
                    adapter = adapter->Next;
                }

                // WSACleanup();
                clean_up_network();

            #else
                struct ifaddrs* addresses, *this_address;
                
                if (getifaddrs(&addresses)) {
                    std::fprintf(stderr, "Failed retrieving adapter information. Error %d\n", get_socket_errno());
                    std::exit(EXIT_FAILURE);
                }

                this_address = addresses;
                
                while (this_address) {
                    if (this_address->ifa_addr->sa_family == AF_INET || this_address->ifa_addr->sa_family == AF_INET6) {
                        adapter_name = std::string(this_address->ifa_name);
                        ip_address_version = std::string((this_address->ifa_addr->sa_family == AF_INET) ? "IP Version 4" : "IP Version 6");
                        // fill_buffer_with(0, ap_buffer, buffer_size);
                        std::memset(ap_buffer, 0, buffer_size);
                        getnameinfo(this_address->ifa_addr, this_address->ifa_addr->sa_family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6), ap_buffer, buffer_size, 0, 0, NI_NUMERICHOST);
                        ip_address = std::string(ap_buffer);
                        // add adapter_name if it's not present.
                        if (the_answer.find(adapter_name) == the_answer.end()) {
                            
                            // create the internal map with ip_address mapped to a vector, if not present
                            std::map<std::string, std::vector<std::string> > internal_map;
                            std::vector<std::string> new_list;
                            new_list.push_back(ip_address);
                            internal_map.insert(std::make_pair(ip_address_version, new_list));
                            the_answer.insert(std::make_pair(adapter_name, internal_map));

                        }

                        else {
                            the_answer[adapter_name][ip_address_version].push_back(ip_address);
                        }
                    }
                    this_address = this_address->ifa_next;
                }

            #endif
            
            return the_answer;
        }


        namespace HTTP {

            const std::map<std::string, std::vector<std::string> > get_url_ip(const std::string url, std::string port = "80") {
                initialize_network();
                std::map<std::string, std::vector<std::string> > the_answer;
                int status;
                struct addrinfo hints, *server_info, *this_address;
                std::string ip_version;
                char ip_buffer[buffer_size];
                std::memset(&hints, 0, sizeof(hints));
                hints.ai_family = AF_UNSPEC;
                hints.ai_socktype = SOCK_STREAM;

                if ((status = getaddrinfo(url.c_str(), port.c_str(), &hints, &server_info)) != 0) {
                    std::fprintf(stderr, "Failed to retrieve information for URL \"%s\"\nError Message \"%s\"\n", url.c_str(), gai_strerror(status));
                    std::exit(EXIT_FAILURE);
                }

                std::vector<std::string> v4_addresses, v6_addresses;
                the_answer.insert(std::make_pair((std::string) ip_4, v4_addresses));
                the_answer.insert(std::make_pair((std::string) ip_6, v6_addresses));

                for (this_address = server_info; this_address != NULL; this_address = this_address->ai_next) {

                    void* address_info;

                    if (this_address->ai_family == AF_INET) {
                        struct sockaddr_in* ip_version_four = (struct sockaddr_in*) this_address->ai_addr;
                        address_info = &(ip_version_four->sin_addr);
                        ip_version = ip_4;
                    }

                    else {
                        struct sockaddr_in6* ip_version_six = (struct sockaddr_in6*) this_address->ai_addr;
                        address_info = &(ip_version_six->sin6_addr);
                        ip_version = ip_6;
                    }

                    inet_ntop(this_address->ai_family, address_info, ip_buffer, buffer_size);
                    the_answer[ip_version].push_back(std::string(ip_buffer));

                }

                freeaddrinfo(server_info);
                clean_up_network();
                return the_answer;
            }

        }


    }

}