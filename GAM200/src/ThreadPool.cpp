#include <ThreadPool.h>

void ThreadPool::Initialize() {
    num_of_threads = 4;
    for (size_t i = 0; i < num_of_threads; ++i) {
        workers.emplace_back([this, i] { this->workerThread(i); });
    }
}

void ThreadPool::Free() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        while (!tasks.empty())
            tasks.pop();
        stop = true;
    }
    condition.notify_all();
    for (std::thread& worker : workers) {
        worker.join();
    }
}

bool ThreadPool::isQueueEmpty() {
    std::unique_lock<std::mutex> lock(queue_mutex);
    return tasks.empty();
}

int ThreadPool::numOfTasksLeft() {
    return tasks.size();
}

void ThreadPool::workerThread(int i) {
    std::cout << "Worker thread no. " << i << " initialized and running." << std::endl;
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            std::cout << "Worker thread no. " << i << " waiting for task or stop condition." << std::endl;
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            if (stop && tasks.empty()) {
                std::cout << "Stop condition met with stop flag value: " << stop << ". Worker thread no. " << i << " exiting." << std::endl;
                return;
            }
            std::cout << "Worker thread no. " << i << " starting a task." << std::endl;
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
        std::cout << "Worker thread no. " << i << " finished a task." << std::endl;
    }
}