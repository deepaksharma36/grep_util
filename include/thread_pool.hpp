#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <regex>

namespace GrepUtility {

/**
 * @brief A ThreadPool class to manage and execute tasks in parallel.
 * 
 * The ThreadPool class is designed to handle asynchronous task execution by maintaining 
 * a pool of worker threads. It allows tasks (such as file path processing ) 
 * to be submitted, which are then processed by available worker threads.
 */
class ThreadPool {

public:
    /**
     * @brief Constructs a ThreadPool and initializes worker threads.
     * 
     * The number of worker threads is set based on a scaling factor relative to the system's
     * hardware concurrency, allowing efficient use of system resources.
     */
    ThreadPool();

    /**
     * @brief Destructs the ThreadPool, stopping all threads and cleaning up resources.
     */
    ~ThreadPool();

    /**
     * @brief Submits a file path to the task queue for processing.
     * 
     * The file path is added to a queue, where it will be processed by the worker threads.
     * This method is thread-safe.
     * 
     * @param file_path The file path to process.
     */
    void submit(const std::string& file_path);

    /**
     * @brief Registers the task function that worker threads will execute on file paths.
     * 
     * This function is called on each file path that is submitted to the pool. The task function 
     * should take a `const std::string&` parameter, representing the file path to be processed.
     * 
     * @param task The task function to execute for each submitted file path.
     */
    void register_task(const std::function<void(const std::string&)>& task);
    
    /**
     * @brief Stops all worker threads and clears the task queue.
     * 
     * Sets a flag to indicate that threads should stop processing tasks, and then joins all 
     * threads to ensure they exit cleanly.
     */
    void stop();


private:
    /**
     * @brief The main function executed by each worker thread.
     * 
     * Worker threads wait on a condition variable for tasks to be available in the queue, 
     * and execute the registered task function on each file path.
     */
    void worker_thread_function();

    /**
     * @brief Retrieves and processes tasks from the queue.
     * 
     * This function dequeues a file path from the queue and applies the registered task 
     * function to it. If the queue is empty, the thread waits until more tasks are added.
     */
    void work();


    std::vector<std::thread> workers_;                 ///< Vector of worker threads in the pool.
    std::queue<std::string> file_paths_;               ///< Queue of file paths submitted for processing.
    std::mutex queue_mutex_;                           ///< Mutex to protect access to the file_paths_ queue.
    std::condition_variable condition_;                ///< Condition variable to signal availability of new tasks.
    std::atomic<bool> stop_flag{false};                ///< Atomic flag indicating if the pool should stop processing tasks.
    std::function<void(const std::string&)> task_function_; ///< Registered task function for file processing.
    int thread_scaling_factor_ = 3;                    ///< Scaling factor to determine number of threads based on hardware concurrency.
};

} // namespace GrepUtility
