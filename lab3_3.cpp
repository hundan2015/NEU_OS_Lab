#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <semaphore.h>
#include "sys/sem.h"
#include <iostream>
using namespace std;
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};

// static int sem_id = 0;

static int set_semvalue();
static void del_semvalue();
static int semaphore_p();
static int semaphore_v();

/*信号量初始化函数*/
int init_sem(int sem_id, int init_value)
{
    union semun sem_union;
    sem_union.val = init_value; // init_value为初始值
    if ((semctl(sem_id, 0, SETVAL, sem_union)) == -1)
    {
        perror("Initialize semaphore");
        return -1;
    }
    return 0;
}

/*P操作函数*/
int sem_p(int sem_id)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0; // 单个信号量的编号为0
    sem_b.sem_op = -1; // 表示P操作
    // sem_b.sem_flg = SEM_UNDO; // 系统自动释放将会在系统中残留的信号量
    sem_b.sem_flg = 0;
    if (semop(sem_id, &sem_b, 1) == -1)
    {
        perror("P operation");
        return -1;
    }
    return 0;
}
/*V操作函数*/
int sem_v(int sem_id)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0; // 单个信号量的编号为0
    sem_b.sem_op = 1;  // 表示V操作
    // sem_b.sem_flg = SEM_UNDO; // 系统自动释放将会在系统中残留的信号量
    sem_b.sem_flg = 0;
    if (semop(sem_id, &sem_b, 1) == -1)
    {
        perror("V operation");
        return -1;
    }
    return 0;
}
int main()
{
    auto fp = new int[2];
    int sem1, sem2, mutex;
    sem1 = semget((key_t)1234, 1, 0666 | IPC_CREAT);
    sem2 = semget((key_t)2345, 1, 0666 | IPC_CREAT);
    mutex = semget((key_t)1111, 1, 0666 | IPC_CREAT);
    init_sem(sem1, 0);
    init_sem(sem2, 0);
    init_sem(mutex, 1);
    cout << "main!\n";
    auto the_pipe = pipe(fp);
    auto f1 = fork();
    auto f2 = 0;
    char buffer[1000];
    if (f1 != 0)
    {
        f2 = fork();
    }
    if (f1 > 0 && f2 > 0)
    {
        // Main process.
        sem_p(sem1);
        cout << "Sem1 arrive.\n";
        sem_p(sem2);
        cout << "Sem2 arrive.\n";
        // lockf(fp[0], 1, 0);
        sem_p(mutex);
        read(fp[0], buffer, sizeof(buffer));
        sem_v(mutex);
        // lockf(fp[0], 0, 0);
        cout << buffer << endl;
        /* read(fp[0], buffer, sizeof(buffer));
        cout << buffer << endl; */
    }
    else
    {
        cout << "Process child." << getpid() << endl;
        string temp = "child message!" + to_string(getpid()) + " \n";
        char *message = temp.data();
        // cout << message;
        cout << getpid() << " need mutex\n";
        sem_p(mutex);
        write(fp[1], message, temp.size());
        sem_v(mutex);
        if (f2 == 0)
        {
            sem_v(sem2);
        }
        else
        {
            sem_v(sem1);
        }
        cout << getpid() << " end\n";
        exit(0);
    }
}