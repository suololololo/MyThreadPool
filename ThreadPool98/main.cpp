#include "ThreadPool.h"
#include <iostream>
#include <unistd.h>
class concreateTask:public Task
{
    public:
    concreateTask(){}
    virtual int run() override
    {
        std::cout << "thread" <<pthread_self() << (char*)this->arg<< std::endl;
        sleep(1);
        return 0;
    }
};
int main()
{
    char  temp[] = "hello world";
    concreateTask task;
    task.setArg((void *) temp);
    ThreadPool pool(10);
    for (int i = 0; i < 20; i++)
    {
        pool.addTask(&task);
    }
    while(true)  
    {  
        printf("there are still %d tasks need to process\n", pool.size());  
        if (pool.size() == 0)
        {  
            pool.stopPool();
            printf("Now I will exit from main\n"); 
            exit(0);   
        }  
        sleep(2);  
    }  
}