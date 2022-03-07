#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <string>
#include <deque>
#include <pthread.h>
class Task
{
public:
    Task(void *arg_ = NULL, const std::string name = "") : arg(arg_), taskName(name)
    {
    }
    virtual ~Task(){};

    void setArg(void *arg_)
    {
        arg = arg_;
    }
    virtual int run() = 0;

protected:
    void *arg;
    std::string taskName;
};

class ThreadPool
{
public:
    ThreadPool(int threadNum = 10);
    ~ThreadPool();
    int addTask(Task *task);
    void stopPool();
    int size();
private:
    void createThread();
    static void *threadFunc(void *);
    Task* take();

private:
    int m_threadNum;
    std::deque<Task *> m_tasks;
    pthread_t *m_thread;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    volatile bool stop;
};

#endif