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

                //         "Host_name" : (std::string) (The hostname part of the URL)
                //         "Port" : (sock)[0, 65534]

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
                    this->client_info(name, port);
                    connection_socket = Network_Management::get_invalid_socket();
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

        };

    }


    class Server {

        
        private:
            // machine name (remote), machine port (remote)
            // IP addresses (IP4 and IP6)
            Network_Management::Host::Some_Machine this_machine_info;

            /*
                Data structure to hold the remote connections.
                Thinking dictionary with the following structure:
                {
                    "client_name" : {
                        "Connection_type" : (std::string) [keep-alive, close]
                        "Protocol" : (std::string) [http, https]
                        "Host_name" : (std::string) (The hostname part of the URL)
                        "Port" : (sock)[0, 65534]
                        "Path" : (std::string) [/*]
                        "QueryString" : (std::string) [?*]
                        "Hash" : (std::string) [#*]
                    }
                }

            */

    };


}