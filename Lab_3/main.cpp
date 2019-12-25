#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>

void printAttributes(const char *, const char *);

int main(int argc, char* argv[]) {
    std::ofstream fout (argv[4], std::ios_base::out);
    fout.close();
    if (fork() == 0) {
        sleep(static_cast<unsigned int>(atoi(argv[2])));
        printAttributes("ForkChild", argv[4]);
        exit(0);
    } else {
        if (vfork() == 0) {
            execl("/home/matgod/Study/LinuxEltechLabs/Lab_3/Lab_3_vfork/cmake-build-debug/Lab_3_vfork", "Lab_3_vfork",
                  "VForkChild", argv[3], argv[4], NULL);
        } else {
            sleep(static_cast<unsigned int>(atoi(argv[1])));
            printAttributes("PARENT", argv[4]);
        }
    }
    return 0;
}

void printAttributes(const char *name, const char* fname) {
    std::ofstream fout (fname, std::ios_base::app);
    fout << name << ": id процесса - " << getpid() << std::endl;
    fout << name << ": id предка - " << getppid() << std::endl;
    fout << name << ": id сессии процесса - " << getsid(getpid()) << std::endl;
    fout << name << ": id группа процессов - " << getpgid(getpid()) << std::endl;
    fout << name << ": реальный id пользователя - " << getuid() << std::endl;
    fout << name << ": эффективный идентификатор пользователя - " << geteuid() << std::endl;
    fout << name << ": реальный групповой идентификатор - " << getgid() << std::endl;
    fout << name << ": эффективный групповой идентификатор - " << getegid() << std::endl
        << std::endl;
    fout.close();
}