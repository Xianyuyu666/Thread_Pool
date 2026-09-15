#include <iostream>
#include <chrono>
#include <atomic>
#include "../include/thread_pool.h"

std::atomic<int>count{0};

std::mutex print_mutex;

void safe_print(const std::string& msg){
    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << msg;
}

int main()
{
    ThreadPool pool(4);
    std::vector<std::future<int>> results;
    for(int i = 0;i < 20;i++){
        results.push_back(pool.submit([i]()->int{
            safe_print("任务" + std::to_string(i) + "正在执行\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            count++;
            return i * i;
        }));
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));
    for(auto & it : results){
        std::cout << it.get() << std::endl;
    }
    std::cout << "完成" << count << "个任务" << std::endl;
    return 0;
}