#include <error.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#include <iostream>
using namespace std;
int main() {
    auto fp = new int[2];
    sem_t sem1, sem2, mutex;
    cout << sem_init(&sem1, 1, 0);
    cout << sem_init(&sem2, 1, 0);
    cout << sem_init(&mutex, 1, 1);
    cout << "main!\n";
    auto the_pipe = pipe(fp);
    auto f1 = fork();
    auto f2 = 0;
    char buffer[1000];
    if (f1 != 0) {
        f2 = fork();
    }
    cout << f1 + f2 << endl;
    if (f1 > 0 && f2 > 0) {
        // Main process.
        // wait(nullptr);
        // wait(nullptr);
        // sem_wait(&sem1);
        cout << "Sem1 arrive.\n";
        // sem_wait(&sem2);
        cout << "Sem2 arrive.\n";
        // lockf(fp[0], 1, 0);
        read(fp[0], buffer, sizeof(buffer));
        // lockf(fp[0], 0, 0);
        cout << buffer << endl;
        /* read(fp[0], buffer, sizeof(buffer));
        cout << buffer << endl; */
    } else {
        cout << "Process child.\n";
        // lockf(fp[1], 1, 0);
        // sem_wait(&mutex);
        string temp = "child message!" + to_string(getpid()) + " \n";
        char *message = temp.data();
        // cout << message;
        write(fp[1], message, temp.size());
        // sem_post(&mutex);
        //  lockf(fp[1], 0, 0);
        if (f2 == 0) {
            // sem_post(&sem2);
        } else {
            // sem_post(&sem1);
        }
        exit(0);
    }
}
