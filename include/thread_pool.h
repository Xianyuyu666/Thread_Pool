#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
public:
    // 构造函数
    ThreadPool(size_t threads);
    // 析构函数
    ~ThreadPool();
    // 提交任务
    void submit(std::function<void()> task);
    // 禁用拷贝构造
    ThreadPool(const ThreadPool &other) = delete;
    // 禁用赋值构造
    ThreadPool &operator=(const ThreadPool &other) = delete;

private:
    // 工作线程数组
    std::vector<std::thread> workers_;
    // 任务队列
    std::queue<std::function<void()>> tasks_;
    // 互斥锁
    std::mutex queue_mutex_;
    // 条件变量
    std::condition_variable condition_;
    // 停止标志
    bool stop_ = false;
};