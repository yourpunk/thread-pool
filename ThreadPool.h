#pragma once
 
#include <cstddef>
#include <list>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
 
template<typename JobT, typename WorkerT>
class ThreadPool {
private:
    std::list<JobT> job_queue; // Task queue
    std::vector<std::thread> worker_threads; // Vector of consumer threads
    WorkerT worker_fn; // Function executed by the threads
    std::mutex queue_mutex; // Mutex for protecting access to the queue
    std::condition_variable job_available; // Condition variable for task waiting
    bool stop = false; // Flag for termination
 
public:
    ThreadPool(size_t thread_count, WorkerT worker) : worker_fn(worker) {
        for (size_t i = 0; i < thread_count; ++i) {
            worker_threads.emplace_back([this]() { worker_loop(); });
        }
    }
 
    void process(const JobT job) {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            job_queue.push_back(job);
        }
        job_available.notify_one(); // Notify one thread about available work
    }
 
    void join() {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            stop = true;
        }
        job_available.notify_all(); // Notify all threads about termination
        for (auto& worker_thread : worker_threads) {
            worker_thread.join();
        }
    }
 
private:
    void worker_loop() {
        while (true) {
            JobT job;
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                job_available.wait(lock, [this]() { return stop || !job_queue.empty(); });
                 
                if (stop && job_queue.empty()) {
                    return; // Terminate the thread if a stop command is received and there is no more work
                }
                 
                job = job_queue.front();
                job_queue.pop_front();
            }
             
            if (!job) {
                break; // If job == 0, it's a termination signal
            }
             
            worker_fn(job); // Execute the task
        }
    }
};
