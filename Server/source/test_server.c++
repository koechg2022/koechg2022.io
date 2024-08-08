#include "../../libraries/prototypes"



int main(int len, char** args) {
    std::printf("Hello World\n");
    int index;
    for (index = 1; index < len; index = index + 1) {
        std::printf("\t%s\n", args[index]);
    }
    return 0;
}