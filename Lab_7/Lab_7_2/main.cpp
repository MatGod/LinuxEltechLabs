#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <fcntl.h>
#include <fstream>

using namespace std;

pid_t pid_child1, pid_child2;
int fildes[2];
bool ready[2] = {false, false};

int main(int argc, char *argv[]) {
    pipe(fildes);
    fcntl(fildes[0], F_SETFL, O_NONBLOCK);
    signal(SIGUSR1, [](int signum) {
        ready[0] = true;
        signal(SIGUSR1, SIG_IGN);
    });
    signal(SIGUSR2, [](int signum) {
        ready[1] = true;
        signal(SIGUSR2, SIG_IGN);
    });
    signal(SIGTERM, SIG_IGN);
    pid_child1 = fork();
    if (!pid_child1) {
        execl("/home/matgod/Study/LinuxEltechLabs/Lab_7/Lab_7_2/Lab_7_2_execl/cmake-build-debug/Lab_7_2_execl", "1",
                to_string(fildes[0]).c_str(), to_string(fildes[1]).c_str(), argv[2], NULL);
    }
    pid_child2 = fork();
    if (!pid_child2) {
        execl("/home/matgod/Study/LinuxEltechLabs/Lab_7/Lab_7_2/Lab_7_2_execl/cmake-build-debug/Lab_7_2_execl", "2",
                to_string(fildes[0]).c_str(), to_string(fildes[1]).c_str(), argv[3], NULL);
    }
    ifstream fin;
    fin.open(argv[1]);
    string str;
    while (fin >> str) {
        write(fildes[1], str.c_str(), str.length() * sizeof(char));
    }
    fin.close();
    while(!(ready[0] && ready[1])) pause();
    kill(pid_child1, SIGUSR1);
    int status;
    wait(&status);
    wait(&status);
    close(fildes[0]);
    close(fildes[1]);
    return 0;
}