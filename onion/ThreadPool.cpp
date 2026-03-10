#include "ThreadPool.hpp"

namespace onion
{
	ThreadPool::ThreadPool(size_t count)
	{
		for (size_t i = 0; i < count; ++i)
		{
			m_Workers.emplace_back([this](std::stop_token st) { WorkerLoop(st); });
		}
	}

	ThreadPool::~ThreadPool()
	{
		{
			std::lock_guard lock(m_MutexWorkers);

			for (auto& t : m_Workers)
				t.request_stop();

			for (auto& t : m_Workers)
				t.join();
		}
	}

	void ThreadPool::Dispatch(std::function<void()> task)
	{
		m_Tasks.Push(std::move(task));
	}

	size_t ThreadPool::GetPoolsCount() const
	{
		std::lock_guard lock(m_MutexWorkers);
		return m_Workers.size();
	}

	void ThreadPool::SetPoolsCount(size_t count)
	{
		std::lock_guard lock(m_MutexWorkers);

		size_t current = m_Workers.size();

		if (count > current)
		{
			// Spawn new workers
			for (size_t i = current; i < count; ++i)
			{
				m_Workers.emplace_back([this](std::stop_token st) { WorkerLoop(st); });
			}
		}
		else if (count < current)
		{
			size_t toRemove = current - count;

			for (size_t i = 0; i < toRemove; ++i)
			{
				m_Workers[current - 1 - i].request_stop();
			}

			for (size_t i = 0; i < toRemove; ++i)
			{
				m_Workers.back().join();
				m_Workers.pop_back();
			}
		}
	}

	void ThreadPool::WorkerLoop(std::stop_token st)
	{
		std::function<void()> task;
		while (m_Tasks.WaitPop(task, st))
		{
			task();
		}

		//while (!st.stop_requested())
		//{
		//	std::function<void()> task;

		//	{
		//		std::unique_lock lock(m_Mutex);

		//		m_Condition.wait(lock, [&] { return !m_Tasks.empty() || st.stop_requested(); });

		//		if (st.stop_requested())
		//			return;

		//		task = std::move(m_Tasks.front());
		//		m_Tasks.pop();
		//	}

		//	task();
		//}
	}

} // namespace onion
