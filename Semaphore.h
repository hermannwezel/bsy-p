#ifndef BSY_SEMAPHORE_H
#define BSY_SEMAPHORE_H

#include <mutex>
#include <condition_variable>

class Semaphore {
private:
    std::mutex mutex_;
    std::condition_variable condition_;
    unsigned long count_ = 0;

public:
    Semaphore(int n) {
        count_ = n;
    }

    void v() // V-Operation zum Aufheben des Schutzes
    {
        std::lock_guard<std::mutex> lock(mutex_);
        ++count_;
        condition_.notify_one();
    }

    void p() // P-Operation zum Setzen des Schutzes
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while (!count_) // Sperren, wenn der Zaehler null ist
            condition_.wait(lock);
        --count_;
    }
};

#endif //BSY_SEMAPHORE_H
