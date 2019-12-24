#include <iostream>
#include <fstream>

#include <unistd.h>

using std::cout;
using std::endl;
using std::ofstream;
using std::ios_base;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Программа должна принимать имя процесса, время задержки и имя выходного файла" << endl;
        return 0;
    }
    sleep(static_cast<unsigned int>(atoi(argv[2])));
    ofstream fout(argv[3], ios_base::app);

    fout << argv[1] << ": id процесса - " << getpid() << endl;
    fout << argv[1] << ": id предка - " << getppid() << endl;
    fout << argv[1] << ": id сессии процесса - " << getsid(getpid()) << endl;
    fout << argv[1] << ": id группа процессов - " << getpgid(getpid()) << endl;
    fout << argv[1] << ": реальный id пользователя - " << getuid() << endl;
    fout << argv[1] << ": эффективный идентификатор пользователя - " << geteuid() << endl;
    fout << argv[1] << ": реальный групповой идентификатор - " << getgid() << endl;
    fout << argv[1] << ": эффективный групповой идентификатор - " << getegid() << endl << endl;

    fout.close();
    return 0;
}
