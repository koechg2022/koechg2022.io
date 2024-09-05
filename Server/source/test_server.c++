#include "../../libraries/prototypes"
#include <unistd.h>
#include <vector>

#if defined(crap_os)
    #define has_keyboard_input(the_set) _kbhit()
#else
    #define has_keyboard_input(the_set) FD_ISSET(STDIN_FILENO, &the_set)
#endif


const std::vector<std::string> server_options = {
    "exit()",
    "list_func()",
    "list_connected_machines()",
    "broadcast()",
    "send_message()",
    "disconnect()"
};


const std::vector<std::string> server_options_descriptions {
    "close the program.",
    "list the server's usable functions",
    "see all the connected machines.",
    "broadcast a message to all clients.",
    "send a message to an individual client.",
    "disconnect a connected client."
};



void running_server(const char* host_name, const char* port = "8080");


int main(int len, char** args) {
    std::printf("Hello World\n");
    int index;
    for (index = 1; index < len; index = index + 1) {
        std::printf("\t%s\n", args[index]);
    }
    return 0;
}



void running_server(const char* host_name, const char* port) {
    networking::network_structures::tcp_server server((std::string(host_name)), (std::string(port)));
    server.keep_init(false);
    if (!server.retrieve_address_information()) {
        std::fprintf(stderr, "Failed to retrieve address information for %s\n", host_name);
        server.close_server();
        return;
    }

    if (!server.activate_socket()) {
        std::fprintf(stderr, "Failed to active the socket to be used for listening.\n");
        server.close_server();
        return;
    }

    if (!server.bind_socket()) {
        std::fprintf(stderr, "Failed to bind the listening socket.\n");
        server.close_server();
        return;
    }

    if (!server.start_listening()) {
        std::fprintf(stderr, "Failed to start listening on the active socket.\n");
        server.close_server();
        return;
    }
    std::map<socket_type, networking::network_structures::client> known_clients;
    timeval timeout = {0, 100000};
    std::string rec_msg;
    while (server.server_is_listening()) {
        
        // Check if keyboard has input
        fd_set for_keyboard;
        FD_ZERO(&for_keyboard);
        #if defined(unix_os)
            FD_SET(STDIN_FILENO, &for_keyboard);
            if (select(STDIN_FILENO + 1, &for_keyboard, 0, 0, &timeout) < 0) {
                std::fprintf(stderr, "An error occured while trying to retrieve keyboard input.\n");
                std::exit(EXIT_FAILURE);
            }
        #endif


        if (has_keyboard_input(for_keyboard)) {
            rec_msg = string_functions::get_keyboard_input();
            unsigned long remove_index = rec_msg.find("()");
            if (remove_index != std::string::npos) {
                rec_msg = rec_msg.substr(0, remove_index);
            }

        }

    }
}