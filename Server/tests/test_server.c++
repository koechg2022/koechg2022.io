#include "../libs/networking.h++"



int main(int len, char** args) {

    int index;
    std::printf("There are %d arguments:\n", len);
    for (index = 1; index < len; index++) {
        std::printf("\t%d.) %s\n", index, *args + index);
    }

    return 0;
}