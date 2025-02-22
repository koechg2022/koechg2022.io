



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
                        if (address_name.empty()) {
                            continue;
                        }
                        else if (not the_answer.count(adapter_name)) {

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
                        if (address_name.empty()) {
                            continue;
                        }
                        else if (not the_answer.count(adapter_name)) {

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

    bool socket_is_connected(const int the_socket) {
        int error = 0;
        socklen_t len = sizeof(error);
        int retval = 0;

        #ifdef _WIN32
            u_long mode = 1;  // 1 to enable non-blocking socket
            ioctlsocket(sock, FIONBIO, &mode);
        #else
            int flags = fcntl(the_socket, F_GETFL, 0);
            fcntl(the_socket, F_SETFL, flags | O_NONBLOCK);
        #endif

        char buffer[1];
        retval = recv(the_socket, buffer, 1, MSG_PEEK);

        if (retval == 0) {
            return false;  // Connection closed
        } else if (retval < 0) {
            #ifdef _WIN32
                error = WSAGetLastError();
                if (error != WSAEWOULDBLOCK) {
                    return false;  // Error occurred
                }
            #else
                if (errno != EWOULDBLOCK && errno != EAGAIN) {
                    return false;  // Error occurred
                }
            #endif
        }

        return true;  // Connection is still alive
    }

    bool network_structures::connected_host::client::operator<(const client& other) const {
        return this->connected_socket < other.connected_socket;
    }

    bool network_structures::connected_host::server::operator<(const server& other) const {
        return this->hostname < other.hostname and 
            this->portvalue < other.portvalue;
    }

    ////////////////////////host////////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////


    
    
    network_structures::host::host() {
        this->connect_socket = invalid_socket;
        this->connect_address = null;
        this->hostname = this->portvalue = "";
        this->timeout = {0, 100000};
        this->tcp = true;
        this->was_init = is_init;
        this->del_on_except = true;
        this->secure_ = false;
    }

    network_structures::host::host(const std::string host_name, const std::string port, bool use_tcp, long wait_sec, int wait_msec, bool will_del, bool secure) {
        this->connect_socket = invalid_socket;
        this->connect_address = null;
        this->hostname = host_name;
        this->portvalue = port;
        this->timeout = {wait_sec, wait_msec};
        this->tcp = use_tcp;
        this->was_init = is_init;
        this->del_on_except = will_del;
        this->secure_ = secure;
    }

    network_structures::host::~host() {
        
        if (this->connect_address NOT null) {
            freeaddrinfo(this->connect_address);
        }

        if (valid_socket(this->connect_socket)) {
            close_socket(this->connect_socket);
        }

        if (not was_init) {
            uninitialize_network();
        }
    }

    bool network_structures::host::host_name(const std::string new_host) {
        
        // Is there a connection in place?
        if (not valid_socket(this->connect_socket)) {
            this->hostname = new_host;
        }
        return string_functions::same_string(new_host, this->hostname);
    }

    std::string network_structures::host::host_name() const {
        return this->hostname;
    }

    bool network_structures::host::port_value(const std::string new_port) {

        // Is there a connection in place?
        if (not valid_socket(this->connect_socket)) {
            this->portvalue = new_port;
        }
        return string_functions::same_string(new_port, this->portvalue);
    }

    std::string network_structures::host::port_value() const {
        return this->portvalue;
    }

    bool network_structures::host::create_address() {
        if (this->connect_address is null) {
            struct addrinfo hints;
            std::memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = (this->tcp) ? SOCK_STREAM : SOCK_DGRAM;
            hints.ai_flags = AI_PASSIVE;
            if (getaddrinfo(this->hostname.c_str(), this->portvalue.c_str(), &hints, &this->connect_address)) {
                throw exceptions::getaddrinfo_failure("Failed to retrieve address information for \"" + this->hostname + "\"", true, __FILE__, __LINE__ - 1, __FUNCTION__);
            }
        }
        return this->connect_address NOT null;
    }

    bool network_structures::host::create_socket() {
        this->create_address();
        this->connect_socket = socket(this->connect_address->ai_family, this->connect_address->ai_socktype, this->connect_address->ai_protocol);
        return valid_socket(this->connect_socket);
    }

    void network_structures::host::update_timeout(long sec, int m_sec) {
        this->timeout = {sec, m_sec};
    }

    bool network_structures::host::will_delete_on_except() const {
        return this->del_on_except;
    }

    void network_structures::host::will_delete_on_except(const bool new_flag) {
        this->del_on_except = new_flag;
    }

    socket_type network_structures::host::get_connection_socket() const {
        return this->connect_socket;
    }


    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    //////////////////////host end//////////////////////


    /**************************************************/

    
    /////////////////////tcp_server/////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////

// network_structures::host::host(const std::string host_name, const std::string port, bool use_tcp, long wait_sec, int wait_msec, bool will_del, bool secure)
    network_structures::tcp_server::tcp_server(const std::string host, const std::string port, int listen_limit, long seconds_wait, int micro_sec_wait, bool will_del, bool secure) :
        network_structures::host::host(host, port, true, seconds_wait, micro_sec_wait, will_del, secure) {
        

        // Fill the hostname
        if (host.empty()) {
            
            #if defined(unix_os)
                #if defined(mac_os)
                    const std::string rel_adapter = "en0";
                #else
                    const std::string rel_adapter = "enp0s8";
                #endif
            #else
                const std::string rel_adapter = "Wi-Fi 3";
            #endif
            
            std::map<std::string, std::map<std::string, std::vector<std::string> > > adapters = this_machine_adapters();

            for (auto adapter = adapters.begin(); adapter NOT adapters.end(); adapter++) {
                if (not string_functions::same_string(adapter->first, rel_adapter)) {
                    continue;
                }

                for (auto family = adapter->second.begin(); family NOT adapter->second.end(); family++) {

                    for (auto address = family->second.begin(); address NOT family->second.end(); address++) {
                        this->hostname = *address;
                        break;
                    }

                    if (not this->hostname.empty()) {
                        break;
                    }
                }

                if (not this->hostname.empty()) {
                    break;
                }
            }
        }

        else {
            this->hostname = host;
        }


        // Fill the port
        if (port.empty()) {
            this->portvalue = DEFAULT_PORT;
        }

        else {
            this->portvalue = port;
        }


        this->listen_lim = listen_limit;
        this->listening = bound = false;
        this->max_socket = invalid_socket;
    }


    network_structures::tcp_server::~tcp_server() {
        if (this->listening) {
            this->listening = false;
        }

        if (this->bound) {
            this->bound = false;
        }

        if (valid_socket(this->max_socket)) {
            this->max_socket = invalid_socket;
        }

        for (auto client = this->clients.begin(); client NOT clients.end(); client++) {
            if (valid_socket(client->first)) {
                close_socket(client->first);
            }
            this->clients.erase(client->first);
        }
    }

    bool network_structures::tcp_server::server_is_listening() const {
        return this->listening;
    }

    bool network_structures::tcp_server::bind_socket() {

        if (not this->bound) {
            this->create_address();
            this->create_socket();

            if (bind(this->connect_socket, this->connect_address->ai_addr, this->connect_address->ai_addrlen)) {
                (this->del_on_except) ? freeaddrinfo(this->connect_address) : (void) 0;
                throw exceptions::bind_socket_failure("Failed to bind the listening socket for server \"" + this->hostname + "\" on port \"" + this->portvalue + "\"", true, __FILE__, __LINE__ - 2, __FUNCTION__);
            }
            this->bound = true;
        }
        return this->bound;
    }

    bool network_structures::tcp_server::start_listening() {

        if (not this->listening) {
            this->create_address();
            this->create_socket();
            this->bind_socket();

            if (listen(this->connect_socket, this->listen_lim) < 0) {
                (this->del_on_except) ? freeaddrinfo(this->connect_address) : (void) 0;
                (not this->was_init) ? uninitialize_network() : true;
                throw exceptions::listen_socket_failure("Failed to start listening on the listening socket for \"" + this->hostname + "\"", true, __FILE__, __LINE__ - 2, __FUNCTION__);
            }
            this->listening = true;
        }
        return this->listening;
    }

    network_structures::tcp_server::operator bool() const {
        return this->listening;
    }

    int network_structures::tcp_server::listening_limit() const {
        return this->listen_lim;
    }

    std::set<socket_type> network_structures::tcp_server::ready_sockets() {
        fd_set ready;
        FD_ZERO(&ready);
        this->max_socket = invalid_socket;
        for (auto client = this->clients.begin(); client NOT this->clients.end(); client++) {

            if (client->first > this->max_socket) {
                this->max_socket = client->first;
            }
            FD_SET(client->first, &ready);
        }

        if (select(this->max_socket + 1, &ready, 0, 0, &this->timeout) < 0) {
            (this->del_on_except) ? freeaddrinfo(this->connect_address) : (void) 0;
            (not this->was_init) ? uninitialize_network() : true;
            throw exceptions::select_failure("Failed to select the connection sockets that are ready to be read from", true, __FILE__, __LINE__ - 3, __FUNCTION__);
        }
        std::set<socket_type> the_answer;
        for (auto client = clients.begin(); client NOT clients.end(); client++) {
            if (FD_ISSET(client->first, &ready)) {
                the_answer.insert(client->first);
            }
        }
        return the_answer;
    }

    std::set<network_structures::connected_host::client> network_structures::tcp_server::ready_client_info() {
        fd_set ready;
        FD_ZERO(&ready);
        this->max_socket = invalid_socket;
        for (auto client = this->clients.begin(); client NOT this->clients.end(); client++) {
            if (client->first > this->max_socket) {
                this->max_socket = client->first;
            }
            FD_SET(client->first, &ready);
        }
        
        if (select(this->max_socket + 1, &ready, 0, 0, &this->timeout) < 0) {
            (this->del_on_except) ? freeaddrinfo(this->connect_address) : (void) 0;
            (not this->was_init) ? uninitialize_network() : true;
            throw exceptions::select_failure("Failed to select the connection sockets that are ready to be read from", true, __FILE__, __LINE__ - 3, __FUNCTION__);
        }
        std::set<network_structures::connected_host::client> the_answer;

        for (auto client = this->clients.begin(); client NOT this->clients.end(); client++) {
            if (FD_ISSET(client->first, &ready)) {
                the_answer.insert(client->second);
            }
        }
        return the_answer;
    }

    std::set<socket_type> network_structures::tcp_server::connected_sockets() {
        std::set<socket_type> the_answer;
        for (auto client = this->clients.begin(); client NOT clients.end(); client++) {
            the_answer.insert(client->first);
        }
        return the_answer;
    }

    std::set<network_structures::connected_host::client> network_structures::tcp_server::connected_client_info() {
        std::set<network_structures::connected_host::client> the_answer;
        for (auto client = this->clients.begin(); client NOT clients.end(); client++) {
            the_answer.insert(client->second);
        }
        return the_answer;
    }

    bool network_structures::tcp_server::update_connections() {
        bool the_answer = false;
        for (auto client = this->clients.begin(); client NOT this->clients.end(); client++) {
            if (not socket_is_connected(client->first)) {
                close_socket(client->first);
                this->clients.erase(client->first);
                the_answer = true;
            }
        }
        return the_answer;
    }

    bool network_structures::tcp_server::close_server() {
        if (connect_address NOT null) {
            freeaddrinfo(connect_address);
        }

        if (valid_socket(this->connect_socket)) {
            close_socket(this->connect_socket);
        }

        this->listening = this->bound = false;

        for (auto client = this->clients.begin(); client NOT this->clients.end(); client++) {
            close_socket(client->first);
            this->clients.erase(client->first);
        }
        this->max_socket = invalid_socket;
        return this->clients.empty() and not this->listening and not this->bound;
    }

    bool network_structures::tcp_server::new_connection_request() {
        fd_set ready;
        FD_ZERO(&ready);
        FD_SET(this->connect_socket, &ready);

        if (select(this->connect_socket + 1, &ready, 0, 0, &this->timeout) < 0) {
            (this->del_on_except) ? freeaddrinfo(this->connect_address) : (void) 0;
            (not this->was_init) ? uninitialize_network() : true;
            throw exceptions::select_failure("Failed to select the server's listening socket to be read from", true, __FILE__, __LINE__ - 3, __FUNCTION__);
        }

        return FD_ISSET(this->connect_socket, &ready);
    }

    bool network_structures::tcp_server::accept_new_connection() {
        const unsigned long old_size = this->clients.size();
        if (this->new_connection_request()) {
            // the listening socket is ready for a new connection socket
            network_structures::connected_host::client new_client;
            new_client.connected_socket = invalid_socket;
            socklen_t the_size = sizeof(new_client.address_info);
            new_client.connected_socket = accept(this->connect_socket, (struct sockaddr*) &new_client.address_info, &the_size);

            if (valid_socket(new_client.connected_socket)) {
                char address[buffer_size], service[buffer_size];
                std::memset(address, 0, buffer_size);
                std::memset(service, 0, buffer_size);

                if (getnameinfo((struct sockaddr*) &new_client.address_info, the_size, address, buffer_size, service, buffer_size, NI_NUMERICHOST | NI_NUMERICSERV)) {
                    (clean_on_except) ? uninitialize_network() : true;
                    (this->del_on_except) ? this->close_server() : true;
                    throw exceptions::getnameinfo_failure("Failed to retrieve name and/or port of the client that is connecting to this server", true, __FILE__, __LINE__ - 3, __FUNCTION__);
                }
                new_client.hostname = (string_functions::same_char(*address, 0)) ? "Unspecified Host" : std::string(address);
                new_client.portvalue = (string_functions::same_char(*service, 0)) ? "Unspecified Port" : std::string(service);
                this->clients.insert({new_client.connected_socket, new_client});
            }
        }
        return this->clients.size() > old_size;
    }

    bool network_structures::tcp_server::close_connection(const socket_type to_close) {
        
        bool the_answer = false;

        for (auto client = this->clients.begin(); client NOT this->clients.end(); client++) {
            if (to_close is client->first) {
                (socket_is_connected(client->first)) ? close_socket(client->first) : 0;
                this->clients.erase(client->first);
                the_answer = true;
                break;
            }
        }
        this->max_socket = invalid_socket;
        for (auto client = this->clients.begin(); client NOT this->clients.end(); client++) {
            this->max_socket = (client->first > this->max_socket) ? client->first : this->max_socket;
        }

        return the_answer;
    }

    bool network_structures::tcp_server::close_connection(const std::string host_name, const std::string port_name) {
        
        bool the_answer = false;

        for (auto client = this->clients.begin(); client NOT this->clients.end(); client++) {
            if (string_functions::same_string(host_name, client->second.hostname) and string_functions::same_string(port_name, client->second.portvalue)) {
                (socket_is_connected(client->first)) ? close_socket(client->first) : 0;
                this->clients.erase(client->first);
                the_answer = true;
                break;
            }
        }
        this->max_socket = invalid_socket;
        for (auto client = this->clients.begin(); client NOT this->clients.end(); client++) {
            this->max_socket = (client->first > this->max_socket) ? client->first : this->max_socket;
        }

        return the_answer;
    }

    socket_type network_structures::tcp_server::get_max_socket() const {
        return this->max_socket;
    }

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ///////////////////tcp_server end///////////////////


    /**************************************************/

    
    /////////////////////tcp_client/////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////


    
    network_structures::tcp_client::tcp_client() :
        networking::network_structures::host() {
            this->connected = false;
    }

    
    network_structures::tcp_client::tcp_client(const std::string remote_host, const std::string connect_port, const long wait_sec, const int wait_msec, bool will_del, bool secure) :
        networking::network_structures::host(remote_host, connect_port, true, wait_sec, wait_msec, will_del, secure) {
            this->connected = false;
        }

    
    network_structures::tcp_client::~tcp_client() {
        this->disconnect_client();
    }


    bool network_structures::tcp_client::connect_client() {
        if (not this->connected) {
            this->create_address();
            this->create_socket();

            if (connect(this->connect_socket, this->connect_address->ai_addr, this->connect_address->ai_addrlen)) {
                (this->del_on_except) ? freeaddrinfo(this->connect_address) : (void) 0;
                (not this->was_init) ? uninitialize_network() : true;
                throw exceptions::connect_failure("Failed to connect to the server for \"" + this->hostname + "\"", true, __FILE__, __LINE__ - 3, __FUNCTION__);
            }
            this->connected = true;
        }
        return this->connected;
    }

    bool network_structures::tcp_client::client_is_connected() const {
        return this->connected;
    }

    bool network_structures::tcp_client::disconnect_client() {
        if (this->connected) {
            close_socket(this->connect_socket);
            this->connect_socket = invalid_socket;
            this->connected = false;
        }
        return this->connected;
    }

    bool network_structures::tcp_client::server_has_message() {
        fd_set ready;
        FD_ZERO(&ready);
        FD_SET(this->connect_socket, &ready);
        if (select(this->connect_socket + 1, &ready, 0, 0, &this->timeout) < 0) {
            (this->del_on_except) ? freeaddrinfo(this->connect_address) : (void) 0;
            (not this->was_init) ? uninitialize_network() : true;
            throw exceptions::select_failure("Failed to select the client's connection socket to \"" + this->hostname + "\"", true, __FILE__, __LINE__ - 3, __FUNCTION__);
        }
        return FD_ISSET(this->connect_socket, &ready);
    }

    network_structures::connected_host::server network_structures::tcp_client::get_connection_info() const {
        return {this->hostname, this->portvalue, *this->connect_address};
    }

    network_structures::tcp_client::operator bool() const {
        return this->client_is_connected();
    }


    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ///////////////////tcp_client end///////////////////

}