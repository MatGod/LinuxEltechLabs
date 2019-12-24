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
ofstream fout1, fout2;

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
        cout << "Сhild 1 started" << endl;
        close(fildes[1]);
        signal(SIGUSR1, [](int signum) {
            char ch;
            int status = read(fildes[0], &ch, sizeof(ch));
            if (status == -1) {
                kill(-getppid(), SIGTERM);
            }
            else {
                cout << "CHILD 1: " << ch << endl;
                fout1 << ch;
            }
            kill(-getppid(), SIGUSR2);
        });
        signal(SIGTERM, [](int signum) {
            close(fildes[0]);
            fout1.close();
            exit(0);
        });
        fout1.open(argv[2]);
        kill(getppid(), SIGUSR1);
        while(true) pause();
    }
    pid_child2 = fork();
    if (!pid_child2) {
        cout << "Child 2 started" << endl;
        close(fildes[1]);
        signal(SIGUSR2, [](int signum) {
            char ch;
            int status = read(fildes[0], &ch, sizeof(ch));
            if (status == -1)
                kill(-getppid(), SIGTERM);
            if (ch != NULL) {
                cout << "CHILD 2: " << ch << endl;
                fout2 << ch;
            }
            kill(-getppid(), SIGUSR1);
        });
        signal(SIGTERM, [](int signum) {
            close(fildes[0]);
            fout2.close();
            exit(0);
        });
        fout2.open(argv[3]);
        kill(getppid(), SIGUSR2);
        while(true) pause();
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