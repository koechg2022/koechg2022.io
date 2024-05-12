
#include "headers/include.h"

/*
    To compile:
        * Unix : clang++ this_file -o object_file -std=c++20
        * Crap OS : clang++ this_file -o object_file -liphlpapi -lws2_32 -std=c++20
*/

int main(int len, char** args) {
    
    if (len > 1) {
        // Look at the remote machine's IP addresses.
        Network_Management::Host::Some_Machine this_machine(std::string(args[1]), "80");
        std::map<std::string, std::vector<std::string> > remote_addresses = this_machine.get_remote_machine_ips();
        std::printf("%s's IP Address information:\n\n", this_machine.get_remote_address().c_str());
        for (std::map<std::string, std::vector<std::string> >::const_iterator ip_addresses = remote_addresses.begin(); ip_addresses != remote_addresses.end(); ip_addresses++) {
            std::printf("\t%s\n", ip_addresses->first.c_str());
            for (std::vector<std::string>::const_iterator this_ip = ip_addresses->second.begin(); this_ip != ip_addresses->second.end(); this_ip++) {
                std::printf("\t\t%s\n", this_ip->c_str());
            }
        }
    }
    return 0;
}