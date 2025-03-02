#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include <atomic>
#include <condition_variable>
#include <mutex>

class ThreadPool {
public:
    // Constructor: initialize the pool with a specified number of threads
    ThreadPool(size_t numThreads) {
        // Start the threads and add them to the pool
        for (size_t i = 0; i < numThreads; ++i) {
            workers.push_back(std::thread([this] {
                while (true) {
                    std::function<void()> task;

                    // Lock the queue and check if there are tasks
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });

                        if (stop && tasks.empty()) {
                            return;  // If stop is set and no tasks are left, thread should exit
                        }

                        // Get the next task and remove it from the queue
                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    // Execute the task
                    task();
                }
            }));
        }
    }

    // Destructor: join all threads and clean up
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;  // Set stop flag
        }
        condition.notify_all();  // Notify all threads to stop

        // Join all worker threads to ensure they complete execution
        for (std::thread& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    // Submit a task to the thread pool
    template <class F>
    void submit(F&& f) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.push(std::function<void()>(std::forward<F>(f)));
        }
        condition.notify_one();  // Notify one worker thread to pick up the task
    }

private:
    std::vector<std::thread> workers;  // Threads in the pool
    std::queue<std::function<void()>> tasks;  // Queue of tasks to be executed
    std::mutex queueMutex;  // Mutex for thread-safe access to the task queue
    std::condition_variable condition;  // Condition variable for task synchronization
    std::atomic<bool> stop{false};  // Flag to stop all threads
};