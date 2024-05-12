#include "../../libraries/include.h"


/*
    Will create a Server object to handle client communication. Host_Machine object in namespace Network_Management::Host will
    be used to hold client machine information.
*/

namespace Server_Management {

    namespace {

        class Client {
            
            private:
                // machine name (remote), machine port (local)
                // IP addresses (IP4 and IP6)

                // {
                //     "client_name" : {
                //         "Connection_type" : (std::string) [keep-alive, close]
                //         "Protocol" : (std::string) [http, https]
                //-----------------------------------------------------------------------------
                //         "Host_name" : (std::string) (The hostname part of the URL)
                //         "Port" : (sock)[0, 65534]
                //-----------------------------------------------------------------------------
                //         "Path" : (std::string) [/*]
                //         "QueryString" : (std::string) [?*]
                //         "Hash" : (std::string) [#*]
                //     }
                // }


                Network_Management::Host::Some_Machine client_info; // should contains hostname and port
                std::string connection_type, protocol, path, query_string, hash;
                sock connection_socket, listening_socket;
                bool active_connect_sock, active_listen_sock;

            public:

                Client(std::string name, std::string port = "80") {
                    // this->client_info = Network_Management::Host::Some_Machine(name, port);
                    this->client_info.update_remote_address(name);
                    this->client_info.update_remote_port(port);
                    connection_socket = listening_socket = Network_Management::get_invalid_socket();

                }

                ~Client() {
                    if (this->active_connect_sock) {
                        close_socket(connection_socket);
                    }

                    if (this->active_listen_sock) {
                        close_socket(active_listen_sock);
                    }
                }

                void update_host_name(const std::string new_hostname) {
                    this->client_info.update_remote_address(new_hostname);
                }

                const std::string get_host_name() const {
                    return this->client_info.get_remote_address();
                }

                void update_host_port(const std::string new_port) {
                    this->client_info.update_remote_port(new_port);
                }

                const std::string get_host_port() const {
                    return this->client_info.get_remote_port();
                }

                const std::string get_connection_type() const {
                    return this->connection_type;
                }

                void update_connection_type(const std::string connect_type) {
                    this->connection_type = connect_type;
                }

                const std::string get_protocol() const {
                    return this->protocol;
                }

                void update_protocol(const std::string new_protocol) {
                    this->protocol = new_protocol;
                }

                const std::string get_path() const {
                    return this->path;
                }

                void update_path(const std::string new_path) {
                    this->path = new_path;
                }

                const std::string get_query_string() const {
                    return this->query_string;
                }

                void update_query_string(const std::string new_query_string) {
                    this->query_string = new_query_string;
                }

                const std::string get_hash() const {
                    return this->hash;
                }

                void update_hash(const std::string new_hash) {
                    this->hash = new_hash;
                }

                const sock get_connection_socket() const {
                    return this->connection_socket;
                }

                void update_connection_socket(const sock new_socket) {
                    this->connection_socket = new_socket;
                }

                const sock get_listening_socket() const {
                    return this->listening_socket;
                }

                void update_listening_socket(const sock new_socket) {
                    this->listening_socket = new_socket;
                }

        };

    }


    class Server {

        
        private:
            // machine name (remote), machine port (remote)
            // IP addresses (IP4 and IP6)
            Network_Management::Host::Some_Machine this_machine_info;
            std::map<std::string, Client> connected_machines;

            /*
                Data structure to hold the remote connections.
                Thinking dictionary with the following structure:
                {
                    "client_name" : {
                        "Connection_type" : (std::string) [keep-alive, close]
                        "Protocol" : (std::string) [http, https]
                        "Host_name" : (std::string) (The hostname part of the URL)
                        "Port" : (sock)[0, 65534]
                        "Path" : (std::string) [/ *]
                        "QueryString" : (std::string) [?*]
                        "Hash" : (std::string) [#*]
                    }
                }

            */

        public:

            Server() {
                this->this_machine_info = Network_Management::Host::Some_Machine();
            }

            const unsigned long connection_count() const {
                return connected_machines.size();
            }



    };


}