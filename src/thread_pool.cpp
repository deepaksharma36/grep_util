#include <iostream>
#include <thread>
#include <regex>
#include "thread_pool.hpp"

namespace GrepUtility {


    ThreadPool::ThreadPool() {
    // Create worker threads
    size_t num_threads = thread_scaling_factor_ * std::thread::hardware_concurrency();
    if (num_threads == 0) {
        num_threads = 4;
    }
    for (size_t i = 0; i < num_threads; ++i) {
        std::thread t([this] {worker_thread_function();});
        workers_.push_back(std::move(t));  // Add worker thread to the pool
        }
    }

    ThreadPool::~ThreadPool() {
        stop();
    }
    
    void ThreadPool::register_task(const std::function<void(const std::string&)>& task) {
        task_function_ = task;  // Register the callback function
    }

    void ThreadPool::submit(const std::string& file_path) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        file_paths_.push(file_path);  // Add file path to the queue
    }
        condition_.notify_one();  // Notify one worker thread to pick up the task
    }

    void ThreadPool::stop() {
        if (stop_flag.exchange(true)) {
            return;
        }
        stop_flag = true;
        condition_.notify_all();
        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();  // Wait for threads to finish
            }
        }
    }

    void ThreadPool::work() {
        std::string file_path;
        file_path = std::move(file_paths_.front());
        file_paths_.pop();

        if (task_function_) {
            task_function_(file_path);

        }

    }

    void ThreadPool::worker_thread_function() {
        while(true) {
            std::unique_lock<std::mutex> lock(queue_mutex_);

            while (!stop_flag && file_paths_.empty()) {
                condition_.wait(lock);
            }

            if (stop_flag && file_paths_.empty()) {
                return;
            }

            work();
       } 
    }





} // namespace GrepUtility
