#include "ThreadPool.h"
#include <iostream>
int main()
{
    std::vector<std::future<int>> results;
    ThreadPool threadPool(2);
    for (int i = 0; i < 3; i++)
    {
        results.emplace_back(
            threadPool.enque(
                [i]
                {
                    std::cout << "hello" << i <<std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    return i*i;
                }
            )
        );
    }
    for (auto &&result:results)
    {
        std::cout <<result.get()<<std::endl;
    }
    std::cout <<std::endl;
}