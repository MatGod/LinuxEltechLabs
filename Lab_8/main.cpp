#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <csignal>
#include <sys/time.h>
#include <unistd.h>

int id_server;

void endServer(int);

int main(int argc, char* argv[]) {
    id_server = msgget(1, 0600 | IPC_CREAT);
    if (id_server == -1) {
        std::cout << "Ошибка. Не удалось создать очередь" << std::endl;
        return 1;
    }
    signal(SIGINT, endServer);
    signal(SIGALRM, [](int signo) {
        std::cout << "Программа завершена по таймеру" << std::endl;
        msgctl(id_server, IPC_RMID, nullptr);
        _exit(0);
    });
    struct itimerval tval{};
    tval.it_interval.tv_sec = atoi(argv[1]);
    tval.it_interval.tv_usec = 0;
    tval.it_value.tv_sec = atoi(argv[1]);
    tval.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &tval, nullptr);
    int interval;
    while(1) {
        int status = msgrcv(id_server, &interval, sizeof(interval), 0, IPC_NOWAIT);
        if (status > 0) {
            tval.it_interval.tv_sec = interval;
            tval.it_interval.tv_usec = 0;
            tval.it_value.tv_sec = interval;
            tval.it_value.tv_usec = 0;
            setitimer(ITIMER_REAL, &tval, nullptr);
            std::cout << "Получено сообщение. Таймаут: " << interval << " sec" << std::endl;
        }
    }
}

void endServer(int signo) {
    msgctl(id_server, IPC_RMID, nullptr);
    _exit(0);
}