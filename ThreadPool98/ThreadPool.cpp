#include "ThreadPool.h"
#include <iostream>
ThreadPool::ThreadPool(int threadNum):m_threadNum(threadNum),stop(false)
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    createThread();
}


void ThreadPool::createThread()
{
    m_thread = (pthread_t *)malloc(sizeof(pthread_t) * m_threadNum) ;
    for (int i = 0; i < m_threadNum; i++)
    {
        pthread_create(m_thread+i, NULL, threadFunc, this);
    }
}

int ThreadPool::addTask(Task *task)
{
    pthread_mutex_lock(&mutex);
    m_tasks.push_back(task);
    int n = m_tasks.size();

    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);
    return n;
}

void* ThreadPool::threadFunc(void * arg)
{

    pthread_t tid = pthread_self();
    ThreadPool* pool = static_cast<ThreadPool*>(arg);
    while(!pool->stop)
    {
        Task* task = pool->take();
        if (!task)
        {
            std::cout <<" thread not exist" << std::endl;
            break;
        }
        task->run();
    }
    return 0;
}

Task* ThreadPool::take()
{
    Task* task = NULL;
    while (!task)
    {
        pthread_mutex_lock(&mutex);
        while (!stop && m_tasks.empty())
        {
            pthread_cond_wait(&cond, &mutex);
        }
        if (stop)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        else if(m_tasks.empty())
        {
            pthread_mutex_unlock(&mutex);
            continue;
        }
        task = m_tasks.front();
        m_tasks.pop_front();
        pthread_mutex_unlock(&mutex);
    }
    return task;
}
void ThreadPool::stopPool()
{
    if (stop)
        return;
    stop = true;
    pthread_cond_broadcast(&cond);
    for (int i = 0; i < m_threadNum; i++)
    {
        pthread_join(m_thread[i], NULL);
    }
    free(m_thread);
    m_thread = NULL;
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}
ThreadPool::~ThreadPool()
{
   stopPool();
   for (std::deque<Task*>::iterator it = m_tasks.begin(); it != m_tasks.end(); ++it)
   {
       delete *it;
   }
   m_tasks.clear();
}

int ThreadPool::size()
{
    pthread_mutex_lock(&mutex);
    int size = m_tasks.size();
    pthread_mutex_unlock(&mutex);
    return size;
}