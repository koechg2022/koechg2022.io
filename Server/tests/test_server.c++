
#include "../libs/networking.h++"
#include "../libs/misc_functions.h++"


// const std::string UNDER_CONSTRUCTION = "UNDER CONSTRUCTION";



void list_machine_adapters();

void resolve_hostname();

void test_server();

void test_client();

void test_secure_server();

void test_secure_client();

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

        else if (string_functions::same_string(std::string(args[index]), "tcp_server_secure") or string_functions::same_string(std::string(args[index]), "tss")) {
            test_secure_server();
        }

        else if (string_functions::same_string(std::string(args[index]), "tcp_client_secure") or string_functions::same_string(std::string(args[index]), "tcs")) {
            test_secure_client();
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
    
    if (server.run()) {
        std::printf("Server is listening...\n");
        std::cout << "Connect to the server using:" << std::endl;
        std::cout << "\t" << server.host_name() << " : " << server.port_value() << std::endl;
        std::set<networking::network_structures::connected_host::client> clients;
        char msg[kilo_byte];
        ssize_t msg_len;
        std::string msg_string;
        std::memset(msg, 0, kilo_byte);

        while (server) {
            
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
                const std::string now = misc_functions::get_current_time();
                std::memset(msg, 0, kilo_byte);
                msg_len = recv(client->connected_socket, msg, kilo_byte, 0);
                if (msg_len > 0) {
                    msg_string = std::string(msg);
                    std::cout << client->hostname << "(" << now << ") : " << std::endl;
                    std::cout << "\"" << msg_string << "\"" << std::endl;
                    continue;
                }
                server.close_connection(client->connected_socket);
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
                    
                    msg_string = string_functions::get_input("Message to broadcast: ");
                    clients = server.connected_client_info();
                    ssize_t len;
                    for (auto client = clients.begin(); client NOT clients.end(); client++) {
                        len = send(client->connected_socket, msg_string.c_str(), msg_string.length(), 0);
                        if (len < 1) {
                            std::cerr << "Error sending message to client \"" << client->hostname << "\"" << std::endl;
                            server.close_connection(client->connected_socket);
                            continue;
                        }
                        if ((unsigned long) len == msg_string.length()) {
                            std::cout << "Successfully sent message to client \"" << client->hostname << "\"" << std::endl;
                        }
                        else {
                            std::cerr << "Failed to send complete message to client \"" << client->hostname << "\"" << std::endl << "Only sent " << len << " bytes of " << msg_string.length() << " bytes." << std::endl;
                        }
                    }
                }

                else if (string_functions::same_string(msg_string, "message_client()") or string_functions::same_string(msg_string, "msgc")) {
                    
                    networking::network_structures::connected_host::client the_client;
                    the_client.connected_socket = invalid_socket;
                    the_client.hostname = the_client.portvalue = "";
                    clients = server.connected_client_info();

                    for (auto client = clients.begin(); client NOT clients.end(); client++) {
                        std::cout << client->hostname << std::endl;
                        std::cout << "\t" << client->portvalue << std::endl;
                        std::cout << "\t" << client->connected_socket << std::endl;
                        std::cout << "--------------------------------------------------------" << std::endl;
                    }
                    msg_string = string_functions::get_input("Client : ");

                    // socket or port
                    if (string_functions::all_numbers(msg_string.c_str())) {
                        // socket or port
                        #if defined(unix_os)
                            socket_type the_socket = std::stoi(msg_string);
                        #else
                            socket_type the_socket = std::stoull(msg_string);
                        #endif

                        while (not valid_socket(the_client.connected_socket)) {
                            for (auto client = clients.begin(); client NOT clients.end(); client++) {
                                if (string_functions::same_string(msg_string, client->portvalue) or the_socket == client->connected_socket) {
                                    the_client = *client;
                                    break;
                                }
                            }

                            if (not valid_socket(the_client.connected_socket)) {
                                clients = server.connected_client_info();
                                for (auto client = clients.begin(); client NOT clients.end(); client++) {
                                    std::cout << client->hostname << std::endl;
                                    std::cout << "\t" << client->portvalue << std::endl;
                                    std::cout << "\t" << client->connected_socket << std::endl;
                                    std::cout << "--------------------------------------------------------" << std::endl;
                                }
                                msg_string = string_functions::get_input("Client : ");
                            }
                        }
                    }

                    // hostname
                    else if (networking::is_ipstring(msg_string, true) or 
                                networking::is_ipstring(msg_string, false)) {
                        while (not valid_socket(the_client.connected_socket)) {
                            
                            for (auto client = clients.begin(); client NOT clients.end(); client++) {
                                if (string_functions::same_string(msg_string, client->hostname)) {
                                    the_client = *client;
                                    break;
                                }
                            }

                            if (not valid_socket(the_client.connected_socket)) {
                                clients = server.connected_client_info();
                                for (auto client = clients.begin(); client NOT clients.end(); client++) {
                                    std::cout << client->hostname << std::endl;
                                    std::cout << "\t" << client->portvalue << std::endl;
                                    std::cout << "\t" << client->connected_socket << std::endl;
                                    std::cout << "--------------------------------------------------------" << std::endl;
                                }
                                msg_string = string_functions::get_input("Client : ");
                            }
                        }
                    }
                    
                    // Unrecotnized client passed in
                    else {
                        std::cerr << "Unrecognized client passed in " << msg_string << std::endl;
                        continue;
                    }
                    msg_string = string_functions::get_input("Message to send : ");
                    msg_len = send(the_client.connected_socket, msg_string.c_str(), msg_string.length(), 0);
                    if (msg_len < 1) {
                        std::cerr << "Error occurred while trying to send message." << std::endl;
                        server.close_connection(the_client.connected_socket);
                        continue;
                    }

                    std::cout << "Successfully sent " << msg_len << " bytes out of " << msg_string.length() << " bytes" << std::endl;
                }

                else {
                    std::cout << "Unrecognized command " << msg_string << std::endl << "Use these are the recognized commands:" << std::endl;
                    std::cout << "\texit()/exit to close the server" << std::endl;
                    std::cout << "\tlist_connected_machines()/lcm to list all the clients that are connected to the server" << std::endl;
                    std::cout << "\tbroadcast()/brdcst to send a message to all connected clients" << std::endl;
                    std::cout << "\tmessage_client()/msgc to message a specific client only" << std::endl;
                }

            }

        }
        std::printf("Server is disconnected...\n");
    }
}


void test_client() {

    networking::network_structures::tcp_client client(string_functions::get_input("Host to connect to : "));
    // std::cout << "Successfully created the tcp_client." << std::endl;
    
    if (client.start()) {
        char msg[kilo_byte];
        std::memset(msg, 0, kilo_byte);
        std::string message;
        ssize_t len, chunk;
        std::cout << "Client successfully connected" << std::endl;
        while (client) {
            
            if (client.server_has_message()) {
                len = recv(client.get_connection_socket(), msg, kilo_byte, 0);
                if (len less than 1) {
                    // Connection is closed
                    std::cout << "Connection closed by server..." << std::endl;
                    client.disconnect();
                    continue;
                }
                std::cout << "Message from server (" << misc_functions::get_current_time() << ") : " << std::endl << "\"" << std::string(msg) << "\"" << std::endl;
            }

            if (string_functions::has_keyboard_input()) {

                message = string_functions::get_input();

                if (string_functions::same_string(message, "exit()") or string_functions::same_string(message, "exit")) {
                    client.disconnect();
                }

                else if (string_functions::same_string(message, "send_message()") or string_functions::same_string(message, "smsg")) {
                    message = string_functions::get_input("Message to send: ");
                    chunk = send(client.get_connection_socket(), message.c_str(), message.length(), 0);

                    if (chunk greater than 0) {
                        if ((unsigned long) chunk == message.length()) {
                            std::printf("Successfully sent message...\n");
                        }
                        ((unsigned long) chunk < message.length()) ? std::printf("Message was not successfully sent. Only sent %zd bytes of %lu bytes\n", chunk, message.length()) : 0;
                        continue;
                    }
                    std::cout << "Client was disconnected by server." << std::endl;
                    client.disconnect();
                }

                else if (string_functions::same_string(message, "connection_info()") or string_functions::same_string(message, "ci")) {
                    networking::network_structures::connected_host::server server_info = client.get_connection_info();
                    std::cout << server_info.hostname << " : " << server_info.portvalue << std::endl;
                }

            }

        }
    }
    else {
        std::cout << "Client failed to connect" << std::endl;
    }

    client.disconnect();
    std::printf("Disconnected client\n");
}


void test_secure_server() {
    networking::network_structures::tcp_server server("", DEFAULT_PORT, 10, 0, 100000, true, true);
    std::cout << "Successfully created a secure tcp_server structure" << std::endl;
    std::cout << ((server.create_address()) ? "Successfully created the secure tcp_server's address" : "Failed to create the secure tcp_server's address") << std::endl;
    std::cout << ((server.create_socket()) ? "Successfully created the secure tcp_server's socket" : "Failed to create the secure tcp_server's socket") << std::endl;
    std::cout << ((server.bind_socket()) ? "Successfully bound the socket of the tcp_server's socket" : "Failed to bind the tcp_server's socket") << std::endl;
    std::cout << ((server.run()) ? "Successfully started listening on the tcp_server" : "Failed to start listening on the tcp_server") << std::endl;

    // server
    if (server.run()) {
        std::printf("Server is listening...\n");
        std::cout << "Connect to the server using:" << std::endl;
        std::cout << "\t" << server.host_name() << " : " << server.port_value() << std::endl;
        std::set<networking::network_structures::connected_host::client> clients;
        char msg[kilo_byte];
        int msg_len;
        std::string msg_string;
        std::memset(msg, 0, kilo_byte);
        while (server) {

            if (server.accept_new_connection()) {
                std::cout << "New connection accepted. Here are the new clients:" << std::endl;
                clients = server.connected_client_info();

                for (auto client = clients.begin(); client NOT clients.end(); client++) {
                    std::cout << "\t" << client->hostname << " : " << client->portvalue << std::endl;
                }
            }

            // // New connected have been accounted for
            // // Are there any new messages from the clients?
            clients = server.ready_client_info();
            for (auto client = clients.begin(); client NOT clients.end(); client++) {
                const std::string now = misc_functions::get_current_time();
                msg_len = SSL_read(client->secure_sockets_layer, msg, kilo_byte);
                
                if (msg_len <= 0) {
                    std::cerr << "The read operation was not successful, " << 
                    "because either the connection was closed, an error occurred " << 
                    "or action must be taken by the calling process." << 
                    ERR_error_string(SSL_get_error(client->secure_sockets_layer, msg_len), NULL) << std::endl;
                    server.close_connection(client->connected_socket);
                    continue;
                }

                std::cout << "(" << now << ") : " << std::endl;
                std::cout << "\"" << msg << "\"" << std::endl;
            }


            // // Control the server from the command line.
            if (string_functions::has_keyboard_input()) {
                msg_string = string_functions::get_input();

                if (string_functions::same_string(msg_string, "exit()") or string_functions::same_string(msg_string, "exit")) {
                    server.close_server();
                }

                else if (string_functions::same_string(msg_string, "list_connected_machines()") or string_functions::same_string(msg_string, "lcm")) {
                    clients = server.connected_client_info();
                    if (clients.empty()) {
                        std::cout << "No clients connected to this server..." << std::endl;
                        continue;
                    }
                    for (auto client = clients.begin(); client NOT clients.end(); client++) {
                        std::cout << client->hostname << " : " << std::endl;
                        std::cout << "\tConnection socket : " << client->connected_socket << std::endl;
                        std::cout << "\tSecure Socket Layer : " << client->secure_sockets_layer << std::endl;
                        std::cout << "\tPort : " << client->portvalue << std::endl;
                    }
                }

                else if (string_functions::same_string(msg_string, "broadcast()") or string_functions::same_string(msg_string, "brdcst")) {

                    msg_string = string_functions::get_input("Message to broadcast : ");
                    clients = server.connected_client_info();
                    for (auto client = clients.begin(); client NOT clients.end(); client++) {
                        msg_len = SSL_write(client->secure_sockets_layer, msg_string.c_str(), msg_string.length());
                        if (msg_len <= 0) {
                            std::cerr << "The write operation was not successful, " << 
                            "because either the connection was closed, an error occurred " << 
                            "or action must be taken by the calling process." << 
                            ERR_error_string(SSL_get_error(client->secure_sockets_layer, msg_len), NULL) << std::endl;
                            server.close_connection(client->connected_socket);
                            continue;
                        }
                        std::cout << "Successfully sent " << msg_len << " bytes out of " << msg_string.length() << " bytes" << std::endl;
                    }
                }

                else if (string_functions::same_string(msg_string, "message_client()") or string_functions::same_string(msg_string, "msgc")) {
                    
                    networking::network_structures::connected_host::client the_client;
                    the_client.connected_socket = invalid_socket;
                    the_client.hostname = the_client.portvalue = "";
                    clients = server.connected_client_info();

                    for (auto client = clients.begin(); client NOT clients.end(); client++) {
                        std::cout << client->hostname << std::endl;
                        std::cout << "\t" << client->portvalue << std::endl;
                        std::cout << "\t" << client->connected_socket << std::endl;
                        std::cout << "--------------------------------------------------------" << std::endl;
                    }
                    msg_string = string_functions::get_input("Client : ");

                    // socket or port
                    if (string_functions::all_numbers(msg_string.c_str())) {
                        // socket or port
                        #if defined(unix_os)
                            socket_type the_socket = std::stoi(msg_string);
                        #else
                            socket_type the_socket = std::stoull(msg_string);
                        #endif

                        while (not valid_socket(the_client.connected_socket)) {
                            for (auto client = clients.begin(); client NOT clients.end(); client++) {
                                if (string_functions::same_string(msg_string, client->portvalue) or the_socket == client->connected_socket) {
                                    the_client = *client;
                                    break;
                                }
                            }

                            if (not valid_socket(the_client.connected_socket)) {
                                clients = server.connected_client_info();
                                for (auto client = clients.begin(); client NOT clients.end(); client++) {
                                    std::cout << client->hostname << std::endl;
                                    std::cout << "\t" << client->portvalue << std::endl;
                                    std::cout << "\t" << client->connected_socket << std::endl;
                                    std::cout << "--------------------------------------------------------" << std::endl;
                                }
                                msg_string = string_functions::get_input("Client : ");
                            }
                        }
                    }

                    // hostname
                    else if (networking::is_ipstring(msg_string, true) or 
                                networking::is_ipstring(msg_string, false)) {
                        while (not valid_socket(the_client.connected_socket)) {
                            
                            for (auto client = clients.begin(); client NOT clients.end(); client++) {
                                if (string_functions::same_string(msg_string, client->hostname)) {
                                    the_client = *client;
                                    break;
                                }
                            }

                            if (not valid_socket(the_client.connected_socket)) {
                                clients = server.connected_client_info();
                                for (auto client = clients.begin(); client NOT clients.end(); client++) {
                                    std::cout << client->hostname << std::endl;
                                    std::cout << "\t" << client->portvalue << std::endl;
                                    std::cout << "\t" << client->connected_socket << std::endl;
                                    std::cout << "--------------------------------------------------------" << std::endl;
                                }
                                msg_string = string_functions::get_input("Client : ");
                            }
                        }
                    }
                    
                    // Unrecotnized client passed in
                    else {
                        std::cerr << "Unrecognized client passed in " << msg_string << std::endl;
                        continue;
                    }
                    msg_string = string_functions::get_input("Message to send : ");
                    msg_len = send(the_client.connected_socket, msg_string.c_str(), msg_string.length(), 0);
                    if (msg_len < 1) {
                        std::cerr << "Error occurred while trying to send message." << std::endl;
                        server.close_connection(the_client.connected_socket);
                        continue;
                    }

                    std::cout << "Successfully sent " << msg_len << " bytes out of " << msg_string.length() << " bytes" << std::endl;
                }

                else {
                    std::cout << "Unrecognized command " << msg_string << std::endl << "Use these are the recognized commands:" << std::endl;
                    std::cout << "\texit()/exit to close the server" << std::endl;
                    std::cout << "\tlist_connected_machines()/lcm to list all the clients that are connected to the server" << std::endl;
                    std::cout << "\tbroadcast()/brdcst to send a message to all connected clients" << std::endl;
                    std::cout << "\tmessage_client()/msgc to message a specific client only" << std::endl;
                }

            }


        }
    }


}


void test_secure_client() {
    networking::network_structures::tcp_client client(string_functions::get_input("Host to connect to : "), DEFAULT_PORT, 0, 100000, true, true);
    std::cout << "Successfully created a secure tcp_client structure" << std::endl;
    std::cout << ((client.create_address()) ? "Successfully created the secure tcp_client's address" : "Failed to create the secure tcp_client's address") << std::endl;
    std::cout << ((client.create_socket()) ? "Successfully created the secure tcp_client's socket" : "Failed to create the secure tcp_client's socket") << std::endl;
    std::cout << ((client.start()) ? "Successfully connected the secure tcp_client to the server" : "Failed to connect the secure tcp_client to the server") << std::endl;

    // client is connected
    while (client) {
        std::cout << "Client is connected" << std::endl;
        break;
    }

    std::cout << "Client disconnected" << std::endl;
}