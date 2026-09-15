#include <iostream>
#include <chrono>
#include <unistd.h>
#include "thread_pool.h"

int main(){
    ThreadPool pool(4);
    for(int i = 1;i <= 10;i++){
        pool.submit([i](){
            std::cout << "任务" << i << " ";
            std::cout << "线程id = " << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        });
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    return 0;
}