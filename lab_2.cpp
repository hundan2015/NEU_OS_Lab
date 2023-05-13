#include <iostream>
#include <mutex>
#include <queue>
#include <semaphore>
using namespace std;
const char produce = 'p';
const char take = 't';
const char dead = 'e';

class Plate {
    const int size_ = 3;
    mutex plate_mutex_;
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
    Plate plate(3);
    char command;
    cin >> command;
    while (command != dead) {
        switch (command) {
            case (produce): {
                char shit = 'E';
                bool result = plate.Insert(shit);
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
            default:{
                cout<<"No no no. What do you want?"<<endl;
            }
        }
        cin >> command;
    }
}