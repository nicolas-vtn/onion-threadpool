# Onion Thread Pool

A lightweight, dynamic C++20 thread pool for dispatching asynchronous tasks.

It allows submitting tasks that are executed by a pool of persistent worker threads.  
The pool size can be changed at runtime, and workers sleep efficiently when no tasks are available.

The implementation is based on modern C++20 features such as `std::jthread` and `std::stop_token`.

---

## Features

* C++20 compliant
* Dynamic worker thread count
* Thread-safe task dispatch
* Efficient worker sleep using `std::condition_variable`
* Graceful thread shutdown using `std::stop_token`
* No busy waiting
* No external dependencies
* Simple and lightweight

---

## Adding to Your CMake Project

If the library is included in your repository:

```cmake
add_subdirectory(libs/threadsafequeue)
add_subdirectory(libs/threadpool)

target_link_libraries(your_target
    PRIVATE
        onion::threadpool
)
```

> Note: `onion::threadpool` requires the `onion::threadsafequeue` target to already exist.

---

## Basic Usage

```cpp
#include <onion/ThreadPool.hpp>
#include <iostream>

onion::ThreadPool pool(4);

pool.Dispatch([]()
{
    std::cout << "Hello from worker thread!" << std::endl;
});
```

Tasks are executed asynchronously by the worker threads.

---

## Dispatching Multiple Tasks

```cpp
onion::ThreadPool pool(4);

for (int i = 0; i < 10; i++)
{
    pool.Dispatch([i]()
    {
        std::cout << "Task " << i << std::endl;
    });
}
```

Tasks are queued and executed by available workers.

---

## Changing the Pool Size

The number of worker threads can be adjusted at runtime.

```cpp
pool.SetPoolsCount(8);
```

New workers are created immediately.

Reducing the count gracefully stops excess workers.

```cpp
pool.SetPoolsCount(2);
```

Stopped workers exit their loop after finishing their current task.

---

## Querying the Pool Size

```cpp
size_t workers = pool.GetPoolsCount();
```

Returns the current number of worker threads.

---

## Dependency

This library requires:

* **onion::threadsafequeue**

The build will fail if the CMake target `onion::threadsafequeue` is not available.
