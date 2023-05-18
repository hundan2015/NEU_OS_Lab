#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

using namespace std;
/**
 * @brief 线程信号量的模拟类
 */
class Semaphore {
   public:
    explicit Semaphore(int count = 0) : count_(count) {}

    void Signal() {
        std::unique_lock<std::mutex> lock(mutex_);
        ++count_;
        cv_.notify_one();
    }

    void Wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [=] { return count_ > 0; });
        --count_;
    }

   private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int count_;
};

/**
 * @brief Plate类是一个互斥区的模拟类。
 */
class Plate {
    const int size_ = 3;
    mutex plate_mutex_;
    bool is_work_in_muti_ = false;

   public:
    // counting_semaphore have_border;
    // counting_semaphore<0> empty_border;
    // 上边界信号量
    Semaphore have_border = Semaphore(0);
    // 下边界信号量
    Semaphore empty_border = Semaphore(0);

    explicit Plate(int size, bool is_work_in_muti = true)
        : size_(size), is_work_in_muti_(is_work_in_muti) {
        for (int i = 0; i < size; i++) {
            // 用一种传统的方式快速定义信号量的大小
            empty_border.Signal();
        }
    }

    queue<char> plate;

    void Insert(char target) {
        empty_border.Wait();
        plate_mutex_.lock();
        cout << "Insert char " << target << endl;
        plate.emplace(target);
        plate_mutex_.unlock();
        have_border.Signal();
    }

    char Get() {
        have_border.Wait();
        plate_mutex_.lock();
        char result = plate.front();
        cout << "Get char " << result << endl;
        plate.pop();
        plate_mutex_.unlock();
        empty_border.Signal();
        return result;
    }
};

int main() {
    Plate plate(3);
    // 定义两个群组中生产者与消费者的人数
    const int producer_count = 3;
    const int consumer_count = 3;
    // 使用vector统一管理多线程
    vector<thread> producers;
    vector<thread> consumers;
    for (int i = 0; i < producer_count; ++i) {
        // 在生产者群组中添加生产者线程。 
        producers.emplace_back(thread([=, &plate]() {
            int num = i;
            while (true) {
                char shit = to_string(num)[0];
                cout << "Producer " << num << " try insert.\n";
                plate.Insert(shit);
                cout << "Producer " << num << " insert success.\n";
            }
        }));
    }
    for (int i = 0; i < consumer_count; ++i) {
        // 在消费者群组中添加消费者线程。
        consumers.emplace_back(thread([=, &plate]() {
            int num = i;
            while (true) {
                cout << "Consumer " << num << " try insert.\n";
                char result = plate.Get();
                cout << "Consumer " << num << " get " << result
                     << " success.\n";
            }
        }));
    }
    // 业务结束需要等待回收这些线程。
    for (auto &i : producers) {
        i.join();
    }
    for (auto &i : consumers) {
        i.join();
    }
}