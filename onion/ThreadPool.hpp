#pragma once

#include <onion/ThreadSafeQueue.hpp>

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace onion
{
	/// @brief A simple thread pool implementation that allows dispatching tasks to a pool of worker threads.
	class ThreadPool
	{
		// ----- Constructors & Destructor -----
	  public:
		/// @brief Constructs a ThreadPool with the specified number of worker threads. Each worker thread will continuously fetch and execute tasks from the task queue until a stop is requested.
		/// @param count The number of worker threads to create in the thread pool.
		ThreadPool(size_t count);
		~ThreadPool();

		// ----- Public API -----
	  public:
		/// @brief Dispatches a task to the thread pool. The task will be executed by one of the worker threads when it becomes available.
		/// @param task The task to be executed, represented as a std::function<void()>.
		void Dispatch(std::function<void()> task);

		/// @brief Closes the thread pool by requesting all worker threads to stop and waiting for them to finish executing their current tasks. After calling this method, the thread pool will no longer accept new tasks and all worker threads will be terminated.
		void Close();

		// ----- Getters & Setters -----
	  public:
		/// @brief Gets the current number of worker threads in the thread pool.
		/// @return The number of worker threads.
		size_t GetPoolsCount() const;
		/// @brief Sets the number of worker threads in the thread pool. If the new count is greater than the current count, new worker threads will be spawned. If the new count is less than the current count, excess worker threads will be stopped and removed.
		/// @param count The new number of worker threads to set.
		void SetPoolsCount(size_t count);

		// ----- Private Methods -----
	  private:
		/// @brief The main loop for each worker thread. Continuously fetches and executes tasks from the task queue until a stop is requested.
		/// @param st The stop token used to request the worker thread to stop.
		void WorkerLoop(std::stop_token st);

		// ----- Private Members -----
	  private:
		mutable std::mutex m_MutexWorkers;
		/// @brief A vector of worker threads that are part of the thread pool. Each thread runs the WorkerLoop function to process tasks from the task queue.
		std::vector<std::jthread> m_Workers;
		/// @brief A queue of tasks to be executed by the worker threads.
		ThreadSafeQueue<std::function<void()>> m_Tasks;
	};
} // namespace onion
