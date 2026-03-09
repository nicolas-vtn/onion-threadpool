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
		for (auto& t : m_Workers)
			t.request_stop();

		m_Condition.notify_all();
	}

	void ThreadPool::Dispatch(std::function<void()> task)
	{
		{
			std::lock_guard lock(m_Mutex);
			m_Tasks.push(std::move(task));
		}

		m_Condition.notify_one();
	}

	size_t ThreadPool::GetPoolsCount() const
	{
		std::lock_guard lock(m_Mutex);
		return m_Workers.size();
	}

	void ThreadPool::SetPoolsCount(size_t count)
	{
		std::lock_guard lock(m_Mutex);

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
				m_Workers.back().request_stop();
				m_Workers.pop_back();
			}

			m_Condition.notify_all();
		}
	}

	void ThreadPool::WorkerLoop(std::stop_token st)
	{
		while (!st.stop_requested())
		{
			std::function<void()> task;

			{
				std::unique_lock lock(m_Mutex);

				m_Condition.wait(lock, [&] { return !m_Tasks.empty() || st.stop_requested(); });

				if (st.stop_requested())
					return;

				task = std::move(m_Tasks.front());
				m_Tasks.pop();
			}

			task();
		}
	}

} // namespace onion
