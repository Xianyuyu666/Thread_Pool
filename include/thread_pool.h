#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <utility>
#include <type_traits>

class ThreadPool
{
public:
    // 构造函数
    ThreadPool(size_t threads);
    // 析构函数
    ~ThreadPool();
    // 提交任务
    template <class F, class... Args>
    auto submit(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        auto res = task->get_future();
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            tasks_.emplace([task]()
                           { (*task)(); });
        }
        condition_.notify_one();
        return res;
    }
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