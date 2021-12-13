#include <iostream>

#include "ThreadPool.hpp"

using namespace cli;


ThreadPool::ThreadPool(const std::size_t count)
{
    threads.reserve(count);
}


ThreadPool::~ThreadPool()
{
    JoinAll();
}


void ThreadPool::Join(std::size_t i)
{
    if (i >= size())
        throw std::out_of_range(
            "ThreadPool::Join(std::size_t i) : i{"
            + std::to_string(i) + "} >= size(){" +
            std::to_string(size()) + "}"
        );
    else Join(begin() + i);
}


void ThreadPool::Join(iterator iter)
{
    if (iter->joinable())
        iter->join();
    threads.erase(iter);
}


void ThreadPool::JoinAll()
{
    while (!threads.empty())
    {
        std::thread& thr = threads.back();
        if (thr.joinable())
            thr.join();
        threads.pop_back();
    }
}

