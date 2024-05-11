
#include "headers/include.h"

/*
    To compile:
        * Unix : clang++ this_file -o object_file -std=c++20
        * Crap OS : clang++ this_file -o object_file -liphlpapi -lws2_32 -std=c++20
*/

int main(int len, char** args) {

    std::map<std::string, std::map<std::string, std::vector<std::string> > > adapters = Network_Management::Host::get_network_adapters();

    for (std::map<std::string, std::map<std::string, std::vector<std::string> > >::const_iterator pair = adapters.begin(); pair != adapters.end(); pair++) {
        
        // pair->first is the adapter's name
        // pair->second is the map of the ip version to each of it's corresponding IP addresses.

        std::printf("Adapter %s Information:\n", pair->first.c_str());
        for (std::map<std::string, std::vector<std::string> >::const_iterator ip_pair = pair->second.begin(); ip_pair != pair->second.end(); ip_pair++) {
            /*
                ip_pair is of structure
                ip_pair->first : std::string
                ip_pair->second : std::vector<std::string>
            */
            std::printf("\t%s addresse%s:\n", ip_pair->first.c_str(), (ip_pair->second.size() > 1) ? "ses" : "s");
            for (unsigned long index = 0; index < ip_pair->second.size(); index = index + 1) {
                std::printf("\t\t%s\n", ip_pair->second[index].c_str());
            }
        }

    }

    if (len > 1) {
        std::printf("-------------------------------------------------------------------------------------------------------------------------\n");
        std::map<std::string, std::vector<std::string> > http_ip_info = Network_Management::Host::get_url_ip(std::string(args[1]));
        std::printf("IP address information for \"%s\":\n", args[1]);

        for (std::map<std::string, std::vector<std::string> >::const_iterator http_url = http_ip_info.begin(); http_url != http_ip_info.end(); http_url++) {

            // http_url->first is the IP Version
            std::printf("\t%s:\n", http_url->first.c_str());
            for (std::vector<std::string>::const_iterator ip_address = http_url->second.begin(); ip_address != http_url->second.end(); ip_address++) {
                std::printf("\t\t%s\n", ip_address->c_str());
            }

        }

    }

    return 0;
}