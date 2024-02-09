#pragma once
/* !
@file	ThreadPool.h
@author Tan Yee Ann (t.yeeann@digipen.edu)
@date	2/11/2023

This file contains the declarations for the ThreadPool system class
*//*__________________________________________________________________________*/

#include <Debug.h>
#include <Interface_System.h>
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <future>
#include <iostream>

class ThreadPool : public ISystems {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop = false;
public:
    int num_of_threads;
    
    ThreadPool() {}
    ~ThreadPool() { Free(); }

    /******************************************************************************
        enqueue
        - Queues a function to be executed by a worker thread

        NOTE: Do not enqueue any function that have opengl function calls as they must
        be executed in the thread that initializes it (in this case, the main thread)

        @param f - The function to execute
        @param args - The arguments to pass to the function

        @return - an std::future that contains the returned value of the function
                  (if you pass a non-void function to it)
    *******************************************************************************/
    template <typename Func, typename... Args>
    auto enqueue(Func&& f, Args&&... args) -> std::future<typename std::result_of<Func(Args...)>::type> {
        using return_type = typename std::invoke_result<Func, Args...>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<Func>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            std::cout << "Enqueuing a task." << std::endl;
            tasks.push([task]() { (*task)(); });
        }
        condition.notify_one();

        return res;
    }

    void Initialize();
    void Update() {};

    bool isQueueEmpty();
    int numOfTasksLeft();

    std::string SystemName() override { return "ThreadPool"; }

private:
    void Free() override;
    void workerThread(int i);
};

extern ThreadPool* thread_pool;