#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Ошибка. Программа должна принимать 3 аргумента" << std::endl;
        return 0;
    }
    int fildes[2];
    pipe(fildes);
    std::ofstream fout;
    fout.open(argv[3]);
    if(fork() == 0) {
        execl("/home/matgod/Study/LinuxEltechLabs/Lab_7/Lab7_1/cmake-build-debug/Lab7_1", "Lab7_1", argv[1],
                std::to_string(fildes[0]).c_str(), std::to_string(fildes[1]).c_str(), NULL);
        std::cout << "impossible to exec 1";
    } else {
        if (fork() == 0) {
            execl("/home/matgod/Study/LinuxEltechLabs/Lab_7/Lab7_1/cmake-build-debug/Lab7_1", "Lab7_1", argv[2],
                  std::to_string(fildes[0]).c_str(), std::to_string(fildes[1]).c_str(), NULL);
            std::cout << "impossible to exec 2";
        }
    }
    char ch;
    close(fildes[1]);
    while (read(fildes[0], &ch, sizeof(ch)) > 0) {
        std::cout << ch;
        fout << ch;
    }
    close(fildes[0]);
    int status;
    wait(&status);
    wait(&status);
    fout.close();
    return 0;
}