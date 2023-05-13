#include <iostream>
#include <vector>
#include <unistd.h>
#include <wait.h>
#include <queue>
#include <map>
#define RANDROUND(a, b) (rand() % (b - a + 1)) + a
using namespace std;
const int generate_count = 100;
const int container_size = 3;

void Fifo(vector<int> &access_series)
{
    int disefficient = 0;
    int total_instruction = 0;
    queue<int> container;
    // 关于table_count主要是用来快速寻找已经存在过的元素。之前对FIFO的理解有
    // 误，因此是个map。其实这里可以抽象成一个类。但是因为代码量过于少因此使用了
    // 面向过程的方式。
    map<int, int> table_count;
    for (auto &i : access_series)
    {
        total_instruction++;
        auto iter = table_count.find(i);
        if (iter == table_count.end())
        {
            cout << "FIFO:Push " << i << endl;
            table_count.insert(make_pair(i, 1));
            container.push(i);
            disefficient++;
        }
        else
        {
            iter->second++;
        }

        if (table_count.size() > container_size)
        {
            cout << "FIFO:Full!" << endl;
            auto front_iter = table_count.find(container.front());
            /* while (front_iter->second != 1)
            {
                front_iter->second--;
                container.pop();
                front_iter = table_count.find(container.front());
            } */
            cout << "FIFO:Pop front " << front_iter->first << endl;
            container.pop();
            table_count.erase(front_iter);
        }
    }
    float dis_hit_ratio = (float)disefficient / total_instruction;
    cout << "FIFO:dis_hit_ratio:" << dis_hit_ratio << endl;
    cout << "FIFO end." << endl;
    cout << endl;
}

void Random(vector<int> &access_series)
{
    int disefficient = 0;
    int total_instruction = 0;
    queue<int> container;
    // 关于table_count主要是用来快速寻找已经存在过的元素。之前对FIFO的理解有
    // 误，因此是个map。其实这里可以抽象成一个类。但是因为代码量过于少因此使用了
    // 面向过程的方式。
    map<int, int> table_count;
    for (auto &i : access_series)
    {
        total_instruction++;
        auto iter = table_count.find(i);
        if (iter == table_count.end())
        {
            cout << "RANDOM:Push " << i << endl;
            table_count.insert(make_pair(i, 1));
            disefficient++;
        }
        else
        {
            iter->second++;
        }

        if (table_count.size() > container_size)
        {
            cout << "RANDOM:Full!" << endl;
            int random = RANDROUND(0, container_size - 1);
            auto iter = table_count.begin();
            for (int i = 0; i < random; ++i)
            {
                iter++;
            }
            cout << "Random:Pop " << iter->first << endl;
            table_count.erase(iter);
        }
    }
    float dis_hit_ratio = (float)disefficient / total_instruction;
    cout << "RANDOM:dis_hit_ratio:" << dis_hit_ratio << endl;
    cout << "RANDOM end." << endl;
    cout << endl;
}

void Lru(vector<int> &access_series)
{
    int disefficient = 0;
    int total_instruction = 0;
    map<int, int> table_count;
    for (auto &i : access_series)
    {
        total_instruction++;
        for (auto &iter_inner : table_count)
        {
            iter_inner.second++;
        }
        auto iter = table_count.find(i);
        if (iter == table_count.end())
        {
            cout << "LRU:Insert " << i << endl;
            table_count.insert(make_pair(i, 0));
            disefficient++;
        }
        else
        {
            iter->second--;
        }

        if (table_count.size() > container_size)
        {
            cout << "LRU:Full!" << endl;
            int max_data = -1;
            int max_count = -1;
            for (auto &i : table_count)
            {
                if (i.second > max_count)
                {
                    max_data = i.first;
                    max_count = i.second;
                }
            }
            cout << "LRU:Pop " << max_data << endl;
            table_count.erase(max_data);
        }
    }
    float dis_hit_ratio = (float)disefficient / total_instruction;
    cout << "LRU:dis_hit_ratio:" << dis_hit_ratio << endl;
    cout << "LRU:end." << endl;
    cout << endl;
}

void MainShit()
{
    // Main thread.
    auto fifo_wait = wait(nullptr);
    auto lru_wait = wait(nullptr);
    cout << "Two child over." << endl;
}

int main()
{
    cout << "shit" << endl;
    // Belady sequence. 3->0.75;4->0.83
    vector<int> access_series = {4, 3, 2, 1, 4, 3, 5, 4, 3, 2, 1, 5};
    /* for (int i = 0; i < generate_count; ++i)
    {
        access_series.push_back(RANDROUND(1, 100));
    } */
    for (auto i : access_series)
    {
        cout << i << " ";
    }
    cout << endl;
    auto fifo = fork();
    int lru = -1;
    if (fifo != 0)
    {
        lru = fork();
        if (lru != 0)
        {
            /* wait(nullptr);
            wait(nullptr);
            Fifo(access_series); */
            MainShit();
        }
        else
        {
            Lru(access_series);
            Random(access_series);
        }
    }
    else
    {
        Fifo(access_series);
    }
}