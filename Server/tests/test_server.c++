#include "../libs/misc_functions.h++"
#include "../libs/networking.h++"




const std::string method = "GET";


std::map<std::string, std::string> parse_message(std::string message);


bool send_resources(networking::network_structures::connected_host::client client, std::map<std::string, std::string> headers);


void simple_server();



void simple_client();


void write16(std::ofstream& file, uint16_t value);


void write32(std::ofstream& file, uint32_t value);


void create_favicon_file(const std::string file_name = "../Client/files/favicon.ico");


int main(int len, char** args) {

    int index;
    for (index =  1; index < len; index++) {
        if (string_functions::same_string(args[index], "simple server") or string_functions::same_string(args[index], "ss")) {
            simple_server();
        }
        else if (string_functions::same_string(args[index], "simple client") or string_functions::same_string(args[index], "sc")) {
            simple_client();
        }
        else if (string_functions::same_string(args[index], "create_favicon()") or string_functions::same_string(args[index], "cf")) {
            create_favicon_file();
        }
        else {
            std::fprintf(stderr, "Unknown argument '%s'\n", args[index]);
        }
    }
}


std::map<std::string, std::string> parse_message(std::string message) {
    std::map<std::string, std::string> the_answer;
    
    std::stringstream ss;
    std::string line, key, value;
    
    size_t pos = 0;
    std::string delimiter = "\r\n";
    while ((pos = message.find(delimiter)) != std::string::npos) {
        ss << message.substr(0, pos) << std::endl;
        message.erase(0, pos + delimiter.length());
    }
    ss << message; // Add the last part
    
    while (std::getline(ss, line)) {
        // std::cout << line << std::endl;
        pos = line.find_first_of(":");
        if (pos == std::string::npos) {
            if (line.find("GET") != line.length()) {
                pos = line.find_first_of(" ");
                key = line.substr(0, pos);
                value = line.substr(pos);
                string_functions::strip(key, " ");
                string_functions::strip(value, " ");
            }
            else if (line.find("PULL") != line.length()) {
                pos = line.find_first_of(" ");
                key = line.substr(0, pos);
                value = line.substr(pos);
                string_functions::strip(key, " ");
                string_functions::strip(value, " ");
            }
            the_answer[key] = value;
            continue;
        }
        key = line.substr(0, pos);
        value = line.substr(pos + 1);
        string_functions::strip(key, " ");
        string_functions::strip(value, " ");
        
        the_answer[key] = value;
    }

    return the_answer;
}


bool send_resources(networking::network_structures::connected_host::client client, std::map<std::string, std::string> headers) {

    std::map<std::string, std::string> content_type = {
        {".css", "text/css"},
        {".csv", "text/csv"},
        {".gif", "image/gif"},
        {".htm", "text/html"},
        {".html", "text/html"},
        {".ico", "image/x-icon"},
        {".jpeg", "image/jpeg"},
        {".jpg", "image/jpeg"},
        {".js", "application/javascript"},
        {".json", "applicatoin/json"},
        {".png", "image/png"},
        {".pdf", "application/pdf"},
        {".svg", "image/svg+xml"},
        {".txt", "text/plain"}
    };

    
    std::string message, buffer, the_content_type;
    std::string path = headers[method].substr(0, headers[method].find_first_of("HTTP"));
    const std::string ending = "\r\n";
    uintmax_t file_length;
    int bytes;
    unsigned long index;
    
    string_functions::strip(path, " ");
    
    if (string_functions::same_string("/", path)) {
        path = "/html/homepage.html";
    }

    if (path.length() > 100) {
        message = 
                "HTTP/1.1 400 Bad Request\r\n"
                "Connection: close\r\n"
                "Content-Length: 11\r\n\r\nBad Request";
        send(client.connected_socket, message.c_str(), message.length(), 0);
        return false;
    }

    if (path.find("../") != path.length()) {
        message = 
                "HTTP/1.1 404 Not Found\r\n"
                "Connection: close\r\n"
                "Content-Length: 9\r\n\r\nNot Found";
        send(client.connected_socket, message.c_str(), message.length(), 0);
        return false;
    }

    #if defined(crap_os)
        string_functions::replace_all(path, "/", "\\");
    #endif

    string_functions::replace_all(path, "%20", " ");
    if (path[path.length() - 1] == 'J') {
        path = path + "PG";
    }

    path = "../Client" + path;
    std::printf("Path is : '%s'\n", path.c_str());
    if (not std::filesystem::exists(path)) {
        std::fprintf(stderr, "No file '%s' exists\n", path.c_str());
        return false;
    }
    file_length = std::filesystem::file_size(path);
    
    the_content_type = "";
    index = path.find_last_of(".");

    if (index > path.length()) {
        // There are no extensions
        return false;
    }
    
    the_content_type = content_type[path.substr(index)];
    std::printf("content_type is '%s'\n", the_content_type.c_str());

    buffer = "HTTP/1.1 200 OK" + ending;
    bytes = send(client.connected_socket, buffer.c_str(), buffer.length(), 0);

    buffer = "Connection: Keep-Alive" + ending;
    bytes = send(client.connected_socket, buffer.c_str(), buffer.length(), 0);

    buffer = "Content-Length: " + std::to_string(file_length) + ending;
    bytes = send(client.connected_socket, buffer.c_str(), buffer.length(), 0);

    buffer = "Content-Type: " + the_content_type + ending;
    bytes = send(client.connected_socket, buffer.c_str(), buffer.length(), 0);

    buffer = ending;
    bytes = send(client.connected_socket, buffer.c_str(), buffer.length(), 0);
    
    // read data from the file and send it
    std::fstream open_file(path, std::ios_base::in | std::ios_base::binary);
    if (not open_file.is_open()) {
        std::fprintf(stderr, "Failed to open '%s'\n", path.c_str());
        return false;
    }

    char read[4 * kilo_byte];
    while (open_file.read(read, sizeof(buffer)).gcount() > 0) {
        bytes = send(client.connected_socket, read, open_file.gcount(), 0);
        if (bytes <= 0) {
            open_file.close();
            return false;
        }
    }
    
    open_file.close();
    return true;
}


void simple_server() {

    networking::network_structures::http_server server(false, 10);
    server.run();


    // networking::network_structures::tcp_server server;

    // if (not server.start()) {
    //     std::fprintf(stderr, "Failed to start the server.\n");
    //     std::exit(EXIT_FAILURE);
    // }

    // std::set<networking::network_structures::connected_host::client> clients;
    // networking::network_structures::connected_host::client client;
    // std::string message;
    // char message_buffer[kilo_byte * 2];
    // ssize_t bytes;
    // std::map<std::string, std::string> header_message;
    // char send_count = 0;

    // std::printf("Connect to server using %s:%s\n", server.host_name().c_str(), server.port_value().c_str());


    // while (server) {

    //     if (server.new_connection()) {

    //         for (auto client = clients.begin(); client != clients.end(); client++) {
    //             std::printf("%s:\n", client->hostname.c_str());
    //             std::printf("\tPort: %s\n", client->portvalue.c_str());
    //             std::printf("\tConnection socket: %d\n", client->connected_socket);
    //         }
    //     }

    //     clients = server.get_clients();

    //     if (not clients.empty()) {

    //         for (auto client = clients.begin(); client != clients.end(); client++) {

    //             bytes = recv(client->connected_socket, message_buffer, kilo_byte * 2, 0);
    //             if (bytes < 1) {
    //                 std::printf("Closing connection after %d sends.\n", send_count);
    //                 std::printf("Connection closed by peer.\n");
    //                 server.close_connection(client->connected_socket);
    //                 send_count = 0;
    //                 continue;
    //             }

    //             std::printf("Message from %s:\n\n%.*s", client->hostname.c_str(), (int) bytes, message_buffer);
    //             header_message = parse_message(std::string(message_buffer));
                
    //             if (not send_resources(*client, header_message)) {
    //                 std::printf("Closing connection after %d sends.\n", send_count);
    //                 server.close_connection(client->connected_socket);
    //                 send_count = 0;
    //                 continue;
    //             }
    //             send_count = send_count + 1;

    //         }

    //     }

    //     if (string_functions::has_keyboard_input()) {

    //         message = string_functions::get_input();

    //         if (string_functions::same_string(message, "exit()") or string_functions::same_string(message, "exit")) {
    //             server.close_server();
    //         }

    //     }

    // }
}


void simple_client() {
    std::printf("UNDER CONSTRUCTION\n");
}


void write16(std::ofstream& file, uint16_t value) {
    file.put(value & 0xFF);
    file.put((value >> 8) & 0xFF);
}


void write32(std::ofstream& file, uint32_t value) {
    file.put(value & 0xFF);
    file.put((value >> 8) & 0xFF);
    file.put((value >> 16) & 0xFF);
    file.put((value >> 24) & 0xFF);
}



void create_favicon_file(const std::string file_name) {
    std::ofstream file(file_name, std::ios::binary);

    // ICO file header
    write16(file, 0);  // Reserved. Must always be 0.
    write16(file, 1);  // Specifies image type: 1 for icon (.ICO) image
    write16(file, 1);  // Specifies number of images in the file

    // Image entry
    file.put(16);  // Width, 0 means 256
    file.put(16);  // Height, 0 means 256
    file.put(0);   // Color palette, 0 means no palette
    file.put(0);   // Reserved. Should be 0.
    write16(file, 1);  // Color planes
    write16(file, 32); // Bits per pixel
    write32(file, 40 + 16*16*4); // Size of image data
    write32(file, 22); // Offset of image data from the beginning of the file

    // DIB header
    write32(file, 40);  // DIB header size
    write32(file, 16);  // Width
    write32(file, 32);  // Height (2 * actual height for icons)
    write16(file, 1);   // Color planes
    write16(file, 32);  // Bits per pixel
    write32(file, 0);   // Compression (0 = uncompressed)
    write32(file, 16*16*4); // Image size
    write32(file, 0);   // X pixels per meter
    write32(file, 0);   // Y pixels per meter
    write32(file, 0);   // Total colors
    write32(file, 0);   // Important colors

    // Image data (16x16 pixels, 32 bits per pixel, blue color)
    for (int i = 0; i < 16*16; ++i) {
        file.put(static_cast<char>(255));  // Blue
        file.put(0);    // Green
        file.put(0);    // Red
        file.put(static_cast<char>(255));  // Alpha
    }

    file.close();
}




