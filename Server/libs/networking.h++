#include "../headers/included.h++"
#include "string_functions.h++"


#if not defined(_NETWORKING_FUNC_)
    #define _NETWORKING_FUNC_


    namespace networking {

        static bool clean_on_except = true;

        namespace {
            #if defined(crap_os)
                bool is_init = false;
            #else
                const bool is_init = true;
            #endif
        }

        namespace network_address_families {

            const std::string unspec_address_family = "Unspecific Address family";
            const std::string ip_version4_address_family = "IP Version 4 family";
            const std::string ip_version6_address_family = "IP Version 6 family";

            #if defined(unix_os)
                
                #if defined(mac_os)
                    const std::string link_layer_address_family = "Link-layer Interface Address family";
                #else
                    const std::string netlink_address_family = "Netlink Address family";
                    const std::string packet_address_family = "Packet Address family";
                #endif
            #else
                
                const std::string netbios_address_family = "NetBIOS Address family";
                const std::string irda_address_family = "IrDa Address family";
                const std::string bluetooth_address_family = "Bluetooth Address family";

            #endif

            const std::string unrecognized_address_family = "Unrecognized Address family";


            const std::set<std::string> get_address_families();


            const std::string resolve_address_family_to_string(socket_family_type the_family);

        }


        namespace exceptions {

            class base_exception {

                protected:
                    std::string except_name, message, file, func;
                    int line;
                
                public:

                    base_exception(const std::string name = "Exception", const std::string msg = "Simple exception thrown", bool print = true, const std::string file_name = __FILE__, const int except_line = __LINE__, const std::string function = "Unknown function");

                    ~base_exception() throw();

                    const std::string msg() const;

            };

            class unexpected_exception : public base_exception {
                public:
                    unexpected_exception(const std::string msg = "An unexpected exception has occured", bool print = true, const std::string file_name = __FILE__, const int except_line = __LINE__, const std::string function = "Unknown function");
            };

            class memory_exception : public base_exception {
                public:
                    memory_exception(const std::string msg = "A memory exception has occured", bool print = true, const std::string file_name = __FILE__, const int except_line = __LINE__, const std::string function = "Unknown function");
            };

            class initialize_network_failure : public base_exception {
                public:
                    initialize_network_failure(const std::string msg = "Failed to initialize the network", bool print = true, const std::string file_name = __FILE__, const int except_line = __LINE__, const std::string function = "Unknown function");
            };

            class getaddrinfo_failure : public base_exception {
                public:
                    getaddrinfo_failure(const std::string msg = "Failed to retrieve address information", bool print = true, const std::string file_name = __FILE__, const int except_line = __LINE__, const std::string function = "Unknown function");
            };

            class getifaddrs_failure : public base_exception {
                public:
                    getifaddrs_failure(const std::string msg = "Failed to retrieve adapter information", bool print = true, const std::string file_name = __FILE__, const int except_line = __LINE__, const std::string function = "Unknown function");

            };

            class create_socket_failure : public base_exception {
                public:
                    create_socket_failure(const std::string msg = "Failed to create a socket", bool print = true, const std::string file_name = __FILE__, const int except_line = __LINE__, const std::string function = "Unknown function");
            };

            class bind_socket_failure : public base_exception {
                public:
                    bind_socket_failure(const std::string msg = "Failed to bind the socket", bool print = true, const std::string file_name = __FILE__, const int except_line = __LINE__, const std::string function = "Unknown function");
            };

            class listen_socket_failure : public base_exception {
                public:
                    listen_socket_failure(const std::string msg = "Failed to start the socket listening", bool print = true, const std::string file_name = __FILE__, const int except_line = __LINE__, const std::string function = "Unknown function");
            };

            class select_failure : public base_exception {
                public:
                    select_failure(const std::string msg = "select function failure", bool print = true, const std::string file_name = __FILE__, const int except_line = __LINE__, const std::string function = "Unknown function");
            };

            class getnameinfo_failure : public base_exception {
                public:
                    getnameinfo_failure(const std::string msg = "getnameinfo function failure", bool print = true, const std::string file_name = __FILE__, const int except_line = __LINE__, const std::string function = "Unknown function");
            };

            class connect_failure : public base_exception {
                public:
                    connect_failure(const std::string msg = "connection failure", bool pirnt = true, const std::string file_name = __FILE__, const int except_line = __LINE__, const std::string function = "Unknown function");
            };

        }

        
        /**
        *
        *   @brief Initialize the network for use.
        *   
        *
        *   @returns `true` if the network is successfully
        *   initialized, `false` otherwise.
        *
        *   @note This only does something on windows machines.
        *
        *
        */
        bool initialize_network();

        
        /**
        *
        *   @brief Uninitialize the network for use.
        *
        *
        *   @returns `true` if the network was successfully 
        *   uninitialized, `false` otherwise.
        *
        *   @note This only does something on windows machines.
        *
        *
        */
        bool uninitialize_network();


        /**
        *
        *
        *   @brief Get the IP address of a hostname
        *
        *   @param hostname (`const std::string`) : The hostname whose IP address is to be retrieved.
        *
        *   @param port (`const std::string`) : Defaults to `DEFAULT_PORT`. The port to be used 
        *   in resolving the hostname.
        *
        *   @returns A `std::vector<std::string>` of the IP addresses associated with the `hostname` passed in.
        *
        *
        */
        std::vector<std::string> resolve_hostname(const std::string hostname, const std::string port = DEFAULT_PORT);


        
        /**
        *
        *
        *   @brief Get the adapters on this machine and return then in the form of a std::map with the characteristics:
        *       std::map<std::string, std::map<std::string, std::vector<std::string> > > : {
        *           adapter name : {
        *                           adapter family : {std::set of adapter addresses for this family}
        *                           }
        *       }
        *
        *
        *   @returns A `std::map<std::string, std::map<std::string, std::set<std::string> > >` with the adapters,
        *   the adapter's adapter families, and their IP addresses.
        *
        */
        std::map<std::string, std::map<std::string, std::vector<std::string> > > this_machine_adapters();


        namespace network_structures {

            namespace connected_host {
                
                typedef struct client {
                    std::string hostname, portvalue;
                    socket_type connected_socket;
                    struct sockaddr_storage address_info;

                    bool operator<(const client& other) const;
                } client;

                typedef struct server{
                    std::string hostname, portvalue;
                    struct addrinfo address_info;

                    bool operator<(const server& other) const;
                } server;

            }

            class host {

                protected:

                    socket_type connect_socket;
                    std::string hostname, portvalue;
                    struct addrinfo* connect_address;
                    struct timeval timeout;
                    bool tcp, was_init, del_on_except, secure_, ssl_lib_init, openssl_alg_init, ssl_error_strings_init;
                    SSL_CTX* ctx;
                    SSL* ssl;
                    X509* certification;

                    bool socket_is_connected(const socket_type check_sock);

                public:

                    host();
                    

                    host(const std::string host_name, const std::string port = DEFAULT_PORT, bool use_tcp = true, long wait_sec = 0, int wait_msec = 100000, bool will_del = true, bool secure = false);

                    
                    ~host();

                    
                    /**
                    *
                    *   @brief Update the `hostname` that this host refers to.
                    *
                    *   @param new_host `const std::string` 
                    *   The new hostname that this host will refer to.
                    *
                    *   
                    *   @returns `true` if the hostname was successfully updated, `false` otherwise
                    *
                    *   @note if the host has address_information, then the hostname will not be updated.
                    *   
                    */
                    bool host_name(const std::string new_host);

                    
                    /**
                    *
                    *   @brief Get the hostname that this host references.
                    *
                    *   @returns `const std::string` The hostname.
                    *
                    */
                    std::string host_name() const;
                    
                    /**
                    *
                    *   @brief Update the port that this host will use for connection.
                    *
                    *
                    *   @param new_port `const std::string` The new port that this host will use.
                    *
                    *
                    *   @note If the host has address_information, then the port will not be updated.
                    */
                    bool port_value(const std::string new_port);

                    
                    /**
                    *
                    *   @brief Get the port value that this host is using.
                    *
                    *   @returns `const std::string` The port value that this host is using.
                    *
                    */
                    std::string port_value() const;

                    
                    /**
                    *
                    *   @brief Creates the address for this host.
                    *
                    *   @returns `true` if the address is fulfilled, `false` otherwise.
                    */
                    bool create_address();

                    
                    /**
                    *
                    *   @brief Creates the socket that this host will use for communication
                    *
                    *   @returns `true` if the socket was successfully created, `false` otherwise.
                    */
                    bool create_socket();

                    
                    /**
                    *
                    *   @brief update the timeout for the select function that is used by the host
                    *   when selecting between different sockets that are in an active state.
                    *
                    *   @param sec `long` The number of seconds that the select function should check for
                    *
                    *   @param m_sec `int` The number of milliseconds that the select function should check for.
                    *   
                    *
                    */
                    void update_timeout(long sec, int m_sec);


                    /**
                    *
                    *   @brief See if this host will delete it's content when an exception is thrown or not.
                    *
                    *   @returns `true` if the host will delete it's content when an exception is thrown,
                    *   `false` otherwise.
                    *
                    */
                    bool will_delete_on_except() const;

                    
                    /**
                    *
                    *
                    *   @brief Tell the host to delete it's content when an exception is thrown or not.
                    *
                    *   @param new_flag `const bool` The flag that will tell the host whether to delete
                    *   it's content or not upon an exception being thrown.
                    *
                    */
                    void will_delete_on_except(const bool new_flag);


                    /**
                    *
                    *   @brief Get the socket that this host is using for commuication.
                    *
                    *
                    *   @returns A `socket_type` of the socket that is being used by this host.
                    *
                    */
                    socket_type get_connection_socket() const;



            };

            class tcp_server : public host {
                private:

                    int listen_lim;
                    bool listening, bound;
                    socket_type max_socket;
                    std::map<socket_type, connected_host::client> clients;

                    /**
                        @brief Checks if the address is bound or not.
                        If the address is bound, the method returns `true`,
                        and if it isn't the method returns `false`.
                     */
                    bool address_is_bound();


                public:

                    tcp_server(const std::string host = "", const std::string port = "8080", int listen_limit = 10, long seconds_wait = 0, int micro_sec_wait = 100000, bool will_del = true, bool secure = false);


                    ~tcp_server();


                    /**
                        @brief Check if the server is listening or not.
                        @returns `true` if the server is listening, `false` otherwise.
                     */
                    bool server_is_listening() const;

                    
                    /**
                        @brief Binds the socket, if it's not bound.
                        @note If the socket is bound, yeyy... but nothing happens.
                        @returns `true` if the socket is bound, `false` otherwise.
                     */
                    bool bind_socket();


                    /**
                        @brief Starts the server listening.
                        @returns `true` if the server is listening, `false` otherwise.
                     */
                    bool start_listening();


                    operator bool() const;


                    /**
                        @brief Get the listening limit on this tcp_server.
                        @returns The max number of listening instances that
                        the tcp server can listen for.
                     */
                    int listening_limit() const;


                    /**
                        @brief Get the sockets that are ready with data to be read.
                        @returns A `std::set<socket_type>` of all the sockets that are ready to
                        be read from.
                     */
                    std::set<socket_type> ready_sockets();


                    /**
                        @brief Get the clients that are ready in the form of `connected_host::client` objects
                        where each `connected_host::client` has the information of the connected client.

                        @returns A `std::set<connected_host::client>` with all the clients that are ready
                        to be listned to.
                     */
                    std::set<connected_host::client> ready_client_info();


                    /**
                        @brief Get a `std::set<socket_type>` of all the sockets connected to this
                        server.
                        
                        @returns A `std::set<socket_type>` with all the connected sockets, regardless of 
                        whether they currently have data to be read from or not.
                     */
                    std::set<socket_type> connected_sockets();


                    /**
                        @brief Get a `std::set<connected_host::client>` of all the clients connected to this
                        server.
                        
                        @returns A `std::set<connected_host::client>` with all the connected clients, regardless of 
                        whether they currently have data to be read from or not.
                     */
                    std::set<connected_host::client> connected_client_info();


                    /**
                        @brief Check if there are any clients that were disconnected by the client. 
                        @note If there are, then that client is disconnected from the server via 
                        this method.
                        @returns `true` if a client is disconnected, `false` otherwise.
                     */
                    bool update_connections();


                    /**
                        @brief Closes down the server and stops it from listening.
                        @returns `true` if the server is successfully closed, `false` otherwise.
                     */
                    bool close_server();


                    /**
                        @brief Check if there is a new connection request from a client.
                        @returns `true` if there is a new connection request, `false` otherwise.
                     */
                    bool new_connection_request();


                    /**
                        @brief Accept a new connection if there is indeed a new connection, if there 
                        is no new connection, no biggie--nothing is accepted...
                        @returns `true` if a new connection was accepted, `false` otherwise.
                     */
                    bool accept_new_connection();


                    /**
                        @brief Closes the socket that was passed in 
                        (and is currently being used for a connection by the server) 
                        and returns `true` if the socket was successfully closed.
                        @param to_close (`socket_type`) : The socket to be closed.
                     */
                    bool close_connection(const socket_type to_close);


                    /**
                        @brief Closes the connection to the host (via the port) that were passed in.
                        @param host_name (`const std::string`) : The host with the connection to be closed.
                        @param port_name (`const std::string`) : The port being used to sustain the connection to `host_name`.
                        @returns `true` if the connection to `host_name` via `port_name` 
                                        is successfully closed, `false` if it's not closed.
                     */
                    bool close_connection(const std::string host_name, const std::string port_name);


                    /**
                        @brief Get the maximum socket that the server is currently using.
                        @note, if an invalid socket is returned, then there are no connections 
                        to this server.
                     */
                    socket_type get_max_socket() const;


                    /**
                        @brief Check if the socket passed in is connected or not.
                     */
                    bool connected_socket(const socket_type sock);



            };

            class tcp_client : public host {
                private:
                    bool connected;

                public:

                    tcp_client();


                    tcp_client(const std::string remote_host, const std::string connect_port = DEFAULT_PORT, const long wait_sec = 0, const int wait_msec = 100000, bool will_del = true, bool secure = false);


                    ~tcp_client();


                    /**
                    *
                    *
                    *   @brief Connect the client client to the server.
                    *
                    *
                    *   @returns `true` if the connection to the server is successfully established,
                    *   `false` otherwise.
                    *
                    *
                    */
                    bool connect_client();


                    
                    /**
                    *
                    *
                    *   @brief Check if the client is connected or not
                    *
                    *
                    *   @returns `true` if the client is connected,
                    *   `false` otherwise.
                    *
                    */
                    bool client_is_connected();


                    /**
                    *
                    *   @brief Disconnect the client from the server.
                    *
                    *
                    *
                    *   @returns `true` if the client is successfully disconnected,
                    *   `false` otherwise.
                    *
                    */
                    bool disconnect_client();

                    
                    /**
                    *
                    *
                    *   @brief Check if there is a message from the server.
                    *
                    *
                    *   @returns `true` if the server has a message,
                    *   `false` otherwise.
                    *
                    */
                    bool server_has_message();


                    /**
                    *
                    *   @brief Get the connection information that this
                    *   client is using to connect to the server.
                    *
                    *   @returns `networking::connected_host::server` that holds the 
                    *   connection information of the client to it's connected server.
                    *
                    */
                    connected_host::server get_connection_info() const;

            };


        }

    }


#endif