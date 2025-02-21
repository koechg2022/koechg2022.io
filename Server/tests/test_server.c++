
#include "../libs/networking.h++"
#include "../libs/misc_functions.h++"


const std::string UNDER_CONSTRUCTION = "UNDER CONSTRUCTION";



void list_machine_adapters();

void resolve_hostname();

void test_server();

void test_client();

int main(int len, char** args) {

    int index;
    std::printf("There are %d arguments:\n", len - 1);
    for (index = 1; index < len; index++) {
        std::printf("\t%d.) %s\n", index, args[index]);
        if (string_functions::same_string(std::string(args[index]), "list_adapters") or string_functions::same_string(std::string(args[index]), "la")) {
            list_machine_adapters();
        }

        else if (string_functions::same_string(std::string(args[index]), "resolve_host") or string_functions::same_string(std::string(args[index]), "rh")) {
            resolve_hostname();
        }

        else if (string_functions::same_string(std::string(args[index]), "tcp_server") or string_functions::same_string(std::string(args[index]), "ts")) {
            test_server();
        }

        else if (string_functions::same_string(std::string(args[index]), "tcp_client") or string_functions::same_string(std::string(args[index]), "tc")) {
            test_client();
        }
    }

    return 0;
}



void list_machine_adapters() {
    std::map<std::string, std::map<std::string, std::vector<std::string> > > 
    adapters = networking::this_machine_adapters();

    for (auto this_adapter = adapters.begin(); this_adapter NOT adapters.end(); this_adapter++) {
        std::printf("%s:\n", this_adapter->first.c_str());
        for (auto this_family = this_adapter->second.begin(); this_family NOT this_adapter->second.end(); this_family++) {
            std::printf("\t\"%s\":\t", this_family->first.c_str());
            const unsigned long len = this_family->second.size();
            unsigned long index = 0;
            for (auto this_address = this_family->second.begin(); this_address NOT this_family->second.end(); this_address++) {
                std::printf("%s%s", this_address->c_str(), (index + 1 is len) ? "\n" : ", ");
                index++;
            }
        }
    }
}

void resolve_hostname() {
    std::string host;
    std::cout << "Hostname : ";
    std::cin >> host;
    std::cout << std::endl;
    std::vector<std::string> addresses = networking::resolve_hostname(host);
    std::cout << host << ":" << std::endl;
    unsigned long index;
    for (index = 0; index < addresses.size(); index++) {
        std::cout << "\t" << addresses[index] << std::endl;
    }
}

void test_server() {

    networking::network_structures::tcp_server server;
    std::printf("Successfully creates the tcp_server.\n");
    
    if (server.start_listening()) {
        std::printf("Server is listening...\n");
        std::cout << "Connect to the server using:" << std::endl;
        std::cout << "\t" << server.host_name() << " : " << server.port_value() << std::endl;
        std::set<networking::network_structures::connected_host::client> clients;
        char msg[kilo_byte];
        ssize_t msg_len, len;
        std::string msg_string;
        std::memset(msg, 0, kilo_byte);

        while (server.server_is_listening()) {
            
            if (server.accept_new_connection()) {
                std::cout << "New connection accepted. Here are the new clients:" << std::endl;
                clients = server.connected_client_info();

                for (auto client = clients.begin(); client NOT clients.end(); client++) {
                    std::cout << "\t" << client->hostname << " : " << client->portvalue << std::endl;
                }
            }

            // New connected have been accounted for
            // Are there any new messages from the clients?
            clients = server.ready_client_info();
            for (auto client = clients.begin(); client NOT clients.end(); client++) {
                msg_len = 0;
                msg_string = "";
                while ((len = recv(client->connected_socket, msg, kilo_byte - msg_len, 0)) > 0) {
                    if (msg_len is kilo_byte) {
                        msg_string = std::string(msg);
                        std::memset(msg, 0, kilo_byte);
                    }
                }
                if (msg_len < 1) {
                    std::cout << "Disconnected \"" << client->hostname << "\"" << std::endl;
                    server.close_connection(client->connected_socket);
                    continue;
                }
                msg_string = msg_string + std::string(msg);
                std::cout << client->hostname << "(" << misc_functions::get_current_time() << ") : " << std::endl;
                std::cout << "\"" << msg_string << "\"" << std::endl;
            }

            // Control the server from the command line.
            if (string_functions::has_keyboard_input()) {
                msg_string = string_functions::get_input();
                
                
                if (string_functions::same_string(msg_string, "exit()") or string_functions::same_string(msg_string, "exit")) {
                    server.close_server();
                }


                else if (string_functions::same_string(msg_string, "list_connected_machines()") or string_functions::same_string(msg_string, "lcm")) {
                    clients = server.connected_client_info();
                    if (clients.empty()) {
                        std::cout << "No clients connected to this server..." << std::endl;
                    }
                    else {
                        for (auto client = clients.begin(); client NOT clients.end(); client++) {
                            std::cout << client->hostname << ":" << std::endl;
                            std::cout << "\tConnection socket : " << client->connected_socket << std::endl;
                            std::cout << "\tPort : " << client->portvalue << std::endl;
                        }
                    }
                }

                else if (string_functions::same_string(msg_string, "broadcast()") or string_functions::same_string(msg_string, "brdcst")) {
                    std::cout << UNDER_CONSTRUCTION << std::endl;
                }

            }

        }
        std::printf("Server is disconnected...\n");
    }
}



void test_client() {

    networking::network_structures::tcp_client client(string_functions::get_input("Host to connect to : "));
    std::cout << "Successfully created the tcp_client." << std::endl;
    
    if (client.connect_client()) {
        std::cout << "Client successfully connected" << std::endl;
    }
    else {
        std::cout << "Client failed to connect" << std::endl;
    }

    client.disconnect_client();
    std::printf("Disconnected client\n");
}