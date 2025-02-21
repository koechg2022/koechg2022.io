



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

    bool network_structures::host::socket_is_connected(const socket_type check_sock) {
        int err;
        socklen_t err_size = sizeof(err);

        if (getsockopt(check_sock, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&err), &err_size)) {
            return false;
        }

        return err is 0;
    }

    network_structures::host::host() {
        this->connect_socket = invalid_socket;
        this->hostname = DEFAULT_HOST;
        this->portvalue = DEFAULT_PORT;
        this->connect_address = null;
        this->timeout = {0, 100000};
        this->tcp = true;
        this->was_init = is_init;
        this->del_on_except = clean_on_except;
        this->secure_ = this->ssl_lib_init = openssl_alg_init = ssl_error_strings_init = false;
        this->ctx = null;
        this->ssl = null;
        this->certification = null;
    }

    network_structures::host::host(const std::string host_name, const std::string port, bool use_tcp, long wait_sec, int wait_msec, bool will_del, bool secure) {
        this->connect_socket = invalid_socket;
        this->hostname = host_name;
        this->portvalue = port;
        this->connect_address = null;
        this->timeout = {wait_sec, wait_msec};
        this->tcp = use_tcp;
        this->was_init = is_init;
        this->del_on_except = will_del;
        this->secure_ = secure;
        this->ssl_lib_init = openssl_alg_init = ssl_error_strings_init = false;
        this->ctx = null;
        this->ssl = null;
        this->certification = null;
    }

    network_structures::host::~host() {
        
        if (this->secure_) {
            
            if (this->ssl NOT null) {
                SSL_shutdown(this->ssl);
            }

            if (valid_socket(this->connect_socket)) {
                close_socket(this->connect_socket);
                this->connect_socket = invalid_socket;
            }

            if (this->ssl NOT null) {
                SSL_free(this->ssl);
            }

            if (this->ctx NOT null) {
                SSL_CTX_free(this->ctx);
            }

        }

        if (valid_socket(this->connect_socket)) {
            close_socket(this->connect_socket);
            this->connect_socket = invalid_socket;
        }

        if (this->connect_address NOT null) {
            freeaddrinfo(this->connect_address);
        }

        if (not was_init) {
            uninitialize_network();
        }
    }

    bool network_structures::host::host_name(const std::string new_host) {
        if (this->connect_address NOT null) {
            this->hostname = new_host;
        }
        return string_functions::same_string(this->hostname, new_host);
    }

    std::string network_structures::host::host_name() const {
        return this->hostname;
    }

    bool network_structures::host::port_value(const std::string new_port) {
        if (this->connect_address is null) {
            this->portvalue = new_port;
        }
        return string_functions::same_string(this->portvalue, new_port);
    }

    std::string network_structures::host::port_value() const {
        return this->portvalue;
    }

    bool network_structures::host::create_address() {

        if (not this->connect_address) {

            if (this->hostname.empty()) {
                throw exceptions::connect_failure("No hostname passed in", true, __FILE__, __LINE__ - 1, __FUNCTION__);
            }
            this->portvalue = (this->portvalue.empty()) ? DEFAULT_PORT : this->portvalue;
            struct addrinfo hints;
            std::memset(&hints, 0, sizeof(hints));
            hints.ai_socktype = SOCK_STREAM;

            (not this->ssl_lib_init) ? SSL_library_init() : 0;
            (not this->openssl_alg_init) ? OpenSSL_add_all_algorithms() : 0;
            (not this->ssl_error_strings_init) ? SSL_load_error_strings() : 0;

            this->ctx = SSL_CTX_new(TLS_client_method());

            if (not this->ctx) {
                throw exceptions::unexpected_exception("Failed to Create the SSL ctx", true, __FILE__, __LINE__ - 3, __FUNCTION__);
            }

            if (getaddrinfo(this->hostname.c_str(), this->portvalue.c_str(), &hints, &this->connect_address)) {
                SSL_CTX_free(this->ctx);
                throw exceptions::getaddrinfo_failure("Failed to retrieve address information for host \"" + this->hostname + "\"", true, __FILE__,__LINE__ - 2, __FUNCTION__);
            }
            // char address_buffer[buffer_size], service_buffer[buffer_size];
            // std::memset(address_buffer, 0, buffer_size);
            // std::memset(service_buffer, 0, buffer_size);
            this->create_socket();

            if (not valid_socket(this->connect_socket)) {
                this->create_socket();
            }

            if (not valid_socket(this->connect_socket)) {
                freeaddrinfo(this->connect_address);
                SSL_CTX_free(this->ctx);
                throw exceptions::create_socket_failure("Failed to create the connection socket", true, __FILE__, __LINE__ - 3, __FUNCTION__);
            }


            if (connect(this->connect_socket, this->connect_address->ai_addr, this->connect_address->ai_addrlen)) {
                freeaddrinfo(this->connect_address);
                close_socket(this->connect_socket);
                SSL_CTX_free(this->ctx);
                throw exceptions::connect_failure("Successfully created the socket, but failed to connect the socket", true, __FILE__, __LINE__ - 2, __FUNCTION__);
            }

            // The address is created. Now to create the ssl

            if (this->secure_) {
                this->ssl = SSL_new(this->ctx);
                if (not this->ssl) {
                    close_socket(this->connect_socket);
                    SSL_CTX_free(this->ctx);
                    freeaddrinfo(this->connect_address);
                    throw exceptions::unexpected_exception("Failed to create a new SSL connection", true, __FILE__, __LINE__ - 5, __FUNCTION__);
                }

                if (not SSL_set_tlsext_host_name(this->ssl, this->hostname.c_str())) {
                    SSL_shutdown(this->ssl);
                    close_socket(this->connect_socket);
                    SSL_CTX_free(this->ctx);
                    freeaddrinfo(this->connect_address);
                    throw exceptions::unexpected_exception("Failed to set the hostname", true, __FILE__, __LINE__ - 5, __FUNCTION__);
                }

                SSL_set_fd(this->ssl, this->connect_socket);
                if (SSL_connect(this->ssl) is -1) {
                    SSL_shutdown(this->ssl);
                    freeaddrinfo(this->connect_address);
                    close_socket(this->connect_socket);
                    throw exceptions::unexpected_exception("Failed to connect the secure tunnel", true, __FILE__, __LINE__ - 4, __FUNCTION__);
                }

                std::printf("SSL/TLS using %s\n", SSL_get_cipher(this->ssl));

                this->certification = SSL_get_peer_certificate(this->ssl);

                if (not this->certification) {
                    SSL_shutdown(this->ssl);
                    freeaddrinfo(this->connect_address);
                    close_socket(this->connect_socket);
                    throw exceptions::unexpected_exception("Failed to create the SSL certification", true, __FILE__, __LINE__ - 6, __FUNCTION__);
                }

                char* temp;

                if ((temp = X509_NAME_oneline(X509_get_subject_name(this->certification), 0, 0))) {
                    std::printf("Issuer : %s\n", temp);
                    OPENSSL_free(temp);
                }

                X509_free(this->certification);

                return this->connect_address NOT null and this->ssl NOT null and this->ctx NOT null;
            }

        }

        return this->connect_address NOT null;
    }

    bool network_structures::host::create_socket() {
        if (not this->connect_address) {
            this->create_address();
        }
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


    bool network_structures::tcp_server::address_is_bound() {
        return this->bound;
    }

    network_structures::tcp_server::tcp_server(const std::string host, const std::string port, int listen_limit, long seconds_wait, int micro_sec_wait, bool will_del, bool secure) :
        network_structures::host::host(host, port, true, seconds_wait, micro_sec_wait, will_del, secure) {
            this->listen_lim = listen_limit;
            this->listening = this->bound = false;
            this->max_socket = invalid_socket;
    }

    network_structures::tcp_server::~tcp_server() {
        if (valid_socket(this->connect_socket)) {
            close_socket(this->connect_socket);
            this->connect_socket = invalid_socket;
            this->listening = false;
        }

        for (auto this_client = this->clients.begin(); this_client != this->clients.end(); this_client++) {
            if (this->socket_is_connected(this_client->first)) {
                close_socket(this_client->first);
            }
            this->clients.erase(this_client->first);
        }
    }

    bool network_structures::tcp_server::server_is_listening() const {
        return this->listening;
    }

    bool network_structures::tcp_server::bind_socket() {

        if (not this->bound) {
            if (not this->create_address()) {
                return false;
            }

            if (not this->create_socket()) {
                return false;
            }

            // To get here the socket is created and bound.
            if (bind(this->connect_socket, this->connect_address->ai_addr, this->connect_address->ai_addrlen)) {
                // TODO : Add appropriate mem management
                throw exceptions::bind_socket_failure("Failed to bind tcp server's listening socket", true, __FILE__, __LINE__ - 2, __FUNCTION__);
            }

            this->bound = true;
        }
        return this->bound;
    }

    bool network_structures::tcp_server::start_listening() {
        if (not this->listening) {

            if (not this->create_address()) {
                return false;
            }

            if (not this->create_socket()) {
                return false;
            }

            if (not this->bind_socket()) {
                return false;
            }

            if (listen(this->connect_socket, this->listen_lim)) {
                // TODO : Add appropriate mem management
                throw exceptions::listen_socket_failure("Failed to start listening with tcp server", true, __FILE__, __LINE__ - 1, __FUNCTION__);
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
        std::set<socket_type> the_answer;
        fd_set ready;
        FD_ZERO(&ready);
        this->max_socket = invalid_socket;
        for (auto client = this->clients.begin(); client NOT this->clients.end(); client++) {
            this->max_socket = (client->first > this->max_socket) ? client->first : this->max_socket;
            FD_SET(client->first, &ready);
        }

        if (valid_socket(this->max_socket)) {
            if (select(this->max_socket, &ready, 0, 0, &this->timeout) < 0) {
                // TODO : Mem management
                throw exceptions::select_failure("Failed to select the clients with information to be read", true, __FILE__, __LINE__, __FUNCTION__);
            }

            for (auto client = this->clients.begin(); client NOT clients.end(); client++) {
                if (FD_ISSET(client->first, &ready)) {
                    the_answer.insert(client->first);
                }
            }
        }
        return the_answer;
    }

    std::set<network_structures::connected_host::client> network_structures::tcp_server::ready_client_info() {
        std::set<network_structures::connected_host::client> the_answer;
        fd_set ready;
        this->max_socket = invalid_socket;
        for (auto client = this->clients.begin(); client NOT this->clients.end(); client++) {
            this->max_socket = (client->first > this->max_socket) ? client->first : this->max_socket;
            FD_SET(client->first, &ready);
        }

        if (valid_socket(this->max_socket)) {
            for (auto client = this->clients.begin(); client NOT this->clients.end(); client++) {
                if (FD_ISSET(client->first, &ready)) {
                    the_answer.insert(client->second);
                }
            }
        }
        return the_answer;
    }

    std::set<socket_type> network_structures::tcp_server::connected_sockets() {
        std::set<socket_type> the_answer;
        this->max_socket = invalid_socket;
        for (auto client = this->clients.begin(); client NOT this->clients.end(); client++) {
            this->max_socket = (client->first > this->max_socket) ? client->first : this->max_socket;
            the_answer.insert(client->first);
        }
        return the_answer;
    }

    std::set<network_structures::connected_host::client> network_structures::tcp_server::connected_client_info() {
        std::set<network_structures::connected_host::client> the_answer;
        this->max_socket = invalid_socket;
        for (auto client = this->clients.begin(); client NOT this->clients.end(); client++) {
            this->max_socket = (client->first > this->max_socket) ? client->first : this->max_socket;
            the_answer.insert(client->second);
        }
        return the_answer;
    }

    bool network_structures::tcp_server::update_connections() {
        
        bool the_answer = false;
        char buffer[buffer_size];
        std::set<socket_type> ready_sockets = this->ready_sockets();
        int bytes_received;
        for (auto this_socket = ready_sockets.begin(); this_socket NOT ready_sockets.end(); this_socket++) {
            std::memset(buffer, 0, buffer_size);
            bytes_received = recv(*this_socket, buffer, buffer_size, 0);
            if (bytes_received < 1) {
                this->clients.erase(*this_socket);
                the_answer = true;
            }
        }
        return the_answer;
    }

    bool network_structures::tcp_server::close_server() {

        if (valid_socket(this->connect_socket)) {
            close_socket(this->connect_socket);
            this->connect_socket = invalid_socket;
            this->listening = this->bound = false;
            this->listen_lim = 0;
        }
        std::set<socket_type> to_remove;
        for (std::map<socket_type, connected_host::client>::iterator client = this->clients.begin(); client != this->clients.end(); client++) {
            // (this->socket_is_connected(client->first)) valid close_socket(client->first) : 0;
            close_socket(client->first);
            to_remove.insert(client->first);
        }

        for (std::set<socket_type>::const_iterator client = to_remove.begin(); client != to_remove.end(); client++) {
            this->clients.erase(*client);
        }

        return not valid_socket(this->connect_socket) and not this->listening and not this->bound and not this->listen_lim and not this->clients.size();
    }

    bool network_structures::tcp_server::new_connection_request() {
        if (this->listening) {
            fd_set ready;
            FD_ZERO(&ready);
            FD_SET(this->connect_socket, &ready);
            if (select(this->connect_socket, &ready, 0, 0, &this->timeout) < 0) {
                // TODO : Implement memory management
                throw exceptions::select_failure("Error occured while selecting the actively listening socket", true, __FILE__, __LINE__, __FUNCTION__);
            }
            return FD_ISSET(this->connect_socket, &ready);
        }
        return false;
    }

    bool network_structures::tcp_server::accept_new_connection() {
        unsigned long old_size = this->clients.size();
        if (this->listening) {
            fd_set ready;
            FD_ZERO(&ready);
            FD_SET(this->connect_socket, &ready);
            
            // Check if the connection socket is ready
            if (select(this->connect_socket, &ready, 0, 0, &this->timeout) < 0) {
                // TODO : Implement the proper memory management and disconnections
                throw exceptions::select_failure("Failed to select on the listening socket to see if there is an incomming connection or not", true, __FILE__, __LINE__, __FUNCTION__);
            }

            if (FD_ISSET(this->connect_socket, &ready)) {
                // The there is a new connection
                network_structures::connected_host::client new_client;
                socklen_t client_size = sizeof(new_client.address_info);
                new_client.connected_socket = accept(this->connect_socket, (struct sockaddr*) &new_client.address_info, &client_size);

                if (not valid_socket(new_client.connected_socket)) {
                    // TODO : Implement the proper memory management disconnections
                    throw exceptions::create_socket_failure("Failed to create the new connection socket", true, __FILE__, __LINE__, __FUNCTION__);
                }

                char address[buffer_size], service[buffer_size];
                std::memset(address, 0, buffer_size);
                std::memset(service, 0, buffer_size);

                if (getnameinfo((struct sockaddr*) &new_client.address_info, client_size, address, buffer_size, service, buffer_size, NI_NUMERICHOST)) {
                    // TODO : Implement the proper memory management disconnections
                    throw exceptions::getnameinfo_failure("Failed to retrieve name and service names of the new connecting client", true, __FILE__, __LINE__, __FUNCTION__);
                }

                new_client.hostname = std::string(address);
                new_client.portvalue = std::string(service);
                this->clients.insert({new_client.connected_socket, new_client});
            }
        }
        return this->clients.size() greater than old_size;
    }

    bool network_structures::tcp_server::close_connection(const socket_type to_close) {
        socket_type this_socket = invalid_socket;
        for (auto client = this->clients.begin(); client NOT this->clients.end(); client++) {
            if (to_close is client->first) {
                this_socket = client->first;
                break;
            }
        }

        if (not valid_socket(this_socket)) {
            // close the connection
            close_socket(this_socket);
            this->clients.erase(this_socket);
        }
        return not this->clients.contains(to_close);
    }

    bool network_structures::tcp_server::close_connection(const std::string host_name, const std::string port_name) {
        
        socket_type this_socket = invalid_socket;
        for (auto client = this->clients.begin(); client NOT this->clients.end(); client++) {
            if (string_functions::same_string(client->second.hostname, host_name) and string_functions::same_string(client->second.portvalue, port_name)) {
                this_socket = client->first;
                break;
            }
        }

        if (valid_socket(this_socket)) {
            close_socket(this_socket);
            this->clients.erase(this_socket);
        }
        return valid_socket(this_socket) and not this->clients.contains(this_socket);
    }

    socket_type network_structures::tcp_server::get_max_socket() const {
        return this->max_socket;
    }

    bool network_structures::tcp_server::connected_socket(const socket_type sock) {
        return socket_is_connected(this->connect_socket);
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
        network_structures::host() {
        this->connected = false;
    }


    network_structures::tcp_client::tcp_client(const std::string remote_host, const std::string connect_port, const long wait_sec, const int wait_msec, bool will_del, bool secure):
        network_structures::host(remote_host, connect_port, true, wait_sec, wait_msec, will_del, secure) {
            this->connected = false;
    }


    network_structures::tcp_client::~tcp_client() {
        this->connected = false;
    }


    bool network_structures::tcp_client::connect_client() {
        if (not this->connected) {
            this->create_address();
            this->create_socket();
            if (valid_socket(this->connect_socket)) {
                if (connect(this->connect_socket, this->connect_address->ai_addr, this->connect_address->ai_addrlen)) {
                    // TODO : Implement memeory management when this error occurs
                    throw exceptions::connect_failure("Failed to connect to server \"" + this->hostname + "\"", true, __FILE__, __LINE__, __FUNCTION__);
                }
                this->connected = true;
            }
        }
        return this-connected;
    }


    bool network_structures::tcp_client::disconnect_client() {
        if (valid_socket(this->connect_socket)) {
            close_socket(this->connect_socket);
            this->connected = false;
        }
        return this->connected is false;
    }


    bool network_structures::tcp_client::server_has_message() {
        if (this->connected) {

            fd_set ready;
            FD_ZERO(&ready);
            FD_SET(this->connect_socket, &ready);

            if (select(this->connect_socket, &ready, 0, 0, &this->timeout) < 0) {
                // TODO : Implement memory management for exception
                throw exceptions::select_failure("Failed to select the active listening socket for client", true, __FILE__, __LINE__, __FUNCTION__);
            }

            return FD_ISSET(this->connect_socket, &ready);
        }
        return false;
    }


    network_structures::connected_host::server network_structures::tcp_client::get_connection_info() const {
        return {this->hostname, this->portvalue, *this->connect_address};
    }

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ///////////////////tcp_client end///////////////////

}