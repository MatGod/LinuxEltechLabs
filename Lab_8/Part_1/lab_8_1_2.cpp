#include <iostream>

#include <sys/msg.h>
#include <errno.h>

using namespace std;

struct message_t {
    long type;
    int new_timer_interval;
};

int main() {
    static const key_t queue_key = 121;
    int queue_id = msgget(queue_key, 0662);

    if (queue_id == -1) {
        cout << "Queue does not exist yet! Exiting..." << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Queue Id: " << queue_id << endl;

    int new_timeout {};
    cout << "Input NEW timeout (seconds): ";
    cin >> new_timeout;

    long message_type = 42;
    message_t message {message_type, new_timeout};
    int operation_result = msgsnd(queue_id, &message,
            sizeof(message_t) - sizeof(message_t::type),
            0);

    if (operation_result == -1) {
        cout << "Error. Message not sent" << endl;
        cout << "Errno: " << errno << endl;
    } else {
        cout << "Message sent!" << endl;
    }

    return 0;
}