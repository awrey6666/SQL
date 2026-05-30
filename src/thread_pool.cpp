#include "thread_pool.h"

ThreadPool::ThreadPool(size_t threads) : running(true) {

    for (size_t i = 0; i < threads; i++) {

        workers.emplace_back([this]() {

            while (true) {

                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(mtx);

                    cv.wait(lock, [&]() {
                        return !tasks.empty() || !running;
                    });

                    if (!running && tasks.empty())
                        return;

                    task = std::move(tasks.front());
                    tasks.pop();
                }

                task();
            }
        });
    }
}

void ThreadPool::enqueue(std::function<void()> task) {

    {
        std::lock_guard<std::mutex> lock(mtx);
        tasks.push(std::move(task));
    }

    cv.notify_one();
}

void ThreadPool::stop() {

    {
        std::lock_guard<std::mutex> lock(mtx);
        running = false;
    }

    cv.notify_all();

    for (auto &t : workers)
        if (t.joinable())
            t.join();
}

ThreadPool::~ThreadPool() {
    stop();
}