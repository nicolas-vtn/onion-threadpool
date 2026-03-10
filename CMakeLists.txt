cmake_minimum_required(VERSION 3.20)

project(onion_threadpool LANGUAGES CXX)

# ---- Dependency : onion_threadsafequeue ----
if (NOT TARGET onion::threadsafequeue)
    message(FATAL_ERROR "onion::threadsafequeue is required for onion::threadpool")
endif()

# ---- Library ----
add_library(onion_threadpool
	"onion/ThreadPool.cpp"
)
add_library(onion::threadpool ALIAS onion_threadpool)

target_include_directories(onion_threadpool
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(onion_threadpool
	PUBLIC
		onion::threadsafequeue
)

target_compile_features(onion_threadpool PUBLIC cxx_std_20)
 
set_target_properties(onion_threadpool PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# ---- Demo ----
option(ONION_BUILD_DEMO "Build Threadpool demo" OFF)

if (ONION_BUILD_DEMO)
    add_subdirectory(demo)
endif()
