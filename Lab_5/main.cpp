#include <iostream>
#include <csignal>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        std::cout << "Не задан тип ошибки" << std::endl;
        return 0;
    }
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGFPE);
    sigaddset(&set, SIGSEGV);
    if (strcmp(argv[1], "SIGFPE") == 0) {
        signal(SIGFPE, [](int signum) {
            std::cout << "Ошибка. Деление на ноль" << std::endl;
            exit(signum);
        });
        int answer = 1/0;
        std::cout << "RESULT: " << answer << std::endl;
    } else if (strcmp(argv[1], "SIGSEGV") == 0) {
        signal(SIGSEGV, [](int signum) {
            std::cout << "Ошибка. Нарушение защиты памяти" << std::endl;
            exit(signum);
        });
        int* p = 0x00000000;
        *p = 10;
    } else
        std::cout << "Ошибка. Неверно введен тип ошибки" << std::endl;
    return 0;
}