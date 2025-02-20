



#include "networking.h++"






namespace networking {
    
    
    namespace network_address_families {

        const std::set<std::string> get_address_families() {
            std::set<std::string> the_answer;
            the_answer.insert(unspec_address_family);
            the_answer.insert(unrecognized_address_family);
            the_answer.insert(ip_version4_address_family);
            the_answer.insert(ip_version6_address_family);
            #if defined(unix_os)
                #if defined(mac_os)
                    the_answer.insert(link_layer_address_family);
                #else
                    the_answer.insert(netlink_address_family);
                    the_answer.insert(packet_address_family);
                #endif
            #else
                the_answer.insert(netbios_address_family);
                the_answer.insert(irda_address_family);
                the_answer.insert(bluetooth_address_family);
            #endif
            return the_answer;
        }

        const std::string resolve_address_family_to_string(socket_family_type the_family) {
            std::string the_answer = unrecognized_address_family;
            switch(the_family) {

                case (AF_UNSPEC) : {
                    the_answer = network_address_families::unspec_address_family;
                    break;
                }

                case (AF_INET) : {
                    the_answer = network_address_families::ip_version4_address_family;
                    break;
                }

                case (AF_INET6) : {
                    the_answer = network_address_families::ip_version6_address_family;
                    break;
                }

                #if defined(unix_os)
                    
                    #if defined(mac_os)

                        case (AF_LINK) : {
                            the_answer = network_address_families::link_layer_address_family;
                            break;
                        }
                    
                    #else

                        case (AF_NETLINK) : {
                            the_answer = network_address_families::netlink_address_family;
                            break;
                        }

                        case (AF_PACKET) : {
                            the_answer = network_address_families::packet_address_family;
                            break;
                        }

                    #endif

                #else

                    case (AF_NETBIOS) : {
                        the_answer = network_address_families::netbios_address_family;
                        break;
                    }

                    case (AF_IRDA) : {
                        the_answer = network_address_families::irda_address_family;
                        break;
                    }

                    case (AF_BTH) : {
                        the_answer = network_address_families::bluetooth_address_family;
                        break;
                    }
                    
                #endif

                default : {
                    the_answer = network_address_families::unrecognized_address_family;
                    break;
                }
            }
            return the_answer;
        }

    }


    namespace exceptions {

        base_exception::base_exception(const std::string name, const std::string msg, bool print, const std::string file_name, const int except_line, const std::string function) {
            this->except_name = name;
            this->message = msg;
            this->file = file_name;
            this->line = except_line;
            this->func = function;
            if (print) {
                std::fprintf(stderr, "\"%s\" Thrown:\n%s:%s:line, %d:\n%s\n", this->except_name.c_str(), this->file.c_str(), this->func.c_str(), this->line, this->message.c_str());
            }
        }

        base_exception::~base_exception() throw() {}

        const std::string base_exception::msg() const {
            return "\"" + this->except_name + "\" Thrown:\n" + this->file + ":" + this->func + "line, " + std::to_string(this->line) + "\n" + this->message.c_str() + "\n";
        }
        

        unexpected_exception::unexpected_exception(const std::string msg, bool print, const std::string file_name, const int except_line, const std::string function) : 
            base_exception("unexpected_exception", msg, print, file_name, except_line, function) {}

        memory_exception::memory_exception(const std::string msg, bool print, const std::string file_name, const int except_line, const std::string function) : 
            base_exception("memory_exception", msg, print, file_name, except_line, function) {}

        initialize_network_failure::initialize_network_failure(const std::string msg, bool print, const std::string file_name, const int except_line, const std::string function) : 
            base_exception("initialize_network_failure", msg, print, file_name, except_line, function) {}

        getaddrinfo_failure::getaddrinfo_failure(const std::string msg, bool print, const std::string file_name, const int except_line, const std::string function) : 
            base_exception("getaddrinfo_failure", msg, print, file_name, except_line, function) {}

        getifaddrs_failure::getifaddrs_failure(const std::string msg, bool print, const std::string file_name, const int except_line, const std::string function) : 
            base_exception("getifaddrs_failure", msg, print, file_name, except_line, function) {}

        create_socket_failure::create_socket_failure(const std::string msg, bool print, const std::string file_name, const int except_line, const std::string function) : 
            base_exception("create_socket_failure", msg, print, file_name, except_line, function) {}

        bind_socket_failure::bind_socket_failure(const std::string msg, bool print, const std::string file_name, const int except_line, const std::string function) : 
            base_exception("bind_socket_failure", msg, print, file_name, except_line, function) {}

        listen_socket_failure::listen_socket_failure(const std::string msg, bool print, const std::string file_name, const int except_line, const std::string function) : 
            base_exception("listen_socket_failure", msg, print, file_name, except_line, function) {}

        select_failure::select_failure(const std::string msg, bool print, const std::string file_name, const int except_line, const std::string function) : 
            base_exception("select_failure", msg, print, file_name, except_line, function) {}

        getnameinfo_failure::getnameinfo_failure(const std::string msg, bool print, const std::string file_name, const int except_line, const std::string function) : 
            base_exception("getnameinfo_failure", msg, print, file_name, except_line, function) {}

        connect_failure::connect_failure(const std::string msg, bool print, const std::string file_name, const int except_line, const std::string function) : 
            base_exception("connect_failure", msg, print, file_name, except_line, function) {}

    }


    bool initialize_network() {
        #if defined(crap_os)
            if (not is_init) {
                WSADATA d;
                if (WSAStartup(MAKEWORD(2, 2), &d)) {
                    return false;
                }
                is_init = true;
            }
        #endif
        return is_init;
    }


    bool uninitialize_network() {
        #if defined(crap_os)
            if (!is_init) {
                if (WSACleanup()) {
                    return is_init;
                }
                is_init = false;
            }
        #endif
        return !is_init;
    }


    std::vector<std::string> resolve_hostname(const std::string hostname, const std::string port) {
        
        bool was_init = is_init;

        if (not was_init) {
            if (not initialize_network()) {
                (clean_on_except) ? uninitialize_network() : true;
                throw exceptions::initialize_network_failure("Failed to initialize. ", true, __FILE__, __LINE__ - 2, "resolve_host_name");
            }
        }
        // To get here, the network was successfully initialized.
        std::vector<std::string> the_answer;
        struct addrinfo hints, *current, *all_addresses;
        char buffer[buffer_size];
        std::memset(&hints, 0, sizeof(hints));
        std::memset(&buffer, 0, buffer_size);
        hints.ai_flags = AI_ALL;

        if (getaddrinfo(hostname.c_str(), port.c_str(), &hints, &all_addresses)) {
            (clean_on_except and not was_init) ? uninitialize_network() : true;
            throw exceptions::getaddrinfo_failure("Failed to get address information for host " + hostname, true, __FILE__, __LINE__ - 2, "resolve_hostname");
        }

        for (current = all_addresses; current NOT null; current = current->ai_next) {

            if (not getnameinfo(current->ai_addr, current->ai_addrlen, buffer, buffer_size, 0, 0, NI_NUMERICHOST | AI_ALL)) {
                the_answer.push_back(std::string(buffer));
            }

            if (not getnameinfo(current->ai_addr, current->ai_addrlen, buffer, buffer_size, 0, 0, NI_NAMEREQD | AI_ALL)) {
                the_answer.push_back(std::string(buffer));
            }
        }

        freeaddrinfo(all_addresses);

        if (not was_init) {
            if (not uninitialize_network()) {
                throw exceptions::initialize_network_failure("Failed to uninitialize network", true, __FILE__, __LINE__ - 1, "resolve_hostname");
            }
        }
        return the_answer;
    }


    std::map<std::string, std::map<std::string, std::vector<std::string> > > this_machine_adapters() {
        bool was_init = is_init;
        
        if (not was_init) {

            if (not initialize_network()) {
                (clean_on_except) ? uninitialize_network() : true;
                throw exceptions::initialize_network_failure("Failed to initialize. ", true, __FILE__, __LINE__ - 2, __FUNCTION__);
            }
        }
        
        std::map<std::string, std::map<std::string, std::vector<std::string> > > the_answer;

        ifaddrs_adapter_type all_adapters;
        #if defined(crap_os)

            all_adapters = NULL;
            DWORD memory_size = 20000;
            std::string this_line;
            
            while (not all_adapters) {
                
                all_adapters = (ifaddrs_adapter_type) malloc(memory_size);
                this_line = std::to_string(__LINE__ prev);

                if (not all_adapters) {
                    (clean_on_except) ? uninitialize_network() : true;
                    throw exceptions::memory_exception("Failed to aquire " + std::to_string(memory_size) + " bytes of memory", true, __FILE__, line, __FUNCTION__);
                }

                int resp = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAGS_INCLUDE_PREFIX, 0, all_adapters, &memory_size);
                this_line = std::to_string(__LINE__ prev);

                if (resp is ERROR_BUFFER_OVERFLOW) {
                    ifaddrs_free_adapters(all_adapters);
                }

                else if (resp is ERROR_SUCCESS) {
                    break;
                }

                else {
                    (clean_on_except) ? uninitialize_network() : true;
                    ifaddrs_free_adapters(all_adapters);
                    throw exceptions::unexpected_exception("An unexpected exception occured while trying to retrieve this machine's network adapter information", true, __FILE__, line, __FUNCTION__);
                }

            }

        #else

            if (getifaddrs(&all_adapters)) {
                (clean_on_except) ? uninitialize_network() : true;
                throw exceptions::getifaddrs_failure("Failed to retrieve adater information", true, __FILE__, __LINE__ - 2, __FUNCTION__);
            }

        #endif

        ifaddrs_adapter_type this_adapter;
        ifaddrs_address_type this_address;
        char buffer[buffer_size];
        std::string adapter_name, address_family, address_name;
        std::memset(buffer, 0, buffer_size);
        for (this_adapter = all_adapters; this_adapter; this_adapter = ifaddrs_get_next_adapter(this_adapter)) {

            adapter_name = ifaddrs_get_adapter_name(this_adapter);

            for (this_address = ifaddrs_pull_adapter_address(this_adapter); this_address; this_address = ifaddrs_get_next_address(this_address)) {

                address_family = network_address_families::resolve_address_family_to_string(ifaddrs_get_address_family(this_address));
                // now to pull the address
                string_functions::same_char(*buffer, 0, false) ? (void*) 0 : std::memset(buffer, 0, buffer_size);

                switch (getnameinfo(ifaddrs_get_address_sockaddr(this_address), ifaddrs_get_address_sockaddrlen(this_address), buffer, buffer_size, 0, 0, NI_NUMERICHOST)) {

                    case 0 : {

                        address_name = std::string(buffer);

                        if (not the_answer.count(adapter_name)) {

                            std::map<std::string, std::vector<std::string> > new_map;
                            std::vector<std::string> new_list;
                            new_list.push_back(address_name);
                            new_map.insert(std::make_pair(address_family, new_list));

                            the_answer.insert(std::make_pair(adapter_name, new_map));
                        }

                        else if (not the_answer[adapter_name].count(address_family)) {

                            std::vector<std::string> new_list;
                            new_list.push_back(address_name);

                            the_answer[adapter_name].insert(std::make_pair(address_family, new_list));
                        }

                        else {
                            the_answer[adapter_name][address_family].push_back(address_name);
                        }
                    }


                }

                string_functions::same_char(*buffer, 0, false) ? (void*) 0 : std::memset(buffer, 0, buffer_size);

                switch (getnameinfo(ifaddrs_get_address_sockaddr(this_address), ifaddrs_get_address_sockaddrlen(this_address), buffer, buffer_size, 0, 0, NI_NAMEREQD)) {

                    case 0 : {

                        address_name = std::string(buffer);

                        if (not the_answer.count(adapter_name)) {

                            std::map<std::string, std::vector<std::string> > new_map;
                            std::vector<std::string> new_list;
                            new_list.push_back(adapter_name);
                            new_map.insert(std::make_pair(address_family, new_list));

                            the_answer.insert(std::make_pair(adapter_name, new_map));
                        }

                        else if (not the_answer[adapter_name].count(address_family)) {

                            std::vector<std::string> new_list;
                            new_list.push_back(address_name);

                            the_answer[adapter_name].insert(std::make_pair(address_family, new_list));
                        }
                        
                        else {
                            the_answer[adapter_name][address_family].push_back(address_name);
                        }
                    }
                }


            }

        }
        
        ifaddrs_free_adapters(all_adapters);
        return the_answer;

    }

}