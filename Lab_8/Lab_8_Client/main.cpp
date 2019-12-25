#include <iostream>
#include <sys/msg.h>
#include <unistd.h>

using namespace std;

void stop();

int main(int argc, char* argv[]) {
    int id_server = msgget(1, IPC_EXCL);
    if (id_server == -1)
        stop();
    id_server = msgget(1, IPC_CREAT);
    if (id_server == -1) {
        msgctl(id_server, IPC_RMID, nullptr);
        stop();
    }
    int timeout = atoi(argv[1]);
    msgsnd(id_server, &timeout, sizeof(timeout), 0);
    cout << "Сообщение отправлено. timeout: " << timeout << " sec" << endl;
    return 0;
}

void stop() {
    cout << "Ошибка. Не удалось установить связь с сервером" << endl;
    _exit(0);
}