#include "../include/thread_pool.h"

// 构造函数
ThreadPool::ThreadPool(size_t threads)
{
    for (size_t i = 0; i < threads; i++)
    {
        workers_.emplace_back([this]()
                              {
            while(true){
                //加锁
                std::unique_lock<std::mutex> lock(queue_mutex_);
                //停止或者有任务唤醒
                condition_.wait(lock,[this](){
                    return stop_ || !tasks_.empty();
                });
                //如果要停止且任务队列为空，线程退出
                if(stop_ && tasks_.empty()){
                    break;
                }
                else{
                    //取任务
                    auto task = std::move(tasks_.front());
                    tasks_.pop();
                    //解锁
                    lock.unlock();
                    //执行任务
                    task();
                }
            } });
    }
}

// 析构函数
ThreadPool::~ThreadPool()
{
    // 加锁
    std::unique_lock<std::mutex> lock(queue_mutex_);
    // 修改变量
    stop_ = true;
    // 释放锁
    lock.unlock();
    // 唤醒所有线程
    condition_.notify_all();
    // 等待每个线程结束
    for (auto &it : workers_)
    {
        it.join();
    }
}