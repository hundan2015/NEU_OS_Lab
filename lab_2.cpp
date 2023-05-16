#include <iostream>
#include <mutex>
#include <queue>
#include <semaphore>
using namespace std;
// 生产的指令。将物品放入盘子内。
const char produce = 'p';
// 消费的指令。将物品从盘子内取出。
const char take = 't';
// 结束指令。输入此指令将结束。
const char dead = 'e';
/**
 * @brief Plate类是一个互斥区的模拟类。
 *
 */
class Plate {
    // 指定盘子的大小。这个量在运行时不可改变，只能在初始化盘子的时候定义
    const int size_ = 3;
    // 访问盘子的Mutex
    mutex plate_mutex_;
    // 询问是否运行在多线程环境下。在目前程序段中没有用处。
    bool is_work_in_muti_ = false;

   public:
    // counting_semaphore have_border;
    // counting_semaphore<0> empty_border;
    explicit Plate(int size, bool is_work_in_muti = false)
        : size_(size), is_work_in_muti_(is_work_in_muti) {
        for (auto i = 0; i < size; ++i) {
            // empty_border.release();
        }
    }
    queue<char> plate;
    /**
     * @brief 处理放入盘子的事件。
     * @return 返回是否成功放入盘子。
     * @param target 放入的物品。
     */
    bool Insert(char &target) {
        if (is_work_in_muti_) {
            // have_border.release();
            // empty_border.acquire();
        }
        auto lock_g = lock_guard(plate_mutex_);
        if (plate.size() >= size_) {
            cout << "Plate is full. Failed." << endl;
            return false;
        }
        cout << "Insert char " << target << endl;
        plate.emplace(move(target));
        return true;
    }
    /**
     * @brief 返回盘子中的物品的方法。
     * @return 盘子中的物品。如果盘子为空的直接返回0。
     */
    char Get() {
        if (is_work_in_muti_) {
            // empty_border.release();
            // have_border.acquire();
        }
        auto lock_g = lock_guard(plate_mutex_);
        if (plate.empty()) {
            cout << "Plate is empty. Don't be a dick." << endl;
            return 0;
        }
        char result = plate.front();
        cout << "Get char " << result << endl;
        plate.pop();
        return result;
    }
};
int main() {
    // 定义一个空盘子。这里盘子的大小为3
    Plate plate(3);
    char command;
    cin >> command;
    // 在程序没有死亡的情况下处理业务。
    while (command != dead) {
        switch (command) {
            case (produce): {
                // 为了更简洁地展示过程，我们直接使用字符E作为统一的产品。
                char production = 'E';
                bool result = plate.Insert(production);
                if (result) {
                    cout << "Insert success." << endl;
                } else {
                    cout << "Insert failed." << endl;
                }
                break;
            }

            case (take): {
                auto result = plate.Get();
                if (result != 0) {
                    cout << "Get " << result << " success." << endl;
                } else {
                    cout << "Get failed." << endl;
                }
                break;
            }
            default: {
                cout << "No no no. What do you want?" << endl;
            }
        }
        cin >> command;
    }
}