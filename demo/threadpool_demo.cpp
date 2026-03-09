#include <iostream>

#include <onion/ThreadPool.hpp>

void DummyTask(int taskId)
{
	auto threadId = std::this_thread::get_id();
	std::cout << "Executing dummy task " << taskId << " on thread: " << threadId << std::endl;

	for (int i = 0; i < 5; ++i)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::cout << "Dummy task " << taskId << " iteration " << i + 1 << " on thread: " << threadId << std::endl;
	}

	std::cout << "Dummy task " << taskId << " completed on thread: " << threadId << std::endl;
}

int main()
{
	onion::ThreadPool threadPool(2);

	threadPool.Dispatch([]() { DummyTask(1); });
	threadPool.Dispatch([]() { DummyTask(2); });
	threadPool.Dispatch([]() { DummyTask(3); });

	std::this_thread::sleep_for(std::chrono::seconds(10));

	return 0;
}
