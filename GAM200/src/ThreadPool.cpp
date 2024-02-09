/* !
@file	ThreadPool.cpp
@author Tan Yee Ann
@date	2/11/2023

This file contains the definitions of the functions that implements a thread pool
for use in the game engine
*//*__________________________________________________________________________*/
#include <ThreadPool.h>

/******************************************************************************
    Initialize
    - Initializes the threadpool
*******************************************************************************/
void ThreadPool::Initialize() {
    num_of_threads = std::thread::hardware_concurrency();
    for (int i = 0; i < num_of_threads; ++i) {
        workers.emplace_back([this, i] { this->workerThread(i); });
    }
}

/******************************************************************************
    Free
    - Cleanly shuts down the threadpool
*******************************************************************************/
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

/******************************************************************************
    isQueueEmpty
    - Checks to see if there is no tasks queued for execution
*******************************************************************************/
bool ThreadPool::isQueueEmpty() {
    std::unique_lock<std::mutex> lock(queue_mutex);
    return tasks.empty();
}

/******************************************************************************
    numOfTasksLeft
    - Returns the number of tasks left in teh queue
*******************************************************************************/
int ThreadPool::numOfTasksLeft() {
    return (int)tasks.size();
}

/******************************************************************************
    workerThread
    - Spawns a worker thread

    @param i - ID of the worker thread
*******************************************************************************/
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