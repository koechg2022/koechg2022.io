#include "prototypes"


/**********************misc_function definitions**********************************/

std::string misc_functions::get_current_time() {
    std::time_t current_time = std::time(nullptr);
    std::string the_answer = (const std::string) (std::ctime(&current_time));
    return the_answer.substr(0, the_answer.length() - 1);
}

template <typename data_> std::list<data_> misc_functions::set_to_list(std::set<data_> the_set) {
    std::list<data_> the_answer;
    for (typename std::set<data_>::const_iterator iter = the_set.begin(); iter != the_set.end(); iter++) {
        the_answer.push_back(*iter);
    }
    return the_answer;
}

template <typename data_> std::set<data_> misc_functions::list_to_set(std::list<data_> the_list) {
    std::set<data_> the_answer;
    for (typename std::list<data_>::const_iterator iter = the_list.begin(); iter != the_list.end(); iter++) {
        the_answer.push_back(*iter);
    }
    return the_answer;
}


/**********************string_function definitions********************************/


unsigned long string_functions::string_length(const char *the_string, const char term) {
    unsigned long the_answer;
    for (the_answer = 0; the_string[the_answer] != term; the_answer++);
    return the_answer;
}

char string_functions::to_caps(const char c) {
    return (string_functions::is_lower(c)) ? (c - ('a' - 'A')) : c;
}

char string_functions::to_lower(const char c) {
    return (string_functions::is_caps(c)) ? (c + ('a' - 'A')) : c;
}

bool string_functions::is_caps(const char c) {
    return (c >= 'A' && c <= 'Z');
}

bool string_functions::is_lower(const char c) {
    return (c >= 'a' && c <= 'z');
}

bool string_functions::is_number(const char c) {
    return (c >= '0' && c <= '9');
}

bool string_functions::same_char(const char a, const char b, bool ignore_case) {
    return (ignore_case) ? string_functions::to_caps(a) == string_functions::to_caps(b) : a == b;
}

bool string_functions::same_string(const char *first, const char *second, bool ignore_case, const char term) {
    unsigned long index;
    for (index = 0;
        !string_functions::same_char(first[index], term, ignore_case) &&
        !string_functions::same_char(second[index], term, ignore_case) &&
        string_functions::same_char(first[index], second[index], ignore_case);
        index = index + 1
    );
    return string_functions::same_char(first[index], term, ignore_case) && string_functions::same_char(second[index], term, ignore_case);
}


/**********************networking namespace definitions***************************/

const std::set<std::string> networking::network_families::get_families() {
    std::set<std::string> the_answer;
    the_answer.insert(unspec_address);
    the_answer.insert(ip_ver4_address);
    the_answer.insert(ip_ver6_address);
    the_answer.insert(unrecognized_address);
    #if defined(mac_os)
        the_answer.insert(link_layer_address);
    #elif defined(linux_os)
        the_answer.insert(netlink_address);
        the_answer.insert(packet_address);
    #else
        the_answer.insert(netbios_address);
        the_answer.insert(irda_address);
        the_answer.insert(bluetooth_address);
    #endif
    return the_answer;
}

// For the anonymous namespaces inside the networking namespace
namespace networking {
    namespace {

        bool init_namespace() {
            #if defined(crap_os)
                WSADATA d;
                if (WSAStartup(MAKEWORD(2, 2), &d)) {
                    std::fprintf(stderr, "Failed to initialize the network for this crappy operating system.Error %d\n", socket_error());
                    return false;
                }
                is_init = true;
            #endif
            return is_init;
        }

        bool uninit_namespace() {
            #if defined(crap_os)
                if (is_init) {
                    if (WSACleanup()) {
                        return false;
                    }
                    is_init = false;
                }
            #endif
            return !is_init;
        }

        std::string get_address_family_name(socket_type sock_fam) {
            /*
                For Windows:
                    AF_UNSPEC, AF_INET, AF_INET6, AF_NETBIOS, AF_IRDA, AF_BTH
                
                For Mac:
                    AF_UNSPEC, AF_UNIX, AF_INET, AF_INET6, AF_LINK

                For Linux:
                    AF_UNSPEC, AF_UNIX, AF_INET, AF_INET6, AF_NETLINK, AF_PACKET
            */

            std::string the_answer;

            switch (sock_fam) {

                case AF_UNSPEC : {
                    the_answer = network_families::unspec_address;
                    break;
                }

                case AF_INET : {
                    the_answer = network_families::ip_ver4_address;
                    break;
                }

                case AF_INET6 : {
                    the_answer = network_families::ip_ver6_address;
                    break;
                }
                
                #if defined(unix_os)
                        #if defined(mac_os)
                            case AF_LINK : {
                                the_answer = network_families::link_layer_address;
                                break;
                            }
                        #else
                            
                            case AF_NETLINK : {
                                the_answer = network_families::netlink_address;
                                break;
                            }

                            case AF_PACKET : {
                                the_answer = network_families::packet_address;
                                break;
                            }

                        #endif
                #else
                        case AF_NETBIOS : {
                            the_answer = network_families::netbios_address;
                            break;
                        }

                        case AF_IRDA : {
                            the_answer = network_families::irda_address;
                            break;
                        }

                        case AF_BTH : {
                            the_answer = network_families::bluetooth_address;
                            break;
                        }

                #endif

                default : {
                    the_answer = network_families::unrecognized_address;
                    break;
                }

            }

            return the_answer;
        }

    }
}


std::list<std::string> networking::resolve_hostname(const std::string host, const std::string port) {
    std::list<std::string> the_answer;
    bool was_init = is_init;
    
    if (!is_init) {
        if (!init_namespace()) {
            return the_answer;
        }
    }

    struct addrinfo hints, *all_addresses, *current_address;
    char buffer[buffer_size];
    std::memset(buffer, 0, buffer_size);
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_ALL;
    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &all_addresses)) {
        std::fprintf(stderr, "Failed to retrieve address information for \"%s\" on port %s. Error %d\n", host.c_str(), port.c_str(), socket_error());
        if (clean_on_failure) {
            uninit_namespace();
        }
        return the_answer;
    }

    for (current_address = all_addresses; current_address; current_address = current_address->ai_next) {
        if (!getnameinfo(current_address->ai_addr, current_address->ai_addrlen, buffer, buffer_size, 0, 0, NI_NUMERICHOST | AI_ALL)) {
            the_answer.push_back(std::string(buffer));
        }

        if (!getnameinfo(current_address->ai_addr, current_address->ai_addrlen, buffer, buffer_size, 0, 0, NI_NAMEREQD | AI_ALL)) {
            the_answer.push_back(std::string(buffer));
        }
    }

    freeaddrinfo(all_addresses);

    if (!was_init) {
        uninit_namespace();
    }

    return the_answer;
}

std::map<std::string, std::map<std::string, std::list<std::string> > > networking::get_machine_adapters(bool ignore_empty) {
    std::map<std::string, std::map<std::string, std::list<std::string> > > the_answer;
    bool was_init = is_init;
    if (!was_init) {
        if (!networking::init_namespace()) {
            return the_answer;
        }
    }

    // Now to get the adapters for this machine
    ifaddr_adapter_type this_machine_adapters;
    
    #if defined(crap_os)
        this_machine_adapters = NULL;
        DWORD memory_size = 20000;
        while (!this_machine_adapters) {
            this_machine_adapters = (ifaddr_adapter_type) malloc(memory_size);

            if (!this_machine_adapters) {
                if (clean_on_failure) {
                    uninit_namespace();
                }
                return the_answer;
            }

            int response = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, this_machine_adapters, &memory_size);
            
            if (response == ERROR_BUFFER_OVERFLOW) {
                std::free(this_machine_adapters);
            }

            else if (response == ERROR_SUCCESS) {
                break;
            }

            else {
                ifaddrs_free_adapters(this_machine_adapters);

                if (clean_on_failure) {
                    uninit_namespace();
                }
                return the_answer;
            }

        }
    #else
        if (getifaddrs(&this_machine_adapters)) {
            std::fprintf(stderr, "Error retrieving this machine's adapter information. Error %d\n", socket_error());
            if (clean_on_failure) {
                uninit_namespace();
            }
            return the_answer;
        }
    #endif

    ifaddr_adapter_type this_adapter;
    ifaddrs_address_type this_address;
    
    std::string adapter_name, address_family, address_ip;
    socket_family_type this_family;
    char buffer[buffer_size];
    for (this_adapter = this_machine_adapters; this_adapter; this_adapter = ifaddrs_get_next_adapter(this_adapter)) {
        adapter_name = ifaddrs_get_adapter_name(this_adapter);
        for (this_address = ifaddrs_get_address_from_adapter(this_adapter); this_address; this_address = ifaddrs_get_next_address(this-address)) {
            this_family = ifaddrs_address_family(this_address);
            address_family = get_address_family_name(this_family);
            // std::memset(buffer, 0, buffer_size);
            switch (getnameinfo(ifaddrs_address_sockaddr(this_address), ifaddrs_address_sockaddrlen(this_address), buffer, buffer_size, 0, 0, NI_NUMERICHOST)) {
                
                // Success getting the IP address
                case 0 : {
                    address_ip = std::string(buffer);

                    if (ignore_empty && address_ip.empty()) {
                        continue;
                    }
                    else {

                        // new adapter.
                        if (the_answer.find(adapter_name) == the_answer.end()) {
                            std::map<std::string, std::list<std::string> > new_map;
                            std::list<std::string> new_list;
                            new_list.push_back(address_ip);
                            new_map.insert(std::make_pair(address_family, new_list));
                            the_answer.insert(std::make_pair(adapter_name, new_map));
                        }

                        // adapter exists, new address family though
                        else if (the_answer[adapter_name].find(address_family) == the_answer[adapter_name].end()) {
                            std::list<std::string> new_list;
                            new_list.push_back(address_ip);
                            the_answer[adapter_name].insert(std::make_pair(address_family, new_list));
                        }

                        // adapter and address family exist. Need to add new address.
                        else {
                            the_answer[adapter_name][address_family].push_back(address_ip);
                        }
                    }
                    
                    break;
                }
            }

            switch(getnameinfo(ifaddrs_address_sockaddr(this_address), ifaddrs_address_sockaddrlen(this_address), buffer, buffer_size, 0, 0, NI_NAMEREQD)) {

                // Success getting the name address
                case 0 : {
                    address_ip = std::string(buffer);

                    if (ignore_empty && address_ip.empty()) {
                        continue;
                    }

                    else {
                        // new adapter.
                        if (the_answer.find(adapter_name) == the_answer.end()) {
                            std::map<std::string, std::list<std::string> > new_map;
                            std::list<std::string> new_list;
                            new_list.push_back(address_ip);
                            new_map.insert(std::make_pair(address_family, new_list));
                            the_answer.insert(std::make_pair(adapter_name, new_map));
                        }

                        // adapter exists, new address family though
                        else if (the_answer[adapter_name].find(address_family) == the_answer[adapter_name].end()) {
                            std::list<std::string> new_list;
                            new_list.push_back(address_ip);
                            the_answer[adapter_name].insert(std::make_pair(address_family, new_list));
                        }

                        // adapter and address family exist. Need to add new address.
                        else {
                            the_answer[adapter_name][address_family].push_back(address_ip);
                        }
                    }
                    break;
                }
            }
            
        }
    }

    ifaddrs_free_adapters(this_machine_adapters);

    if (!was_init) {
        uninit_namespace();
    }

    return the_answer;
}

          /************Host class definitions*****************/


bool networking::Host::socket_is_connected(socket_type this_socket) {
    int err;
    socklen_t err_size = sizeof(err);

    if (getsockopt(this_socket, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&err), &err_size)) {
        std::fprintf(stderr, "An error has occured while running getsockopt. Error number %d\n", socket_error());
        return false;
    }

    return err == 0;
}


networking::Host::Host(const std::string host, const std::string connect_port, socket_type connect_socket, long seconds_wait, int msec_wait, bool using_tcp) {
    this->was_init = is_init;
    
    if (!this->was_init) {
        networking::init_namespace();
    }

    if (host.empty()) {
        std::map<std::string, std::map<std::string, std::list<std::string> > > adapters = networking::get_machine_adapters();
        for (auto this_adapter = adapters.begin(); this_adapter != adapters.end(); this_adapter++) {
            if (!string_functions::same_string(this_adapter->first.c_str(), networking::rel_adapter.c_str())) {
                continue;
            }
        }
    }
    else {
        this->hostname = host;
    }

}