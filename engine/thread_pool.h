#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <future>
#include <memory>
#include <functional>

#define AMOUNT_THREADS 6

/**
 * Thread pool class implemented as singleton
 */
class ThreadPool {
public:
    using Task = std::function<void()>;
    static ThreadPool &getInstance() {
        static auto *threadPool = new ThreadPool();
        return *threadPool;
    }

    template<typename T>
    auto enqueue(T task) -> std::future<decltype(task())> {
        auto wrapper = std::make_shared<std::packaged_task<decltype(task())()>>(task);
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace([=] {
                (*wrapper)();
            });
        }
        condition.notify_one();
        return wrapper->get_future();
    }

    // Delete constructors, so no new instances can be created
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

private:
    ThreadPool() {
        threads.resize(AMOUNT_THREADS);

        for(int i = 0; i < AMOUNT_THREADS; i++) {
            threads[i] = std::thread([] {
                while(true) {
                    Task task;

                    {
                        std::unique_lock<std::mutex> lock(getInstance().queueMutex);

                        getInstance().condition.wait(lock, []{
                            return getInstance().terminate || !getInstance().tasks.empty();
                        });

                        if(getInstance().terminate) {
                            break;
                        }

                        task = getInstance().tasks.front();
                        getInstance().tasks.pop();
                    }
                    task();
                }
            });
        }

    }


    ~ThreadPool() {
        std::unique_lock<std::mutex> lock(queueMutex);
        terminate = true;
        condition.notify_all();

        for(int i = 0; i < AMOUNT_THREADS; i++) {
            threads[i].join();
        }
    }

    bool terminate = false;

    std::queue<Task> tasks;
    std::vector<std::thread> threads;
    std::mutex queueMutex;
    std::condition_variable condition;
};
