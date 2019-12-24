#include <iostream>
#include <pthread.h>

void* thread_func(void *args) {
    int64_t fd = *((int64_t*)(&args));
    int sheduler = sched_getscheduler(0);
    pthread_t thread = pthread_self();
    sched_param schedParam {};
    pthread_getschedparam(thread, &sheduler, &schedParam);
    std::cout << (sheduler == 0 ? "SCHED_FIFO" : (sheduler == 1 ? "SCHED_RR" : "SCHED_OTHER")) << std::endl;
    std::cout << "Current thread priority: " << schedParam.sched_priority << std::endl;
    std::cout << "Min thread priority: " << sched_get_priority_min(sheduler) << std::endl;
    std::cout << "Max thread priority: " << sched_get_priority_max(sheduler) << std::endl;
    FILE* file = fdopen(fd, "r");
    char str[255];
    while (fgets(str, sizeof(str), file) != nullptr)
        std::cout << str;
    //fclose(file);
    pthread_exit(nullptr);
}

int main() {
    pthread_t thread;
    int64_t status_addr = 0;
    FILE *file = fopen("out.txt", "r");
    int64_t fd = fileno(file);
    if (pthread_create(&thread, nullptr, thread_func, (void*)fd))
        std::cout << "Ошибка. Не удалось создать поток" << std::endl;
    pthread_join(thread, nullptr);
    if (!fclose(file))
        std::cout << "Файл принудительно закрыт" << std::endl;
    return 0;
}
