#include <iostream>
#include <unistd.h>
#include <fstream>

int main(int argc, char *argv[]) {
    int fildes[2] = {atoi(argv[2]), atoi(argv[3])};
    std::ifstream fin;
    fin.open(argv[1]);
    std::string str;
    fin >> str;
    str += '\n';
    close(fildes[0]);
    for (int i = 0; i != str.length(); ++i) {
        write(fildes[1], &str[i], sizeof(str[i]));
    }
    close(fildes[1]);
    fin.close();
}
