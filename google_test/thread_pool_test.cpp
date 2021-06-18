#include <gtest/gtest.h>
#include "thread_pool.h"

TEST(ThreadPool, SimpleTask) {
    auto f = ThreadPool::getInstance().enqueue([] {
        return 10;
    });

    ASSERT_EQ(f.get(), 10);
}

TEST(ThreadPool, MultipleTasks) {

    for(int i = 0; i < 50; i++) {
        auto f = ThreadPool::getInstance().enqueue([] {
            unsigned long long sum = 0;
            for(int i = 0; i < 100000; i++) {
                sum += i*i+49;
            }
            return sum;
        });
        ASSERT_EQ(f.get(), 18103508527376);
    }

}