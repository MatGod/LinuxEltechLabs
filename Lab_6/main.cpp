#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <sys/time.h>

void output(bool);

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Ошибка. Программа должна принимать два аргумента" << std::endl;
        return 1;
    }

    int iterations = atoi(argv[1]);
    int interval = atoi(argv[2]);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGALRM, [](int signum) {
        pid_t childPid = fork();
        if(!childPid){
            output(false);
        } else {
            waitpid(childPid, NULL, 0);
            output(true);
        }
    });
    std::cout << "Iterations: " << iterations << " | Interval: " << interval << std::endl;
    itimerval timer;
    timer.it_value.tv_sec = interval;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = interval;
    timer.it_interval.tv_usec = 0;
    setitimer (ITIMER_REAL, &timer, NULL);
    for (int i = 0; i != iterations; ++i) {
        pause();
    }
}

void output(bool isParent) {
    char buffer[255];
    time_t seconds = time(nullptr);
    tm* timeinfo = localtime(&seconds);
    char* format = const_cast<char *>("%d.%B.%Y %H:%M:%S");
    strftime(buffer, sizeof(buffer), format, timeinfo);
    std::cout << (isParent ? "Parent." : "Child.") <<" PID: " << getpid() << " TIME: " << buffer << std::endl;
    if (!isParent)
        _exit(0);
}