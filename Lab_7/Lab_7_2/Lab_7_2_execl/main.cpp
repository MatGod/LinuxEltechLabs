#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>
#include <cstring>


std::ofstream fout;
int fildes[2];
int child1;

int main(int argc, char *argv[]) {
    fildes[0] = atoi(argv[1]);
    fildes[1] = atoi(argv[2]);
    close(fildes[1]);
    child1 = strcmp("1", argv[0]);
    signal(child1 ? SIGUSR2 : SIGUSR1, [](int signum) {
        char ch;
        int status = read(fildes[0], &ch, sizeof(ch));
        if (status == -1) {
            kill(-getppid(), SIGTERM);
        }
        else {
            std::cout << "CHILD 1: " << ch << std::endl;
            fout << ch;
        }
        kill(-getppid(), child1 ? SIGUSR1 : SIGUSR2);
    });
    signal(child1 ? SIGUSR1 : SIGUSR2, SIG_IGN);
    signal(SIGTERM, [](int signum) {
        close(fildes[0]);
        fout.close();
        exit(0);
    });
    fout.open(argv[3]);
    kill(getppid(), child1 ? SIGUSR2 : SIGUSR1);
    while(true) pause();
}