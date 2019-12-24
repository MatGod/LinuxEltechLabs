#include <fstream>
#include <string>

void* threadFunc(void*);
void cleanFile(const std::string&);

struct thread_args {
    std::string str;
    std::string file_addr;
};

int main() {
    cleanFile("output1.txt");
    cleanFile("output2.txt");
    std::ifstream fin;
    fin.open("input.txt");
    thread_args args_1, args_2;
    args_1.file_addr = "output1.txt";
    args_2.file_addr = "output2.txt";
    while (fin >> args_1.str) {
        pthread_t thread_1, thread_2;
        pthread_create(&thread_1, nullptr, threadFunc, (void*)&args_1);
        if (!(fin >> args_2.str)) {
            pthread_join(thread_1, nullptr);
            break;
        }
        pthread_create(&thread_2, nullptr, threadFunc, (void*)&args_2);
        pthread_join(thread_1, nullptr);
        pthread_join(thread_2, nullptr);
    }
    fin.close();
    return 0;
}

void cleanFile(const std::string& addr) {
    std::ofstream fout;
    fout.open(addr, std::ofstream::out | std::ofstream::trunc);
    fout.close();
}

void* threadFunc(void* args) {
    thread_args arg = *(thread_args*)args;
    std::ofstream fout;
    fout.open(arg.file_addr, std::ios_base::in | std::ios_base::app);
    fout << arg.str << std::endl;
    fout.close();
    pthread_exit(0);
}