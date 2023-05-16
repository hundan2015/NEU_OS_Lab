#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct {
    long type;       // 消息类型，必须大于0
    char data[128];  // 消息内容
} MSG_T;

int main(int argc, char **argv) {
    key_t key;
    int msgid;
    pid_t pid;
    MSG_T s_msg, r_msg;

    key = ftok(".", 66);
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid < 0) {
        printf("msgget error\n");
        exit(EXIT_FAILURE);
    } else {
        printf("msgget success msgid:%d\n", msgid);
    }

    /* 通过ipcs命令查看创建的消息队列 */
    // system("ipcs -q");

    pid = fork();
    if (pid < 0) {
        printf("fork error\n");
        exit(EXIT_FAILURE);
    }

    /* 父进程，发送消息 */
    if (pid > 0) {
        s_msg.type = 1;
        memset(s_msg.data, 0, sizeof(s_msg.data));
        strncpy(s_msg.data, "msg queue data", strlen("msg queue data"));
        msgsnd(msgid, &s_msg, strlen(s_msg.data), 0);
        wait(NULL);
        msgctl(msgid, IPC_RMID, NULL);
        // system("ipcs -q");
        exit(EXIT_SUCCESS);
    }

    /* 子进程, 接收消息 */
    if (pid == 0) {
        msgrcv(msgid, &r_msg, sizeof(r_msg.data), 0, 0);
        printf("recv msg data type: %ld, data: %s\n", r_msg.type, r_msg.data);
        exit(EXIT_SUCCESS);
    }

    exit(EXIT_SUCCESS);
}