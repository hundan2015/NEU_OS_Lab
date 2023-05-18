#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#include <iostream>
using namespace std;
int main() {
    int pid;
    string str = "shit";
    cout << "hello!" << endl;
    for (int i = 0; i < 2; ++i) {
        pid = fork();
        if (pid == 0) {
            printf("%d child %d %d %d \n", i, getppid(), getpid(), pid);
            str += to_string(getpid());
            cout << str << endl;
        } else {
            printf("%d parent %d %d %d \n", i, getppid(), getpid(), pid);
            str += to_string(getpid());
            cout << str << endl;
        }
    }
}