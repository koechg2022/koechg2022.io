#include "../libs/misc_functions.h++"
#include "../libs/networking.h++"




void server();


void client();


int main(int len, char** args) {

    int index;
    for (index =  1; index < len; index++) {
        if (string_functions::same_string(args[index], "server") or string_functions::same_string(args[index], "rs")) {
            server();
        }
        else if (string_functions::same_string(args[index], "client") or string_functions::same_string(args[index], "rc")) {
            client();
        }
        else {
            std::fprintf(stderr, "Unknown argument '%s'\n", args[index]);
        }
    }
}


void server() {
    networking::network_structures::http_server server(false, 10);
    server.run();
}


void client() {
    std::printf("UNDER CONSTRUCTION\n");
}