#include <iostream>
#include <list>
#include <queue>
#include <set>
using namespace std;
/**
 * 位于不同状态的表。
 */
list<char> ready;
list<char> ready_out;
list<char> create;
list<char> run;
list<char> jam;
list<char> jam_out;
list<char> end_state;

const int OUT_NUM = 2;
/**
 * 用于展示不同状态的函数。打印不同状态中所拥有的进程名。
 */
void Display() {
    cout << "create:";
    for (auto &i : create) {
        cout << i << " ";
    }
    cout << endl;
    cout << "ready:";
    for (auto &i : ready) {
        cout << i << " ";
    }
    cout << endl;
    cout << "ready_out:";
    for (auto &i : ready_out) {
        cout << i << " ";
    }
    cout << endl;
    cout << "run:";
    for (auto &i : run) {
        cout << i << " ";
    }
    cout << endl;
    cout << "jam:";
    for (auto &i : jam) {
        cout << i << " ";
    }
    cout << endl;
    cout << "jam_out:";
    for (auto &i : jam_out) {
        cout << i << " ";
    }
    cout << endl;
    cout << "end_state:";
    for (auto &i : end_state) {
        cout << i << " ";
    }
    cout << endl;
    cout << endl;
}
/**
 * 转换成就绪态。
 */
void ToReady(char name) {
    ready.push_back(name);
    if (ready.size() >= OUT_NUM) {
        char shit = ready.back();
        ready.pop_back();
        ready_out.push_back(shit);
    }
}

/**
 * 转换成阻塞态。
 */

void ToJam(char name) {
    jam.push_back(name);
    if (jam.size() >= OUT_NUM) {
        char shit = jam.front();
        jam.pop_front();
        jam_out.push_back(shit);
    }
}
/**
 * 对想要进入就绪态的进程进行预处理。如果满足条件将ready_out中的进程转进ready,
 * 然后ready中的进入运行态。
 */

void ReadyTick() {
    if (run.empty() && (!ready.empty())) {
        char name = ready.front();
        ready.pop_front();
        run.push_back(name);
    }
    while (ready.size() < OUT_NUM - 1 && (!ready_out.empty())) {
        char name = ready_out.front();
        ready_out.pop_front();
        ready.push_back(name);
    }
}
void JamTick() {
    while (jam.size() < OUT_NUM - 1 && (!jam_out.empty())) {
        char name = jam_out.front();
        jam_out.pop_front();
        jam.push_back(name);
    }
}
/**
 * 用Order下指令。
 * 使用方法：[order] [target]
 * create：创建新的进程。
 * event：发起一个事件。需要指定事件的接受进程。
 * yield：要求正在运行的进程停止。
 * jam：要求正在运行的进程进入阻塞态。
 * end：要求正在运行的进程终止。
 */
void Order() {
    string command;
    cin >> command;
    char name;
    cin >> name;
    if (command == "create") {
        create.push_back(name);
        cout << "Create:" << name << endl;
        Display();
        create.pop_back();
        ToReady(name);
    } else if (command == "event") {
        bool is_dead = false;
        for (auto i : jam) {
            if (i == name) {
                jam.remove(i);
                ToReady(name);
                is_dead = true;
                break;
            }
        }
        if (!is_dead) {
            for (auto i : jam_out) {
                if (i == name) {
                    jam_out.remove(i);
                    ready_out.push_back(name);
                    Display();
                    break;
                }
            }
        }
    } else if (command == "yield") {
        name = run.front();
        run.pop_front();
        ToReady(name);
    } else if (command == "jam") {
        name = run.front();
        run.pop_front();
        ToJam(name);
    } else if (command == "end") {
        name = run.front();
        run.pop_front();
        end_state.push_back(name);
        Display();
        end_state.pop_back();
    }
    /**
     * 这里借用了游戏引擎中Tick的思想。首先把所有数据都准备好，然后再用Tick完成所有数据的运作。
     */
    ReadyTick();
    JamTick();
    Display();
}

int main() {
    while (1) {
        Order();
    }
}