#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <cstddef>
#include <future>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <functional>
#include <stdexcept>
class ThreadPool
{
public:
    ThreadPool(size_t);
    template <class F, class... Args>
    auto enque(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>;
    ~ThreadPool();

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    bool stop;
    std::mutex queue_mutex;
    std::condition_variable condition;
};

inline ThreadPool::ThreadPool(size_t thread_num) : stop(false)
{
    for (int i = 0; i < thread_num; i++)
    {
        // 通过lambda函数创建线程
        workers.emplace_back(
            [this]() -> void
            {
                for (;;)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock,
                                             [this]
                                             { return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty())
                            return;
                        task = move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            }

        );
    }
}

template <class F, class... Args>
inline auto ThreadPool::enque(F &&f, Args&&...args)
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(this->queue_mutex);
        if (stop)
        {
            throw std::runtime_error("enqueue error on stopped ThreadPool");
        }
        tasks.emplace(
            [task]
            {
                (*task)();
            });
        }
    condition.notify_one();
    return res;
}

inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(this->queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker:workers)
    {
        worker.join();
    }

}

#endif