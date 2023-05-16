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
    auto fp = new int[2];
    auto the_pipe = pipe(fp);
    // 创建一个子线程
    auto f1 = fork();
    auto f2 = 0;
    char buffer[1000];
    /**
     * 如果f1不为0则证明其为主线程。
     */
    if (f1 != 0) {
        f2 = fork();
    }
    cout << f1 + f2 << endl;
    if (f1 > 0 && f2 > 0) {
        // Main process.
        // 等待两个线程的内容
        wait(nullptr);
        wait(nullptr);
        // 读取管道里的内容。
        read(fp[0], buffer, sizeof(buffer));
        cout << buffer << endl;
        /* read(fp[0], buffer, sizeof(buffer));
        cout << buffer << endl; */
    } else {
        lockf(fp[1], 1, 0);
        string temp = "child message!" + to_string(getpid()) + " \n";
        char *message = temp.data();
        // cout << message;
        write(fp[1], message, temp.size());
        lockf(fp[1], 0, 0);
        exit(0);
    }
}
