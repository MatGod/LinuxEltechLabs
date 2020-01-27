#include <errno.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <fcntl.h>

#include <iostream>

using namespace std;

struct message_t {
    long type;
    int new_timer_interval;
};

void timer_handler(int);
void create_timer(int timeout);

int messagequeue_id;

int main() {
    int timeout {3};

    cout << "Input timeout (seconds): ";
    cin >> timeout;

    static const key_t queue_key = 121;
    int queue_id = msgget(queue_key, 0662 | IPC_CREAT);
    messagequeue_id = queue_id;
    if (queue_id == -1) {
        cout << "Failed to create queue!" << endl;
        cout << "Error code: " << errno << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Queue Id: " << queue_id << endl;
    cout << "Waiting for message..." << endl;

    while (true) {
        create_timer(timeout);

        message_t received_message;
        long message_type = 42;

        int receiving_result = msgrcv(queue_id, &received_message,
                sizeof(message_t) - sizeof(message_t::type),
                message_type, 0);

        if (receiving_result == -1) {
            cout << "Error reding message. Error code: " << errno << endl;
            exit(EXIT_FAILURE);
        }

        cout << "Message received:" << endl;
        timeout = received_message.new_timer_interval;
        cout << "New interval: " << timeout << "s" << endl;
    }

    return 0;
}

void timer_handler(int) {
    cout << "Time up! Exiting..." << endl;
    int msqid = messagequeue_id;
    cout << "Queue to close: " << msqid << endl;
    int operation_result = msgctl(msqid, IPC_RMID, nullptr);
    if (operation_result == -1) {
        cout << "Error deleting queue. Error code: " << errno << endl; 
    }
    exit(EXIT_SUCCESS);
}

void create_timer(int timeout) {
    setitimer(ITIMER_REAL, nullptr, nullptr);
    itimerval value {
        {timeout, 0},
        {timeout, 0}
    };

    itimerval ovalue {};
    signal(SIGALRM, &timer_handler);
    setitimer(ITIMER_REAL, &value, &ovalue);
}