#include "prototypes"
#include <sys/socket.h>


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

std::map<std::string, std::map<std::string, std::list<std::string> > > get_machine_adapters(bool ignore_empty) {
    std::map<std::string, std::map<std::string, std::list<std::string> > > the_answer;
    
    return the_answer;
}