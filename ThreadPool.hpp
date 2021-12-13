#pragma once

#include <thread>
#include <vector>
#include <cstddef>


namespace cli
{
    class ThreadPool final
    {
    public:
        using iterator = std::vector<std::thread>::iterator;

        ThreadPool() = default;
        explicit ThreadPool(std::size_t count);

        ~ThreadPool();

        template < class Fn, class...ArgTs >
        std::thread& Launch(Fn&& fn, ArgTs&&...args)
        {
            threads.emplace_back(
                std::forward<Fn>(fn),
                std::forward<ArgTs>(args)...
            );
            return threads.back();
        }

        std::thread& At(std::size_t i);
        const std::thread& At(std::size_t i) const;

        iterator begin() { return threads.begin(); }
        iterator end() { return threads.end(); }
        std::thread* data() { return threads.data(); }
        std::size_t size() const { return threads.size(); }

        void Join(std::size_t i);
        void Join(iterator);
        void JoinAll();
        void TryJoinAll();

    private:
        std::vector<std::thread> threads { };
    };
}

