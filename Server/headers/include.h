

#include "../../libraries/network_headers.h"

#define buffer_size 100


namespace Network_Management {

    namespace Network_Exceptions {

        class NetworkInitialization : std::exception {
            private:
                std::string msg;
            public:
                NetworkInitialization(std::string msg = "Failed to Initialize Network") : std::exception(){
                    this->msg = msg;
                }

                ~NetworkInitialization() throw() {
                    return;
                }

                char* what() {
                    return (char*) this->msg.c_str();
                }

        };

        class MemoryManagement : std::exception {

            private:
                std::string msg;
            
            public:

                MemoryManagement(const std::string msg = "Memory failure") : std::exception() {
                    this->msg = msg;
                }

                ~MemoryManagement() throw() {
                    return;
                }

                char* what() {
                    return (char*) this->msg.c_str();
                }

        };

        class IllegalAdapter : std::exception {
            private:
                std::string msg;
            
            public:
                IllegalAdapter(std::string msg = "Illegal Adapter used") : std::exception() {
                    this->msg = msg;
                }

                ~IllegalAdapter() throw() {
                    return;
                }

                char* what() {
                    return (char*) this->msg.c_str();
                }

        };

    }


    namespace {

        const std::string ip_4 = "IP Version 4";
        const std::string ip_6 = "IP Version 6";

        bool network_initialized = false;

        bool is_caps(char c) {
            return (c >= 'A' && c <= 'Z');
        }

        bool is_lower(char c) {
            return (c >= 'a' && c <= 'z');
        }

        bool is_letter(char c) {
            return (is_caps(c) || is_lower(c));
        }

        bool is_number(char c) {
            return (c >= '0' && c <= '9');
        }

        char to_caps(char c) {
            return (is_lower(c)) ? c - ('a' - 'A') : c;
        }

        char to_lower(char c) {
            return (is_caps(c)) ? c + ('a' - 'A') : c;
        }

        bool same_char(char a, char b, bool ignore_case = true) {
            return (ignore_case) ? to_caps(a) == to_caps(b) : a == b;
        }

        bool same_string(char* first, char* second, bool ignore_case = true) {
            unsigned long index;
            for (index = 0; first[index] != '\0' && second[index] != '\0'; index = index + 1) {
                if (!same_char(first[index], second[index], ignore_case)) {
                    return false;
                }
            }
            return (first[index] == '\0' && second[index] == '\0');
        }

        bool same_string(std::string first, std::string second, bool ignore_case = true) {
            unsigned long index;
            for (index = 0; index < first.length() && index < second.length(); index = index + 1) {
                if (!same_char(first[index], second[index], ignore_case)) {
                    return false;
                }
            }
            return (index == first.length() && index == second.length());
        }

        void fill_buffer_with(char to_fill, char* buffer, unsigned long length) {
            unsigned long index;
            for (index= 0; index < length; index = index + 1) {
                buffer[index] = to_fill;
            }
        }

        void initialize_network() {
            #if defined(crap_os)
                if (!network_initialized) {
                    
                    WSADATA d;
                    if (WSAStartup(MAKEWORD(2, 2), &d)) {
                        std::fprintf(stderr, "Failed to initialize startup for Crap Operating System. Maybe you should consider a better OS\n");
                        std::exit(EXIT_FAILURE);
                    }
                    network_initialized = true;

                }
            #endif
        }

        void clean_up_network() {
            #if defined(crap_os)
                if (network_initialized) {
                    WSACleanup();
                }
                network_initialized = false;
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
                PIP_ADAPTER_ADDRESSES adapters;

                do {
                    adapters = (PIP_ADAPTER_ADDRESSES) malloc(size);

                    if (!adapters) {
                        std::printf("Couldn't allocate %ld bytes for adapters.\n", size);
                        clean_up_network();
                        std::exit(EXIT_FAILURE);
                    }

                    int r = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, adapters, &size);
                    if (r == ERROR_BUFFER_OVERFLOW) {
                        std::printf("GetAdaptersAddresses wants %ld bytes.\n", size);
                        free(adapters);
                    }

                    else if (r == ERROR_SUCCESS) {
                        break;
                    }

                    else {
                        std::printf("Error from GetAdaptersAddresses: %d\n", r);
                        free(adapters);
                        clean_up_network();
                        std::exit(EXIT_FAILURE);
                    }

                } while (!adapters);

                PIP_ADAPTER_ADDRESSES this_adapter = adapters;

                while (this_adapter) {
                    adapter_name = std::string((char*) (this_adapter->FriendlyName));

                    PIP_ADAPTER_UNICAST_ADDRESS this_address = this_adapter->FirstUnicastAddress;
                    while (this_address) {
                        ip_address_version = (this_address->Address.lpSockaddr->sa_family == AF_INET) ? ip_4 : ip_6;
                        std::memset(ap_buffer, 0, buffer_size);
                        getnameinfo(this_address->Address.lpSockaddr, this_address->Address.iSockaddrLength, ap_buffer, buffer_size, 0, 0, NI_NUMERICHOST);
                        ip_address = std::string(ap_buffer);

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
                        this_address = this_address->Next;
                    }
                    this_adapter = this_adapter->Next;
                }

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

        class Host_Machine {

            private:

                std::string remote_address, remote_port;
                std::map<std::string, std::vector<std::string> > connected_machine_ips;

                const std::map<std::string, std::map<std::string, std::vector<std::string> > > this_machine_adapters = get_network_adapters();

                void initialize_for_crap_os() {
                    #if defined(crap_os)
                        if (!network_initialized) {
                            
                            WSADATA this_data;
                            if (WSAStartup(MAKEWORD(2, 2), &this_data)) {
                                throw Network_Exceptions::NetworkInitialization("Failed to initialize startup for Crap Operating System. Maybe you should consider a better OS\n");
                            }

                        }
                    #endif
                }

                void clean_up_for_crap_os() {
                    #if defined(crap_os)
                        if (network_initialized) {
                            WSACleanup();
                        }
                    #endif
                }

            public:

                Host_Machine(std::string remote_address = "localhost", std::string port_no = "80") {
                    this->remote_address = remote_address;
                    this->remote_port = port_no;
                    this->connected_machine_ips = get_url_ip(this->remote_address, this->remote_port);
                }

                std::map<std::string, std::vector<std::string> > get_adapter_ip(std::string adapter_name, bool exact_case = false) {
                    std::map<std::string, std::vector<std::string> > the_answer;
                    for (std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator 
                        adapter_pair = this->this_machine_adapters.begin();
                        adapter_pair != this->this_machine_adapters.end();
                        adapter_pair++
                    ){
                        if (same_string(adapter_name, adapter_pair->first, !exact_case)) {
                            the_answer = adapter_pair->second;
                            break;
                        }
                    }
                    return the_answer;
                }

        };

    }

}